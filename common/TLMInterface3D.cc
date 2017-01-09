#include "TLMInterface3D.h"
#include "TLMCommUtil.h"
#include "TLMPlugin.h"
#include <deque>
#include <string>
#include "double33Mat.h"
#include "double3Vec.h"

//TODO: This is used both by 1D and 3D, should probably be defined in one place. /robbr
static const double TLM_DAMP_DELAY = 1.5;

using namespace tlmMisc;

TLMInterface3D::TLMInterface3D(TLMClientComm &theComm, std::string &aName, double StartTime, std::string Domain)
    : TLMInterface(theComm, aName, StartTime, 6, "Bidirectional", Domain)
{

}

TLMInterface3D::~TLMInterface3D() {
    if(DataToSend.size() != 0) {
        TLMErrorLog::Log(std::string("Interface ") + GetName() + " sends rest of data for time= " +
                         TLMErrorLog::ToStdStr(DataToSend.back().time));

        Comm.PackTimeDataMessage3D(InterfaceID, DataToSend, Message);
        TLMCommUtil::SendMessage(Message);
    }
}


void TLMInterface3D::UnpackTimeData(TLMMessage &mess)
{
    TLMErrorLog::Log(std::string("Interface ") + GetName());
    Comm.UnpackTimeDataMessage3D(mess, TimeData);

    NextRecvTime =  TimeData.back().time + Params.Delay;
}




// The GetTimeData methods read the Instance.time field and fills in
// the other field by interpolating/extrapolating the available data.
void TLMInterface3D::GetTimeData(TLMTimeData3D& Instance) {
    GetTimeData(Instance, TimeData, false);
    if( (Params.alpha > 0) && (Instance.time != TLMPlugin::TIME_WITHOUT_DATA) && (DampedTimeData.size() > 0)) {
        TLMTimeData3D Buf;

        Buf.time = Instance.time - Params.Delay * TLM_DAMP_DELAY;
        GetTimeData(Buf, DampedTimeData, true);

        for(int i = 0; i < 6; i++) {
            Instance.GenForce[i] =
                    Instance.GenForce[i] * (1 - Params.alpha)
                    + Buf.GenForce[i] * Params.alpha;
        }
    }
}

