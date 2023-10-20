cmake_minimum_required(VERSION 3.12)
project(M6502Test)

enable_testing()

add_subdirectory(googletest)
include_directories(googletest/include)

# source for the test executable
set (M6502_SOURCES
      "mainUnitTests.cpp"
      "LoadRegisterTests.cpp")

add_executable(M6502Test ${M6502_SOURCES})
add_dependencies(M6502Test M6502Lib)

target_link_libraries(M6502Test gtest)
target_link_libraries(M6502Test M6502Lib)

include(GoogleTest)
gtest_discover_tests(M6502Test)




include(GoogleTest)
gtest_discover_tests(M6502Test)
