#pragma once
class ProteinData
{
public:

struct Atom
{
    unsigned int serialNumber;
    char name[5];
    char alternateLocationIndicator;
    char residueName[4];
    char chainID;
    int residueSeqNumber;
    char codeForInsertion;
    float xCoord;
    float yCoord;
    float zCoord;
    float occupancy;
    float temperatureFactor;
    char segmentID[5];
    char elementSymbol[3];
};

};

