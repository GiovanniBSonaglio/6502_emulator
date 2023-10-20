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

    cpu.CPUMem[0xFFFC] = CPU::INS_LDA_IM;
    cpu.CPUMem[0xFFFD] = 0x30;
    u32 ExpectedCycles = 2;

    // When:
    s32 CyclesUsed = cpu.Execute(ExpectedCycles);

    return 0;
}