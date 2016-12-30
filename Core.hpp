#include "SDL2.hpp"
#include "ResourceManager.hpp"
#include "Graphics.hpp"
#include <string.h>
#include <string>
#include <ctime>
#include <iostream>
#include "Lua.hpp"

#define MAX_CONSOLE_LINES 6
#define SYS_FONT_SIZE 18

#ifdef _WIN32
#include <Windows.h>
#endif

#pragma once

class Core {
public:
	Core();
	~Core();
	void run(SDL_Window* wnd, lua_State* l);
private:
	void registerLuaCFunctions(lua_State* l);
	void renderLogConsole(const Dimension<int>& screen, TTF_Font* sysFont);
	bool m_running;
};

static int lua_debugPrint(lua_State* l);
