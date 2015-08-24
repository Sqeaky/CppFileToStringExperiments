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
    std::ifstream ResultReader(FileName, std::ios::binary|std::ios::ate);
    auto FileSize = ResultReader.tellg();
    ResultReader.seekg(std::ios::beg);
    std::string Results(FileSize,0);
    ResultReader.read(&Results[0],FileSize);
    return Results;
}

std::string CTellSeekRead(std::string FileName)
{
    std::FILE *ResultReader = std::fopen(FileName.c_str(), "rb");
    std::string Results;
    std::fseek(ResultReader, 0, SEEK_END);
    Results.resize(std::ftell(ResultReader));
    std::rewind(ResultReader);
    std::fread(&Results[0], 1, Results.size(), ResultReader);
    std::fclose(ResultReader);
    return(Results);
}


std::string AssignIterator(std::string FileName)
{
    std::ifstream ResultReader(FileName, std::ios::in | std::ios::binary);
    std::string Results;
    ResultReader.seekg(0, std::ios::end);
    Results.reserve(ResultReader.tellg());
    ResultReader.seekg(0, std::ios::beg);
    Results.assign((std::istreambuf_iterator<char>(ResultReader)), std::istreambuf_iterator<char>());
    ResultReader.close();
    return(Results);
}


std::string BackInsertIterator(std::string FileName)
{
    std::ifstream in(FileName, std::ios::in | std::ios::binary);
    std::string Results;
    in.seekg(0, std::ios::end);
    Results.reserve(in.tellg());
    in.seekg(0, std::ios::beg);
    std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(Results));
    in.close();
    return(Results);
}
