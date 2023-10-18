#include <stdio.h>
#include <stdlib.h>

#ifndef _INCL_DEFINE
    #include "globals.h"
#endif

#include "cpu.h"

int main()
{
    CPU cpu;
    cpu.Reset();

    // LDA - INS_LDA_INDY
    cpu.Y = 0x10;
    cpu.CPU_mem[0xFFFC] = CPU::INS_LDA_INDY;
    cpu.CPU_mem[0xFFFD] = 0x86;
    cpu.CPU_mem[0x0086] = 0x28;
    cpu.CPU_mem[0x0087] = 0x40;
    cpu.CPU_mem[0x4028] = 0x81;
    cpu.Execute(5);

    return 0;
}