#pragma once

#include "proteinData.h"
#include <string>
#include <vector>


class PDBParser
{
public:
    static const std::string pathToModels;

    static ProteinData parse(const std::string& filename);

    static PDBParser& getInstance();
    PDBParser(const PDBParser&)      = delete;
    void operator=(const PDBParser&) = delete;

private:
    static Atom parseAtom(const std::string& line);
    static Helix parseHelix(const std::string& line);
    static Sheet parseSheet(const std::string& line);

    /** destBuff should have enough space required  */
    static void trimSpace(const std::string& line, int startIdx, int endIdx, char* destBuff);
    static void trimAndStore(const std::string& line, int startIdx, int endIdx, char* dest);
    static void trimAndStore(const std::string& line, int startIdx, int endIdx, float* dest);
    static void trimAndStore(const std::string& line, int startIdx, int endIdx, int* dest);

    PDBParser() {};
};

