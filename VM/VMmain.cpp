// VM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VM.h"


int main()
{
	VM vm;
	// std::vector<char> bytes;
	// bytes.push_back(LITERAL);
	// bytes.push_back(2);
	// bytes.push_back(LITERAL);
	// bytes.push_back(6);
	// bytes.push_back(DIV);
	// vm.interpret(bytes);
	vm.readFile("test.wade");
	while (1);
    return 0;
}

