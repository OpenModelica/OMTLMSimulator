#ifndef BOOLPACK_H
#define BOOLPACK_H
#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>

using namespace std;
//BZ1869
class boolPack{

private:
    //!size of unsigned char
    static const int nrUnsignhedChar;

public:

    //! This function packs an array of booleans to a char*
    void boolean_pack(bool *booleanToPack, unsigned int lengthToPack, unsigned char *packedBoolean);

    //! This function unpacks a char* to an array of booleans
    void boolean_unpack(unsigned char *packedBoolean, bool *unpackedBoolean);

    //! This function unpacks char* to a vector of booleans
    vector<bool> boolean_vector_unpack(unsigned char *packedBoolean);

    //! This function returns the memory needed to pack an array/vector of booleans
    int getMemoNeededForPack(unsigned int boolLength);

    //! This function resturns the memory needed to unpack an array of booleans
    int getMemoNeededForUnpack(unsigned char *packedBoolean);

    //! This function packs a vector of booleans to char*
    void boolean_vector_pack(vector <bool> booleanToPack,unsigned char *packedBoolean);

    //! Function to test pack and unpack of a vector of booleans
    void testVectorBoolPackAndUnpack(vector<bool> vBooleanToPack);

    //! Function to test a array of booleans
    void testBoolPackAndUnpack(bool *booleanToPack);

};

#endif // BOOLPACK_H
