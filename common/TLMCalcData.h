//!
//! \file TLMCalcData.h
//! 
//! TLM interface parameters & time data. 
//! This structures are needed for distributed apps working with
//!  TLM plugin.
//!
//!
//! \author   Iakov Nakhimovski
//!
#ifndef TLMCalcData_h_
#define TLMCalcData_h_

//! TLMConnectionParams structure encapsulates the parameters of a TLM connection
//! The data is directly transferred to a message therefore only 'double' fields
//! with continious storage are allowed.
class TLMConnectionParams {
public:
    //! Constructor initialized everything to 0.0.
    TLMConnectionParams() : 
        Delay(0.0), 
        Zf(0.0), 
        Zfr(0.0), 
        alpha(0.0), 
        //Position,
        //RotMatrix,
        mode(0.0) 
    {
        Position[0] = 0.0;
        Position[1] = 0.0;
        Position[2] = 0.0;

        RotMatrix[0] = 1.0;
        RotMatrix[1] = 0.0;
        RotMatrix[2] = 0.0;

        RotMatrix[3] = 0.0;
        RotMatrix[4] = 1.0;
        RotMatrix[5] = 0.0;

        RotMatrix[6] = 0.0;
        RotMatrix[7] = 0.0;
        RotMatrix[8] = 1.0;
    }

    //! time delay in the line
    double Delay; 
    //! impedance
    double Zf;   
    //! impedance for rotation
    double Zfr;   
    //! damping 
    double alpha;

    //! Position cX_R_cG_cG of the component in a common coordinate system (typically global) cG.
    double Position[3];

    //! Rotation matrix cX_A_cG stored row-wise. 
    double RotMatrix[9];

    //! Connection mode, currently: 
    //! 0.0 = Real simulation
    //! 1.0 = Interface data request
    double mode;
};

//! Time stamped data that is send over between connected TLM interfaces.
//! Note that the strucutre MUST:
//! - contain only "double" number that are transmitted (important for byte swapping)
//! - have continious storage
class TLMTimeData {
public:
    //! The time instance
    double time;

    //! Position in a common coordinate system (typically global) cG
    double Position[3];

    //! Rotation matrix cG_A_ci stored row-wise. For a given point in the
    //! TLM interface coordinate ci system p_ci, the global position is given
    //! by: Position + cG_A_ci * p_ci
    double RotMatrix[9];

    //! Velocity + Angular velocity of the control-point.
    double Velocity[6];

    //! Force and moment "waves" acting and expressed in this TLM interface
    double GenForce[6];

    TLMTimeData() 
        : time(0.0)
        // Position
        // RotMatrix
        // Velocity
        // GenForce
    {
        Position[0] = 0.0; Position[1] = 0.0; Position[2] = 0.0;

        RotMatrix[0] = 1.0; RotMatrix[1] = 0.0; RotMatrix[2] = 0.0;
        RotMatrix[3] = 0.0; RotMatrix[4] = 1.0; RotMatrix[5] = 0.0;
        RotMatrix[6] = 0.0; RotMatrix[7] = 0.0; RotMatrix[8] = 1.0;

        Velocity[0] = 0.0; Velocity[1] = 0.0; Velocity[2] = 0.0;
        Velocity[3] = 0.0; Velocity[4] = 0.0; Velocity[5] = 0.0;

        GenForce[0] = 0.0; GenForce[1] = 0.0; GenForce[2] = 0.0;
        GenForce[3] = 0.0; GenForce[4] = 0.0; GenForce[5] = 0.0;
    }

    TLMTimeData& operator=(const TLMTimeData& td){
        if( &td != this ){
            time = td.time;

            int i=0;
            for( i=0; i<3 ; i++) Position[i] = td.Position[i];
            for( i=0; i<9 ; i++) RotMatrix[i] = td.RotMatrix[i];
            for( i=0; i<6 ; i++) Velocity[i] = td.Velocity[i];
            for( i=0; i<6 ; i++) GenForce[i] = td.GenForce[i];
        }
        return *this;
    }

};


#endif
