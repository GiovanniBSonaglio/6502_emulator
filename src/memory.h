#include "globals.h"

#pragma once

class Memory {
    public:
        static constexpr u32 MAX_MEM = 1024 * 64; 
        BYTE Data[MAX_MEM];

        // Operator for reading 1 Byte
        BYTE operator[](u32 Address) const {
            // todo :: assert that Addres is < MAX_MEM
            return Data[Address];
        }

        // Operator for writing 1 Byte
        BYTE& operator[](u32 Address) {
            // todo :: assert that Addres is < MAX_MEM
            return Data[Address];
        }

        void Reset();
};