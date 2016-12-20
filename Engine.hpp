#include "Debug.hpp"
#include "SDL2.hpp"
#include "Core.hpp"
#include <iostream>
#include <fstream>
#include <time.h>
#include "Graphics.hpp"
#include "Lua.hpp"
#pragma once

class Engine {

public:

	Engine();
	~Engine();

	void DevMode(bool on) { m_devMode = on; };
	bool Initialize(const char* title, int width, int height, bool fullScreen);
	bool RunConfigAndInitilize(const char* file);
	SDL_Window* GetWindowHandle(void) const { return m_window; }

private:
	SDL_GLContext m_context;
	SDL_Window* m_window;
	int m_width;
	int m_height;
	bool m_devMode;
	lua_State* m_luaState;
};

