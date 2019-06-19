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
            Helix helix = parseHelix(line);
            helices.push_back(helix);            
        }
        else if (!line.rfind("SHEET", 0))
        {
            Sheet sheet = parseSheet(line);
            sheets.push_back(sheet);
        }
    }

    proteinData.alphaHelicis = std::move(helices);
    proteinData.betaSheets = std::move(sheets);
    
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
    //atom.xCoord = -atom.xCoord;
    //atom.yCoord = -atom.yCoord;
    atom.zCoord = -atom.zCoord;

    return atom;
}


Helix PDBParser::parseHelix(const std::string& line)
{
    Helix helix;
    char buffer[35];

    // helix serial number
    trimSpace(line, 7, 9, buffer);
    helix.serialNumber = atoi(buffer);

    // helix ID
    trimSpace(line, 11, 13, buffer);
    strcpy(helix.ID, buffer);

    // initial residue name
    trimSpace(line, 15, 17, buffer);
    strcpy(helix.initialResidueName, buffer);

    // chain identifier
    helix.chainID = line[19];

    // residue sequence number
    trimSpace(line, 21, 24, buffer);
    helix.residueSeqNumber = atoi(buffer);

    // code for insertion of residues
    helix.codeForInsertion = line[25];

    // terminal residue name
    trimSpace(line, 27, 29, buffer);
    strcpy(helix.terminalResidueName, buffer);

    // chain identifier
    helix.chainID2 = line[31];

    // residue sequence number
    trimSpace(line, 33, 36, buffer);
    helix.residueSeqNumber2 = atoi(buffer);

    // code for insertions of residues
    helix.codeForInsertion2 = line[37];

    // type of helix
    trimSpace(line, 38, 39, buffer);
    helix.type = atoi(buffer);

    // comment
    trimSpace(line, 40, 69, buffer);
    strcpy(helix.comment, buffer);

    // length of helix
    trimSpace(line, 71, 75, buffer);
    helix.length = atoi(buffer);

    return helix;
}


Sheet PDBParser::parseSheet(const std::string& line)
{
    Sheet sheet;
    char  buffer[35];

    // strand number
    trimAndStore(line, 8, 9, &sheet.strandNumber);

    // sheet identifier 
    trimAndStore(line, 11, 13, sheet.sheetID);

    // number of strands
    trimAndStore(line, 14, 15, &sheet.noStrands);

    // initial residue name
    trimAndStore(line, 17, 19, sheet.initialRedisueName);

    // chain ID
    sheet.chainID = line[21];

    // residue sequence number
    trimAndStore(line, 22, 25, &sheet.residueSeqNumber);

    // code for insertions of residues
    sheet.codeForInsertions = line[26];

    // terminal residue name
    trimAndStore(line, 28, 30, sheet.terminalResidueName);

    // chain identifier
    sheet.chainID2 = line[32];

    // residue sequence number
    trimAndStore(line, 33, 36, &sheet.residueSeqNumber2);

    // code for insertions of residue
    sheet.codeForInsertions2;

    // strand sense with respect to previous
    trimAndStore(line, 38, 39, &sheet.strandSense);


    /* START RESIDUE */
    {
        // atom name
        trimAndStore(line, 41, 44, sheet.startAtomName);

        // residue name
        trimAndStore(line, 45, 47, sheet.startResidueName);

        // chain id
        sheet.startChainID = line[49];

        // residue sequence number
        trimAndStore(line, 50, 53, &sheet.startResidueSeqNumber);

        // code for indertions of residues
        sheet.startCodeForInsertions = line[54];
    }

    /* END RESIDUE */
    {   
        // atom name
        trimAndStore(line, 56, 59, sheet.endAtomName);

        // residue name
        trimAndStore(line, 60, 62, sheet.endResidueName);

        // chain id
        sheet.endChainID = line[64];

        // residue sequence number
        trimAndStore(line, 65, 68, &sheet.endResidueSeqNumber);

        // code for indertions of residues
        sheet.endCodeForInsertions = line[69];
    }

    return sheet;
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


void PDBParser::trimAndStore(const std::string& line, int startIdx, int endIdx, char* dest)
{
    trimSpace(line, startIdx, endIdx, dest);
}

void PDBParser::trimAndStore(const std::string& line, int startIdx, int endIdx, int* dest)
{
    char buffer[15];
    trimSpace(line, startIdx, endIdx, buffer);
    *dest = atoi(buffer);
}

void PDBParser::trimAndStore(const std::string& line, int startIdx, int endIdx, float* dest)
{
    char buffer[15];
    trimSpace(line, startIdx, endIdx, buffer);
    *dest = atof(buffer);
}
