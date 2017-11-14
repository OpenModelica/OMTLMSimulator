#include "Interfaces/TLMInterfaceSignal.h"
#include "Communication/TLMCommUtil.h"
#include "Plugin/TLMPlugin.h"
#include <deque>
#include <string>
#include "double33.h"


//TODO: This is used both by 1D and 3D, should probably be defined in one place. /robbr
static const double TLM_DAMP_DELAY = 1.5;

TLMInterfaceSignal::TLMInterfaceSignal(TLMClientComm &theComm, std::string &aName, double StartTime,
                                       int Dimensions, std::string Causality,
                                       std::string Domain)
    : TLMInterface(theComm, aName, StartTime, Dimensions, Causality, Domain) {}

TLMInterfaceSignal::~TLMInterfaceSignal() {}



void TLMInterfaceSignal::UnpackTimeData(TLMMessage &mess) {
    Comm.UnpackTimeDataMessageSignal(mess, TimeData);

    NextRecvTime =  TimeData.back().time + Params.Delay;
}

void TLMInterfaceSignal::SendAllData() {
    LastSendTime = DataToSend.back().time;

    TLMErrorLog::Log(std::string("Interface ") + GetName() + " sends data for time= " +
                     TLMErrorLog::ToStdStr(LastSendTime));

    Comm.PackTimeDataMessageSignal(InterfaceID, DataToSend, Message);
    TLMCommUtil::SendMessage(Message);
    DataToSend.resize(0);

    // In data request mode we shutdown after sending the first data package.
    if( Params.mode > 0.0 ) waitForShutdownFlg = true;
}

void TLMInterfaceSignal::SetInitialValue(double value)
{
    InitialValue = value;
}

void TLMInterfaceSignal::clean_time_queue(std::deque<TLMTimeDataSignal>& Data, double CleanTime) {
    while( (Data.size() > 3) && (CleanTime > Data[2].time)) {
        Data.pop_front();
    }
}



// The GetTimeData methods read the Instance.time field and fills in
// the other field by interpolating/extrapolating the available data.
void TLMInterfaceSignal::GetTimeData(TLMTimeDataSignal& Instance) {
    GetTimeData(Instance, TimeData);
}


// The GetTimeData methods read the Instance.time field and fills in
// the other field by interpolating/extrapolating the available data.
void TLMInterfaceSignal::GetTimeData(TLMTimeDataSignal& Instance, std::deque<TLMTimeDataSignal>& Data) {
    double time = Instance.time;

    // find the appropriate time interval in the Data vector
    const int size = Data.size();

    if(size == 0) { // no data so far. Simulation startup
        // The time before data is received is a problem:
        // no way to handle simulation restart in a good way.
        // We always assume no waves initially.
        Instance.Value = 0.0;
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
            hermite_interpolate(Instance, it);
        }
        else
#endif
        {
            // linear interpolation
            linear_interpolate(Instance, Data[CurrentIntervalIndex], Data[CurrentIntervalIndex+1]);
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
                    linear_interpolate(Instance, Data[size-2], Data[size-1]);
                }
                else {
                    Instance = Data[0];
                }
            }
        }
    }
}



// linear_interpolate is called with a vector containing 2 points
// computes the interpolation (or extrapolation) point with the the linear
// interpolation (extrapolation) The points are submitted using the p0 & p1
//  The desired time is given by the Instance.time. Results are stored in Instance
void TLMInterfaceSignal::linear_interpolate(TLMTimeDataSignal &Instance, TLMTimeDataSignal &p0, TLMTimeDataSignal &p1) {

    double time = Instance.time; // needed time point
    // two time points
    const double t0 = p0.time;
    const double t1 = p1.time;

    // interpolate value
    Instance.Value = TLMInterface::linear_interpolate(time, t0, t1, p0.Value, p1.Value);
}

