#pragma once
#include "stdafx.h"
#include <map>
#include <stack>
#include <fstream>

enum Instruction : char {
	LITERAL = 0,
	ADD,
	SUB,
	DIV,
	MUL,
	PRINT,
	LABEL,
};

class VM
{
public:
	VM();
	~VM();

	// Run the VM
	void interpret(std::vector<char> bytecode);
	void readFile(std::string filename);

private:
	// Stack memory functions
	void push(int val);
	int pop();

	std::stack<int> stack; // Memory stack
	std::vector<char>::size_type ip; // Instruction pointer
	std::map<std::string, int> labels; // labels for control flow instructions <Label name, pointer position>
};

