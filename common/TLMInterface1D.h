#ifndef TLMINTERFACE1D_H
#define TLMINTERFACE1D_H

#include "TLMInterface.h"

class TLMTimeData1D;

class TLMInterface1D : public TLMInterface
{
public:
  TLMInterface1D(TLMClientComm &theComm, std::string &aName, double StartTime, InterfaceDomain Domain=DomainMechanical);

  //! Destructor. Sends the rest of the data if necessary.
  ~TLMInterface1D();

  //!  TimeData is the deque of data received from the coupled simulation.
  //!  The data is "pushed back" when received and "poped front" when the
  //!  time goes forward more than  TLM delay and old data is not needed any longer.
  std::deque<TLMTimeData1D> TimeData;

  //!  DampedTimeData is the deque of data computed using damping coefficient alfa
  //!  from TimeData.
  //!  The data is "pushed back" when computed and "poped front" when the
  //!  time goes forward more than TLM delay and old data is not needed any longer.
  std::deque<TLMTimeData1D> DampedTimeData;

  //!  DataToSend stores the motion data from the interface. The data is sent
  //! in packet for a time period of [half] TLM delay [depends on solver type]
  std::vector<TLMTimeData1D> DataToSend;
  void UnpackTimeData(TLMMessage &mess);

  void GetTimeData(TLMTimeData1D &Instance);
  void GetTimeData(TLMTimeData1D &Instance, std::deque<TLMTimeData1D> &Data, bool OnlyForce);
  void GetForce(double time, double position, double speed, double *force);
  void SetTimeData(double time, double position, double speed);
  void SendAllData();

  //! linear_interpolate is called with a vector containing 2 points
  //! computes the interpolation (or extrapolation) point with the the linear
  //! interpolation (extrapolation) The points are submitted using the p0 & p1
  //!  The desired time is given by the Instance.time. Results are stored in Instance.
  //! If OnleForce is set, then the position and velocity are not computed.
  static void linear_interpolate(TLMTimeData1D& Instance, TLMTimeData1D& p0, TLMTimeData1D& p1, bool OnlyForce);


  //! hermite_interpolate is called with a vector containing 4 points
  //! computes the interpolation point with the the polynomial that
  //! interpolates point 2 and 3 and have the derivative in these points
  //! equal to the center difference approximation at these points.
  //! The points are submitted using the iterator 'it' giving the
  //! first point in the sequence. The desired time is given
  //! by the Instance.time. Results are stored in Instance.
  //! If OnleForce is set, then the position and velocity are not computed.
  static void hermite_interpolate(TLMTimeData1D& Instance, std::deque<TLMTimeData1D>::iterator& it, bool OnlyForce);

  // Remove the data that is not needed (Simulation time moved forward)
  // We leave two time points intact, so that interpolation work
  static void clean_time_queue(std::deque<TLMTimeData1D> &Data, double CleanTime);
};

#endif // TLMINTERFACE1D_H
