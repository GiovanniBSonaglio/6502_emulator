#include <gtest/gtest.h>
#include "cpu.h"

class LoadRegisterTests : public testing::Test
{
	public:	
        virtual void SetUp() {
            cpu.Reset();
        }

        virtual void TearDown() {}
        CPU cpu;
};

static void VerfifyUnmodifiedFlagsFromLoadRegister(CPU& cpu, CPU& CPUCopy) {
	EXPECT_EQ(cpu.Status & 0b01111101, CPUCopy.Status & 0b01111101); // Checking if all flags except Negative and Zero are the same
}

TEST_F(LoadRegisterTests, LDAImmediateCanLoadAValueIntoTheARegister) {
    // Given:
    cpu.CPUMem[0xFFFC] = 0xA9; // LDA - IMM
    cpu.CPUMem[0xFFFD] = 0x30;
	BYTE CyclesToRun = 10;

    // When:
    CPU CPUCopy = cpu;
    for (int i = 0; i < CyclesToRun; i++)
        cpu.Clock();

    // Expect:
    EXPECT_EQ(cpu.A, 0x30);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAImmediateCanAffectTheZeroFlag) {
	// Given:
    cpu.CPUMem[0xFFFC] = 0xA9; // LDA - IMM
    cpu.CPUMem[0xFFFD] = 0x00;
	BYTE CyclesToRun = 10;

    // When:
    CPU CPUCopy = cpu;
    for (int i = 0; i < CyclesToRun; i++)
        cpu.Clock();

    // Expect:
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_TRUE(cpu.GetFlag(cpu.Z));
	EXPECT_FALSE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAImmediateCanAffectTheNegativeFlag) {
	// Given:
    cpu.CPUMem[0xFFFC] = 0xA9; // LDA - IMM
    cpu.CPUMem[0xFFFD] = 0xFF;
	BYTE CyclesToRun = 10;

    // When:
    CPU CPUCopy = cpu;
    for (int i = 0; i < CyclesToRun; i++)
        cpu.Clock();

    // Expect:
    EXPECT_EQ(cpu.A, 0xFF);
    EXPECT_EQ(cpu.Cycles, 0);
	EXPECT_FALSE(cpu.GetFlag(cpu.Z));
	EXPECT_TRUE(cpu.GetFlag(cpu.N));
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};