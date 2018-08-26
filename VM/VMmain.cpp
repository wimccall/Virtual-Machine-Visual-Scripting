// VM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VM.h"
#include "Debuglog.h"

int main(int argc, char** argv)
{
	WVM vm;
	Debuglog debug("program_main_debug_log.txt");
	// vm.executeFile(argv[1]);

	const int fib = 1;
	std::vector<int> fibonacci = {
		// int fib(n) {
		//     if(n == 0) return 0;
		FUN, LOAD, -3, // Load the last function arg
		LITERAL, 0, // push 0 onto stack
		EQUAL, JMPF, 11, // check if N == 0. If not, jump to instruction 11
		LITERAL, 0, RETF, // If N == 0, return 0
		//     if(n < 3) return 1;
		LOAD, -3, // Load the argument again
		LITERAL, 3, // put 3
		LESS_THAN, // check if 3 is less than N
		JMPF, 21, // if 3 is not less than N goto 21
		LITERAL, 1, RETF, // Return 1
		//     else return fib(n-1) + fib(n-2);
		LOAD, -3, // load the function arg again
		LITERAL, 1, // PUT 1
		SUB, // Put N - 1
		CALLF, fib, 1, // CALL the fib function with 1 arg from stack
		LOAD, -3, // Load N again
		LITERAL, 2, // Put 2
		SUB, // Put N - 2
		CALLF, fib, 1, // CALL fib again
		ADD, // ADD the return value from the 2 FIB calls
		RETF,
		END_FUN, // function delimeter
		// Main () {
		LITERAL, 21, // put 6
		CALLF, fib, 1, // Call fib(6)
		// End. 
	};
	vm.interpret(fibonacci);

	// In debug mode, don't quit when the program ends.
	#ifndef NDEBUG
	debug.LOG_ALWAYS("Program done.");
	debug.printToFile();
	while (1);
	#endif
    return 0;
}