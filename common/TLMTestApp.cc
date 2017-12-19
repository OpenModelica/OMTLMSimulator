//\begin{verbatim}
// This is a test TLM application. A dynamic simulation with variable time-step is emulated.
// We solve dx/dt = v; dv/dt = - |k1| x + k2 sin(t) + TLM-force.
// The ODE is solved with backward Euler method:
// the time step dt = (MAX_STEP + Rand(1..MAX_STEP))/2
// x(t+dt) = x(t) + v(t+dt) * dt
// v(t+dt) = v(t) + dt * (-|k1| x(t+dt) + k2 sin(t+dt) + TLM-force(t+dt))

// v(t+dt) = (v(t) + dt * (-|k1| x(t) + k2 sin(t+dt) + TLM-force(t+dt))) / (1 + |k1| dt^2)

// The program uses TLM plug-in as any other tool should.

#include "Plugin/TLMPlugin.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argn, char* argv[]) {

    double MaxStep = 0;
    double Time = 0;
    double TimeEnd = 0;
    char* ServerName;
    char* Model;

    if(argn != 8) {
        cout << "TLM test application. Usage: " << endl
             << "tlmtest  <k1> <k2> <Model> <FromTime> <ToTime> <Step> <Server:port>" << endl;
        exit(1);
    }
    // Coefficients used in the ODE
    const double k1 = atof(argv[1]);
    const double k2 = atof(argv[2]);

    Model = argv[3];
    Time = atof(argv[4]);
    TimeEnd = atof(argv[5]);
    MaxStep = atof(argv[6]);
    ServerName = argv[7];

    TLMPlugin* TlmForce  = TLMPlugin::CreateInstance();

    if(!TlmForce->Init(Model, Time, TimeEnd, MaxStep, ServerName)) {
        cerr << "Failed to init TLM plugin" << endl;
        exit(1);
    }

    // This is the only TLM interface of this model: px
    int forceID = TlmForce->RegisteTLMInterface("px");

    if(forceID < 0) {
        cerr << "Failed to register TLM interface" << endl;
        exit(1);
    }

    // Initialize the random number generator
    srand((int)Model[0]);

    // Initial position and velocity are set to 0
    double x = 0;
    double v = 0;
    // Time step will vary dinamically
    double dt;
    // Variable send over TLM interface: force/torque + position/velocity
    double force[6] = {0,0,0,0,0,0};
    double position[3] = {0,0,0};
    double speed[3] = {0,0,0};
    double orientation[9] = {1,0,0,0,1,0,0,0,1};
    double ang_speed[3] = {0,0,0};

    string OutFN = string(Model) + ".out";
    std::ofstream outstr(OutFN.c_str(), std::ios::out);

    // do simulation
    while(Time < TimeEnd) {

        // Current position needed to request info from TLM interface
        position[0] = x;
        speed[0] = v;

        // Time step is random between 0.5 MaxStep and MaxStep
        dt = (MaxStep + rand() * MaxStep / RAND_MAX) / 2;

        // Get force & moment from TLM connection
        TlmForce->GetForce3D(forceID, Time+dt,  position, orientation, speed, ang_speed, force);

        cout<< "Mdl: " << Model << " t=" << Time+dt << " got f= " << force[0] << "; v= " << v << endl;

        // Calculate derivatives according to the implicit Euler method
        double v_next = (v + dt * (-k1* x + k2 * sin(Time+dt) + force[0])) / (1 + k1 * dt * dt);
        double x_next = x + v_next * dt;

        x = x_next;
        v = v_next;

        // Preparing to send to TLM
        position[0] = x;
        speed[0] = v;

        // Pass info to the tlm
        TlmForce->SetMotion3D(forceID, Time + dt, position, orientation, speed, ang_speed);

        cout<< "Model " << Model << " time: " << Time << "; x = " << x << "; v = " << v << endl;

        outstr << Time << ' ' << x << ' ' << v << ' ' << force[0] << endl;

        // do the time step
        Time += dt;
    }

    delete TlmForce;

    cerr << "Simulation done for model " << Model << endl;

    return 0;
}
// \end{verbatim}
