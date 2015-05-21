#include "boolPack.h"

const int boolPack::nrUnsignhedChar(255);

void boolPack::boolean_vector_pack(vector <bool> booleanToPack,unsigned char *packedBoolean)
{
    unsigned int charPos = 0;
    unsigned int lengthToPack = booleanToPack.size();

    //if the length of the boolean to be packed is longer than 255 (char limit) save 255
    //and shift to next pos until there are less than 255
    unsigned int newLen = lengthToPack;


    while(newLen>nrUnsignhedChar)
    {
        packedBoolean[charPos] = nrUnsignhedChar;
        newLen = newLen-nrUnsignhedChar;
        charPos++;

    }

    //save the rest of the boolean length
    packedBoolean[charPos] = newLen;
    charPos++;

    //Pack boolean to char*
    for (unsigned int i=0; i < lengthToPack; ++i){

        if (booleanToPack.at(i)){
            packedBoolean[charPos] = 1;
        }else{
            packedBoolean[charPos] = 0;
        }

        //increase pos in char*;
        charPos++;
    }

}

vector<bool> boolPack::boolean_vector_unpack( unsigned char *packedBoolean)
{

    vector<bool> unpackedBoolean;

    //Extract size of boolean from packed boolean.
    int charPos = 0;
    int boolLength = 0;

    while(packedBoolean[charPos]==nrUnsignhedChar)
    {
        boolLength =+nrUnsignhedChar;
        charPos++;
    }

    boolLength = packedBoolean[charPos]+boolLength;
    charPos++;

    unpackedBoolean.insert(unpackedBoolean.begin(),packedBoolean+charPos,packedBoolean+charPos+boolLength);

    return unpackedBoolean;

}

//! This function packs an array of booleans to a char*
void boolPack::boolean_pack(bool *booleanToPack,unsigned int lengthToPack,unsigned char *packedBoolean)
{

    unsigned int charPos = 0;


    //if the length of the boolean to be packed is longer than 255 (char limit) save 255
    //and shift to next pos until there are less than 255
    unsigned int newLen = lengthToPack;

    while(newLen>nrUnsignhedChar)
    {
        packedBoolean[charPos] = nrUnsignhedChar;
        newLen = newLen-nrUnsignhedChar;
        charPos++;

    }

    //save the rest of the boolean length
    packedBoolean[charPos] = newLen;
    charPos++;

    //Pack boolean to char*
    for (unsigned int i=0; i < lengthToPack; ++i){

        if (booleanToPack[i]){
            packedBoolean[charPos] = 1;
        }else{
            packedBoolean[charPos] = 0;
        }

        //increase pos in char*;
        charPos++;
    }

}

//! This function unpacks a char* to an array of booleans
void boolPack::boolean_unpack( unsigned char *packedBoolean, bool *unpackedBoolean)
{

    //Extract size of boolean from packed boolean.
    int charPos = 0;
    int boolLength = 0;

    while(packedBoolean[charPos]==nrUnsignhedChar)
    {
        boolLength =+nrUnsignhedChar;
        charPos++;
    }

    boolLength = packedBoolean[charPos]+boolLength;
    charPos++;

    for (int i=0; i < boolLength; ++i){
        unpackedBoolean[i] = packedBoolean[charPos];
        charPos++;
    }

}

//! This function returns the memory needed to pack an array/vector of booleans
int boolPack::getMemoNeededForPack(unsigned int boolLength){

    //numbers of booleans needed to pack
    unsigned int lenIn = ceil((double)boolLength/(double)nrUnsignhedChar);

    return boolLength+lenIn;
}


//! This function resturns the memory needed to unpack an array of booleans
int boolPack::getMemoNeededForUnpack(unsigned char* packedBoolean){
    //numbers of packed booleans

    int charPos = 0;
    int boolLength = 0;

    while(packedBoolean[charPos]==nrUnsignhedChar)
    {
        boolLength =+nrUnsignhedChar;
        charPos++;
    }

    boolLength = packedBoolean[charPos]+boolLength;

    return boolLength;
}


//! function to test a array of booleans
void boolPack::testBoolPackAndUnpack(bool *booleanToPack){

    unsigned int boolLen=sizeof(booleanToPack);
    bool unPackedBool[boolLen];

    unsigned char *cPtr = new unsigned char[getMemoNeededForPack(boolLen)];
    boolean_pack(booleanToPack,boolLen,cPtr);

    bool *bool2Ptr=new bool[getMemoNeededForUnpack(cPtr)];
    boolean_unpack(cPtr, bool2Ptr);

    for(unsigned int i = 0; i<boolLen;i++)
    {
        unPackedBool[i]=bool2Ptr[i];
    }

    if (equal(booleanToPack, booleanToPack + sizeof booleanToPack / sizeof *booleanToPack, unPackedBool))
        cout<<"bool pack and unpack test success"<<endl;
    else
        cout <<"bool pack and unpack test  failed"<<endl;

}

//! Function to test pack and unpack of a vector of booleans
void boolPack::testVectorBoolPackAndUnpack(vector<bool> vBooleanToPack){

    vector<bool> unPackedBool1;

    unsigned char *charPtr = new unsigned char[getMemoNeededForPack( (unsigned int)vBooleanToPack.size())];

    boolean_vector_pack(vBooleanToPack,charPtr);
    unPackedBool1 = boolean_vector_unpack(charPtr);

    if(vBooleanToPack == unPackedBool1){
        cout<<"vector bool pack and unpack test  success"<<endl;
    }else{
        cout <<"vector bool pack and unpack test  failed"<<endl;
    }

}
