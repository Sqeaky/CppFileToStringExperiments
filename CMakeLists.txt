# © Copyright 2015 Sqeaky - Joseph Toppi
# This is covered under the MIT License see LICENSE for details
# Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

project(CppFileToStringExperiments)
cmake_minimum_required(VERSION 2.8)

message ("${PROJECT_NAME} - Starting Configuration.")

# Source Files
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++14")

set(SRC_LIST
    "main.cpp"
)

set(HEADER_LIST
    "main.h"
)

##############################################################################
# Defining the one and only Executable

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADER_LIST})
