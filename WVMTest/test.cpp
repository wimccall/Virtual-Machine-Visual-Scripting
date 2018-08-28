#include "pch.h"
#include "BytecodePrograms.h"

TEST(BytecodePrograms, Fibonacci) {
	EXPECT_EQ(8, VMFibonacciImp(6));
	EXPECT_EQ(34, VMFibonacciImp(9));
	EXPECT_EQ(13, VMFibonacciImp(7));
}