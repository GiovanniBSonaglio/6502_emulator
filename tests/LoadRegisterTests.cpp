#include <gtest/gtest.h>
#include "cpu.h"

class LoadRegisterTests : public testing::Test {
	public:	
        virtual void SetUp(){
            cpu.Reset();
        }

        virtual void TearDown(){}
        CPU cpu;
};

static void VerfifyUnmodifiedFlagsFromLoadRegister(CPU& cpu, CPU& CPUCopy){
    // Checking if all flags except Negative and Zero are the same
	EXPECT_EQ(cpu.Status & 0b01111101, CPUCopy.Status & 0b01111101);
}

TEST_F(LoadRegisterTests, LDAImmediateCanLoadAValueIntoTheARegister){
    // Given:
    cpu.CPUMem[0xFFFC] = 0xA9; // LDA - IMM
    cpu.CPUMem[0xFFFD] = 0x30;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(10);

    // Expect:
    EXPECT_EQ(cpu.A, 0x30);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAZP0CanLoadAValueIntoTheARegister){
    // Given:
    cpu.CPUMem[0xFFFC] = 0xA5; // LDA - ZP0
    cpu.CPUMem[0xFFFD] = 0x30;
    cpu.CPUMem[0x0030] = 0x48;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(11);

    // Expect:
    EXPECT_EQ(cpu.A, 0x48);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAZPXCanLoadAValueIntoTheARegister){
    // Given:
	cpu.X = 0x10;
    cpu.CPUMem[0xFFFC] = 0xB5; // LDA - ZPX
    cpu.CPUMem[0xFFFD] = 0x30;
    cpu.CPUMem[0x0040] = 0x30;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(12);

    // Expect:
    EXPECT_EQ(cpu.A, 0x30);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAABSCanLoadAValueIntoTheARegister){
    // Given:
    cpu.CPUMem[0xFFFC] = 0xAD; // LDA - ABS
    cpu.CPUMem[0xFFFD] = 0x30;
    cpu.CPUMem[0xFFFE] = 0x48;
	cpu.CPUMem[0x4830] = 0x20;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(12);

    // Expect:
    EXPECT_EQ(cpu.A, 0x20);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAABXCanLoadAValueIntoTheARegister){
    // Given:
    cpu.X = 0x10;
    cpu.CPUMem[0xFFFC] = 0xBD; // LDA - ABX
    cpu.CPUMem[0xFFFD] = 0x20;
    cpu.CPUMem[0xFFFE] = 0x40;
	cpu.CPUMem[0x4030] = 0x15;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(12);

    // Expect:
    EXPECT_EQ(cpu.A, 0x15);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAABXCanHandlePageCrossing){
    // Given:
    cpu.X = 0x10;
    cpu.CPUMem[0xFFFC] = 0xBD; // LDA - ABX
    cpu.CPUMem[0xFFFD] = 0xFF;
    cpu.CPUMem[0xFFFE] = 0x40;
	cpu.CPUMem[0x410F] = 0x15;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(13);

    // Expect:
    EXPECT_EQ(cpu.A, 0x15);
    EXPECT_EQ(cpu.Cycles, 0); // If cycles != 0, then the page crossing wasnt handled correctly
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAABYCanLoadAValueIntoTheARegister){
    // Given:
    cpu.Y = 0x10;
    cpu.CPUMem[0xFFFC] = 0xB9; // LDA - ABY
    cpu.CPUMem[0xFFFD] = 0x20;
    cpu.CPUMem[0xFFFE] = 0x40;
	cpu.CPUMem[0x4030] = 0x15;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(12);

    // Expect:
    EXPECT_EQ(cpu.A, 0x15);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAABYCanHandlePageCrossing){
    // Given:
    cpu.Y = 0x10;
    cpu.CPUMem[0xFFFC] = 0xB9; // LDA - ABY
    cpu.CPUMem[0xFFFD] = 0xFF;
    cpu.CPUMem[0xFFFE] = 0x40;
	cpu.CPUMem[0x410F] = 0x15;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(13);

    // Expect:
    EXPECT_EQ(cpu.A, 0x15);
    EXPECT_EQ(cpu.Cycles, 0); // If cycles != 0, then the page crossing wasnt handled correctly
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAIZXCanLoadAValueIntoTheARegister){
    // Given:
    cpu.X = 0x10;
    cpu.CPUMem[0xFFFC] = 0xA1; // LDA - IZX
    cpu.CPUMem[0xFFFD] = 0x30;
    cpu.CPUMem[0x0040] = 0x28;
	cpu.CPUMem[0x0041] = 0x15;
    cpu.CPUMem[0x1528] = 0x20;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(14);

    // Expect:
    EXPECT_EQ(cpu.A, 0x20);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAIZYCanLoadAValueIntoTheARegister){
    // Given:
    cpu.Y = 0x10;
    cpu.CPUMem[0xFFFC] = 0xB1; // LDA - IZY
    cpu.CPUMem[0xFFFD] = 0xA4;
    cpu.CPUMem[0x00A4] = 0x51;
    cpu.CPUMem[0x00A5] = 0x3F;
	cpu.CPUMem[0x3F61] = 0x15;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(13);

    // Expect:
    EXPECT_EQ(cpu.A, 0x15);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAIZYCanHandlePageCrossing){
    // Given:
    cpu.Y = 0x10;
    cpu.CPUMem[0xFFFC] = 0xB1; // LDA - IZY
    cpu.CPUMem[0xFFFD] = 0xA4;
    cpu.CPUMem[0x00A4] = 0xFF;
    cpu.CPUMem[0x00A5] = 0x3F;
	cpu.CPUMem[0x400F] = 0x15;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(14);

    // Expect:
    EXPECT_EQ(cpu.A, 0x15);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDACanAffectTheZeroFlag){
	// Given:
    cpu.CPUMem[0xFFFC] = 0xA9; // LDA - IMM
    cpu.CPUMem[0xFFFD] = 0x00;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(10);

    // Expect:
    EXPECT_EQ(cpu.A, 0x00);
	EXPECT_TRUE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDACanAffectTheNegativeFlag){
	// Given:
    cpu.CPUMem[0xFFFC] = 0xA9; // LDA - IMM
    cpu.CPUMem[0xFFFD] = 0xFF;

    // When:
    CPU CPUCopy = cpu;
   	cpu.ExecuteNCycles(10);

    // Expect:
    EXPECT_EQ(cpu.A, 0xFF);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_TRUE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};