#include "cpu.h"
#include <cstdio>

void CPU::Reset(){
    PC = 0XFFFC;
    SP = 0x0100;
    Flag.C = Flag.Z = Flag.I = Flag.D = Flag.B = Flag.V = Flag.N = 0;
    A = X = Y = 0;
}

void CPU::Execute(u32 Cycles){
    while (Cycles > 0){
        BYTE Instruction = FetchByte(Cycles);

        BYTE AddrLSB, AddrMSB, Value;
        WORD MemAddr;

        switch (Instruction)
        {
            // LDA - Immediate
            case INS_LDA_IM:
            {
                A = FetchByte(Cycles);
                SetZeroAndNegativeFlags(A);
            } break;

            // LDA - Zero Page
            case INS_LDA_ZP:
            {
                BYTE ZeroPageAddr = FetchByte(Cycles);
                LoadRegister(Cycles, ZeroPageAddr, A);
            } break;

            case INS_LDA_ZPX:
            {
                BYTE ZeroPageAddr = FetchByte(Cycles);
                ZeroPageAddr += X;
                Cycles--;
                LoadRegister(Cycles, ZeroPageAddr, A);
            } break;

            // LDA - Absolute
            case INS_LDA_ABS:
            {
                WORD AbsAddr = FetchWord(Cycles);
                LoadRegister(Cycles, AbsAddr, A);
            } break;

            case INS_LDA_ABSX:
            {            
                WORD AbsAddr = FetchWord( Cycles );
                WORD AbsAddrX = AbsAddr + X;
                
                const bool CrossedPageBoundary = (AbsAddr ^ AbsAddrX) >> 8;
                if ( CrossedPageBoundary )
                {
                    Cycles--;
                }
                LoadRegister(Cycles, AbsAddrX, A);
            } break;

            case INS_LDA_ABSY:
            {
                WORD AbsAddr = FetchWord( Cycles );
                WORD AbsAddrY = AbsAddr + Y;

                const bool CrossedPageBoundary = (AbsAddr ^ AbsAddrY) >> 8;
                if ( CrossedPageBoundary )
                {
                    Cycles--;
                }
                LoadRegister(Cycles, AbsAddrY, A);
            } break;

            case INS_LDA_INDX:
            {
                BYTE ZPAddr = FetchByte(Cycles);
                ZPAddr += X;
                Cycles--;

                WORD EffectiveAddr = ReadWord(Cycles, ZPAddr);

                LoadRegister(Cycles, EffectiveAddr, A);
            } break;

            case INS_LDA_INDY:
            {
                BYTE ZPAddr = FetchByte(Cycles);
                WORD EffectiveAddr = ReadWord(Cycles, ZPAddr);
                WORD EffectiveAddrY = EffectiveAddr + Y;

                const bool CrossedPageBoundary = (EffectiveAddr ^ EffectiveAddrY) >> 8;
                if ( CrossedPageBoundary )
                {
                    Cycles--;
                }
                LoadRegister(Cycles, EffectiveAddr, A);
            } break;
            
            default:
            {
                printf("\n Instruction is not handled %d", Instruction);
            } break;
        }
        printf("\n Cycles: %d", Cycles);
    }
}

BYTE CPU::FetchByte(u32 &Cycles){
    BYTE Data = CPU_mem[PC];
    PC++;
    Cycles--;
    return Data;
}

WORD CPU::FetchWord(u32 &Cycles){
    BYTE LoByteData = CPU_mem[PC];
    BYTE HiByteData = CPU_mem[PC+1];
    PC += 2;

    WORD Data = AppendBytes(LoByteData, HiByteData);
    Cycles -= 2;
    return Data;
}

BYTE CPU::ReadByte(u32 &Cycles, WORD Addr){
    BYTE Data = CPU_mem[Addr];
    Cycles--;
    return Data;
}

WORD CPU::ReadWord(u32 &Cycles, WORD Addr){
    BYTE LoByteData = CPU_mem[Addr];
    BYTE HiByteData = CPU_mem[Addr+1];

    WORD Data = AppendBytes(LoByteData, HiByteData);
    Cycles -= 2;
    return Data;
}

WORD CPU::AppendBytes(BYTE LoByte, BYTE HiByte){
    return LoByte | (HiByte << 8);
}

void CPU::SetZeroAndNegativeFlags( BYTE Register ){
    Flag.Z = (Register == 0);
    Flag.N = (Register & NegativeFlagBit) > 0;
}

void CPU::LoadRegister(u32 &Cycles, WORD Address, BYTE& Register){
    Register = ReadByte( Cycles, Address );
    SetZeroAndNegativeFlags( Register );
}