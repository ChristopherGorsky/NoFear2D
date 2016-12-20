#include "Engine.hpp"
#undef main

int main(int argc, const char* argv[]) {

	Engine l_eng;
	l_eng.RunConfigAndInitilize("config.lua");

	return 0;
}