#pragma once

#include "mathLib.h"
#include <string>
#include <vector>

class InputParameterParser
{
public:
    InputParameterParser();
    ~InputParameterParser();
    bool parse(const char* input) const;

private:
    static std::vector<std::string> splitParams(std::string const &input);
    static void trim(std::string& line);
    static bool equal(const std::string& a, const std::string& b);
};

