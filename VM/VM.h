#pragma once
#include "stdafx.h"
#include <map>
#include <stack>
#include <fstream>
#include "Debuglog.h"

enum Instruction : char {
	LITERAL = 0, // Requires 1 argument. E.G. LITERAL 54; // stores 54 on the stack.
	ADD,
	SUB,
	DIV,
	MUL,
	PRINT,
	LESS_THAN,
	EQUAL,
	// TODO implement string in bytecode for jump labels
	LABEL,
	JMP, // unconditional jump. 1 arg, instruction pointer
	JMPT, // Jump if true. 1 arg, instruction pointer
	JMPF, // Jump if false. 1 arg, instruction pointer
	GLOAD, // Load a global variable
	GSTORE, // Store a globale variable
	LOAD, // Local load
	STORE, // Local store
	CALLF, // Call globally-defined function
	RETF, // Return from globally-defined function
	FUN, // Declare a function
	END_FUN // end function delimeter
};

// Wade's virtual machine
class WVM
{
public:
	WVM();
	~WVM();

	// Run the VM
	int interpret(std::vector<int> bytecode);
	void executeFile(std::string filename);

private:
	// Stack memory functions
	void push(int val);
	int pop();

	std::vector<int> stack; // Memory stack
	std::vector<int> heap; // Memory heap
	int fp; // Frame pointer
	std::vector<int>::size_type ip; // Instruction pointer
	std::map<std::string, int> labels; // labels for control flow instructions <Label name, pointer position>

	// For debugging
	Debuglog debug;
};
