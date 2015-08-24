// This is covered under the MIT License see LICENSE for details
// Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

#include "config.h"
#include "tests.h"

const std::string filename("temp.txt");
const std::vector<uint64_t> Sizes = {100, 512, 1024, 1000000, 10000000};//, 100000000};

std::vector<Function> ToBenchmark = {Appending,
                                     Eof,
                                     Iterator,
                                     Rdbuf,
                                     RdbufMove,
                                     TellSeekRead,
                                     CTellSeekRead,
                                     AssignIterator,
                                     BackInsertIterator
                                    };
std::map<Function, std::string> Names = { {Appending, "Appending"},
                                          {Eof, "Eof"},
                                          {Iterator, "Iterator"},
                                          {Rdbuf, "Rdbuf"},
                                          {RdbufMove, "RdbufMove"},
                                          {TellSeekRead, "TellSeekRead"},
                                          {CTellSeekRead, "CTellSeekRead"},
                                          {AssignIterator, "AssignIterator"},
                                          {BackInsertIterator, "BackInsertIterator"}
                                        };
std::ostream& Out = std::cout;

const int NameColumn = 20;
const int Column = 12;
const uint64_t TestIterations = 1000;

std::random_device RandomDevice;
std::mt19937 Generator(RandomDevice());

std::string HardToOptimizeOutResultsStorage;
