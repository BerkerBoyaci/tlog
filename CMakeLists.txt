cmake_minimum_required(VERSION 3.16)

project(Logger)

# set compiler
set(CMAKE_CXX_COMPILER /usr/bin/clang++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -fsanitize=address -pthread")

# set build files
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/tlogger)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/examples)

set(PROJECT_SOURCES 
            ${PROJECT_SOURCE_DIR}/LogMacroToFileInChar.cpp

    )

set(PROJECT_HEADERS
            ${PROJECT_INCLUDE_DIR}/Formatter.h
            ${PROJECT_INCLUDE_DIR}/Logger.h
    )


include_directories(${PROJECT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})