// The GetTimeData methods read the Instance.time field and fills in
// the other field by interpolating/extrapolating the available data.
void TLMInterface3D::GetTimeData(TLMTimeData3D& Instance, std::deque<TLMTimeData3D>& Data, bool OnlyForce) {
    double time = Instance.time;

    // find the appropriate time interval in the Data vector
    const int size = Data.size();

    if(size == 0) { // no data so far. Simulation startup
        // The time before data is received is a problem:
        // no way to handle simulation restart in a good way.
        // We always assume no waves initially.
        int i = 0;
        while(i < 6) {
            Instance.GenForce[i++] = 0.0;
        }

        double3Vec ci_R_cX_cX(Params.Nom_cI_R_cX_cX[0], Params.Nom_cI_R_cX_cX[1], Params.Nom_cI_R_cX_cX[2]);
        double33Mat ci_A_cX(Params.Nom_cI_A_cX[0], Params.Nom_cI_A_cX[1], Params.Nom_cI_A_cX[2],
                Params.Nom_cI_A_cX[3], Params.Nom_cI_A_cX[4], Params.Nom_cI_A_cX[5],
                Params.Nom_cI_A_cX[6], Params.Nom_cI_A_cX[7], Params.Nom_cI_A_cX[8]);

        double3Vec cX_R_cG_cG(Params.cX_R_cG_cG[0], Params.cX_R_cG_cG[1], Params.cX_R_cG_cG[2]);
        double33Mat cX_A_cG(Params.cX_A_cG[0], Params.cX_A_cG[1], Params.cX_A_cG[2],
                Params.cX_A_cG[3], Params.cX_A_cG[4], Params.cX_A_cG[5],
                Params.cX_A_cG[6], Params.cX_A_cG[7], Params.cX_A_cG[8]);

        double33Mat ci_A_cG =  ci_A_cX*cX_A_cG;
        double3Vec ci_R_cG_cG = cX_R_cG_cG + ci_R_cX_cX*cX_A_cG;

        i = 0;
        while(i < 3) {
            Instance.Position[i] = ci_R_cG_cG(i+1);
            i++;
        }

        i = 0;
        while(i < 9) {
            Instance.RotMatrix[i] = ci_A_cG((i%3)+1,(i/3)+1);
            i++;
        }

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
            linear_interpolate(Instance,
                               Data[CurrentIntervalIndex], Data[CurrentIntervalIndex+1],OnlyForce);
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



void TLMInterface3D::GetForce( double time,
                             double position[],
                             double orientation[],
                             double speed[],
                             double ang_speed[],
                             double* force) {
    TLMTimeData3D request;
    request.time = time - Params.Delay;
    GetTimeData(request);

    TLMPlugin::GetForce3D(position, orientation,
                        speed, ang_speed,
                        request, Params,
                        force);


}



// Set motion data and communicate if necessary.
void TLMInterface3D::SetTimeData(double time,
                               double position[],
                               double orientation[],
                               double speed[],
                               double ang_speed[]) {
    // put the variables into TLMTimeData structure and the end of  DataToSend vector
    int lastInd = DataToSend.size();
    DataToSend.resize( lastInd + 1);
    TLMTimeData3D& item = DataToSend[lastInd];
    item.time = time;
    item.Position[0] = position[0];
    item.Position[1] = position[1];
    item.Position[2] = position[2];
    for(int i = 0; i<9; i++)
        item.RotMatrix[i] = orientation[i];

    item.Velocity[0] = speed[0];
    item.Velocity[1] = speed[1];
    item.Velocity[2] = speed[2];

    item.Velocity[3] = ang_speed[0];
    item.Velocity[4] = ang_speed[1];
    item.Velocity[5] = ang_speed[2];

    // Get the data (NOTE! damped)
    TLMTimeData3D request;
    request.time = time - Params.Delay;
    GetTimeData(request);

    // Store the data if damping is used
    if((Params.alpha > 0) && (request.time !=  TLMPlugin::TIME_WITHOUT_DATA)) {
        DampedTimeData.push_back(request);
    }

    TLMPlugin::GetForce3D(position, orientation,
                        speed, ang_speed,
                        request, Params,
                        item.GenForce);

    // The wave to send is: (- Force + Impedance * Velocity)
    for(int i = 0; i < 3; i++) {
        item.GenForce[i]   = -item.GenForce[i]   +  Params.Zf * speed[i];
        item.GenForce[i+3] = -item.GenForce[i+3] +  Params.Zfr * ang_speed[i];
    }

    TLMErrorLog::Log(std::string("Interface ") + GetName() +
                     " SET for time= " + TLMErrorLog::ToStdStr(time)
                     //  		     + " force:"
                     //  		     + TLMErrorLog::ToStdStr(item.GenForce[0])+ ", "
                     //  		     + TLMErrorLog::ToStdStr(item.GenForce[1])+ ", "
                     //  		     + TLMErrorLog::ToStdStr(item.GenForce[2])+ ", "
                     //  		     + " position:"
                     //  		     + TLMErrorLog::ToStdStr(item.Position[0])+ ", "
                     //  		     + TLMErrorLog::ToStdStr(item.Position[1])+ ", "
                     //  		     + TLMErrorLog::ToStdStr(item.Position[2])+ ", "
                     // 		     + "torque: "
                     // 		     + TLMErrorLog::ToStdStr(item.GenForce[3])+ ", "
                     // 		     + TLMErrorLog::ToStdStr(item.GenForce[4])+ ", "
                     // 		     + TLMErrorLog::ToStdStr(item.GenForce[5]));
                     );

    // Send the data if we past the synchronization point or are in data request mode.
    if(time >= LastSendTime + Params.Delay / 2 || Params.mode > 0.0 ) {
        SendAllData();
    }

    // Remove the data that is not needed (Simulation time moved forward)
    // We leave two time points intact, so that interpolation work
    clean_time_queue(TimeData, time - Params.Delay);
    clean_time_queue(DampedTimeData,  time - Params.Delay * ( 1 + TLM_DAMP_DELAY));
}


void TLMInterface3D::TransformTimeDataToCG(std::vector<TLMTimeData3D>& timeData, TLMConnectionParams& params)
{

    std::vector<TLMTimeData3D>::iterator iter;
    for( iter=timeData.begin() ; iter!=timeData.end() ; iter++ ){
        TLMTimeData3D& data = *iter;

        double3Vec ci_R_cX_cX(data.Position[0], data.Position[1], data.Position[2]);
        double33Mat ci_A_cX(data.RotMatrix[0], data.RotMatrix[1], data.RotMatrix[2],
                data.RotMatrix[3], data.RotMatrix[4], data.RotMatrix[5],
                data.RotMatrix[6], data.RotMatrix[7], data.RotMatrix[8]);

        double3Vec cX_R_cG_cG(params.cX_R_cG_cG[0], params.cX_R_cG_cG[1], params.cX_R_cG_cG[2]);
        double33Mat cX_A_cG(params.cX_A_cG[0], params.cX_A_cG[1], params.cX_A_cG[2],
                params.cX_A_cG[3], params.cX_A_cG[4], params.cX_A_cG[5],
                params.cX_A_cG[6], params.cX_A_cG[7], params.cX_A_cG[8]);

        double33Mat ci_A_cG =  ci_A_cX*cX_A_cG;
        double3Vec ci_R_cG_cG = cX_R_cG_cG + ci_R_cX_cX*cX_A_cG;

        // Transform force and moment
        double3Vec F_cG(data.GenForce[0], data.GenForce[1], data.GenForce[2]);
        double3Vec M_cG(data.GenForce[3], data.GenForce[4], data.GenForce[5]);
        F_cG = F_cG*cX_A_cG;
        M_cG = M_cG*cX_A_cG;

        // Transform velocity and angular-velocity
        double3Vec vR_cG(data.Velocity[0], data.Velocity[1], data.Velocity[2]);
        double3Vec Omega_cG(data.Velocity[3], data.Velocity[4], data.Velocity[5]);
        vR_cG = vR_cG*cX_A_cG;
        Omega_cG = Omega_cG*cX_A_cG;

        // Store data
        memcpy(data.Position, &ci_R_cG_cG(1), 3*sizeof(double));
        memcpy(data.RotMatrix, &ci_A_cG(1,1), 9*sizeof(double));

        memcpy(&data.GenForce[0], &F_cG(1), 3*sizeof(double));
        memcpy(&data.GenForce[3], &M_cG(1), 3*sizeof(double));

        memcpy(&data.Velocity[0], &vR_cG(1), 3*sizeof(double));
        memcpy(&data.Velocity[3], &Omega_cG(1), 3*sizeof(double));
    }
}


void TLMInterface3D::SendAllData() {
    LastSendTime = DataToSend.back().time;

    TLMErrorLog::Log(std::string("Interface ") + GetName() + " sends data for time= " +
                     TLMErrorLog::ToStdStr(LastSendTime));

    // Transform to global inertial system cG ans send
    TransformTimeDataToCG(DataToSend, Params);

    Comm.PackTimeDataMessage3D(InterfaceID, DataToSend, Message);
    TLMCommUtil::SendMessage(Message);
    DataToSend.resize(0);

    // In data request mode we shutdown after sending the first data package.
    if( Params.mode > 0.0 ) waitForShutdownFlg = true;
}


// linear_interpolate is called with a vector containing 2 points
// computes the interpolation (or extrapolation) point with the the linear
// interpolation (extrapolation) The points are submitted using the p0 & p1
//  The desired time is given by the Instance.time. Results are stored in Instance
void TLMInterface3D::linear_interpolate(TLMTimeData3D& Instance, TLMTimeData3D& p0, TLMTimeData3D& p1, bool OnlyForce) {

    double time = Instance.time; // needed time point
    // two time points
    const double t0 = p0.time;
    const double t1 = p1.time;

    int   j = 6;

    while(j-- > 0) { // interpolate force "wave"
        Instance.GenForce[j] =
                TLMInterface::linear_interpolate(time, t0, t1, p0.GenForce[j], p1.GenForce[j]);
    }

    if(OnlyForce) return;

    // The rest is optional

    j = 3;
    while(j-- > 0) { // interpolate position
        Instance.Position[j] =
                TLMInterface::linear_interpolate(time, t0, t1, p0.Position[j], p1.Position[j]);
    }

    j = 6;
    while(j-- > 0) { // interpolate velocity
        Instance.Velocity[j] =
                TLMInterface::linear_interpolate(time, t0, t1, p0.Velocity[j], p1.Velocity[j]);
    }

    // interpolation of angles require special treatment.
    // We start by introducing relative angles between
    // the points relative the first one. That angles are then interpolated
    // and finally the interpolated rotation matrix is constructed.
    // lightmat structures & functions  are used

    // first convert the matrices into double33 format

    double* a = p0.RotMatrix;
    double33Mat A0(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8]);
    a = p1.RotMatrix;
    double33Mat A1(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8]);

    // construct relative rotation matrix, hopefully representing
    // small angles, convert to angles:
    A1 = A0.T() * A1;
    double3Vec phi = ATophi321(A1);

    j = 4;
    while(--j > 0) {
        phi(j) = TLMInterface::linear_interpolate(time, t0, t1, 0.0, phi(j));
    }
    // now get the matrix (into A[0]):
    A0 *= A321(phi);

    // copy into array
    a = Instance.RotMatrix;
    A0.Get(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8]);

}


