#include "memory.h"

void Memory::Initialize(){
    for (u32 i = 0; i < Memory::MAX_MEM; i++)
    {
        Memory::Data[i] = 0;
    }
}