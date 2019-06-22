#include "inputParameterParser.h"
#include "pdbParser.h"

#include <iterator>
#include <sstream>

typedef unsigned int uint;

extern std::string __proteinFilename;


InputParameterParser::InputParameterParser()
{
}


InputParameterParser::~InputParameterParser()
{
}



std::vector<std::string> InputParameterParser::splitParams(std::string const &input) {
    std::istringstream buffer(input);
    std::vector<std::string> ret;

    std::copy(std::istream_iterator<std::string>(buffer),
        std::istream_iterator<std::string>(),
        std::back_inserter(ret));
    return ret;
}

bool InputParameterParser::equal(const std::string& a, const std::string& b)
{
    if (!a.rfind(b, 0)) return true;
    return false;
}

void InputParameterParser::trim(std::string& line)
{
    uint startIdx = 0;
    uint endIdx = line.length() - 1;
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

    line = line.substr(startIdx, endIdx + 1);
}



bool InputParameterParser::parse(const char* input) const
{
    std::string s{ input };
    auto params = splitParams(s);

    __proteinFilename = params[0];

    for (int i = 1; i < params.size(); ++i)
    {
        std::string& param = params[i];
        
        uint ptsi, ptei; // parameter type start/end index
        ptsi = param.find_first_of("--") + 2;
        ptei = param.find_first_of("=");
        std::string paramType = param.substr(ptsi, ptei-ptsi);        
        std::string paramValue = param.substr(ptei + 1);

        if (equal(paramType, "helixRGB"))
        {
            Vec3 color{ paramValue };
        }

    }

    return true;
}

