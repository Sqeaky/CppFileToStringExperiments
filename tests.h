// This is covered under the MIT License see LICENSE for details
// Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

#ifndef _tests_h
#define _tests_h

#include <string>

std::string Appending(std::string FileName);
std::string Eof(std::string FileName);
std::string Iterator(std::string FileName);
std::string Rdbuf(std::string FileName);
std::string RdbufMove(std::string FileName);
std::string TellSeekRead(std::string FileName);
std::string CTellSeekRead(std::string FileName);
std::string AssignIterator(std::string FileName);
std::string BackInsertIterator(std::string FileName);

#endif
