#pragma once

#include "globals.h"
#include <array>

class Memory {
    public:
        static constexpr u32 MAX_MEM = 1024 * 64; 
        std::array<BYTE, MAX_MEM> ram;

        // Operator for reading 1 Byte
        BYTE operator[](WORD Address) const {
            if(Address >= 0x0000 && Address <= 0xFFFF){
                return ram[Address];
            }
        }

        // Operator for writing 1 Byte
        BYTE& operator[](WORD Address){
            if(Address >= 0x0000 && Address <= 0xFFFF){
                return ram[Address];
            }
        }

        void Initialise(){
            for (auto &i : ram) i = 0x00;
        }
};