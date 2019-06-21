#include "inputParameterParser.h"

#include <iterator>
#include <sstream>

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



bool InputParameterParser::parse(const char* input) const
{
    std::string s{ input };
    auto params = splitParams(s);

    __proteinFilename = params[0];

    //for (int i = 1; i < params.size(); ++i)
    //{
    //    std::string 
    //    
    //}
}

