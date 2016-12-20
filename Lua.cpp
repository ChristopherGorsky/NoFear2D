#include "Lua.hpp"

void lua_stackDump(lua_State *L) {
	
	Debug& dbg = Debug::GetInstance();
	dbg.Print("---------- [Lua Stack Dump] ----------");

	int top = lua_gettop(L);
	dbg.Print("(btm:top)\t\tStack Size: %i", top);

	for (int i = 1; i <= top; i++) {  /* repeat for each level */
		
		int t = lua_type(L, i);
		switch (t) {

		case LUA_TSTRING: 
			dbg.Print("(%i,%i) `%s'", i, i - (top + 1), lua_tostring(L, i));
			break;

		case LUA_TBOOLEAN:
			dbg.Print("(%i,%i) %s", i, i - (top + 1), lua_toboolean(L, i) ? "TRUE" : "FALSE");
			break;

		case LUA_TNUMBER:
			dbg.Print("(%i,%i) %g", i, i - (top + 1), lua_tonumber(L, i));
			break;

		default: 
			dbg.Print("(%i,%i) %s", i, i - (top + 1), lua_typename(L, t));
			break;
		}
	}

	dbg.Print("--------------------------------------");
}

void lua_registerGlobal(lua_State* l, void* obj, const char* name) {

	lua_pushlightuserdata(l, obj);
	lua_setglobal(l, name);
}

void lua_printError(lua_State * l) {

	Debug& dbg = Debug::GetInstance();
	dbg.Print("Error: %s", lua_tostring(l, 1));
	lua_pop(l, 1);
}

lua_State* lua_createState(void) {

	lua_State* l = luaL_newstate();
	luaL_openlibs(l);

	Debug& dbg = Debug::GetInstance();
	dbg.Print("Initializing %s", lua_getString(l, "_VERSION"));

	return l;
}

const char* lua_getString(lua_State* l, const char* name) {

	lua_getglobal(l, name);
	const char* global = lua_tostring(l, 1);
	lua_pop(l, 1);

	return global;
}

int lua_getInteger(lua_State* l, const char* name) {

	lua_getglobal(l, name);
	int global = lua_tointeger(l, 1);
	lua_pop(l, 1);

	return global;
}

bool lua_getBoolean(lua_State* l, const char* name) {
	
	lua_getglobal(l, name);
	bool global = bool(lua_toboolean(l, 1));
	lua_pop(l, 1);

	return global;
}

void* lua_getUserData(lua_State* l, const char* name) {

	lua_getglobal(l, name);
	void* global = lua_touserdata(l, 1);
	lua_pop(l, 1);

	return global;
}