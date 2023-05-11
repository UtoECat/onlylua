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

/* #define __incr_top(L)   {L->top++; api_check(L, L->top <= L->ci->top, \
				"stack overflow");}
*/
				
/* 
 * gets FIXED typename string for given type/object
 * also looks for the __index metamethod.
 * DON'T PUSHES ANYTHING ONTO THE STACK!!!
 */
LUA_API const char* lua_objtypename(lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return luaT_objtypename(L, o);
}

/* 
 * works like function above, but ALSO pushes FIXED string
 * on top of the stack. Useful to get better perfomacne
 * on big amount of `type(a) == "something"` checks.
 */
LUA_API void lua_pushobjtype(lua_State* L, int idx) {
	const TValue *o = index2value(L, idx);
	api_checknelems(L, 1);
  setsvalue2s(L, L->top, luaT_objtypestr(L, o));
	api_incr_top(L);
  return;
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


