#include "pdbParser.h"

#include <fstream>
#include <sstream>
#include <string>

const std::string PDBParser::pathToModels = "ProteinModels/";

PDBParser& PDBParser::getInstance()
{
    static PDBParser instance;
    return instance;
}

ProteinData PDBParser::parse(const std::string& filename)
{
    std::string combinedPath = pathToModels + filename;
    std::ifstream fin;
    fin.open(combinedPath);
    if (!fin)
    {
        throw "Invalid protein file input!";
    }

    std::string line;
    line.reserve(100);
    while (std::getline(fin, line))
    {
        if (!line.rfind("ATOM", 0))
        {
            Atom atom = parseAtom(
        }
        else if (!line.rfind("TER", 0))
        {

        }
    }

}

Atom PDBParser::parseAtom(const std::string& line)
{
    int a;
    char * copyLine = new char[line.size() + 2]; // todo: get rid of this copy (or new at least)
    strcpy(copyLine, line.c_str());

    Atom atom;

    sscanf
    (
        copyLine,
        "ATOM "\
        "%6d"\
        "%4s"\
        "%c"\
        "%3s "\
        "%c"\
        "%3d"\
        "%c"\
        "%f"\
        "%f"\
        "%f"\
        "%f"\
        "%f"\
        "      %4s"\
        "%2s",
        &atom.serialNumber,
        atom.name,
        &atom.alternateLocationIndicator,
        atom.residueName,
        &atom.chainID,
        &atom.residueSeqNumber,
        &atom.codeForInsertion,
        &atom.xCoord,
        &atom.yCoord,
        &atom.zCoord,
        &atom.occupancy,
        &atom.temperatureFactor,
        atom.segmentID,
        atom.elementSymbol
    );

    delete [] copyLine;
    return atom;
}




