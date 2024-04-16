cmake_minimum_required(VERSION 3.16)

project(Logger)

# set compiler
#set(CMAKE_CXX_COMPILER /usr/bin/clang++-10)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -fsanitize=address -pthread")


# set build files
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/tlogger)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/examples)
set(PROJECT_TEST_DIR ${PROJECT_DIR}/test)

set(PROJECT_SOURCES 
            #${PROJECT_SOURCE_DIR}/LogMacroToFileInChar.cpp
            #${PROJECT_SOURCE_DIR}/BasicLogInCharToFile.cpp
        #${PROJECT_SOURCE_DIR}/BasicLogInCharToConsole.cpp
        #${PROJECT_SOURCE_DIR}/BasicLogInWcharToConsole.cpp
        #${PROJECT_SOURCE_DIR}/BasicLogInWcharToFile.cpp
        #${PROJECT_SOURCE_DIR}/BasicMultiLanguageTestInWchar.cpp
        #${PROJECT_SOURCE_DIR}/LogMacroToFileInChar.cpp
        # ${PROJECT_SOURCE_DIR}/LogMacroToConsoleChar.cpp
        #${PROJECT_SOURCE_DIR}/MacroToConsoleInWchar.cpp
        #${PROJECT_SOURCE_DIR}/MacroToFileInStringView.cpp
        #${PROJECT_SOURCE_DIR}/MacroToFileInWchar.cpp
        #${PROJECT_SOURCE_DIR}/MacroToFileInWStringView.cpp
        #${PROJECT_SOURCE_DIR}/tlogger.cpp
        #${PROJECT_SOURCE_DIR}/BasicBenchmarkTestIInChar.cpp
        #${PROJECT_SOURCE_DIR}/BasicBenchmarkTestInWchar.cpp
        ${PROJECT_SOURCE_DIR}/BasicThreadSafetyTest.cpp
)

set(PROJECT_HEADERS
            ${PROJECT_INCLUDE_DIR}/formatter.h
            ${PROJECT_INCLUDE_DIR}/logger.h
            ${PROJECT_INCLUDE_DIR}/test.h
    )

include_directories(${PROJECT_INCLUDE_DIR})


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


