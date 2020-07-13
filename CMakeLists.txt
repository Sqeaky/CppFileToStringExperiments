# © Copyright 2015 Sqeaky - Joseph Toppi
# This is covered under the MIT License see LICENSE for details
# Sqeaky - Joseph Toppi - toppij@blacktoppstudios.com

project(CppFileToStringExperiments)
cmake_minimum_required(VERSION 2.8)

message(STATUS "${PROJECT_NAME} - Starting Configuration.")

# Source Files
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++14")

set(SRC_LIST
    "config.cpp"
    "main.cpp"
    "tests.cpp"
)

set(HEADER_LIST
    "config.h"
    "main.h"
    "tests.h"
)

add_custom_target(${PROJECT_NAME}_SpareFileGrabber ALL
                  COMMAND ${CMAKE_COMMAND} -E echo "Ensuring spare files are made visible to IDEs."
                  WORKING_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}
                  VERBATIM
                  SOURCES ${${PROJECT_NAME}_SOURCE_DIR}/README.md
)

##############################################################################
# Defining the one and only Executable

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADER_LIST})
