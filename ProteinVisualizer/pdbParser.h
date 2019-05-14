#pragma once

#include "proteinData.h"
#include <string>
#include <vector>


class PDBParser
{
public:
    static const std::string pathToModels;

    static ProteinData parse(const std::string& filename);

public:
    static PDBParser& getInstance();
    PDBParser(const PDBParser&)      = delete;
    void operator=(const PDBParser&) = delete;

private:
    static Atom parseAtom(const std::string& line);

private:
    PDBParser() {};
};

