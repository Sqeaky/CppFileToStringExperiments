// This is covered under the MIT License see LICENSE for details
// Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

#include <cstdlib>
#include <string>
#include <fstream>
#include <chrono>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cassert>

#include "main.h"

// Configuration
const std::string filename("temp.txt");
const std::vector<uint64_t> Sizes = {100, 512, 1024, 1000000, 10000000};//, 100000000};

std::vector<Function> ToBenchmark = {Appending, Eof, Iterator, Rdbuf};
std::map<Function, std::string> Names = { {Appending, "Appending"},
                                          {Eof, "Eof"},
                                          {Iterator, "Iterator"},
                                          {Rdbuf, "Rdbuf"}
                                        };
std::ostream& Out = std::cout;

const int NameColumn = 20;
const int Column = 12;
const uint64_t TestIterations  = 500;

std::random_device RandomDevice;
std::mt19937 Generator(RandomDevice());

// Store everything for output of one random char to defeat optimization.
std::string HardToOptimizeOutResultsStorage;

int main(void)
{
    // Shuffle test order to allow tests to run in different order to see if allocator
    // Degradation is a factor.
    std::shuffle(ToBenchmark.begin(), ToBenchmark.end(), Generator);

    HardToOptimizeOutResultsStorage.reserve(TestIterations);

    Out << "Doing " << TestIterations << " iterations of the file read in each benchmark."
        << std::endl;

    Out << std::left << std::scientific << std::setw(NameColumn) << "Name / Size";
    for(auto CurrentSize : Sizes)
        { Out << std::setw(Column) << CurrentSize; }
    Out << std::setw(Column) << "Average" << std::endl;

    for(Function CurrentBenchmark : ToBenchmark)
    {
        Out << std::setw(NameColumn) << Names[CurrentBenchmark];
        uint64_t RunningTotal = 0;
        uint64_t CurrentTime = 0;
        for(auto CurrentSize : Sizes)
        {
            CurrentTime = DoBenchmark(CurrentBenchmark, CurrentSize);
            RunningTotal += CurrentTime;
            Out << std::setw(Column) << CurrentTime;
        }
        Out << std::setw(Column) << (RunningTotal/Sizes.size()) << std::endl;
    }

    std::uniform_int_distribution<> RandomChar(0,HardToOptimizeOutResultsStorage.size()-1);
    Out << "To Prevent Really smart compilers from optimizing anything out here is a random "
        << "char of the intermediary output: "
        << HardToOptimizeOutResultsStorage[RandomChar(Generator)]
        << std::endl;

    return EXIT_SUCCESS;
}



std::string Appending()
{
    std::string Results;
    std::ifstream ResultReader(filename);
    while(ResultReader)
    {
        std::getline(ResultReader, Results);
        Results.push_back('\n');
    }
    return Results;
}

std::string Eof()
{
    std::string Results;
    std::ifstream ResultReader(filename);
    std::getline(ResultReader, Results, (char)std::char_traits<char>::eof());
    return Results;
}

std::string Iterator()
{
    std::ifstream ResultReader(filename);
    std::string Results((std::istreambuf_iterator<char>(ResultReader)),
                         std::istreambuf_iterator<char>());
    return Results;
}

std::string Rdbuf()
{
    std::ifstream ResultReader(filename);
    std::ostringstream Results;
    Results << ResultReader.rdbuf();
    return Results.str();
}

void CreateFile(uint64_t Size)
{
    std::ofstream truncate(filename);
    truncate.close();

    std::ofstream fill(filename);
    for(uint64_t i = 0; i<Size; i++)
        { fill << "A"; }
    fill.flush();
}

uint64_t DoBenchmark(Function ToBenchmark, uint64_t Size)
{
    CreateFile(Size);
    std::uniform_int_distribution<> RandomChar(0,Size-1);
    auto start = std::chrono::high_resolution_clock::now();
    for(uint64_t Counter = 0; Counter < TestIterations; Counter++)
       { HardToOptimizeOutResultsStorage.push_back( ToBenchmark()[RandomChar(Generator)] ); }
    auto stop = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count();
    CreateFile(0);
}

