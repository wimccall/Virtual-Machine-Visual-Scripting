#include "stdafx.h"
#include "Debuglog.h"


Debuglog::Debuglog()
{
	#ifndef NDEBUG
	file.open("DEBUG_LOG.txt");
	#endif
}

Debuglog::Debuglog(std::string filename)
{
	#ifndef NDEBUG
	file.open(filename);
	#endif
}


Debuglog::~Debuglog()
{
	#ifndef NDEBUG
	printToFile();
	file.close();
	#endif
}

std::string Debuglog::getStringCurrentTimeMS() {
	#ifndef NDEBUG
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::string s = std::ctime(&t);
	s.pop_back();
	return s;
	#else
	return "";
	#endif
}

void Debuglog::LOG(std::string message) {
	#ifndef NDEBUG
	if (DEBUGLOG_LEVEL >= 2) {
		LOG_ALWAYS(message);
	}
	#endif
}

void Debuglog::LOG_VERBOSE(std::string message) {
	#ifndef NDEBUG
	if (DEBUGLOG_LEVEL >= 3) {
		LOG_ALWAYS(message);
		std::cout << message + "\n";
	}
	#endif
}

void Debuglog::LOG_ALWAYS(std::string message) {
	#ifndef NDEBUG
	if (DEBUGLOG_LEVEL >= 1) {
		if (bImmediatelyPrint) {
			file << getStringCurrentTimeMS() + ": " + message + "\n";
		} else {
			logBuffer += getStringCurrentTimeMS() + ": " + message + "\n";
		}
	}
	#endif
}

void Debuglog::printToFile() {
	#ifndef NDEBUG
	file << logBuffer;
	logBuffer.clear();
	#endif
}

void Debuglog::immediatelyPrintLogs() {
	#ifndef NDEBUG
	bImmediatelyPrint = true;
	printToFile();
	#endif
}

void Debuglog::waitToPrint() {
	#ifndef NDEBUG
	bImmediatelyPrint = false;
	#endif
}