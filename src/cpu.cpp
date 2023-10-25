#include "cpu.h"
#include <cstdio>

CPU::CPU(){
    // Initializing lookup table with the default value
    for (auto &i : Lookup) i = {&CPU::XXX, &CPU::IMP, 2};

    // Populating lookup table
    Lookup[0x06] = {&CPU::ASL, &CPU::ZP0, 5};
    Lookup[0x0A] = {&CPU::ASL, &CPU::ACC, 2};
    Lookup[0x0E] = {&CPU::ASL, &CPU::ABS, 6};
    Lookup[0x16] = {&CPU::ASL, &CPU::ZPX, 6};
    Lookup[0x1E] = {&CPU::ASL, &CPU::ABX, 7};
    Lookup[0x21] = {&CPU::AND, &CPU::IZX, 6};
    Lookup[0x25] = {&CPU::AND, &CPU::ZP0, 3};
    Lookup[0x29] = {&CPU::AND, &CPU::IMM, 2};
    Lookup[0x31] = {&CPU::AND, &CPU::IZY, 5};
    Lookup[0x35] = {&CPU::AND, &CPU::ZPX, 4};
    Lookup[0x2D] = {&CPU::AND, &CPU::ABS, 4};
    Lookup[0x39] = {&CPU::AND, &CPU::ABY, 4};
    Lookup[0x3D] = {&CPU::AND, &CPU::ABX, 4};
    Lookup[0x60] = {&CPU::ADC, &CPU::ABS, 4};
    Lookup[0x61] = {&CPU::ADC, &CPU::IZX, 6};
    Lookup[0x65] = {&CPU::ADC, &CPU::ZP0, 3};
    Lookup[0x69] = {&CPU::ADC, &CPU::IMM, 2};
    Lookup[0x70] = {&CPU::ADC, &CPU::ABX, 4};
    Lookup[0x75] = {&CPU::ADC, &CPU::ZPX, 4};
    Lookup[0x79] = {&CPU::ADC, &CPU::ABY, 4};
    Lookup[0x71] = {&CPU::ADC, &CPU::IZY, 5};
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

/********************
 * Helper Functions
********************/

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

void CPU::WriteByte(WORD Addr, BYTE Value){
    CPUMem[Addr] = Value;
}

void CPU::WriteWord(WORD Addr, WORD Value){
    CPUMem[Addr] = Value & 0x00FF;
    CPUMem[Addr+1] = Value & 0xFF00;
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

/********************
 * Operates
********************/

BYTE CPU::ADC(){
    // TODO: Handle decimal mode
	BYTE Data = ReadByte(AbsoluteAddr);

    // Performed sum in 16bit to get overflow
    WORD Sum = A + Data + GetFlag(C);

    SetFlag(C, Sum > 0xFF);
    SetFlag(Z, Sum & 0x00FF == 0x00);
    SetFlag(N, Sum & 0x80);
    SetFlag(V, ~( (A ^ Data) & 0x80) && ( (A ^ Sum) & 0x80) );

    A = Sum & 0x00FF;

    return 1;
}

BYTE CPU::AND(){
	BYTE Data = ReadByte(AbsoluteAddr);
    BYTE A = A & Data;

    SetFlag(Z, A == 0x00);
    SetFlag(N, A & 0x80);

    return 1;
}

BYTE CPU::ASL(){
    BYTE Result, Data;
    
    if(Lookup[InstructionAddr].AddrMode == &CPU::ACC){
        Data = A;
        Result = Data << 1;
        A = Result;
    }else{
        Data = ReadByte(AbsoluteAddr);
        Result = Data << 1;
        WriteByte(AbsoluteAddr, Result);
    }

    SetFlag(C, Data & 0x80);
    SetFlag(Z, Result == 0x00);
    SetFlag(N, Result & 0x80);

    return 1;
}

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

/********************
 * Addressing Modes
********************/

BYTE CPU::ACC(){
    return 0;
}

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