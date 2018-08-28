#include "VM.h"

WVM::WVM()
: debug("WVM_Debug_log.txt")
{
	debug.immediatelyPrintLogs();
	ip = 0;
	fp = 0;
}


WVM::~WVM()
{
}

// reads the file, then calls interpret on the resulting bytecode. No validation checking.
void WVM::executeFile(std::string filename) {
	std::ifstream file(filename);
	std::vector<int> bytes;
	std::string line;
	if (file.is_open()) {
		// Read the bytecode from the file
		while (getline(file, line))
		{
			bytes.push_back(stoi(line));
		}
		file.close();
		debug.LOG("bytecode successfully loaded from " + filename);
	} else {
		std::cout << "Error! could not open file!";
	}
	interpret(bytes);
}

// run the bytecode
int WVM::interpret(std::vector<int> bytecode) {
	for (; ip != bytecode.size(); ip++) {
		int instr = bytecode[ip];
		switch (instr) {
			// case statements
			case LITERAL: {
				// Read the next byte from the bytecode as a value.
				int value = bytecode[++ip];
				push(value);
				debug.LOG_VERBOSE("LITERAL value " + std::to_string(value));
				break;
			}
			case ADD: {
				int a = pop();
				int b = pop();
				debug.LOG_VERBOSE("ADD operands " + std::to_string(a) + " " + std::to_string(b));
				push(a + b);
				break;
			}
			case SUB: {
				int a = pop();
				int b = pop();
				debug.LOG_VERBOSE("SUB operands " + std::to_string(a) + " " + std::to_string(b));
				push(b - a);
				break;
			}
			case MUL: {
				int a = pop();
				int b = pop();
				debug.LOG_VERBOSE("MUL operands " + std::to_string(a) + " " + std::to_string(b));
				push(a * b);
				break;
			}
			case DIV: {
				int a = pop();
				int b = pop();
				debug.LOG_VERBOSE("DIV operands " + std::to_string(a) + " " + std::to_string(b));
				push(b / a);
				break;
			}
			case PRINT:
				// TODO... How to store values larger than a byte in the stack?
				// std::cout << pop();
				break;
			case LABEL:
				// TODO... How to store values larger than a byte in the stack?
				// Get string and store it in the labels map
				break;
			case LESS_THAN: {
				int a = pop();
				int b = pop();
				debug.LOG_VERBOSE("LESS_THAN operands " + std::to_string(a) + " " + std::to_string(b));
				push((b < a) ? 1 : 0);
				break;
			}
			case EQUAL: {
				int a = pop();
				int b = pop();
				debug.LOG_VERBOSE("EQAUL operands " + std::to_string(a) + " " + std::to_string(b));
				push((a == b) ? 1 : 0);
				break;
			}
			// TODO (JMP, JMPF, JMPT) check if next op code is a string literal and then check against jump labels
			case JMP: { // move the instruction pointer to the address of the next value in the program. 
				// Read the next byte from the bytecode as the instruction pointer.
				ip = (bytecode[++ip] - 1);
				debug.LOG_VERBOSE("JMP operand " + std::to_string(ip + 1));
				break;
			}
			case JMPT: {
				int addr = bytecode[++ip];
				if (pop()) ip = (addr - 1);
				debug.LOG_VERBOSE("JMPT operand " + std::to_string(ip));
				break;
			}
			case JMPF: {
				int addr = bytecode[++ip];
				if (!pop()) ip = (addr - 1);
				debug.LOG_VERBOSE("JMPF operand " + std::to_string(ip));
				break;
			}
			case GSTORE: {
				int value = pop();
				int addr = bytecode[++ip];
				if (addr > heap.size()) {
					heap.resize(addr + 1);
				}
				heap[addr] = value;
				debug.LOG_VERBOSE("GSTORE value " + std::to_string(value));
				break;
			}
			case GLOAD: {
				int addr = pop();
				// TODO figure out how to throw an exception in our language implementation... 
				//		Ignore instructions until we find a catch... ? 
				if (addr >= heap.size()) {
					std::cout << "ERROR: Attempting to access value outside of heap!"; 
					#ifndef NDEBUG
					ip = bytecode.size(); // Exit the loop instead of returning early so we still get our debug info
					#else
					return -1;
					#endif
				}
				int value = heap[addr];
				push(value);
				debug.LOG_VERBOSE("GLOAD value pushed " + std::to_string(value));
				break;
			}
			case LOAD: {
				int offset = bytecode[++ip];
				debug.LOG_VERBOSE("LOAD operand " + std::to_string(offset));
				// TODO figure out how to throw an exception in our language implementation... 
				//		Ignore instructions until we find a catch... ? 
				if (fp + offset >= stack.size()) {
					std::cout << "ERROR: Attempting to access value outside of heap!"; 
					#ifndef NDEBUG
					ip = bytecode.size(); // Exit the loop instead of returning early so we still get our debug info
					break;
					#else
					return -1;
					#endif
				}
				push(stack[fp+offset]);
				debug.LOG_VERBOSE("LOAD value pushed " + std::to_string(stack[fp + offset]));
			} break;
			case STORE: {
				int value = pop();
				int offset = bytecode[++ip];
				debug.LOG_VERBOSE("STORE operand " + std::to_string(offset));
				if (fp + offset >= heap.size()) {
					heap.resize(fp + offset + 1);
				}
				heap[fp + offset] = value;
				debug.LOG_VERBOSE("STORE value added to heap " + std::to_string(heap[fp + offset]));
			} break;
			case CALLF: {
				// we expect all args to be on the stack
				int addr = bytecode[++ip]; // First, the address of the function
				int argc = bytecode[++ip]; // Second, the number of arguments
				debug.LOG_VERBOSE("CALLF operands address: " + std::to_string(addr) + " number of args (on stack): " + std::to_string(argc));
				push(argc); // So we know how many args to pop
				push(fp); // So we can return to the current heap location
				push(ip); // So we can return to the current instruction being performed
				fp = (stack.size() - 1);
				ip = (addr - 1);
			} break;
			case RETF: {
				int rval = pop();
				while (fp < (stack.size() - 1)) pop();
				ip = pop();
				fp = pop();
				int argc = pop();
				while (argc) {
					pop();
					argc--;
				}
				debug.LOG_VERBOSE("RETF return value" + std::to_string(rval));
				push(rval);
			} break;
			// instead of tokenizing functions, for now, we will just ignore the code on the inside and assume the compiler knows where to point
			case FUN: {
				int isEndFun = 0;
				while (isEndFun != END_FUN) {
					// Ignore opcode arguments
					if (isEndFun == LITERAL) isEndFun = bytecode[++ip];
					if (isEndFun == JMP) isEndFun = bytecode[++ip];
					if (isEndFun == JMPT) isEndFun = bytecode[++ip];
					if (isEndFun == JMPF) isEndFun = bytecode[++ip];
					if (isEndFun == LOAD) isEndFun = bytecode[++ip];
					if (isEndFun == STORE) isEndFun = bytecode[++ip];
					if (isEndFun == GSTORE) isEndFun = bytecode[++ip];
					if (isEndFun == CALLF) isEndFun = bytecode[(ip += 2)];
					isEndFun = bytecode[++ip];
				}
				debug.LOG_VERBOSE("END_FUN found at instruction " + std::to_string(ip));
				break;
			}
			// we've reached the end of a function, but didn't return anything.
			case END_FUN: {
				debug.LOG_VERBOSE("END_FUN executed");
				while (fp < (stack.size() - 1)) pop();
				ip = pop();
				fp = pop();
				int argc = pop();
				while (argc) {
					pop();
					argc--;
				}
			}
			default:
				std::cout << "Unimplemented instruction type! " << bytecode[ip] << "\n";
				// TODO - Throw exception?
				break;
		}
	}
	
	int retVal = 0;
	if (!stack.empty()) retVal = stack.back();

	#ifndef NDEBUG
	if (!stack.empty()) {
		while (!stack.empty()) {
			std::cout << pop() << "\n";
		}
	}

	debug.printToFile();
	#endif

	return retVal;
}

void WVM::push(int val) {
	stack.push_back(val);
}

int WVM::pop() {
	try {
		int val = stack.back();
		stack.pop_back();
		return val;
	} catch (const std::out_of_range& oor) {
		debug.printToFile();
	}
	return -1;
}