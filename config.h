// This is covered under the MIT License see LICENSE for details
// Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

// Change the config in config.cpp, this file is for sharing the config will all parts of the code.

#ifndef _config_h
#define _config_h

#include <string>
#include <vector>
#include <map>
#include <random>
#include <iostream>

typedef std::string (*Function)(std::string);

extern const std::string filename;
extern const std::vector<uint64_t> Sizes;

extern std::vector<Function> ToBenchmark;
extern std::map<Function, std::string> Names;
extern std::ostream& Out;

extern const int NameColumn;
extern const int Column;
extern const uint64_t TestIterations;

extern std::random_device RandomDevice;
extern std::mt19937 Generator;

extern std::string HardToOptimizeOutResultsStorage;

#endif
