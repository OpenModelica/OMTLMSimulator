// File gfo_wrapper.c
// 
// GFOSUB ADAMS/Solver user subroutine interface implementation
//

// portability header from ADAMS
#include "userPortName.h"
// C-callable subroutines from ADAMS
#include "utilCcallable.h"
// Plugin wrapper header
#include "tlmforce.h"
#include <cstdio>

// String constants for c_usrmes
static char* INFO = "INFO";
static char* FAULT = "FAULT";
static char* WARN = "WARN";

// GFOSUB - is an interfacec function for general 6-component force tensor
// that is 3-component force & 3-component torque.
//  
// id -  id of this external function
// time - current time. Note that adaptive step solver is used in ADAMS
//    Unsuccessful steps are possible. TIMGET function can be used to check
//    is the last call was a final RHS call.
// par - parameters to the function - for GFOSUB expected:
//    a marker, i.e. one parameter
// nPar - number of parameters - should be 1 or 2. If second parameter exists, there will be debug output.
// dflag - true if Jacobian calculation is in progress, i.e. small change of inputs
//      For TLM purposes such calls can be made much faster since TLM force
//       is dependent only on time and not on the states.
// iflag - (init flag) true if solver is only after dependencies between vars
//      We need to make sysary calls to the motion variables that TLM is dependent on.
// result - array of 6 elements with the resulting force
VOID_FUNCTION GFOSUB(int *id, REAL *time, REAL *par, int *nPar, BOOL *dflag, BOOL *iflag, REAL *result){
    int i;             // for loop index

    int    markers[4]; // markers for DISP/VEl sysary calls
    double disp[6];    // general displacement (ADAMS Euler angles used for rotation)
    double rot[9];     // Rotation matrix 
    double vel[6];     // general velocity
    int    ns;         // used in sysary
    BOOL   errflg;
    BOOL   dbgOut;
    char   messBuf[500];
    double lastConvergedTime;

    // Units scaling. See gtunts in ADAMS manual
    BOOL existsUnits;
    double scales[4];
#define UNIT_SCALE_TIME 0
#define UNIT_SCALE_LENGTH 1
#define UNIT_SCALE_FORCE 2
#define UNIT_SCALE_MASS 3
    char units[3 * 4];
#define NPARDBG 5
    // dbgOut is set if we have 4 markers + extra parameter
    dbgOut = ((*nPar) == NPARDBG);
    if(dbgOut) {
	// Printing out GFOSUB input to ADAMS .mes & output files
	sprintf(messBuf,  "GFOSUB called for time %g", *time);
	c_usrmes(dbgOut, messBuf, *id, INFO);

	sprintf(messBuf,  "ID=%d", *id);
	c_usrmes(dbgOut, messBuf, *id, INFO);

	sprintf(messBuf,  "npar=%d: ", *nPar);
	c_usrmes(dbgOut, messBuf, *id, INFO);
    
	for(i = 0; i < *nPar; i++){
	    sprintf(messBuf,  "%g; ", par[i]);
	    c_usrmes(dbgOut, messBuf, *id, INFO);
	}
	sprintf(messBuf,  " dflag=%s iflag=%s", *dflag ? "true":"false",
		*iflag ? "true":"false");
	c_usrmes(dbgOut, messBuf, *id, INFO);
    }
  
    switch (*nPar) {
	case 2: 
	    markers[0] = (int)par[0];
	    markers[1] = (int)par[1];
	    markers[2] = (int)par[1];
	    markers[3] = (int)par[1];
	    break;
	case 3: 
	    markers[0] = (int)par[0];
	    markers[1] = (int)par[1];
	    markers[2] = (int)par[2];
	    markers[3] = (int)par[2];
	    break;
	case 4: 
        case NPARDBG:
	    markers[0] = (int)par[0];
	    markers[1] = (int)par[1];
	    markers[2] = (int)par[2];
	    markers[3] = (int)par[3];
	    break;
        default:
	    c_errmes(1, "Wrong number of parameters to the TLM force\0", *id, "STOP\0"); 	    
    }

    // c_sysary function lets a user-written subroutine read
    // information from ADAMS. Here we need it to get the displacement
    // The parameters of this function are:
    // 
    //  c_sysary(char *fncnam, int *ipar, int nsize, REAL *states, int *nstate, 
    //      BOOL *errflg);
    // Input:
    //  fncnam - name of the function, "DISP" for displacement
    //  ipar, nsize - list of markers and the number of markers
    //    DISP requires 1 to 3 markers where the distance is measured
    //    to marker 1 origin from marker 2 (reference)
    //    in coordinate system of marker 3 (basis) Setting markers
    //     2 and 3 to "0" gives global inertia system as reference.
    //     Markers 2 & 3 are optional.
    //    For our case we use the marker submitted to GFOSUB in parameters
    //    and ground. 
    // Output:
    //   states, nstate - variable values and the number of variable returned
    //   For "DISP" 6 variables giving position and orientation are returned
    //   The orientation is given by Psi, Theta, Phi (ADAMS/Solver Euler angles)
    //   c_rcnvrt(char *sys1, REAL *coord1, char *sys2, REAL *coord2, int *istat);
    //   can be used to convert those to Euler parameters by specifying
    //   sys1 = 'EULER' and sys2 = 'EULPAR' or directly to rotation matrix
    //   (columnwise) by specifying sys2 = 'COSINES'.
    //   errflg - returns TRUE on error.

    c_sysary ("DISP",markers,3,disp,&ns,&errflg);
    c_errmes(errflg, "Problem with a call to sysary for displacement\0", *id, "STOP\0"); 
    if(dbgOut) {
	sprintf(messBuf,  "Got displacements: % g %g %g",
		disp[0],disp[1],disp[2]);
	c_usrmes(dbgOut, messBuf, *id, INFO);
    }

    // Convert the angles from ADAMS "standard" Euler to rotation matrix (9-components)
    c_rcnvrt("EULER",disp+3,"COSINES",rot,&errflg);

    c_sysary ("VEL",markers,4,vel,&ns,&errflg);
    c_errmes(errflg, "Problem with a call to sysary for velocity\0", *id, "STOP\0");
    if(dbgOut) {
	sprintf(messBuf,  "Got velocity: % g %g %g", vel[0],vel[1],vel[2]);
	c_usrmes(dbgOut, messBuf, *id, INFO);
    }


    // TIMGET returns simulation time at the end of the last successful step
    c_timget(&lastConvergedTime);

    if(dbgOut) {
	sprintf(messBuf,  "Timget returned %g",lastConvergedTime);
	c_usrmes(dbgOut, messBuf, *id, INFO);
    }

    // gtunts retunes the unit scales to MKS as used in the model
    c_gtunts(&existsUnits, scales, units);
    if(dbgOut) {
	units[4*2] = 0;
	sprintf(messBuf,  "Units: %g %g %g %g %s",scales[0],scales[1],scales[2],scales[3],units);
	c_usrmes(dbgOut, messBuf, *id, INFO);
    }
    
    if(existsUnits ) {	
	// Scale transitional measures with length units + speeds with time units
	if(scales[UNIT_SCALE_LENGTH] != 1.0) {
	    disp[0] *= scales[UNIT_SCALE_LENGTH];
	    disp[1] *= scales[UNIT_SCALE_LENGTH];
	    disp[2] *= scales[UNIT_SCALE_LENGTH];

	    vel[0] *= scales[UNIT_SCALE_LENGTH];
	    vel[1] *= scales[UNIT_SCALE_LENGTH];
	    vel[2] *= scales[UNIT_SCALE_LENGTH];	    
	}
	if(scales[UNIT_SCALE_TIME] != 1.0) {
	    for( i = 0; i < 6; ++i) {
		vel[i] /= scales[UNIT_SCALE_TIME];
	    }
	}
    }
    else {
	c_usrmes(1, "No units scaling to MKS is defined, co-simulation might give wrong results, ", *id, WARN);
    }
      
    calc_tlm_force(*iflag,  // init flag. No result is needed.
		   *dflag,  // If set, it is derivatives calculation.
				                    // Time stands still.
		   dbgOut,  // should we print debug messages?
		   markers[0],   // The calling marker ID
		   *time,    // Current simulation time
		   lastConvergedTime, // Last converged time 
		   disp, // Marker position data
		   rot, // Marker rotation matrix
		   vel,      // Marker translational velocity
		   vel + 3,  // Marker angular velocity
		   result);      // Output 6-component force


    if(existsUnits ) {
	// Scale force & torque
	if(scales[UNIT_SCALE_FORCE] != 1.0) {
	    for( i = 0; i < 6; ++i) {
		result[i] /= scales[UNIT_SCALE_FORCE];
	    }
	    if(scales[UNIT_SCALE_LENGTH] != 1.0) {
		result[3] /= scales[UNIT_SCALE_LENGTH];
		result[4] /= scales[UNIT_SCALE_LENGTH];
		result[5] /= scales[UNIT_SCALE_LENGTH];
	    }
	}
    }
    if(dbgOut) {
	sprintf(messBuf,  "Returning force (x-component): %g",result[0]);
	c_usrmes(dbgOut, messBuf, *id, INFO);
    }
};
