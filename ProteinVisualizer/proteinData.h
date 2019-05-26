#pragma once

#include <vector>

typedef unsigned int uint;

struct Atom
{
    uint serialNumber;
    char name[5];
    char altLocationIndicator;
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

struct Helix
{
    uint serialNumber;
    char ID[4];
    char initialResidueName[4];
    char chainID;
    int residueSeqNumber;
    char codeForInsertion;
    char terminalResidueName[4];
    char chainID2;
    int residueSeqNumber2;
    char codeForInsertion2;
    int type;
    char comment[32];
    int length;
};

struct Sheet
{

};

using Chain = std::vector<Atom>;
using Model = std::vector<Chain>;

struct ProteinData
{
    std::vector<Model> models;
    std::vector<Helix> alphaHelicis;
    std::vector<Sheet> betaSheets;
};
