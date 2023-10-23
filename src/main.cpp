#include "globals.h"
#include "cpu.h"

int main()
{
    CPU cpu;
    cpu.Reset();

    cpu.CPUMem[0xFFFC] = 0xA9;
    cpu.CPUMem[0xFFFD] = 0x30;
	BYTE CyclesToRun = 10;

    // When:
    for (int i = 0; i < CyclesToRun; i++)
        cpu.Clock();

    return 0;
}