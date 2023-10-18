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
        struct StatusFlags {	
            BYTE C : 1;	        // 0: Carry Flag	
            BYTE Z : 1;	        // 1: Zero Flag
            BYTE I : 1;         // 2: Interrupt disable
            BYTE D : 1;         // 3: Decimal mode
            BYTE B : 1;         // 4: Break
            BYTE Unused : 1;    // 5: Unused
            BYTE V : 1;         // 6: Overflow
            BYTE N : 1;         // 7: Negative
        };

        // Process status bits
        static constexpr BYTE
            NegativeFlagBit = 0b10000000,
            ZeroBit = 0b00000001;

        StatusFlags Flag;

        // Memory Object
        Memory CPU_mem;

        void Reset();
        BYTE FetchByte(u32 &Cycles);
        WORD FetchWord(u32 &Cycles);
        BYTE ReadByte(u32 &Cycles, WORD Addr);
        WORD ReadWord(u32 &Cycles, WORD Addr);
        void Execute(u32 Cycles);
        WORD AppendBytes(BYTE LoByte, BYTE HiByte);
        void SetZeroAndNegativeFlags( BYTE Register );
        void LoadRegister(u32 &Cycles, WORD Address, BYTE& Register);

        /*********** Start Opcodes ***********/

        // LDA
        static constexpr BYTE 
            INS_LDA_IM = 0xA9,
            INS_LDA_ZP = 0xA5,
            INS_LDA_ZPX = 0xB5,
            INS_LDA_ABS = 0xAD,
            INS_LDA_ABSX = 0xBD,
            INS_LDA_ABSY = 0xB9,
            INS_LDA_INDX = 0xA1,
            INS_LDA_INDY = 0xB1;
        /*********** End Opcodes ***********/
};