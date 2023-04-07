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
	if (lua_type(L, arg) != t) throwerr(L, "%d : %s excepted", arg, lua_typename(L, t));
}

// so.. let's add our own functions

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

static int Binext(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	unsigned int i = (unsigned int)lua_tonumber(L, 2); // or 0

  if (lua_inext(L, 1, &i)) {
		lua_pushinteger(L, (lua_Integer)i);
    return 3;
	} else {
    return 0;
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
	int level = lua_tonumber(L, 2);
	if (!level) level = 1;
	lua_settop(L, 1);
	if (!lua_isstring(L, 1)) {
		lua_pushstring(L, "unspecified error");
		lua_insert(L, 1);
		lua_settop(L, 1);
	}

	int s = addstackinfo(L, level);
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

static int privpairs(lua_State* L) {
	unsigned int i = lua_tointeger(L, lua_upvalueindex(1));
	int status = lua_inext(L, 1, &i);
	lua_pushinteger(L, i);
	lua_copy(L, -1, lua_upvalueindex(1));
	return status ? 3 : 0;
}

static int Bfastpairs(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	lua_pushinteger(L, 0);
	lua_pushcclosure(L, privpairs, 1);
	lua_pushvalue(L, 1);
	lua_pushnil(L);
	return 3;
}

static int Bpairs(lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	lua_pushcclosure(L, Bnext, 0);
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

static int Bnewtable(lua_State* L) {
	int al = lua_tonumber(L, 1);
	int hl = lua_tonumber(L, 2);
	lua_createtable(L, al, hl);
	return 1;
}

static int Bcleartable (lua_State* L) {
	typecheck(L, 1, LUA_TTABLE);
	lua_cleartable(L, 1);
	return 1;
}

static const char *const opts[] = {"stop", "restart", "collect",
    "count", "step", "setpause", "setstepmul",
    "isrunning", NULL};

static const int optsnum[] = {LUA_GCSTOP, LUA_GCRESTART, LUA_GCCOLLECT,
    LUA_GCCOUNT, LUA_GCSTEP, LUA_GCSETPAUSE, LUA_GCSETSTEPMUL,
    LUA_GCISRUNNING};

#ifndef LUA_NO_COLLECTGARBAGE

#include <string.h>

static int Bgc (lua_State* L) {
	const char* c = lua_tostring(L, 1);
	c = c ? c : "collect";
	int arg1;
	int extra = lua_tonumber(L, 2);

	for (int i = 0; opts[i]; i++) 
		if (strcmp(c, opts[i]) == 0) {
			arg1 = optsnum[i];
			goto collect;
		}
	throwerr(L, "#1 : bad string argument");
	collect:
	int res = lua_gc(L, arg1, extra);

	switch (arg1) {
    case LUA_GCCOUNT: {
      int b = lua_gc(L, LUA_GCCOUNTB, 0);
      lua_pushnumber(L, (lua_Number)res + ((lua_Number)b/1024));
      return 1;
    }
    case LUA_GCSTEP: case LUA_GCISRUNNING: {
      lua_pushboolean(L, res);
      return 1;
    }
    default: {
      lua_pushinteger(L, res);
      return 1;
    }
  }
}

#endif

struct funcentry {
	const char* name;
	lua_CFunction func;
};

static struct funcentry functions[] = {
	{"assert",        Bassert},
#ifndef LUA_NO_COLLECTGARBAGE
	{"collectgarbage", Bgc},
#endif
	{"pairs",         Bpairs},
	{"fastpairs",     Bfastpairs},
	{"setmetatable",  Bsetmeta},
	{"getmetatable",  Bgetmeta},
	{"error",         Berror},
	{"pcall",         Bpcall},
	{"next",          Bnext},
	{"inext",         Binext},
	{"type",          Btype},
	{"tonumber",      Btonumber},
	{"tostring",      Btostring},
	{"loadstring",    Bloadstring},
	{"select",        Bselect},
	{"rawequal",      Braweq},
	{"rawlen",        Brawlen},
	{"rawget",        Brawget},
	{"rawset",        Brawset},
	{"cleartable",    Bcleartable},
	{"newtable",      Bnewtable},
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

