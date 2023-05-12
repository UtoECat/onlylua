/*
** $Id: lextensions.c $
** Extra nonstandard LUA API functions
** See Copyright Notice in lua.h
*/

#define lext_c
#define LUA_CORE

#include "lprefix.h"

#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lapi.h"
#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"

/*
** Convert an acceptable index to a pointer to its respective value.
** Non-valid indices return the special nil value 'G(L)->nilvalue'.
*/

/* test for pseudo index */
#define ispseudo(i)		((i) <= LUA_REGISTRYINDEX)

static TValue *index2value2 (lua_State *L, int idx) {
  CallInfo *ci = L->ci;
  if (idx > 0) {
    StkId o = ci->func + idx;
    api_check(L, idx <= L->ci->top - (ci->func + 1), "unacceptable index");
    if (o >= L->top) return &G(L)->nilvalue;
    else return s2v(o);
  }
  else if (!ispseudo(idx)) {  /* negative index */
    api_check(L, idx != 0 && -idx <= L->top - (ci->func + 1), "invalid index");
    return s2v(L->top + idx);
  }
  else if (idx == LUA_REGISTRYINDEX)
    return &G(L)->l_registry;
  else {  /* upvalues */
    idx = LUA_REGISTRYINDEX - idx;
    api_check(L, idx <= MAXUPVAL + 1, "upvalue index too large");
    if (ttisCclosure(s2v(ci->func))) {  /* C closure? */
      CClosure *func = clCvalue(s2v(ci->func));
      return (idx <= func->nupvalues) ? &func->upvalue[idx-1]
                                      : &G(L)->nilvalue;
    }
    else {  /* light C function or Lua function (through a hook)?) */
      api_check(L, ttislcf(s2v(ci->func)), "caller not a C function");
      return &G(L)->nilvalue;  /* no upvalues */
    }
  }
}

/* #define __incr_top(L)   {L->top++; api_check(L, L->top <= L->ci->top, \
				"stack overflow");}
*/
				
/* 
 * gets FIXED typename string for given type/object
 * also looks for the __index metamethod.
 * DON'T PUSHES ANYTHING ONTO THE STACK!!!
 */
LUA_API const char* lua_objtypename(lua_State *L, int idx) {
  const TValue *o = index2value2(L, idx);
  return luaT_objtypename(L, o, 1);
}

/* 
 * works like function above, but ALSO pushes FIXED string
 * on top of the stack. Useful to get better perfomacne
 * on big amount of `type(a) == "something"` checks.
 */
LUA_API void lua_pushobjtype(lua_State* L, int idx, int meta) {
	lua_lock(L);
	const TValue *o = index2value2(L, idx);
	api_checknelems(L, 1);
  setsvalue2s(L, L->top, luaT_objtypestr(L, o, meta));
	api_incr_top(L);
	lua_unlock(L);
  return;
}

void luaH_clear(lua_State* L, Table *t, int keep);

LUA_API void (lua_cleartable) (lua_State* L, int idx, int keep) {
	lua_lock(L);
	TValue *o = index2value2(L, idx);
	api_check(L, ttistable(o), "table expected");
	Table *t = hvalue(o);
	/* hehe */
	luaH_clear(L, t, keep); 
	lua_unlock(L);
}

#if 0
#include <stdio.h>

/*
 * DEBUG: returns all fixed GC objects, packed in table
 * NOBARRIER : fixed objects are not collectable
 */
int luaC_getfixed(lua_State* L) {
  global_State *g = G(L);
	GCObject* o = g->fixedgc;
	lua_createtable(L, 0, 0);
	int oldgcstp  = g->gcstp;
	g->gcstp |= GCSTPGC;  /* avoid GC steps */
	int i = 1;
	while (o) {
		setgcovalue(L, s2v(L->top), o);
		__incr_top(L);
		lua_seti(L, -2, i++);
		o = o->next;
	}
	g->gcstp = oldgcstp; /* restore gc state */
	return 1;
}
#else
/* in other case */
int luaC_getfixed(lua_State* L) {lua_createtable(L, 0, 0); return 1;}
#endif

/*
 * Policy system
 */
#include "luapolicy.h"

LUA_API int lua_getpolicy(lua_State* L) {
	return L->l_G->policy;
}	

LUA_API void lua_setpolicy(lua_State* L, int flags) {
	L->l_G->policy = flags;
	api_check(L, (flags >> 16) < 2, "Bad policy flag");
}


