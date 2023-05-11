/*
** $Id: linit.c $
** Initialization of libraries for lua.c and other clients
** See Copyright Notice in lua.h
*/


#define linit_c
#define LUA_LIB

/*
** If you embed Lua in your program and need to open the standard
** libraries, call luaL_openlibs in your program. If you need a
** different set of libraries, copy this file to your project and edit
** it to suit your needs.
**
** You can also *preload* libraries, so that a later 'require' can
** open the library, which is already linked to the application.
** For that, do the following code:
**
**  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
**  lua_pushcfunction(L, luaopen_modname);
**  lua_setfield(L, -2, modname);
**  lua_pop(L, 1);  // remove PRELOAD table
*/

#include "lprefix.h"


#include <stddef.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* debug is NOT OPENED by default */

#define REQUIRE_FUNC "function require(name) local B local A A = loaded[name] if A then return A end A = preload[name] if A then A = A() loaded[name] = A return A end A, B = loadfile(name..'.lua') if not A then error('file '..name..' is not found!\n'..B) end A = A() loaded[name] = A return A end"

LUA_API int luaopen_package(lua_State* L) {
	// some compability hacks :o
	lua_createtable(L, 0, 1);
	lua_pushstring(L, "loaded");
	lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
	lua_rawset(L, -3);
	lua_createtable(L, 0, 0);
	lua_setfield(L, -2, "preload");
	lua_setglobal(L, "package");
	luaL_loadstring(L, REQUIRE_FUNC);
	lua_setglobal(L, "require");
	return 0;
}

static const luaL_Reg loadedlibs[] = {
  {LUA_GNAME, luaopen_base},
	{"package", luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  {NULL, NULL}
};


LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib;
  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);  /* remove lib */
  }

}
