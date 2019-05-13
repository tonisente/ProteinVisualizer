#pragma once

#include "proteinData.h"
#include "string"
#include <vector>


class PDBParser
{
public:
    PDBParser();
    ~PDBParser();

    std::vector<ProteinData::Atom> parse(const char * filename) const;

private:
    ProteinData::Atom parseAtom(const std::string& line) const;
};

