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
    std::ifstream ifs{ filename };
    std::string line;
    line.reserve(100);

    std::vector<ProteinData::Atom> atoms;

    while (std::getline(ifs, line))
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
    char * copyLine = new char[line.size() + 2]; // todo: get rid of this copy
    strcpy(copyLine, line.c_str());

    sprintf(copyLine,
            


    delete copyLine;
}




