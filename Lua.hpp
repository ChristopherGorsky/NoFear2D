#pragma once
#include <stdio.h>
#include "Debug.hpp"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

lua_State* lua_createState(void);
void lua_stackDump(lua_State* l);
void lua_registerGlobal(lua_State* l, void* obj, const char* name);
void lua_printError(lua_State* l);

const char* lua_getString(lua_State* l, const char* name);
int lua_getInteger(lua_State* l, const char* name);
bool lua_getBoolean(lua_State* l, const char* name);
void* lua_getUserData(lua_State* l, const char* name);