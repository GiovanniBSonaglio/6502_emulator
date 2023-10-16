#include "cpu.h"

void CPU::Reset(){
    PC = 0XFFFC;
    SP = 0x0100;
    C = Z = I = D = B = V = N = 0;
    A = X = Y = 0;
}

void CPU::Execute(u32 Cycles){
    while (Cycles > 0){
        BYTE Instruction = FetchByte(Cycles);
    }
}

BYTE CPU::FetchByte(u32 &Cycles){
    BYTE Data = CPU_mem[PC];
    PC++;
    Cycles--;
    return Data;
}