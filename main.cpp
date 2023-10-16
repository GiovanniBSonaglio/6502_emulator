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
    cpu.Execute(2);
    return 0;
}