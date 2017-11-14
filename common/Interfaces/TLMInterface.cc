/**
 * File: TLMInterface.cc
 *
 * Provides implementation for the TLMInterface class methods
 */

#include "Interfaces/TLMInterface.h"
#include "Communication/TLMCommUtil.h"
#include "Plugin/TLMPlugin.h"
#include <deque>
#include <string>
#include "double33.h"

using std::deque;
using std::vector;

#include <iostream>
using std::cout;

#include <fstream>
using std::ofstream;


// TLMInterface constructor
TLMInterface::TLMInterface(TLMClientComm& theComm, std::string& aName, double StartTime,
                           int dimensions, std::string causality, std::string domain):
    LastSendTime(StartTime),
    NextRecvTime(0.0),
    Message(),
    Params(),
    CurrentIntervalIndex(0),
    Name(aName),
    Comm(theComm),
    InterfaceID(-1),
    waitForShutdownFlg(false),
    Dimensions(dimensions),
    Causality(causality),
    Domain(domain) {
    Comm.CreateInterfaceRegMessage(aName, Dimensions, Causality, Domain, Message);
    Message.SocketHandle = Comm.GetSocketHandle();

    TLMCommUtil::SendMessage(Message);

    TLMCommUtil::ReceiveMessage(Message);
    while(Message.Header.MessageType != TLMMessageTypeConst::TLM_REG_INTERFACE) {
        TLMCommUtil::ReceiveMessage(Message);
    }
    InterfaceID =  Message.Header.TLMInterfaceID;

    TLMErrorLog::Log(std::string("Interface ") + GetName() + " got ID " + TLMErrorLog::ToStdStr(InterfaceID));

    Comm.UnpackRegInterfaceMessage(Message, Params);

    NextRecvTime = StartTime + Params.Delay;
}


TLMInterface::~TLMInterface() {}


// Hermite cubic interpolation. For the given 4 data points t[i], f[i] and time,
// such that t[0]<t[1]<time<t[2]<t[3], returns f(time). .
double TLMInterface::InterpolateHermite(double time, double t[4], double f[4]) {
    double fa = f[1];
    double fb = f[2];
    double  ta = t[1];
    double  tb = t[2];
    double  bma = tb-ta;
    double  xa = (time-ta)/bma;
    double  bx = (tb-time)/bma;

    double fout; // Interpolated point

    double fpa = (f[2]-f[0]) / (t[2] - t[0]);
    double fpb = (f[3]-f[1]) / (t[3] - t[1]);

    fout = (1 + 2*xa)*bx*bx*fa +
            (1 + 2*bx)*xa*xa*fb +
            xa*bx*(tb-time)*fpa -
            xa*bx*(time-ta)*fpb;

    return fout;

}










