#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "globals.h"
#include "memory.h"

class CPU {
    public:
        CPU();

        WORD PC;                // Program Counter
        WORD SP;                // Stack Pointer
        BYTE A, X, Y, Status;   // Registers
        u32  Cycles;            // Cycles counter
        BYTE InstructionAddr;   // Instruction
        WORD AbsoluteAddr;      // Absolute Address

        Memory CPUMem;

        // Status Flags
        enum Flags {	
            C = (1 << 0), // Carry Bit
            Z = (1 << 1), // Zero
            I = (1 << 2), // Disable Interrupts
            D = (1 << 3), // Decimal Mode
            B = (1 << 4), // Break
            U = (1 << 5), // Unused
            V = (1 << 6), // Overflow
            N = (1 << 7), // Negative
        };

        /********** Lookup table ************/
        struct INSTRUCTION {
            BYTE(CPU::*Operate)(void)   = nullptr;
            BYTE(CPU::*AddrMode)(void)  = nullptr;
            BYTE Cycles                 = 0;
        };

        std::vector<INSTRUCTION> Lookup = std::vector<INSTRUCTION>(255); 
        
        /*********** Signals ***********/
        void Reset(){
            PC = 0xFFFC;
            SP = 0xFD;
            Status = 0x00 | U;
            A = X = Y = 0;
            Cycles = 0;
            InstructionAddr = 0x00;
            CPUMem.Initialise();

            // Reset takes time
	        Cycles = 8;
        }

        // void InterruptRequest();
        // void NonMaskableInterrupt();
        void Clock();
        void ExecuteNCycles(BYTE CyclesToRun);

        BYTE GetFlag(Flags Flag);

    private:
        BYTE FetchByte();
        WORD FetchWord();
        BYTE ReadByte(WORD Addr);
        WORD ReadWord(WORD Addr);
        WORD AppendBytes(BYTE LoByte, BYTE HiByte);

        void SetFlag(Flags Flag, bool Value);
        
        BYTE LDA();
        BYTE XXX();

        BYTE IMM();
        BYTE ZP0();
        BYTE ZPX();
        BYTE ABS();
        BYTE ABX();
        BYTE ABY();
        BYTE IZX();
        BYTE IZY();
        BYTE IMP();
};