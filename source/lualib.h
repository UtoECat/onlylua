/*
** $Id: lualib.h,v 1.45.1.1 2017/04/19 17:20:42 roberto Exp $
** Lua standard libraries
** See Copyright Notice in lua.h
*/


#ifndef lualib_h
#define lualib_h

#include "lua.h"

/* version suffix for environment variable names */
#define LUA_VERSUFFIX          "_" LUA_VERSION_MAJOR "_" LUA_VERSION_MINOR

LUAMOD_API int (luaopen_base)      (lua_State *L);
LUAMOD_API int (luaopen_coroutine) (lua_State *L);
LUAMOD_API int (luaopen_math)      (lua_State *L);

/* open all previous libraries */
LUALIB_API void (luaL_openlibs)    (lua_State *L);

#if !defined(lua_assert)
#define lua_assert(x)	((void)0)
#endif


#endif
