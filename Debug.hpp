#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <fstream>
#pragma once

class Debug {

public:
	
	Debug(void);
	void Print(const char* format, ...);
	~Debug();

	static Debug& GetInstance(void) {
		static Debug instance;
		return instance;
	}

	std::vector<std::string>& getLog(void) {
		return log;
	}

private:

	std::vector<std::string> log;
	std::ofstream logFile;
};