// hermite_interpolate is called with a vector containing 4 points
// computes the interpolation point with the the polynomial that
// interpolates point 2 and 3 and have the derivative in these points
// equal to the center difference approximation at these points.
// The points are submitted using the iterator 'it' giving the
// first point in the sequence. The desired time is given
// by the Instance.time. Results are stored in Instance
void TLMInterface3D::hermite_interpolate(TLMTimeData3D& Instance, std::deque<TLMTimeData3D>::iterator& it, bool OnlyForce) {
    TLMTimeData3D* p[4]; // pointers to the four data points, get them from iterators
    p[0] = &(*it);
    ++it;
    p[1] = &(*it);
    ++it;
    p[2] = &(*it);
    ++it;
    p[3] = &(*it);

    double time = Instance.time; // needed time point
    double t[4]; // buffer for the four time points
    double f[4]; // buffer for the four values

    int i = 4;
    while(i-- > 0) t[i] = p[i]->time; // get the times

    int
            j = 6;
    while(j-- > 0) { // interpolate force "wave"
        i = 4;
        while(i-- > 0) {
            f[i] = p[i]->GenForce[j];
        }
        Instance.GenForce[j] = TLMInterface::hermite_interpolate(time, t, f);
    }

    if(OnlyForce) return;

    // The rest is optional

    j = 3;
    while(j-- > 0) { // interpolate position
        i = 4;
        while(i-- > 0) {
            f[i] = p[i]->Position[j];
        }
        Instance.Position[j] = TLMInterface::hermite_interpolate(time, t, f);
    }

    // interpolation of angles require special treatment.
    // We start by introducing relative angles between
    // the points relative the first one. That angles are then interpolated
    // and finally the interpolated rotation matrix is constructed.
    // lightmat structures & functions  are used
    double3Vec phi[4];

    double33Mat A[4];  // first convert the matrices into double33 format
    i = 4;
    while(i-- > 0) {
        double* a = p[i]->RotMatrix;
        A[i].Set(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8]);
    }
    // construct relative rotation matrices, hopefully representing
    //  small angles, convert to angles:
    phi[0] = 0.0;
    A[1] = A[0].T() * A[1];
    phi[1] = ATophi321(A[1]);
    A[2] = A[0].T() * A[2];
    phi[2] = ATophi321(A[2]);
    A[3] = A[0].T() * A[3];
    phi[3] = ATophi321(A[3]);

    // interpolate angles
    double3Vec phi_out;
    j = 4;
    while(--j > 0) {
        i = 4;
        while(i-- > 0) {
            f[i] = phi[i](j);
        }
        phi_out(j) = TLMInterface::hermite_interpolate(time, t, f);
    }
    // now get the matrix (into A[0]):
    A[0] *= A321(phi_out);

    // copy into array
    double* a = Instance.RotMatrix;
    A[0].Get(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8]);

}


void TLMInterface3D::clean_time_queue(std::deque<TLMTimeData3D>& Data, double CleanTime) {
    while( (Data.size() > 3) && (CleanTime > Data[2].time)) {
        Data.pop_front();
    }
}
