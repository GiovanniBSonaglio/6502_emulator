cmake_minimum_required(VERSION 3.12)

project(M6502Emulator)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Sub-directories where more CMakeLists.txt exist
add_subdirectory(src)
add_subdirectory(tests)