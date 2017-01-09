#include "TLMInterface1D.h"
#include "TLMCommUtil.h"
#include "TLMPlugin.h"
#include <deque>
#include <string>
#include "double33Mat.h"


//TODO: This is used both by 1D and 3D, should probably be defined in one place. /robbr
static const double TLM_DAMP_DELAY = 1.5;

TLMInterface1D::TLMInterface1D(TLMClientComm &theComm, std::string &aName, double StartTime, InterfaceDomain Domain)
    : TLMInterface(theComm, aName, StartTime, Type1D, Domain){}

TLMInterface1D::~TLMInterface1D() {
    if(DataToSend.size() != 0) {
        TLMErrorLog::Log(std::string("Interface ") + GetName() + " sends rest of data for time= " +
                         TLMErrorLog::ToStdStr(DataToSend.back().time));

        Comm.PackTimeDataMessage1D(InterfaceID, DataToSend, Message);
        TLMCommUtil::SendMessage(Message);
    }
}


void TLMInterface1D::UnpackTimeData(TLMMessage &mess)
{
    Comm.UnpackTimeDataMessage1D(mess, TimeData);

    NextRecvTime =  TimeData.back().time + Params.Delay;
}



// The GetTimeData methods read the Instance.time field and fills in
// the other field by interpolating/extrapolating the available data.
void TLMInterface1D::GetTimeData(TLMTimeData1D& Instance) {
    GetTimeData(Instance, TimeData, false);
    if( (Params.alpha > 0) && (Instance.time != TLMPlugin::TIME_WITHOUT_DATA) && (DampedTimeData.size() > 0)) {
        TLMTimeData1D Buf;

        Buf.time = Instance.time - Params.Delay * TLM_DAMP_DELAY;
        GetTimeData(Buf, DampedTimeData, true);

        Instance.GenForce = Instance.GenForce * (1-Params.alpha) + Buf.GenForce * Params.alpha;
    }
}

// The GetTimeData methods read the Instance.time field and fills in
// the other field by interpolating/extrapolating the available data.
void TLMInterface1D::GetTimeData(TLMTimeData1D& Instance, std::deque<TLMTimeData1D>& Data, bool OnlyForce) {
    double time = Instance.time;

    // find the appropriate time interval in the Data vector
    const int size = Data.size();

    if(size == 0) { // no data so far. Simulation startup
        // The time before data is received is a problem:
        // no way to handle simulation restart in a good way.
        // We always assume no waves initially.
        Instance.GenForce = 0.0;

        Instance.Position = Params.cX_R_cG_cG[0]+Params.Nom_cI_R_cX_cX[0];  //TODO: TLMConnectionParams are hard-coded for 3D. How to solve this? //robbr

        Instance.time = TLMPlugin::TIME_WITHOUT_DATA;

        return;
    }

    // CurrentIntervalIndex is used to improve the speed of search in Data
    if(CurrentIntervalIndex >= size) {
        CurrentIntervalIndex =  size - 1;
    }
    if((time >= Data[0].time) && (time < Data[size-1].time)) {
        // the desired time is in the Data boundaries
        // find interpolation spot in data
        while ( Data[CurrentIntervalIndex].time < time)
            CurrentIntervalIndex++;
        while(Data[CurrentIntervalIndex].time > time)
            CurrentIntervalIndex--;

#if 0
        // linear interpolation with Newton interpolation polynomial
        if ((CurrentIntervalIndex > 1) && (CurrentIntervalIndex < size - 2)) {
            // we use cubic interpolation with 4 points if possible
            deque<TLMTimeData>::iterator it(Data.begin() + (CurrentIntervalIndex-1));
            hermite_interpolate(Instance, it, OnlyForce);
        }
        else
#endif
        {
            // linear interpolation
            linear_interpolate(Instance, Data[CurrentIntervalIndex], Data[CurrentIntervalIndex+1],OnlyForce);
        }
    }
    else {
        if (time <= Data[0].time) {
            TLMErrorLog::Warning(std::string("Interface ") + GetName() + " needs to extrapolate back time= " +
                                 TLMErrorLog::ToStdStr(time));
            Instance = Data[0];
        }
        else{
            if (time == Data[size-1].time) {
                Instance = Data[size-1];
            }
            else {
                TLMErrorLog::Warning(std::string("Interface ") + GetName() + " needs to extrapolate forward time= " +
                                     TLMErrorLog::ToStdStr(time));
                if(size > 1) {
                    // linear extrapolation
                    linear_interpolate(Instance, Data[size-2], Data[size-1], OnlyForce);
                }
                else {
                    Instance = Data[0];
                }
            }
        }
    }
}



