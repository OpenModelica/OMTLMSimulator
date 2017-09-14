// C++ includes
#include <iostream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <map>

#include "gain.h"


using namespace std;


static double reltol = 1e-4;    //Relative tolerance
static double tstart = 0;       //Start time
static double hmax = 1.5e-4;      //Max step-size
static double hcur = hmax;
static double tsolver;          //Solver inernal time
static double tcur;             //Current time
static size_t n_states = 4;     //Number of states
static void *mem;               //Solver memory pointer
static int flag;                //Solver return flag

static double y,u;

void setReal(int idx, double value) {
    switch(idx) {
    case 0:
        y = value;
        break;
    case 1:
        u = value;
        break;
    }
}


double getReal(int idx) {
    switch(idx) {
    case 0:
        return y;
    case 1:
        return u;
    default:
        return 0;
    }
}


int takeStep(double t1) {
    u = y*0.01;

    tcur = t1;

    return 0;
}

