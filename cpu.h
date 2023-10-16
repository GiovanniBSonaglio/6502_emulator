#ifndef _INCL_DEFINE
    #include "globals.h"
#endif

#include "memory.h"

class CPU {
    public:
        WORD PC;        // Program Counter
        WORD SP;        // Stack Pointer
        BYTE A, X, Y;   // Registers
        
        // Status Flags
        BYTE C : 1;
        BYTE Z : 1;
        BYTE I : 1;
        BYTE D : 1;
        BYTE B : 1;
        BYTE V : 1;
        BYTE N : 1;

        // Memory Object
        Memory CPU_mem;

        void Reset();
        BYTE FetchByte(u32 &Cycles);
        void Execute(u32 Cycles);
};