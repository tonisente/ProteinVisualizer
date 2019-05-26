#include "pdbParser.h"

#include <cassert>
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

    // variables need for parsing
    ProteinData proteinData;
    Chain chain;
    Model model;
    std::string line;
    line.reserve(100);
    std::vector<Helix> helices;
    std::vector<Sheet> sheets;

    while (std::getline(fin, line))
    {
        if (!line.rfind("ATOM", 0))
        {
            Atom atom = parseAtom(line);
            chain.push_back(atom);
            //if (/*atom.alternateLocationIndicator == ' ' || atom.alternateLocationIndicator == 'A'*/ atom.serialNumber == 238)
            //{
            //    chain.push_back(atom);
            //}
        }
        else if (!line.rfind("TER", 0))
        {
            model.emplace_back(std::move(chain));
        }
        else if (!line.rfind("MODEL", 0))
        {
            chain = Chain();
            chain.reserve(100); // reserve some space; (todo: magic number)
            model = Model();
        }
        else if (!line.rfind("ENDMDL", 0) || !line.rfind("END", 0))
        {
            proteinData.models.emplace_back(std::move(model));
        }
        else if (!line.rfind("HELIX", 0))
        {
            //Helix helix = parseHelix(line);
            //helices.push_back(helix);            
        }
    }
    
    return proteinData;
}


Atom PDBParser::parseAtom(const std::string& line)
{
    Atom atom;
    char buffer[35];

    // atom serial number
    trimSpace(line, 5, 10, buffer);
    atom.serialNumber = atoi(buffer);

    // atom name
    trimSpace(line, 12, 15, buffer);
    strcpy(atom.name, buffer);

    // alternate location indicator
    atom.altLocationIndicator = line[16];

    // residue name
    trimSpace(line, 17, 19, buffer);
    strcpy(atom.residueName, buffer);

    // chain identifier
    atom.chainID = line[21];

    // residue sequence number
    trimSpace(line, 22, 25, buffer);
    atom.residueSeqNumber = atoi(buffer);

    // code for insertion
    atom.codeForInsertion = line[26];

    // x coord
    trimSpace(line, 30, 37, buffer);
    atom.xCoord = atof(buffer);

    // y coord
    trimSpace(line, 38, 45, buffer);
    atom.yCoord = atof(buffer);

    // z coord
    trimSpace(line, 46, 53, buffer);
    atom.zCoord = atof(buffer);

    // occupancy
    trimSpace(line, 54, 59, buffer);
    atom.occupancy = atof(buffer);

    // temperature factor
    trimSpace(line, 60, 65, buffer);
    atom.temperatureFactor = atof(buffer);

    // segment identifier
    trimSpace(line, 72, 75, buffer);
    strcpy(atom.segmentID, buffer);

    // element symbol
    trimSpace(line, 76, 77, buffer);
    strcpy(atom.elementSymbol, buffer);

    // reverse point (crystallography?!)
    atom.xCoord = -atom.xCoord;
    atom.yCoord = -atom.yCoord;
    atom.zCoord = -atom.zCoord;

    return atom;
}


void PDBParser::trimSpace(const std::string& line, int startIdx, int endIdx, char* destBuff)
{
    assert(startIdx <= endIdx);
    assert(line.length() > endIdx);

    while (startIdx <= endIdx)
    {
        if (line[startIdx] == ' ')
        {
            startIdx++;
        }
        else
        {
            break;
        }
    }
    while (startIdx <= endIdx)
    {
        if (line[endIdx] == ' ')
        {
            endIdx--;
        }
        else
        {
            break;
        }
    }

    // copy string
    int j = 0;
    for (int i = startIdx; i <= endIdx; ++i)
    {
        destBuff[j++] = line[i];
    }
    destBuff[j] = '\0';

    return;
}


