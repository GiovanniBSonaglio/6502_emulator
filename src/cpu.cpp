#include "cpu.h"
#include <cstdio>

CPU::CPU(){
    // Initializing lookup table with the default value
    for (auto &i : Lookup) i = {&CPU::XXX, &CPU::IMP, 2};

    // Populating lookup table
    Lookup[0xA0] = {&CPU::LDY, &CPU::IMM, 2};
    Lookup[0xA1] = {&CPU::LDA, &CPU::IZX, 6};
    Lookup[0xA2] = {&CPU::LDX, &CPU::IMM, 2};
    Lookup[0xA4] = {&CPU::LDY, &CPU::ZP0, 3};
    Lookup[0xA5] = {&CPU::LDA, &CPU::ZP0, 3};
    Lookup[0xA6] = {&CPU::LDX, &CPU::ZP0, 3};
    Lookup[0xA9] = {&CPU::LDA, &CPU::IMM, 2};
    Lookup[0xAC] = {&CPU::LDY, &CPU::ABS, 4};
    Lookup[0xAD] = {&CPU::LDA, &CPU::ABS, 4};
    Lookup[0xAE] = {&CPU::LDX, &CPU::ABS, 4};
    Lookup[0xB1] = {&CPU::LDA, &CPU::IZY, 5};
    Lookup[0xB4] = {&CPU::LDY, &CPU::ZPX, 4};
    Lookup[0xB5] = {&CPU::LDA, &CPU::ZPX, 4};
    Lookup[0xB6] = {&CPU::LDX, &CPU::ZPY, 4};
    Lookup[0xB9] = {&CPU::LDA, &CPU::ABY, 4};
    Lookup[0xBC] = {&CPU::LDY, &CPU::ABX, 4};
    Lookup[0xBD] = {&CPU::LDA, &CPU::ABX, 4};
    Lookup[0xBE] = {&CPU::LDX, &CPU::ABY, 4};  
}

void CPU::Clock(){
    if(Cycles == 0){
        InstructionAddr = FetchByte();

        Cycles = Lookup[InstructionAddr].Cycles;
        
        // Calling functions and retrieving the number of "extra" cycles
        BYTE AdditionalCyclesFromAddrMode = (this->*Lookup[InstructionAddr].AddrMode)();
        BYTE AdditionalCyclesFromOperate = (this->*Lookup[InstructionAddr].Operate)();

        Cycles += (AdditionalCyclesFromAddrMode & AdditionalCyclesFromOperate);
        
        // Always set the unused status flag bit to 1
		SetFlag(U, true);
    }

	Cycles--;
}

/********** Helper Function **********/

void CPU::ExecuteNCycles(BYTE CyclesToRun){
    for (BYTE i = 0; i < CyclesToRun; i++)
        Clock();
}

BYTE CPU::FetchByte(){
    BYTE Data = CPUMem[PC];
    PC++;
    return Data;
}

WORD CPU::FetchWord(){
    BYTE LoByteData = CPUMem[PC];
    BYTE HiByteData = CPUMem[PC+1];
    PC += 2;

    WORD Data = AppendBytes(LoByteData, HiByteData);
    return Data;
}

BYTE CPU::ReadByte(WORD Addr){
    BYTE Data = CPUMem[Addr];
    return Data;
}

WORD CPU::ReadWord(WORD Addr){
    BYTE LoByteData = CPUMem[Addr];
    BYTE HiByteData = CPUMem[Addr+1];

    WORD Data = AppendBytes(LoByteData, HiByteData);
    return Data;
}

WORD CPU::AppendBytes(BYTE LoByte, BYTE HiByte){
    return LoByte | (HiByte << 8);
}

void CPU::SetFlag(Flags Flag, bool Value){
	if(Value)
		Status |= Flag;
	else
		Status &= ~Flag;
}

BYTE CPU::GetFlag(Flags Flag){
    return ((Status & Flag) > 0) ? 1 : 0;
}

/************* Operates *************/

BYTE CPU::LDA(){
	A = ReadByte(AbsoluteAddr);
	SetFlag(Z, A == 0x00);
	SetFlag(N, A & 0x80);
	return 1;
}

BYTE CPU::LDX(){
	X = ReadByte(AbsoluteAddr);
	SetFlag(Z, X == 0x00);
	SetFlag(N, X & 0x80);
	return 1;
}

BYTE CPU::LDY(){
	Y = ReadByte(AbsoluteAddr);
	SetFlag(Z, Y == 0x00);
	SetFlag(N, Y & 0x80);
	return 1;
}

BYTE CPU::XXX(){
	return 0;
}

/********** Addressing Modes **********/

BYTE CPU::IMM(){
    AbsoluteAddr = PC++;
    return 0;
}

BYTE CPU::ZP0(){
    AbsoluteAddr = FetchByte();
    return 0;
}

BYTE CPU::ZPX(){
    AbsoluteAddr = FetchByte() + X;
    AbsoluteAddr &= 0x00FF; // Limiting to zero page
    return 0;
}

BYTE CPU::ZPY(){
    AbsoluteAddr = FetchByte() + Y;
    AbsoluteAddr &= 0x00FF; // Limiting to zero page
    return 0;
}

BYTE CPU::ABS(){
    AbsoluteAddr = FetchWord();
    return 0;
}

BYTE CPU::ABX(){
    WORD AbsoluteAddrBeforeX = FetchWord();
    AbsoluteAddr = AbsoluteAddrBeforeX + X;

    // Handling page crossing
    if((AbsoluteAddrBeforeX ^ AbsoluteAddr) >> 8)
		return 1;
    else
        return 0;
}

BYTE CPU::ABY(){
    WORD AbsoluteAddrBeforeY = FetchWord();
    AbsoluteAddr = AbsoluteAddrBeforeY + Y;

    // Handling page crossing
    if((AbsoluteAddrBeforeY ^ AbsoluteAddr) >> 8)
		return 1;
    else
        return 0;
}

BYTE CPU::IZX(){
    WORD IndirectAddr = FetchByte();
    IndirectAddr += X;
    AbsoluteAddr = ReadWord(IndirectAddr);
    return 0;
}

BYTE CPU::IZY(){
    BYTE IndirectAddr = FetchByte();
    WORD AbsoluteAddrBeforeY = ReadWord(IndirectAddr);
    AbsoluteAddr = AbsoluteAddrBeforeY + Y;

    // Handling page crossing
    if((AbsoluteAddrBeforeY ^ AbsoluteAddr) >> 8)
		return 1;
    else
        return 0;
}

BYTE CPU::IMP(){
    return 0;
}