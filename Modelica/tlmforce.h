#ifndef TLMFORCE_H
#define TLMFORCE_H
//! \file tlmforce.h
//!
//! The file provides declarations for the Modelica specific wrapper
//! function and classes to interface and TLMPlugin

#ifdef __cplusplus
extern "C" {
#endif

//! Initialize one interface
void initialize_interface(const char* markerID);

//! Enable, disable debug output.
void set_debug_mode(int debugFlgIn);

//! Get the TLM delay for the interface.
double get_tlm_delay();


//! Send/store motion in the TLM interface.
//! This function is typically called for each completed/converged/final solver step.
//! Time intervall (time) between calls to this function must be within:
//! - TLM-delay for equidistant solvers and
//! - TLM-delay/2 for variable step size solvers
//!
//! \param interfaceID The interface identifier as specified in the simulation and meta model.
//! \param time        Simulation time when the motion information applies (and should be stored).
//! \param position    The position (x,y,z) of the interface at this point in time. Three component vector/array.
//! \param orientation The orientation matrix of the interface at this point in time. Nine (3x3) component vector/array.
//! \param speed       The velocity vector (x,y,z) of the interface at this point in time. Three component vector/array.
//! \param ang_speed   The angular velocity vector (rotational speed around x,y,z) of the interface at this point in time.
//!                    Three component vector/array.
void set_tlm_motion(const char* interfaceID, // The TLM interface (frame) ID
                    double time,             // Current simulation time
                    double position[],       // Interface position data
                    double orientation[],    // Interface rotation matrix
                    double speed[],          // Interface translational velocity
                    double ang_speed[]);     // Interface angular/rotational velocity



//! Get the reaction force and moment from TLM interface
//! This function is typically called once or multiple times between two solver steps.
//! Time (time) when calling this function must be less than:
//! - lastTimeStep+TLMdelay for equidistant solvers and
//! - lastTimeStep+TLMdelay/2 for variable step size solvers
//!
//! \param interfaceID The interface identifier as specified in the simulation and meta model.
//! \param time        Simulation time when the force is to be calculated.
//! \param position    The position (x,y,z) of the interface at this point in time. Three component vector/array.
//! \param orientation The orientation matrix of the interface at this point in time. Nine (3x3) component vector/array.
//! \param speed       The velocity vector (x,y,z) of the interface at this point in time. Three component vector/array.
//! \param ang_speed   The angular velocity vector (rotational speed around x,y,z) of the interface at this point in time.
//!                    Three component vector/array.
//! \retval force      The resulting force (x,y,z) acting in the interface. Three component vector/array.
//! \retval torque     The resulting torque (x,y,z) acting in the interface. Three component vector/array.
void calc_tlm_force(const char* interfaceID, // The calling TLM interface (frame) ID
                    double time,             // Current simulation time
                    double position[],       // Interface position data
                    double orientation[],    // Interface rotation matrix
                    double speed[],          // Interface translational velocity
                    double ang_speed[],      // Interface angular/rotational velocity
                    double force[],          // Output 3-component force
                    double torque[]);        // Output 3-component torque
#ifdef __cplusplus
}
#endif


#endif
