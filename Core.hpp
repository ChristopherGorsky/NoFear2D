#include "SDL2.hpp"
#include "GameScene.hpp"
#include "SceneHandler.hpp"
#include "ResourceManager.hpp"
#include "Graphics.hpp"
#include <string.h>
#include <string>
#include <ctime>
#include<iostream>
#include "Lua.hpp"
#pragma once

class Core {
public:
	Core();
	~Core();
	void run(SDL_Window* wnd, lua_State* l);
private:
	bool m_running;
};

static int lua_debugPrint(lua_State* l);
