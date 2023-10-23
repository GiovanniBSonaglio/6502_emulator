#include "globals.h"
#include "cpu.h"

int main()
{
    CPU cpu;
    cpu.Reset();    
    
    // Given:
    cpu.Y = 0x10;
    cpu.CPUMem[0xFFFC] = 0xB1; // LDA - IZY
    cpu.CPUMem[0xFFFD] = 0xA4;
    cpu.CPUMem[0x00A4] = 0x51;
    cpu.CPUMem[0x00A5] = 0x3F;
	cpu.CPUMem[0x3F61] = 0x15;

    // When:
   	cpu.ExecuteNCycles(13);

    return 0;
}