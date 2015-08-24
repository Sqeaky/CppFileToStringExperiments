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

#include "main.h"

// Configuration
std::string filename("temp.txt");
std::vector<uint64_t> Sizes = {100};//, 512, 513, 1024, 1025};

std::vector<Function> ToBenchmark = {Appending, Eof, Iterator, Rdbuf};
std::map<Function, std::string> Names = { {Appending, "Appending"},
                                          {Eof, "Eof"},
                                          {Iterator, "Iterator"},
                                          {Rdbuf, "Rdbuf"}
                                        };
std::ostream& Out = std::cout;

int NameColumn = 20;
int Column = 12;
uint64_t TestIterations = 100000;

// Store everything for output of one random char to defeat optimization.
std::vector<std::string> HardToOptimizeOutResultsStorage;

int main(void)
{
    // Shuffle test order to allow tests to run in different order to see if allocator
    // Degradation is a factor.
    std::random_device RandomDevice;
    std::mt19937 Generator(RandomDevice());
    std::shuffle(ToBenchmark.begin(), ToBenchmark.end(), Generator);

    HardToOptimizeOutResultsStorage.reserve(TestIterations*ToBenchmark.size());

    Out << std::left << std::setw(NameColumn) << "Name / Size";
    for(auto CurrentSize : Sizes)
        { Out << std::setw(Column) << CurrentSize; }
    Out << std::endl;

    for(Function CurrentBenchmark : ToBenchmark)
    {
        Out << std::setw(NameColumn) << Names[CurrentBenchmark] << std::setw(Column);
        for(auto CurrentSize : Sizes)
            { Out << DoBenchmark(CurrentBenchmark, CurrentSize); }
        Out << std::endl;
    }

    return EXIT_SUCCESS;
}



std::string Appending()
{
    std::string Results;
    std::ifstream ResultReader("file.txt");
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
    std::ifstream ResultReader("file.txt");
    std::getline(ResultReader, Results, (char)std::char_traits<char>::eof());
    return Results;
}

std::string Iterator()
{
    std::ifstream ResultReader("file.txt");
    std::string Results((std::istreambuf_iterator<char>(ResultReader)),
                         std::istreambuf_iterator<char>());
    return Results;
}

std::string Rdbuf()
{
    std::ifstream ResultReader("file.txt");
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
}

uint64_t DoBenchmark(Function ToBenchmark, uint64_t Size)
{
    std::string Results;
    CreateFile(Size);
    auto start = std::chrono::high_resolution_clock::now();
    for(uint64_t Counter = 0; Counter < TestIterations; Counter++)
    {
        Results = PruneResults(ToBenchmark());
    }
    auto stop = std::chrono::high_resolution_clock::now();
    HardToOptimizeOutResultsStorage.push_back(Results);

    return std::chrono::duration_cast<std::chrono::milliseconds>(start-stop).count();
}

