#include "VM.h"

VM::VM()
{
	ip = 0;
}


VM::~VM()
{
}

// reads the file, then calls interpret on the resulting bytecode. In the event of invalid bytecode, sends an empty vector to interpret.
void VM::readFile(std::string filename) {
	std::ifstream file(filename);
	std::vector<char> bytes;
	std::string line;
	if (file.is_open()) {
		// Read the bytecode from the file
		while (getline(file, line))
		{
			std::cout << line << '\n';
		}
		file.close();
	} else {
		std::cout << "Error! could not open file!";
	}
	interpret(bytes);
}

void VM::interpret(std::vector<char> bytecode) {
	for (; ip != bytecode.size(); ip++) {
		char instr = bytecode[ip];
		switch (instr) {
			// case statements
			case LITERAL: {
				// Read the next byte from the bytecode as a value.
				int value = bytecode[++ip];
				push(value);
				break;
			}
			case ADD: {
				int a = pop();
				int b = pop();
				push(a + b);
				break;
			}
			case SUB: {
				int a = pop();
				int b = pop();
				push(a - b);
				break;
			}
			case MUL: {
				int a = pop();
				int b = pop();
				push(a * b);
				break;
			}
			case DIV: {
				int a = pop();
				int b = pop();
				push(a / b);
				break;
			}
			case PRINT:
				// TODO... How to store values larger than a byte in the stack?
				std::cout << pop();
				break;
			case LABEL:
				// TODO... How to store values larger than a byte in the stack?
				// Get string and store it in the labels map
				break;
			default:
				std::cout << "Unimplemented instruction type!\n";
				break;
		}
	}

	if (!stack.empty()) {
		while (!stack.empty()) {
			std::cout << pop() << "\n";
		}
	}
}

void VM::push(int val) {
	stack.push(val);
}

int VM::pop() {
	int val = stack.top();
	stack.pop();
	return val;
}