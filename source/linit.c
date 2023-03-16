/*
** $Id: linit.c,v 1.39.1.1 2017/04/19 17:20:42 roberto Exp $
** Standard minified lua library.
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
** You can't *preload* libraries :)
*/

#include "lprefix.h"

#include <stddef.h>

#include "lua.h"
#include "lauxlib.h"

// helper functions

static int addstackinfo(lua_State* L, int level) {
	lua_Debug ar;
  if (lua_getstack(L, level, &ar)) {
    lua_getinfo(L, "Sl", &ar);
    if (ar.currentline > 0) { 
      lua_pushfstring(L, "%s:%d: ", ar.short_src, ar.currentline);
			return 1;
    }
  }
	return 0;
}

static int throwerr(lua_State* L, const char* s, ...) {
	lua_pushstring(L, s);
	va_list args;
	va_start(args, s);
	int stat = addstackinfo(L, 1);
	lua_pushvfstring(L, s, args);
	if (stat) lua_concat(L, 2);
	va_end(args);
	lua_error(L);
	return 0; // unreachable
}

static void typecheck(lua_State* L, int arg, int t) {
	if (lua_type(L, arg) != t) throwerr(L, "%i : %s excepted", arg, lua_typename(L, t));
}

// we have NO ANY STANDARD LIBRARY now :D even no print() and assert()...
// so.. let's add ours functions

static int Btostring(lua_State* L) {
	lua_pushstring(L, lua_anytostring(L, 1));
	return 1;
}

static int Btonumber(lua_State* L) {
	if (lua_gettop(L) == 0) return 0;
	int succ = 0;
	lua_Number n = lua_tonumberx(L, 1, &succ);
	if (!succ)
		throwerr(L, lua_isstring(L, 1) ? "#1 : bad string" : "#1 : string excepted");
	lua_pushnumber(L, n);
	return 1;
}

static int Btype(lua_State* L) {
	int t = lua_type(L, 1);
	lua_pushstring(L, lua_typename(L, t));
	return 1;
}

static int Bnext(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	lua_settop(L, 2);
  if (lua_next(L, 1))
    return 2;
  else {
    lua_pushnil(L);
    return 1;
  }
}

// continuation
static int pcallcont(lua_State* L, int status, lua_KContext) {
	if (status != LUA_OK && status != LUA_YIELD) {
		lua_pushboolean(L, 0); lua_insert(L, 1); // status at first
		return 2;
	}
	return lua_gettop(L);
}

static int Bpcall(lua_State* L) {
	typecheck(L, 1, LUA_TFUNCTION);
	return pcallcont(L,
		lua_pcallk(L, lua_gettop(L) - 1, LUA_MULTRET, 0, 0, pcallcont), 0);
}

static int Berror(lua_State* L) {
	if (!lua_isstring(L, 1)) lua_pushstring(L, "unspecified error");
	int level = lua_tonumber(L, 2);
	if (!level) level = 1;
	lua_settop(L, 1);

	int s = addstackinfo(L, 1);
	if (s) {
		lua_insert(L, 1);
		lua_concat(L, 2);
	}
	
	lua_error(L);
	return 0;
}

static int Bgetmeta(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	if (!lua_getmetatable(L, 1)) return 0;
	return 1;
}

static int Bsetmeta(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	if (!lua_istable(L, 2) && !lua_isnoneornil(L, 2))
		throwerr(L, "#2 : table or nil excepted");
	lua_settop(L, 2);
	lua_setmetatable(L, 1);
	return 0;
}

static int Braweq(lua_State* L) {
	if (lua_gettop(L) < 2) throwerr(L, "2 arguments excepted!");
	lua_pushboolean(L, lua_rawequal(L, 1, 2));
	return 1;
}

static int Brawlen(lua_State* L) {
	if (lua_isnoneornil(L, 1)) throwerr(L, "#1 : table or string excepted!");
	lua_pushinteger(L, lua_rawlen(L, 1));
	return 1;
}

static int Brawget(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	if (lua_isnoneornil(L, 2)) throwerr(L, "#2: valid key excepted!");
	lua_settop(L, 2);
	lua_rawget(L, 1);
	return 1;
}

static int Brawset(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	if (lua_isnoneornil(L, 2)) throwerr(L, "#2: valid key excepted!");
	if (lua_gettop(L) < 3) lua_pushnil(L); // add third arg
	lua_settop(L, 3);
	lua_rawset(L, 1);
	return 1;
}

static int Bpairs(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	lua_pushcfunction(L, Bnext);
	lua_pushvalue(L, 1);
	lua_pushnil(L);
	return 3;
}

static int lua_loadbuffer(lua_State* L, const char* s, size_t len) {
	return lua_loadbufferx(L, s, len, "loadstring", "t");
}

static int Bloadstring(lua_State* L) {
	typecheck(L, 1, LUA_TSTRING);
	size_t len = 0;
	const char* c = lua_tolstring(L, 1, &len);
	lua_settop(L, 1);
	if (lua_loadbuffer(L, c, len)) lua_pushnil(L);
	else {
		lua_pushnil(L);
		lua_insert(L, 2);
	}
	return 1;
}

static int Bassert(lua_State* L) {
	if (lua_toboolean(L, 1)) return 0;
	lua_remove(L, 1); // remove bool

	if (lua_gettop(L) < 1) {
		lua_pushstring(L, "assertion failed!");
	}
	lua_settop(L, 1);
	int s = addstackinfo(L, 1);
	if (s) {
		lua_insert(L, 1); // swap
		lua_concat(L, 2);
	}
	lua_error(L);
	return 0;
}

static int Bselect(lua_State* L) {
	int n = lua_gettop(L);
	typecheck(L, 1, LUA_TNUMBER);
	lua_Integer i = lua_tointeger(L, 1);
	if (i == 0) { // return count
		lua_pushnumber(L, n);
		return 1;
	} else if (i < 0) i = n + i;
	else if (i > n) i = n;
	if (i < 1) throwerr(L, "index is out of range");
	return n - i;
};

struct funcentry {
	const char* name;
	lua_CFunction func;
};

static struct funcentry functions[] = {
	{"pairs",        Bpairs},
	{"setmetatable", Bsetmeta},
	{"getmetatable", Bgetmeta},
	{"error",        Berror},
	{"pcall",        Bpcall},
	{"next",         Bnext},
	{"type",         Btype},
	{"tonumber",     Btonumber},
	{"tostring",     Btostring},
	{"loadstring",   Bloadstring},
	{"assert",       Bassert},
	{"select",       Bselect},
	{"rawequal",     Braweq},
	{"rawlen",       Brawlen},
	{"rawget",       Brawget},
	{"rawset",       Brawset},
	{NULL, NULL}
};

LUALIB_API int luaopen_base (lua_State *L) {
	// load functions
	for (struct funcentry* i = functions; i->name; i++) {
		lua_pushcfunction(L, i->func);
		lua_setglobal(L, i->name);
	};

	lua_pushvalue(L, LUA_REGISTRYINDEX);
	lua_geti(L, -1, LUA_RIDX_GLOBALS);
	lua_setglobal(L, "_G");
	lua_pop(L, 1);
	lua_pushfstring(L, "Lua %s.%s", LUA_VERSION_MAJOR, LUA_VERSION_MINOR);
	lua_setglobal(L, "_VERSION");
	return 0;
};

LUALIB_API void luaL_openlibs (lua_State *L) {
	luaopen_base(L);
}

