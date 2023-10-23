cmake_minimum_required(VERSION 3.12)

project( M6502Lib )

set (M6502_SOURCES
    "cpu.cpp"
    "cpu.h"
    "memory.h"
    "main.cpp")
		
source_group("src" FILES ${M6502_SOURCES})

add_library(M6502Lib ${M6502_SOURCES})

target_include_directories (M6502Lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")