void TLMInterface1D::GetForce( double time,
                             double position,
                             double speed,
                             double* force) {
    TLMTimeData1D request;
    request.time = time - Params.Delay;
    GetTimeData(request);

    TLMPlugin::GetForce1D(position, speed, request, Params, force);


}


// Set motion data and communicate if necessary.
void TLMInterface1D::SetTimeData(double time,
                               double position,
                               double speed) {
    // put the variables into TLMTimeData structure and the end of  DataToSend vector
    int lastInd = DataToSend.size();
    DataToSend.resize( lastInd + 1);
    TLMTimeData1D& item = DataToSend[lastInd];
    item.time = time;
    item.Position = position;
    item.Velocity = speed;

    // Get the data (NOTE! damped)
    TLMTimeData1D request;
    request.time = time - Params.Delay;
    GetTimeData(request);

    // Store the data if damping is used
    if((Params.alpha > 0) && (request.time !=  TLMPlugin::TIME_WITHOUT_DATA)) {
        DampedTimeData.push_back(request);
    }

    TLMPlugin::GetForce1D(position, speed, request, Params, &item.GenForce);

    // The wave to send is: (- Force + Impedance * Velocity)
    double oldForce = item.GenForce;
    item.GenForce   = -item.GenForce   +  Params.Zf * speed;

    TLMErrorLog::Log(std::string("Interface ") + GetName() +
                     " SET for time= " + TLMErrorLog::ToStdStr(time));

    // Send the data if we past the synchronization point or are in data request mode.
    if(time >= LastSendTime + Params.Delay / 2 || Params.mode > 0.0 ) {
        SendAllData();
    }

    // Remove the data that is not needed (Simulation time moved forward)
    // We leave two time points intact, so that interpolation work
    clean_time_queue(TimeData, time - Params.Delay);
    clean_time_queue(DampedTimeData,  time - Params.Delay * ( 1 + TLM_DAMP_DELAY));
}


void TLMInterface1D::SendAllData() {
    LastSendTime = DataToSend.back().time;

    TLMErrorLog::Log(std::string("Interface ") + GetName() + " sends data for time= " +
                     TLMErrorLog::ToStdStr(LastSendTime));

    Comm.PackTimeDataMessage1D(InterfaceID, DataToSend, Message);
    TLMCommUtil::SendMessage(Message);
    DataToSend.resize(0);

    // In data request mode we shutdown after sending the first data package.
    if( Params.mode > 0.0 ) waitForShutdownFlg = true;
}


// linear_interpolate is called with a vector containing 2 points
// computes the interpolation (or extrapolation) point with the the linear
// interpolation (extrapolation) The points are submitted using the p0 & p1
//  The desired time is given by the Instance.time. Results are stored in Instance
void TLMInterface1D::linear_interpolate(TLMTimeData1D& Instance, TLMTimeData1D& p0, TLMTimeData1D& p1, bool OnlyForce) {

    double time = Instance.time; // needed time point
    // two time points
    const double t0 = p0.time;
    const double t1 = p1.time;

    // interpolate force "wave"
    Instance.GenForce = TLMInterface::linear_interpolate(time, t0, t1, p0.GenForce, p1.GenForce);

    if(OnlyForce) return;

    // The rest is optional

    // interpolate position
    Instance.Position = TLMInterface::linear_interpolate(time, t0, t1, p0.Position, p1.Position);

    // interpolate velocity
    Instance.Velocity = TLMInterface::linear_interpolate(time, t0, t1, p0.Velocity, p1.Velocity);
}


void TLMInterface1D::clean_time_queue(std::deque<TLMTimeData1D>& Data, double CleanTime) {
    while( (Data.size() > 3) && (CleanTime > Data[2].time)) {
        Data.pop_front();
    }
}
