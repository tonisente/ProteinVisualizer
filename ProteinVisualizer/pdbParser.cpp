#include "pdbParser.h"

#include <fstream>
#include <sstream>
#include <string>


PDBParser::PDBParser()
{
}


PDBParser::~PDBParser()
{
}


std::vector<ProteinData::Atom> PDBParser::parse(const char * filename) const
{
    std::ifstream fin;
    std::vector<ProteinData::Atom> atoms;

    std::string combinedPath = pathToModels + std::string(filename);
    fin.open(combinedPath);
    if (!fin)
    {
        return atoms;
    }

    std::string line;
    line.reserve(100);


    while (std::getline(fin, line))
    {
        if (line.rfind("ATOM", 0) == 0)
        {
            atoms.emplace_back(parseAtom(line));
        }
    }

    return atoms;
}


ProteinData::Atom PDBParser::parseAtom(const std::string& line) const
{
    int a;
    char * copyLine = new char[line.size() + 2]; // todo: get rid of this copy (or new at least)
    strcpy(copyLine, line.c_str());

    ProteinData::Atom atom;

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




