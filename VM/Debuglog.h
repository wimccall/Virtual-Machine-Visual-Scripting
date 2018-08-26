#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <fstream>

// #define DEBUGLOG_LEVEL 3 // 3 is most verbose
#ifndef DEBUGLOG_LEVEL
#define DEBUGLOG_LEVEL 1
#endif

class Debuglog
{
public:
	Debuglog();
	Debuglog(std::string filename);
	~Debuglog();
	void LOG_ALWAYS(std::string message); // Priority 1
	void LOG(std::string message); // Priority 2
	void LOG_VERBOSE(std::string message); // Priority 3
	void printToFile(); // Empty the buffer to the file
	void immediatelyPrintLogs(); // sets bImmediatelyPrint to true
	void waitToPrint(); // sets bImmediatelyPrint to false
	std::string getStringCurrentTimeMS();

private:
	std::string logBuffer;
	bool bImmediatelyPrint = false;
	std::ofstream file;
};