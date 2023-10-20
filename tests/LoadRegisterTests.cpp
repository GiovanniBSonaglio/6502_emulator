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

static void VerfifyUnmodifiedFlagsFromLoadRegister(const CPU& cpu, const CPU& CPUCopy) {
	EXPECT_EQ(cpu.Flag.C, CPUCopy.Flag.C);
	EXPECT_EQ(cpu.Flag.I, CPUCopy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, CPUCopy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, CPUCopy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, CPUCopy.Flag.V);
}

TEST_F(LoadRegisterTests, LDAImmediateCanLoadAValueIntoTheARegister) {
    // Given:
    cpu.CPUMem[0xFFFC] = CPU::INS_LDA_IM;
    cpu.CPUMem[0xFFFD] = 0x30;
    u32 ExpectedCycles = 2;

    // When:
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(ExpectedCycles);

    // Expect:
    EXPECT_EQ(cpu.A, 0x30);
    EXPECT_EQ(CyclesUsed, ExpectedCycles);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAImmediateCanAffectTheZeroFlag) {
	// Given:
	cpu.A = 0x30;
	cpu.CPUMem[0xFFFC] = CPU::INS_LDA_IM;
	cpu.CPUMem[0xFFFD] = 0x0;
	CPU CPUCopy = cpu;

	// When:
	cpu.Execute(2);

	// Expect:
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};

TEST_F(LoadRegisterTests, LDAImmediateCanAffectTheNegativeFlag) {
	// Given:
	cpu.A = 0x44;
	cpu.CPUMem[0xFFFC] = CPU::INS_LDA_IM;
	cpu.CPUMem[0xFFFD] = 0xFF;
	CPU CPUCopy = cpu;

	// When:
	cpu.Execute(2);

	// Expect:
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	VerfifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
};