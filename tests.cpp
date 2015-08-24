// This is covered under the MIT License see LICENSE for details
// Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

#include <string>
#include <fstream>
#include <sstream>

#include "config.h"



std::string Appending(std::string FileName)
{
    std::string Results;
    std::ifstream ResultReader(FileName);
    while(ResultReader)
    {
        std::getline(ResultReader, Results);
        Results.push_back('\n');
    }
    return Results;
}

std::string Eof(std::string FileName)
{
    std::string Results;
    std::ifstream ResultReader(FileName);
    std::getline(ResultReader, Results, (char)std::char_traits<char>::eof());
    return Results;
}

std::string Iterator(std::string FileName)
{
    std::ifstream ResultReader(FileName);
    std::string Results((std::istreambuf_iterator<char>(ResultReader)),
                         std::istreambuf_iterator<char>());
    return Results;
}

std::string Rdbuf(std::string FileName)
{
    std::ifstream ResultReader(FileName);
    std::ostringstream Results;
    Results << ResultReader.rdbuf();
    return Results.str();
}

std::string RdbufMove(std::string FileName)
{
    std::ifstream ResultReader(FileName);
    std::ostringstream Results;
    Results << ResultReader.rdbuf();
    return std::move(Results.str());
}


std::string TellSeekRead(std::string FileName)
{
    std::ifstream ResultReader(FileName, std::ios::ate);
    auto fileSize = ResultReader.tellg();
    ResultReader.seekg(std::ios::beg);
    return std::string(fileSize,0);
}
