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
#include "tests.h"

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
    {
        HardToOptimizeOutResultsStorage.push_back(
            ToBenchmark(filename)[RandomChar(Generator)]
        );
    }
    auto stop = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count();
    CreateFile(0);
}

