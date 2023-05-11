/*
** See Copyright Notice in lua.h
*/

#define lpackage_c
#define LUA_LIB

#include "lprefix.h"


#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lualib/bcmap.h"

LUA_BCDEF(require, 
"function require(name) \
	local B local A \
	\
	A = loaded[name] \
	if A then \
		return A \
	end \
	\
	A = preload[name] \
	if A then \
		A = A() \
		loaded[name] = A \
		return A \
	end \
	 \
	A, B = loadfile(name..'.lua') \
	if not A then \
		error('file '..name..' is not found!\n'..B) \
	end \
	A = A() \
	loaded[name] = A \
	return A \
end")

LUAMOD_API int (luaopen_package) (lua_State *L) {
	// some compability hacks :o
	lua_createtable(L, 0, 1);
	lua_pushstring(L, "loaded");
	lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
	lua_rawset(L, -3);
	lua_createtable(L, 0, 0);
	lua_setfield(L, -2, "preload");
	LUA_BCLOAD(L, require);
	lua_setglobal(L, "require");
	return 0;
}
