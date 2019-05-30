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
    int strandNumber;
    char sheetID[4];
    int noStrands;
    char initialRedisueName[4];
    char chainID;
    int residueSeqNumber;
    char codeForInsertions;
    char terminalResidueName[4];
    char chainID2;
    int residueSeqNumber2;
    char codeForInsertions2;
    int strandSense;

    char startAtomName[5];
    char startResidueName[4];
    char startChainID;
    int startResidueSeqNumber = -1;
    char startCodeForInsertions;

    char endAtomName[5];
    char endResidueName[4];
    char endChainID;
    int endResidueSeqNumber = -1;
    char endCodeForInsertions;
};

using Chain = std::vector<Atom>;
using Model = std::vector<Chain>;

struct ProteinData
{
    std::vector<Model> models;
    std::vector<Helix> alphaHelicis;
    std::vector<Sheet> betaSheets;
};
