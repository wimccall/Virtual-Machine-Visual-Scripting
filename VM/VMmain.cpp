// VM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VM.h"
#include "Debuglog.h"
#ifndef NDEBUG
#include "BytecodePrograms.h"
#endif

int main(int argc, char** argv)
{
	// Initialize our data structures.
	WVM vm;

	// If an argument has been passed in, assume it is a file name and read the file as bytecode
	if (argc > 1) vm.executeFile(argv[1]);

	// Hardcoded program for testing. This is easier than manually writing bytecode because we can use our enum.
	#ifndef NDEBUG
	else {
		VMFibonacciImp(15);
	}
	#endif

	// In debug mode, don't quit when the program ends.
	#ifndef NDEBUG
	while (1);
	#endif
    return 0;
}