#include "globals.h"
#include "cpu.h"

int main()
{
    CPU cpu;
    cpu.Reset();    
    
    // Given:
    cpu.A = 0x80;
    cpu.CPUMem[0xFFFC] = 0x0A; // ASL - ACC

    // When:
   	cpu.ExecuteNCycles(10);

    return 0;
}