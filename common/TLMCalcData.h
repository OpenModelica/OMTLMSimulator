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
        //Nom_cI_R_cX_cX,
        //Nom_cI_A_cX,
        mode(0.0)
    {
        for(int i=0 ; i<3 ; i++){
            cX_R_cG_cG[i] = 0.0;
            Nom_cI_R_cX_cX[i] = cX_R_cG_cG[i];
        }

        for(int i=0 ; i<9 ; i++){
            cX_A_cG[i] = (i%4 == 0 ? 1.0 : 0.0);
            Nom_cI_A_cX[i] = cX_A_cG[i];
        }
    }

    //! time delay in the line
    double Delay;
    //! impedance
    double Zf;
    //! impedance for rotation
    double Zfr;
    //! damping
    double alpha;

    //! Position cX_R_cG_cG of the component in the common coordinate system (typically global) cG.
    //! Notation is:
    //! cX - Xmodel inertial coordinate system
    //! R  - Position vector
    //! cG - Global meta-model inertial coordinate system
    //! cX_R_cG_cG - cX position relative cG, expressed in cG.
    double cX_R_cG_cG[3];

    //! Rotation matrix cX_A_cG stored row-wise.
    //! Notation is:
    //! cX - Xmodel inertial coordinate system
    //! A  - Cartesian orientation matrix (x,y,z)
    //! cG - Global meta-model inertial coordinate system
    //! cX_A_cG - cX orientation relative cG.
    double cX_A_cG[9];

    //! Nominal start position of interface point in component (XModel) system.
    //! Notation is:
    //! cI - Interface point coordinate system
    //! R  - Position vector
    //! cX - XModel inertial coordinate system
    //! cI_R_cX_cX - cI position relative cX, expressed in cX.
    double Nom_cI_R_cX_cX[3];

    //! Nominal start rotation matrix cI_A_cX stored row-wise.
    //! Notation is:
    //! cI - Interface point inertial coordinate system
    //! A  - Cartesian orientation matrix (x,y,z)
    //! cX - Xmodel inertial coordinate system
    //! cI_A_cX - cI orientation relative cX.
    double Nom_cI_A_cX[9];

    //! Connection mode, currently:
    //! 0.0 = Real simulation
    //! 1.0 = Interface data request
    double mode;
};

//! Time stamped 3D data that is send over between connected TLM interfaces.
//! Note that the strucutre MUST:
//! - contain only "double" number that are transmitted (important for byte swapping)
//! - have continious storage
class TLMTimeData3D {
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

    TLMTimeData3D()
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

    TLMTimeData3D& operator=(const TLMTimeData3D& td){
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



//! Time stamped 1D data that is send over between connected TLM interfaces.
//! Note that the strucutre MUST:
//! - contain only "double" number that are transmitted (important for byte swapping)
//! - have continious storage
class TLMTimeData1D {
public:
    //! The time instance
    double time;

    //! Position (or angle)
    double Position;

    //! Velocity (or angular velocity)
    double Velocity;

    //! Force (or torque)
    double GenForce;

    TLMTimeData1D()
        : time(0.0)
        // Position
        // RotMatrix
        // Velocity
        // GenForce
    {
        Position = 0.0;

        Velocity = 0.0;

        GenForce = 0.0;
    }

    TLMTimeData1D& operator=(const TLMTimeData1D& td){
        if( &td != this ){
            time = td.time;

            Position = td.Position;
            Velocity = td.Velocity;
            GenForce = td.GenForce;
        }
        return *this;
    }
};


//! Time stamped signal data that is send over between connected TLM interfaces.
//! Note that the strucutre MUST:
//! - contain only "double" number that are transmitted (important for byte swapping)
//! - have continious storage
class TLMTimeDataSignal {
public:
    //! The time instance
    double time;

    //! Position (or angle)
    double Value;

    TLMTimeDataSignal()
        : time(0.0)
    {
        Value = 0.0;
    }

    TLMTimeDataSignal& operator=(const TLMTimeDataSignal& td){
        if( &td != this ){
            time = td.time;

            Value = td.Value;
        }
        return *this;
    }
};

#endif
