// This is covered under the MIT License see LICENSE for details
// Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

#include <string>

typedef std::string (*Function)(void);

std::string Appending();
std::string Eof();
std::string Iterator();
std::string Rdbuf();

void CreateFile();
uint64_t DoBenchmark(Function ToBenchmark, uint64_t Size);
