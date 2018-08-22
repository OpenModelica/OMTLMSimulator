//!
//! \file TLMInterface3D.h
//!
//! Provides a definition for the TLMInterface3D class
//!
//!
//! \author   Robert Braun
//!

#ifndef TLMINTERFACE3D_H
#define TLMINTERFACE3D_H

#include "Interfaces/TLMInterface.h"

class TLMTimeData3D;

//!
//! TLMInterface1D provides the client side functionality for a single TLM interface of three dimensions
//!
class TLMInterface3D : public omtlm_TLMInterface {
public:
    TLMInterface3D(TLMClientComm& theComm, std::string& aName, double StartTime, std::string Domain="Mechanical");

    //! Destructor. Sends the rest of the data if necessary.
    ~TLMInterface3D();

    //!  TimeData is the deque of data received from the coupled simulation.
    //!  The data is "pushed back" when received and "poped front" when the
    //!  time goes forward more than  TLM delay and old data is not needed any longer.
    std::deque<TLMTimeData3D> TimeData;

    //!  DampedTimeData is the deque of data computed using damping coefficient alfa
    //!  from TimeData.
    //!  The data is "pushed back" when computed and "poped front" when the
    //!  time goes forward more than TLM delay and old data is not needed any longer.
    std::deque<TLMTimeData3D> DampedTimeData;

    //!  DataToSend stores the motion data from the interface. The data is sent
    //! in packet for a time period of [half] TLM delay [depends on solver type]
    std::vector<TLMTimeData3D> DataToSend;

    double InitialForce[6] = {0,0,0,0,0,0};
    double InitialFlow[6]  = {0,0,0,0,0,0};

    //! Evaluate the data from deque for the time specified by this Instance
    //! If OnleForce is set, then the position and velocity are not computed.
    void GetTimeData(TLMTimeData3D& Instance, std::deque<TLMTimeData3D>&  Data, bool OnlyForce);

    void GetTimeData(TLMTimeData3D &Instance);

    void GetForce(double time, double position[], double orientation[], double speed[], double ang_speed[], double *force);
    void GetWave(double time, double *wave);
    void SetTimeData(double time, double position[], double orientation[], double speed[], double ang_speed[]);
    void TransformTimeDataToCG(std::vector<TLMTimeData3D> &timeData, TLMConnectionParams &params);
    void SendAllData();
    void SetInitialForce(double f1, double f2, double f3, double t1, double t2, double t3);
    void SetInitialFlow(double v1, double v2, double v3, double w1, double w2, double w3);

    //! linear_interpolate is called with a vector containing 2 points
    //! computes the interpolation (or extrapolation) point with the the linear
    //! interpolation (extrapolation) The points are submitted using the p0 & p1
    //!  The desired time is given by the Instance.time. Results are stored in Instance.
    //! If OnleForce is set, then the position and velocity are not computed.
    static void InterpolateLinear(TLMTimeData3D& Instance, TLMTimeData3D& p0, TLMTimeData3D& p1, bool OnlyForce);


    //! hermite_interpolate is called with a vector containing 4 points
    //! computes the interpolation point with the the polynomial that
    //! interpolates point 2 and 3 and have the derivative in these points
    //! equal to the center difference approximation at these points.
    //! The points are submitted using the iterator 'it' giving the
    //! first point in the sequence. The desired time is given
    //! by the Instance.time. Results are stored in Instance.
    //! If OnleForce is set, then the position and velocity are not computed.
    static void InterpolateHermite(TLMTimeData3D& Instance, std::deque<TLMTimeData3D>::iterator& it, bool OnlyForce);
    void UnpackTimeData(TLMMessage &mess);


    // Remove the data that is not needed (Simulation time moved forward)
    // We leave two time points intact, so that interpolation work
    static void CleanTimeQueue(std::deque<TLMTimeData3D> &Data, double CleanTime);
};

#endif // TLMINTERFACE3D_H
