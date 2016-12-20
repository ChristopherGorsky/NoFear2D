#include "Debug.hpp"

Debug::Debug(void) {
	
	logFile.open("data/debug.txt");

	std::string applicationTitle;

	#ifdef NDEBUG
		applicationTitle = "Release Build: ";
	#else
		applicationTitle = "Debug Build: ";
	#endif

	applicationTitle += __DATE__;
	applicationTitle += " - ";
	applicationTitle += __TIME__;

	Print("%s", (char*) applicationTitle.c_str());
	Print("Essence Engine");
	Print("Building a dream, one line at a time.");
	Print("Engine by: 252++");
}

void Debug::Print(const char* format, ...) {

	char buffer[255];

	va_list args;
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);

	//write to the file and flush the buffer
	logFile << buffer << "\n";
	logFile.flush();

	//write to the log
	std::cout << buffer << std::endl;
	log.push_back(buffer);
}

Debug::~Debug() {
	logFile.close();
}