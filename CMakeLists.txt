cmake_minimum_required(VERSION 3.16)

project(Logger)

# set compiler
#set(CMAKE_CXX_COMPILER /usr/bin/clang++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -fsanitize=address -pthread")

find_package (Boost REQUIRED COMPONENTS unit_test_framework)


# set build files
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/tlogger)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/examples)
set(PROJECT_TEST_DIR ${PROJECT_DIR}/test)

set(PROJECT_SOURCES 
            #${PROJECT_SOURCE_DIR}/LogMacroToFileInChar.cpp
            ${PROJECT_SOURCE_DIR}/BasicLogInCharToFile.cpp

    )

set(PROJECT_HEADERS
            ${PROJECT_INCLUDE_DIR}/formatter.h
            ${PROJECT_INCLUDE_DIR}/logger.h
            ${PROJECT_INCLUDE_DIR}/test.h
    )

include_directories(${PROJECT_INCLUDE_DIR})


set(SIMPLE "loggerTests")
add_executable(${SIMPLE} "${PROJECT_TEST_DIR}/loggerTest.cpp" )
target_link_libraries(${SIMPLE} Boost::unit_test_framework )

set(FORMATTERTEST "formatterTests")
add_executable(${FORMATTERTEST} "${PROJECT_TEST_DIR}/formatterTest.cpp" )
target_link_libraries(${FORMATTERTEST} Boost::unit_test_framework )

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
#target_link_libraries(${PROJECT_NAME} Boost::unit_test_framework )
  

