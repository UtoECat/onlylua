#define LUA_LIB
#include "lua.h"
/*
 ONLYLUA - minified lua distribution
  - Only theese basic libraries are included :
		`auxlib, baselib, string, coroutine, math, string, table, utf8`
	- Removed dangerous and useless debug library functions
	- Removed auxlib code loading from file, dynamic c libraries loading
	- All sources are packed into lua.h, lualib.c and lua.c files

 Copyright (C) 1994-2020 Lua.org, PUC-Rio.
 Copyright (C) 2023 UtoECat <utopia.egor.cat.allandall@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
// root include ./lualib/lauxlib.c
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//included "lauxlib.h" 
#if !defined(MAX_SIZET)
#define MAX_SIZET ((size_t)(~(size_t)0))
#endif
#define LEVELS1 10 
#define LEVELS2 11 
static void pushfuncname (lua_State *L, lua_Debug *ar) {
 // function argument at the top of the stack is required!
  if (*ar->namewhat != '\0')  
    lua_pushfstring(L, "%s '%s'", ar->namewhat, ar->name);  
  else if (*ar->what == 'm')  
      lua_pushliteral(L, "main chunk");
  else if (*ar->what != 'C')  
    lua_pushfstring(L, "function <%s:%d>", ar->short_src, ar->linedefined);
 else  
    lua_pushliteral(L, "?");
}
static int lastlevel (lua_State *L) {
  lua_Debug ar;
  int li = 1, le = 1;
  
  while (lua_getstack(L, le, &ar)) { li = le; le *= 2; }
  
  while (li < le) {
    int m = (li + le)/2;
    if (lua_getstack(L, m, &ar)) li = m + 1;
    else le = m;
  }
  return le - 1;
}
LUALIB_API void luaL_traceback (lua_State *L, lua_State *L1,
                                const char *msg, int level) {
  luaL_Buffer b;
  lua_Debug ar;
  int last = lastlevel(L1);
  int limit2show = (last - level > LEVELS1 + LEVELS2) ? LEVELS1 : -1;
  luaL_buffinit(L, &b);
  if (msg) {
    luaL_addstring(&b, msg);
    luaL_addchar(&b, '\n');
  }
  luaL_addstring(&b, "stack traceback:");
  while (lua_getstack(L1, level++, &ar)) {
    if (limit2show-- == 0) {  
      int n = last - level - LEVELS2 + 1;  
      lua_pushfstring(L, "\n\t...\t(skipping %d levels)", n);
      luaL_addvalue(&b);  
      level += n;  
    }
    else {
      lua_getinfo(L1, "Slnt", &ar);
      if (ar.currentline <= 0)
        lua_pushfstring(L, "\n\t%s: in ", ar.short_src);
      else
        lua_pushfstring(L, "\n\t%s:%d: in ", ar.short_src, ar.currentline);
      luaL_addvalue(&b);
   if (ar.what[0] == 'C') {
    int oldtop = lua_gettop(L);
       lua_getinfo(L1, "f", &ar); // get function object
    if (oldtop == lua_gettop(L)) goto fallback;// some shit
    luaL_addvalue(&b);
     luaL_addstring(&b, " aka ");
   }
   fallback:
      pushfuncname(L, &ar);
   //lua_remove(L, -2); // remove it
      luaL_addvalue(&b);
      if (ar.istailcall)
        luaL_addstring(&b, "\n\t(...tail calls...)");
    }
  }
  luaL_pushresult(&b);
}
LUALIB_API int luaL_argerror (lua_State *L, int arg, const char *extramsg) {
  lua_Debug ar = {0};
  if (!lua_getstack(L, 0, &ar))  
    return luaL_error(L, "bad argument #%d (%s)", arg, extramsg);
  lua_getinfo(L, "Sn", &ar);
 if (ar.name == NULL) {
  pushfuncname(L, &ar);
    ar.name = lua_tostring(L, -1);
 }
  if (strcmp(ar.namewhat, "method") == 0) {
    arg--;  
    if (arg == 0)  
      return luaL_error(L, "calling '%s' on bad self (%s)", ar.name, extramsg);
  }
  return luaL_error(L, "bad argument #%d to '%s' (%s)", arg, ar.name, extramsg);
}
LUALIB_API int luaL_typeerror (lua_State *L, int arg, const char *tname) {
  const char *msg;
  const char *typearg;  
 typearg = lua_objtypename(L, arg);  
  msg = lua_pushfstring(L, "%s expected, got %s", tname, typearg);
  return luaL_argerror(L, arg, msg);
}
static void tag_error (lua_State *L, int arg, int tag) {
  const char *msg;
  const char *typearg = lua_objtypename(L, arg);  
  msg = lua_pushfstring(L, "%s expected, got %s (%s)", lua_typename(L, tag), typearg, lua_typename(L, lua_type(L, arg)));
  luaL_argerror(L, arg, msg);
}
LUALIB_API void luaL_where (lua_State *L, int level) {
  lua_Debug ar;
  if (lua_getstack(L, level, &ar)) {  
    lua_getinfo(L, "Sl", &ar);  
    if (ar.currentline > 0) {  
      lua_pushfstring(L, "%s:%d: ", ar.short_src, ar.currentline);
      return;
    }
   lua_pushfstring(L, "%s: ", ar.what);  
  return;
  }
  lua_pushfstring(L, "");  
}
LUALIB_API int luaL_error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  luaL_where(L, 1);
  lua_pushvfstring(L, fmt, argp);
  va_end(argp);
  lua_concat(L, 2);
  return lua_error(L);
}
LUALIB_API int luaL_fileresult (lua_State *L, int, const char*) {
 luaL_error(L, "LuaL_fileresult is ABI deprecated!");
}
LUALIB_API int luaL_execresult (lua_State *L, int) {
 luaL_error(L, "LuaL_execresult is ABI deprecated!");
}
LUALIB_API int luaL_newmetatable (lua_State *L, const char *tname) {
  if (luaL_getmetatable(L, tname) != LUA_TNIL)  
    return 0;  
  lua_pop(L, 1);
  lua_createtable(L, 0, 2);  
  lua_pushstring(L, tname);
  lua_setfield(L, -2, "__name");  
  lua_pushvalue(L, -1);
  lua_setfield(L, LUA_REGISTRYINDEX, tname);  
  return 1;
}
LUALIB_API void luaL_setmetatable (lua_State *L, const char *tname) {
 int t = luaL_getmetatable(L, tname);
  if (luai_unlikely(t != LUA_TTABLE))
  luaL_error(L, "%s is not %s metatable!", tname, 
   (t != LUA_TNONE && t != LUA_TNIL) ? "a valid" : "existed");
  lua_setmetatable(L, -2);
}
LUALIB_API void *luaL_testudata (lua_State *L, int ud, const char *tname) {
  void *p = lua_touserdata(L, ud);
  if (p != NULL) {  
    if (lua_getmetatable(L, ud)) {  
      luaL_getmetatable(L, tname);  
      if (!lua_rawequal(L, -1, -2))  
        p = NULL;  
      lua_pop(L, 2);  
      return p;
    }
  }
  return NULL;  
}
LUALIB_API void *luaL_checkudata (lua_State *L, int ud, const char *tname) {
  void *p = luaL_testudata(L, ud, tname);
  luaL_argexpected(L, p != NULL, ud, tname);
  return p;
}
LUALIB_API int luaL_checkoption (lua_State *L, int arg, const char *def,
                                 const char *const lst[]) {
  const char *name = (def) ? luaL_optstring(L, arg, def) :
                             luaL_checkstring(L, arg);
  int i;
  for (i=0; lst[i]; i++)
    if (strcmp(lst[i], name) == 0)
      return i;
  return luaL_argerror(L, arg,
                       lua_pushfstring(L, "invalid option '%s'", name));
}
LUALIB_API void luaL_checkstack (lua_State *L, int space, const char *msg) {
  if (l_unlikely(!lua_checkstack(L, space))) {
    if (msg)
      luaL_error(L, "stack overflow (%s)", msg);
    else
      luaL_error(L, "stack overflow");
  }
}
LUALIB_API void luaL_checktype (lua_State *L, int arg, int t) {
  if (l_unlikely(lua_type(L, arg) != t))
    tag_error(L, arg, t);
}
LUALIB_API void luaL_checkany (lua_State *L, int arg) {
 int t = lua_type(L, arg);
  if (l_unlikely(t == LUA_TNONE || t == LUA_TNIL))
    luaL_argerror(L, arg, "value expected");
}
LUALIB_API const char *luaL_checklstring (lua_State *L, int arg, size_t *len) {
  const char *s = lua_tolstring(L, arg, len);
  if (l_unlikely(!s)) tag_error(L, arg, LUA_TSTRING);
  return s;
}
LUALIB_API const char *luaL_optlstring (lua_State *L, int arg,
                                        const char *def, size_t *len) {
  if (lua_isnoneornil(L, arg)) {
    if (len)
      *len = (def ? strlen(def) : 0);
    return def;
  }
  else return luaL_checklstring(L, arg, len);
}
LUALIB_API lua_Number luaL_checknumber (lua_State *L, int arg) {
  int isnum;
  lua_Number d = lua_tonumberx(L, arg, &isnum);
  if (l_unlikely(!isnum))
    tag_error(L, arg, LUA_TNUMBER);
  return d;
}
LUALIB_API lua_Number luaL_optnumber (lua_State *L, int arg, lua_Number def) {
  return luaL_opt(L, luaL_checknumber, arg, def);
}
static void interror (lua_State *L, int arg) {
  if (lua_isnumber(L, arg))
    luaL_argerror(L, arg, "number has no integer representation");
  else
    tag_error(L, arg, LUA_TNUMBER);
}
LUALIB_API lua_Integer luaL_checkinteger (lua_State *L, int arg) {
  int isnum;
  lua_Integer d = lua_tointegerx(L, arg, &isnum);
  if (l_unlikely(!isnum)) {
    interror(L, arg);
  }
  return d;
}
LUALIB_API lua_Integer luaL_optinteger (lua_State *L, int arg,
                                                      lua_Integer def) {
  return luaL_opt(L, luaL_checkinteger, arg, def);
}
#define freelist (LUA_RIDX_LAST + 1)
LUALIB_API int luaL_ref (lua_State *L, int t) {
  int ref;
  if (lua_isnil(L, -1)) {
    lua_pop(L, 1);  
    return LUA_REFNIL;  
  }
  t = lua_absindex(L, t);
  if (lua_rawgeti(L, t, freelist) == LUA_TNIL) {  
    ref = 0;  
    lua_pushinteger(L, 0);  
    lua_rawseti(L, t, freelist);  
  }
  else {  
    lua_assert(lua_isinteger(L, -1));
    ref = (int)lua_tointeger(L, -1);  
  }
  lua_pop(L, 1);  
  if (ref != 0) {  
    lua_rawgeti(L, t, ref);  
    lua_rawseti(L, t, freelist);  
  }
  else  
    ref = (int)lua_rawlen(L, t) + 1;  
  lua_rawseti(L, t, ref);
  return ref;
}
LUALIB_API void luaL_unref (lua_State *L, int t, int ref) {
  if (ref >= 0) {
    t = lua_absindex(L, t);
    lua_rawgeti(L, t, freelist);
    lua_assert(lua_isinteger(L, -1));
    lua_rawseti(L, t, ref);  
    lua_pushinteger(L, ref);
    lua_rawseti(L, t, freelist);  
  }
}
typedef struct LoadS {
  const char *s;
  size_t size;
} LoadS;
static const char *getS (lua_State *L, void *ud, size_t *size) {
  LoadS *ls = (LoadS *)ud;
  (void)L;  
  if (ls->size == 0) return NULL;
  *size = ls->size;
  ls->size = 0;
  return ls->s;
}
LUALIB_API int luaL_loadbufferx (lua_State *L, const char *buff, size_t size,
                                 const char *name, const char *mode) {
  LoadS ls;
  ls.s = buff;
  ls.size = size;
  return lua_load(L, getS, &ls, name, mode);
}
LUALIB_API int luaL_loadstring (lua_State *L, const char *s) {
  return luaL_loadbuffer(L, s, strlen(s), s);
}
LUALIB_API int luaL_getmetafield (lua_State *L, int obj, const char *event) {
  if (!lua_getmetatable(L, obj))  
    return LUA_TNIL;
  else {
    int tt;
    lua_pushstring(L, event);
    tt = lua_rawget(L, -2);
    if (tt == LUA_TNIL)  
      lua_pop(L, 2);  
    else
      lua_remove(L, -2);  
    return tt;  
  }
}
LUALIB_API int luaL_callmeta (lua_State *L, int obj, const char *event) {
  obj = lua_absindex(L, obj);
  if (luaL_getmetafield(L, obj, event) == LUA_TNIL)  
    return 0;
  lua_pushvalue(L, obj);
  lua_call(L, 1, 1);
  return 1;
}
LUALIB_API lua_Integer luaL_len (lua_State *L, int idx) {
  lua_Integer l;
  int isnum;
  lua_len(L, idx);
  l = lua_tointegerx(L, -1, &isnum);
  if (l_unlikely(!isnum))
    luaL_error(L, "object length is not an integer");
  lua_pop(L, 1);  
  return l;
}
LUALIB_API const char *luaL_tolstring (lua_State *L, int idx, size_t *len) {
  idx = lua_absindex(L,idx);
  if (luaL_callmeta(L, idx, "__tostring")) {  
    if (!lua_isstring(L, -1))
      luaL_error(L, "'__tostring' must return a string");
  }
  else {
    switch (lua_type(L, idx)) {
      case LUA_TNUMBER: {
        if (lua_isinteger(L, idx))
          lua_pushfstring(L, "%I", (LUAI_UACINT)lua_tointeger(L, idx));
        else
          lua_pushfstring(L, "%f", (LUAI_UACNUMBER)lua_tonumber(L, idx));
        break;
      }
      case LUA_TSTRING:
        lua_pushvalue(L, idx);
        break;
      case LUA_TBOOLEAN:
        lua_pushstring(L, (lua_toboolean(L, idx) ? "true" : "false"));
        break;
      case LUA_TNIL:
        lua_pushliteral(L, "nil");
        break;
      default: {
        const char *kind = lua_objtypename(L, idx);
        lua_pushfstring(L, "%s:(%p)", kind, lua_topointer(L, idx));
        break;
      }
    }
  }
  return lua_tolstring(L, -1, len);
}
LUALIB_API void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup, "too many upvalues");
  for (; l->name != NULL; l++) {  
    if (l->func == NULL)  
      lua_pushboolean(L, 0);
    else {
      int i;
      for (i = 0; i < nup; i++)  
        lua_pushvalue(L, -nup);
      lua_pushcclosure(L, l->func, nup);  
    }
    lua_setfield(L, -(nup + 2), l->name);
  }
  lua_pop(L, nup);  
}
LUALIB_API int luaL_getsubtable (lua_State *L, int idx, const char *fname) {
  if (lua_getfield(L, idx, fname) == LUA_TTABLE)
    return 1;  
  else {
    lua_pop(L, 1);  
    idx = lua_absindex(L, idx);
    lua_newtable(L);
    lua_pushvalue(L, -1);  
    lua_setfield(L, idx, fname);  
    return 0;  
  }
}
LUALIB_API void luaL_requiref (lua_State *L, const char *modname,
                               lua_CFunction openf, int glb) {
  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
 lua_assert(lua_type(L, -1) == LUA_TTABLE);
  lua_getfield(L, -1, modname);  
  if (!lua_toboolean(L, -1)) {  
    lua_pop(L, 1);  
    lua_pushcfunction(L, openf);
    lua_pushstring(L, modname);  
    lua_call(L, 1, 1);  
    lua_pushvalue(L, -1);  
    lua_setfield(L, -3, modname);  
  }
  lua_remove(L, -2);  
  if (glb) {
    lua_pushvalue(L, -1);  
    lua_setglobal(L, modname);  
  }
}
LUALIB_API const char *luaL_gsub (lua_State *L, const char *s,
                                  const char *p, const char *r) {
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  luaL_addgsub(&b, s, p, r);
  luaL_pushresult(&b);
  return lua_tostring(L, -1);
}
static void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize) {
  (void)ud; (void)osize;  
  if (nsize == 0) {
    free(ptr);
    return NULL;
  }
  else
    return realloc(ptr, nsize);
}
static int panic (lua_State *L) {
 luaL_traceback(L, L, lua_tostring(L, -1), 1);
 lua_writestringerror(
  "PANIC: Unprotected call to Lua API! \nPANIC: %s!\n",
   lua_tostring(L, -1));
  return 0;  
}
LUALIB_API lua_State *luaL_newstate (void) {
  lua_State *L = lua_newstate(l_alloc, NULL);
  if (l_likely(L)) {
    lua_atpanic(L, &panic);
  }
  return L;
}
LUALIB_API void luaL_checkversion_ (lua_State *L, lua_Number ver, size_t sz) {
  lua_Number v = lua_version(L);
  if (sz != LUAL_NUMSIZES)  
    luaL_error(L, "core and library have incompatible numeric types");
  else if (v != ver)
    luaL_error(L, "version mismatch: app. needs %f, Lua core provides %f",
                  (LUAI_UACNUMBER)ver, (LUAI_UACNUMBER)v);
}
LUALIB_API const char* luaL_policyname(lua_State* L, int flag) {
 switch (flag) {
#define __TOLIT(a) #a
#define DOF(name)case LUAPOLICY_##name : return __TOLIT(name);
  DOF(REGISTRY)
  DOF(BYTECODE)
  DOF(CONTROLGC)
  DOF(CANRUNGC)
  DOF(FILESYSTEM)
  DOF(EXTRADEBUG)
  DOF(POLICYCTL)
 #undef __TOLIT
 #undef DOF
 } 
 return "unknown or complex";
}
LUALIB_API void luaL_checkpolicy(lua_State *L, int flag) {
 if (luai_unlikely(!(lua_getpolicy(L) & flag))) {
  luaL_error(L, "Access denied due to disabled policy flag %s!", 
   luaL_policyname(L, flag)
  );
 }
}
// root include ./lualib/lbaselib.c
#include <ctype.h>
//included "stdio.h" 
//included "stdlib.h" 
//included "string.h" 
//included "lualib.h" 
static int luaB_print (lua_State *L) {
  int n = lua_gettop(L);  
  int i;
  for (i = 1; i <= n; i++) {  
    size_t l;
    const char *s = luaL_tolstring(L, i, &l);  
    if (i > 1)  
      lua_writestring(" ", 1);  
    lua_writestring(s, l);  
    lua_pop(L, 1);  
  }
  lua_writeline();
  return 0;
}
static int luaB_warn (lua_State *L) {
  int n = lua_gettop(L);  
  int i;
  luaL_checkstring(L, 1);  
  for (i = 2; i <= n; i++)
    luaL_checkstring(L, i);  
  for (i = 1; i < n; i++)  
    lua_warning(L, lua_tostring(L, i), 1);
  lua_warning(L, lua_tostring(L, n), 0);  
  return 0;
}
#include <math.h>
static int luaB_tonumber (lua_State *L) {
 int t = lua_type(L, 1);
 switch (t) {
  case LUA_TNUMBER :
  lua_settop(L, 1);
  return 1;
  case LUA_TSTRING:
  if (!lua_stringtonumber(L, lua_tostring(L, -1)))
   lua_pushnil(L);
  return 1;
  case LUA_TBOOLEAN:
   lua_pushnumber(L, lua_toboolean(L, -1));
  return 1;
  default:
   lua_pushnil(L);
  return 1;
 }
}
static int luaB_error (lua_State *L) {
  int level = (int)luaL_optinteger(L, 2, 1);
  lua_settop(L, 1);
  if (lua_type(L, 1) == LUA_TSTRING && level > 0) {
    luaL_where(L, level);   
    lua_pushvalue(L, 1);
    lua_concat(L, 2);
  }
  lua_error(L);
}
static int luaB_getmetatable (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  if (!lua_getmetatable(L, 1)) return 0;  
 lua_pushstring(L, "__metatable");
 if (lua_rawget(L, -2) == LUA_TNIL) {
  lua_pop(L, 1);
 }
  return 1;
}
static int luaB_setmetatable (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_argexpected(L, lua_istable(L, 2) || lua_isnoneornil(L, 2), 2, "nil or table");
  lua_settop(L, 2);
 if (lua_getmetatable(L, 1)) {
  lua_pushstring(L, "__metatable");
  if (lua_rawget(L, -2) == LUA_TNIL) {
   lua_pop(L, 2);
  } else luaL_error(L, "attempt to change protected metatable");
 }
  lua_setmetatable(L, 1);
  return 1;
}
static int luaB_rawequal (lua_State *L) {
 if (lua_gettop(L) < 2) luaL_error(L, "at least 2 arguments excepted!");
 lua_pushboolean(L, lua_rawequal(L, 1, 2));
  return 1;
}
static int luaB_rawlen (lua_State *L) {
  int t = lua_type(L, 1);
  luaL_argexpected(L, t == LUA_TTABLE || t == LUA_TSTRING, 1, "table or string");
  lua_pushinteger(L, lua_rawlen(L, 1));
  return 1;
}
static int luaB_rawget (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2);
  lua_settop(L, 2);
  lua_rawget(L, 1);
  return 1;
}
static int luaB_rawset (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2);
  lua_settop(L, 3);
  lua_rawset(L, 1);
  return 1;
}
static int pushmode (lua_State *L, int oldmode) {
  if (oldmode == -1)
    luaL_pushfail(L);  
  else
    lua_pushstring(L, (oldmode == LUA_GCINC) ? "incremental"
                                             : "generational");
  return 1;
}
#define checkvalres(res) { if (res == -1) break; }
static int luaB_collectgarbage (lua_State *L) {
  static const char *const opts[] = {"stop", "restart", "collect",
    "count", "step", "setpause", "setstepmul",
    "isrunning", "generational", "incremental", NULL};
  static const int optsnum[] = {LUA_GCSTOP, LUA_GCRESTART, LUA_GCCOLLECT,
    LUA_GCCOUNT, LUA_GCSTEP, LUA_GCSETPAUSE, LUA_GCSETSTEPMUL,
    LUA_GCISRUNNING, LUA_GCGEN, LUA_GCINC};
  int o = optsnum[luaL_checkoption(L, 1, "collect", opts)];
  switch (o) {
    case LUA_GCCOUNT: {
      int k = lua_gc(L, o);
      int b = lua_gc(L, LUA_GCCOUNTB);
      checkvalres(k);
      lua_pushnumber(L, (lua_Number)k + ((lua_Number)b/1024));
      return 1;
    }
    case LUA_GCSTEP: {
   luaL_checkpolicy(L, LUAPOLICY_CANRUNGC);
      int step = (int)luaL_optinteger(L, 2, 0);
      int res = lua_gc(L, o, step);
      checkvalres(res);
      lua_pushboolean(L, res);
      return 1;
    }
    case LUA_GCSETPAUSE:
    case LUA_GCSETSTEPMUL: {
   luaL_checkpolicy(L, LUAPOLICY_CONTROLGC);
      int p = (int)luaL_optinteger(L, 2, 0);
      int previous = lua_gc(L, o, p);
      checkvalres(previous);
      lua_pushinteger(L, previous);
      return 1;
    }
    case LUA_GCISRUNNING: {
      int res = lua_gc(L, o);
      checkvalres(res);
      lua_pushboolean(L, res);
      return 1;
    }
    case LUA_GCGEN: {
   luaL_checkpolicy(L, LUAPOLICY_CONTROLGC);
      int minormul = (int)luaL_optinteger(L, 2, 0);
      int majormul = (int)luaL_optinteger(L, 3, 0);
      return pushmode(L, lua_gc(L, o, minormul, majormul));
    }
    case LUA_GCINC: {
   luaL_checkpolicy(L, LUAPOLICY_CONTROLGC);
      int pause = (int)luaL_optinteger(L, 2, 0);
      int stepmul = (int)luaL_optinteger(L, 3, 0);
      int stepsize = (int)luaL_optinteger(L, 4, 0);
      return pushmode(L, lua_gc(L, o, pause, stepmul, stepsize));
    }
    default: {
   luaL_checkpolicy(L, LUAPOLICY_CONTROLGC);
      int res = lua_gc(L, o);
      checkvalres(res);
      lua_pushinteger(L, res);
      return 1;
    }
  }
  luaL_pushfail(L);  
  return 1;
}
static int luaB_rawtype (lua_State *L) {
 if (lua_gettop(L) < 1) luaL_error(L, "bad argument #1 : value excepted");
 lua_pushobjtype(L, 1, 0); // no meta checks
  return 1;
}
static int luaB_type (lua_State *L) {
 if (lua_gettop(L) < 1) luaL_error(L, "bad argument #1 : value excepted");
 lua_pushobjtype(L, 1, 1); // with meta check
 return 1;
}
static int luaB_next (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 2);  
  if (lua_next(L, 1))
    return 2;
  else {
    lua_pushnil(L);
    return 1;
  }
}
static int voidcont3 (lua_State *L, int status, lua_KContext k) {
  (void)L; (void)status; (void)k;  
  return 3;
}
static int luaB_pairs (lua_State *L) {
  luaL_checkany(L, 1);
  if (luaL_getmetafield(L, 1, "__pairs") == LUA_TNIL) {  
    lua_pushcfunction(L, luaB_next);  
    lua_pushvalue(L, 1);  
    lua_pushnil(L);  
  }
  else {
    lua_pushvalue(L, 1);  
    lua_callk(L, 1, 3, 0, voidcont3);  
  }
  return 3;
}
static int ipairsaux (lua_State *L) {
  lua_Integer i = luaL_checkinteger(L, 2);
  i = luaL_intop(+, i, 1);
  lua_pushinteger(L, i);
  return (lua_geti(L, 1, i) == LUA_TNIL) ? 1 : 2;
}
static int luaB_ipairs (lua_State *L) {
  luaL_checkany(L, 1);
  lua_pushcfunction(L, ipairsaux);  
  lua_pushvalue(L, 1);  
  lua_pushinteger(L, 0);  
  return 3;
}
static int load_aux (lua_State *L, int status, int envidx) {
  if (l_likely(status == LUA_OK)) {
    if (envidx != 0) {  
      lua_pushvalue(L, envidx);  
      if (!lua_setupvalue(L, -2, 1))  
        lua_pop(L, 1);  
    }
    return 1;
  }
  else {  
    luaL_pushfail(L);
    lua_insert(L, -2);  
    return 2;  
  }
}
#define RESERVEDSLOT 5
static const char *generic_reader (lua_State *L, void *ud, size_t *size) {
  (void)(ud);  
  luaL_checkstack(L, 2, "too many nested functions");
  lua_pushvalue(L, 1);  
  lua_call(L, 0, 1);  
  if (lua_isnil(L, -1)) {
    lua_pop(L, 1);  
    *size = 0;
    return NULL;
  }
  else if (l_unlikely(!lua_isstring(L, -1)))
    luaL_error(L, "reader function must return a string");
  lua_replace(L, RESERVEDSLOT);  
  return lua_tolstring(L, RESERVEDSLOT, size);
}
static int luaB_load (lua_State *L) {
  int status;
  size_t l;
  const char *s = lua_tolstring(L, 1, &l);
 int canbc = lua_getpolicy(L) & LUAPOLICY_BYTECODE;
  const char *mode = luaL_optstring(L, 3, canbc ? "bt" : "t");
 if (strchr(mode, 'b')) {
  luaL_checkpolicy(L, LUAPOLICY_BYTECODE);
 }
  int env = (!lua_isnone(L, 4) ? 4 : 0);  
  if (s != NULL) {  
    const char *chunkname = luaL_optstring(L, 2, s);
    status = luaL_loadbufferx(L, s, l, chunkname, mode);
  }
  else {  
    const char *chunkname = luaL_optstring(L, 2, "=(load)");
    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_settop(L, RESERVEDSLOT);  
    status = lua_load(L, generic_reader, NULL, chunkname, mode);
  }
  return load_aux(L, status, env);
}
static int luaB_assert (lua_State *L) {
  if (l_likely(lua_toboolean(L, 1)))  
    return lua_gettop(L);  
  else {  
  if (lua_gettop(L) >= 1) 
     lua_remove(L, 1);  
    lua_pushliteral(L, "assertion failed!");  
    lua_settop(L, 1);  
    return luaB_error(L);  
  }
}
static int luaB_select (lua_State *L) {
  int n = lua_gettop(L);
  if (lua_type(L, 1) == LUA_TSTRING && *lua_tostring(L, 1) == '#') {
    lua_pushinteger(L, n-1);
    return 1;
  }
  else {
    lua_Integer i = luaL_checkinteger(L, 1);
    if (i < 0) i = n + i;
    else if (i > n) i = n;
    luaL_argcheck(L, 1 <= i, 1, "index out of range");
    return n - (int)i;
  }
}
static int finishpcall (lua_State *L, int status, lua_KContext extra) {
  if (l_unlikely(status != LUA_OK && status != LUA_YIELD)) {  
    lua_pushboolean(L, 0); lua_replace(L, -3);
    return 2;  
  }
  else
    return lua_gettop(L) - (int)extra;  
}
static int luaB_pcall (lua_State *L) {
  int status;
  luaL_checkany(L, 1);
  lua_pushboolean(L, 1);  
  lua_insert(L, 1);  
  status = lua_pcallk(L, lua_gettop(L) - 2, LUA_MULTRET, 0, 0, finishpcall);
  return finishpcall(L, status, 0);
}
static int luaB_xpcall (lua_State *L) {
  int status;
  luaL_checkany(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);  
  lua_pushboolean(L, 1);  
  lua_insert(L, 3);  
  lua_pushvalue(L, 1);  
  lua_insert(L, 4);  
  int n = lua_gettop(L);
  status = lua_pcallk(L, n - 4, LUA_MULTRET, 2, 2, finishpcall);
  return finishpcall(L, status, 2);
}
static int luaB_tostring (lua_State *L) {
 size_t len = 0;
  luaL_tolstring(L, 1, &len);
 if (len <= 0) lua_pushliteral(L, "");
  return 1;
}
static const luaL_Reg base_funcs[] = {
  {"assert", luaB_assert},
  {"collectgarbage", luaB_collectgarbage},
  {"error", luaB_error},
  {"getmetatable", luaB_getmetatable},
  {"ipairs", luaB_ipairs},
  {"load", luaB_load},
  {"next", luaB_next},
  {"pairs", luaB_pairs},
  {"pcall", luaB_pcall},
  {"print", luaB_print},
  {"warn", luaB_warn},
  {"rawequal", luaB_rawequal},
  {"rawlen", luaB_rawlen},
  {"rawget", luaB_rawget},
  {"rawset", luaB_rawset},
  {"select", luaB_select},
  {"setmetatable", luaB_setmetatable},
  {"tonumber", luaB_tonumber},
  {"tostring", luaB_tostring},
  {"type", luaB_type},
  {"rawtype", luaB_rawtype},
  {"xpcall", luaB_xpcall},
  
  {LUA_GNAME, NULL},
  {"_VERSION", NULL},
  {NULL, NULL}
};
LUAMOD_API int luaopen_base (lua_State *L) {
  
  lua_pushglobaltable(L);
  luaL_setfuncs(L, base_funcs, 0);
  
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, LUA_GNAME);
  
  lua_pushliteral(L, LUA_VERSION);
  lua_setfield(L, -2, "_VERSION");
  return 1;
}
// root include ./lualib/lcorolib.c
//included "stdlib.h" 
static lua_State *getco (lua_State *L) {
  lua_State *co = lua_tothread(L, 1);
  luaL_argexpected(L, co, 1, "thread");
  return co;
}
static int auxresume (lua_State *L, lua_State *co, int narg) {
  int status, nres;
  if (l_unlikely(!lua_checkstack(co, narg))) {
    lua_pushliteral(L, "too many arguments to resume");
    return -1;  
  }
  lua_xmove(L, co, narg);
  status = lua_resume(co, L, narg, &nres);
  if (l_likely(status == LUA_OK || status == LUA_YIELD)) {
    if (l_unlikely(!lua_checkstack(L, nres + 1))) {
      lua_pop(co, nres);  
      lua_pushliteral(L, "too many results to resume");
      return -1;  
    }
    lua_xmove(co, L, nres);  
    return nres;
  }
  else {
    lua_xmove(co, L, 1);  
    return -1;  
  }
}
static int luaB_coresume (lua_State *L) {
  lua_State *co = getco(L);
  int r;
  r = auxresume(L, co, lua_gettop(L) - 1);
  if (l_unlikely(r < 0)) {
    lua_pushboolean(L, 0);
    lua_insert(L, -2);
    return 2;  
  }
  else {
    lua_pushboolean(L, 1);
    lua_insert(L, -(r + 1));
    return r + 1;  
  }
}
static int luaB_auxwrap (lua_State *L) {
  lua_State *co = lua_tothread(L, lua_upvalueindex(1));
  int r = auxresume(L, co, lua_gettop(L));
  if (l_unlikely(r < 0)) {  
    int stat = lua_status(co);
    if (stat != LUA_OK && stat != LUA_YIELD) {  
      stat = lua_resetthread(co);  
      lua_assert(stat != LUA_OK);
      lua_xmove(co, L, 1);  
    }
    if (stat != LUA_ERRMEM &&  
        lua_type(L, -1) == LUA_TSTRING) {  
      luaL_where(L, 1);  
      lua_insert(L, -2);
      lua_concat(L, 2);
    }
    return lua_error(L);  
  }
  return r;
}
static int luaB_cocreate (lua_State *L) {
  lua_State *NL;
  luaL_checktype(L, 1, LUA_TFUNCTION);
  NL = lua_newthread(L);
  lua_pushvalue(L, 1);  
  lua_xmove(L, NL, 1);  
  return 1;
}
static int luaB_cowrap (lua_State *L) {
  luaB_cocreate(L);
  lua_pushcclosure(L, luaB_auxwrap, 1);
  return 1;
}
static int luaB_yield (lua_State *L) {
  return lua_yield(L, lua_gettop(L));
}
#define COS_RUN  0
#define COS_DEAD 1
#define COS_YIELD 2
#define COS_NORM 3
static const char *const statname[] =
  {"running", "dead", "suspended", "normal"};
static int auxstatus (lua_State *L, lua_State *co) {
  if (L == co) return COS_RUN;
  else {
    switch (lua_status(co)) {
      case LUA_YIELD:
        return COS_YIELD;
      case LUA_OK: {
        lua_Debug ar;
        if (lua_getstack(co, 0, &ar))  
          return COS_NORM;  
        else if (lua_gettop(co) == 0)
            return COS_DEAD;
        else
          return COS_YIELD;  
      }
      default:  
        return COS_DEAD;
    }
  }
}
static int luaB_costatus (lua_State *L) {
  lua_State *co = getco(L);
  lua_pushstring(L, statname[auxstatus(L, co)]);
  return 1;
}
static int luaB_yieldable (lua_State *L) {
  lua_State *co = lua_isnone(L, 1) ? L : getco(L);
  lua_pushboolean(L, lua_isyieldable(co));
  return 1;
}
static int luaB_corunning (lua_State *L) {
  int ismain = lua_pushthread(L);
  lua_pushboolean(L, ismain);
  return 2;
}
static int luaB_close (lua_State *L) {
  lua_State *co = getco(L);
  int status = auxstatus(L, co);
  switch (status) {
    case COS_DEAD: case COS_YIELD: {
      status = lua_resetthread(co);
      if (status == LUA_OK) {
        lua_pushboolean(L, 1);
        return 1;
      }
      else {
        lua_pushboolean(L, 0);
        lua_xmove(co, L, 1);  
        return 2;
      }
    }
    default:  
      return luaL_error(L, "cannot close a %s coroutine", statname[status]);
  }
}
static const luaL_Reg co_funcs[] = {
  {"create", luaB_cocreate},
  {"resume", luaB_coresume},
  {"running", luaB_corunning},
  {"status", luaB_costatus},
  {"wrap", luaB_cowrap},
  {"yield", luaB_yield},
  {"isyieldable", luaB_yieldable},
  {"close", luaB_close},
  {NULL, NULL}
};
LUAMOD_API int luaopen_coroutine (lua_State *L) {
  luaL_newlib(L, co_funcs);
  return 1;
}
// root include ./lualib/ldblib.c
//included "stdio.h" 
//included "stdlib.h" 
//included "string.h" 
static const char *const HOOKKEY = "_HOOKKEY";
static void checkstack (lua_State *L, lua_State *L1, int n) {
  if (l_unlikely(L != L1 && !lua_checkstack(L1, n)))
    luaL_error(L, "stack overflow");
}
static int db_getmetatable (lua_State *L) {
  if (lua_type(L, 1) == LUA_TNONE) luaL_error(L, "bad arg #1: value excepted");
  if (!lua_getmetatable(L, 1)) {
    lua_pushnil(L);  
  }
  return 1;
}
static int db_setmetatable (lua_State *L) {
  int t = lua_type(L, 2);
  luaL_argexpected(L, t == LUA_TNIL || t == LUA_TTABLE, 2, "nil or table");
  lua_settop(L, 2);
  lua_setmetatable(L, 1);
  return 1;  
}
static lua_State *getthread (lua_State *L, int *arg) {
  if (lua_isthread(L, 1)) {
    *arg = 1;
    return lua_tothread(L, 1);
  }
  else {
    *arg = 0;
    return L;  
  }
}
static void settabss (lua_State *L, const char *k, const char *v) {
  lua_pushstring(L, v);
  lua_setfield(L, -2, k);
}
static void settabsi (lua_State *L, const char *k, int v) {
  lua_pushinteger(L, v);
  lua_setfield(L, -2, k);
}
static void settabsb (lua_State *L, const char *k, int v) {
  lua_pushboolean(L, v);
  lua_setfield(L, -2, k);
}
static void treatstackoption (lua_State *L, lua_State *L1, const char *fname) {
  if (L == L1)
    lua_rotate(L, -2, 1);  
  else
    lua_xmove(L1, L, 1);  
  lua_setfield(L, -2, fname);  
}
static int db_getinfo (lua_State *L) {
  lua_Debug ar;
  int arg;
  lua_State *L1 = getthread(L, &arg);
  const char *options = luaL_optstring(L, arg+2, "flnSrtu");
  checkstack(L, L1, 3);
  luaL_argcheck(L, options[0] != '>', arg + 2, "invalid option '>'");
  if (lua_isfunction(L, arg + 1)) {  
    options = lua_pushfstring(L, ">%s", options);  
    lua_pushvalue(L, arg + 1);  
    lua_xmove(L, L1, 1);
  }
  else {  
    if (!lua_getstack(L1, (int)luaL_checkinteger(L, arg + 1), &ar)) {
      luaL_pushfail(L);  
      return 1;
    }
  }
  if (!lua_getinfo(L1, options, &ar))
    return luaL_argerror(L, arg+2, "invalid option");
  lua_newtable(L);  
  if (strchr(options, 'S')) {
    lua_pushlstring(L, ar.source, ar.srclen);
    lua_setfield(L, -2, "source");
    settabss(L, "short_src", ar.short_src);
    settabsi(L, "linedefined", ar.linedefined);
    settabsi(L, "lastlinedefined", ar.lastlinedefined);
    settabss(L, "what", ar.what);
  }
  if (strchr(options, 'l'))
    settabsi(L, "currentline", ar.currentline);
  if (strchr(options, 'u')) {
    settabsi(L, "nups", ar.nups);
    settabsi(L, "nparams", ar.nparams);
    settabsb(L, "isvararg", ar.isvararg);
  }
  if (strchr(options, 'n')) {
    settabss(L, "name", ar.name);
    settabss(L, "namewhat", ar.namewhat);
  }
  if (strchr(options, 'r')) {
    settabsi(L, "ftransfer", ar.ftransfer);
    settabsi(L, "ntransfer", ar.ntransfer);
  }
  if (strchr(options, 't'))
    settabsb(L, "istailcall", ar.istailcall);
  if (strchr(options, 'L'))
    treatstackoption(L, L1, "activelines");
  if (strchr(options, 'f'))
    treatstackoption(L, L1, "func");
  return 1;  
}
static int db_getfname(lua_State* L) {
 luaL_checktype(L, 1, LUA_TFUNCTION);
 lua_settop(L, 1);
 lua_pushvalue(L, 1);
 lua_Debug ar;
 lua_getinfo(L, ">nS", &ar);
 if (ar.namewhat[0] != '\0') {
  lua_pushfstring(L, "%s '%s'", ar.namewhat, ar.name);
 } else if (ar.what[0] == 'C') { // C code
  lua_pushfstring(L, "C function:(%p)", lua_topointer(L, 1));
 } else if (ar.what[0] == 'm') { // ???
  lua_pushfstring(L, "main chunk:[in %s]", ar.source);
 } else if (ar.what[0] == 'L') { // lua code
  lua_pushfstring(L, "function:%s[in %s]", ar.name, ar.source);
 } else {
  lua_pushfstring(L, "function:(%p)", lua_topointer(L, 1));
 }
 return 1;
}
static int auxupvalue (lua_State *L, int get) {
  const char *name;
  int n = (int)luaL_checkinteger(L, 2);  
  luaL_checktype(L, 1, LUA_TFUNCTION);  
  name = get ? lua_getupvalue(L, 1, n) : lua_setupvalue(L, 1, n);
  if (name == NULL) return 0;
  lua_pushstring(L, name);
  lua_insert(L, -(get+1));  
  return get + 1;
}
static int db_getupvalue (lua_State *L) {
  return auxupvalue(L, 1);
}
static int db_setupvalue (lua_State *L) {
  luaL_checkany(L, 3);
  return auxupvalue(L, 0);
}
static void *checkupval (lua_State *L, int argf, int argnup, int *pnup) {
  void *id;
  int nup = (int)luaL_checkinteger(L, argnup);  
  luaL_checktype(L, argf, LUA_TFUNCTION);  
  id = lua_upvalueid(L, argf, nup);
  if (pnup) {
    luaL_argcheck(L, id != NULL, argnup, "invalid upvalue index");
    *pnup = nup;
  }
  return id;
}
static int db_upvalueid (lua_State *L) {
  void *id = checkupval(L, 1, 2, NULL);
  if (id != NULL)
    lua_pushlightuserdata(L, id);
  else
    luaL_pushfail(L);
  return 1;
}
static int db_upvaluejoin (lua_State *L) {
  int n1, n2;
  checkupval(L, 1, 2, &n1);
  checkupval(L, 3, 4, &n2);
  luaL_argcheck(L, !lua_iscfunction(L, 1), 1, "Lua function expected");
  luaL_argcheck(L, !lua_iscfunction(L, 3), 3, "Lua function expected");
  lua_upvaluejoin(L, 1, n1, 3, n2);
  return 0;
}
static void hookf (lua_State *L, lua_Debug *ar) {
  static const char *const hooknames[] =
    {"call", "return", "line", "count", "tail call"};
  lua_getfield(L, LUA_REGISTRYINDEX, HOOKKEY);
  lua_pushthread(L);
  if (lua_rawget(L, -2) == LUA_TFUNCTION) {  
    lua_pushstring(L, hooknames[(int)ar->event]);  
    if (ar->currentline >= 0)
      lua_pushinteger(L, ar->currentline);  
    else lua_pushnil(L);
    lua_assert(lua_getinfo(L, "lS", ar));
    lua_call(L, 2, 0);  
  }
}
static int makemask (const char *smask, int count) {
  int mask = 0;
  if (strchr(smask, 'c')) mask |= LUA_MASKCALL;
  if (strchr(smask, 'r')) mask |= LUA_MASKRET;
  if (strchr(smask, 'l')) mask |= LUA_MASKLINE;
  if (count > 0) mask |= LUA_MASKCOUNT;
  return mask;
}
static char *unmakemask (int mask, char *smask) {
  int i = 0;
  if (mask & LUA_MASKCALL) smask[i++] = 'c';
  if (mask & LUA_MASKRET) smask[i++] = 'r';
  if (mask & LUA_MASKLINE) smask[i++] = 'l';
  smask[i] = '\0';
  return smask;
}
static int db_sethook (lua_State *L) {
  int arg, mask, count;
  lua_Hook func;
  lua_State *L1 = getthread(L, &arg);
  if (lua_isnoneornil(L, arg+1)) {  
    lua_settop(L, arg+1);
    func = NULL; mask = 0; count = 0;  
  }
  else {
    const char *smask = luaL_checkstring(L, arg+2);
    luaL_checktype(L, arg+1, LUA_TFUNCTION);
    count = (int)luaL_optinteger(L, arg + 3, 0);
    func = hookf; mask = makemask(smask, count);
  }
  if (!luaL_getsubtable(L, LUA_REGISTRYINDEX, HOOKKEY)) {
    
    lua_pushliteral(L, "k");
    lua_setfield(L, -2, "__mode");  
    lua_pushvalue(L, -1);
    lua_setmetatable(L, -2);  
  }
  checkstack(L, L1, 1);
  lua_pushthread(L1); lua_xmove(L1, L, 1);  
  lua_pushvalue(L, arg + 1);  
  lua_rawset(L, -3);  
  lua_sethook(L1, func, mask, count);
  return 0;
}
static int db_gethook (lua_State *L) {
  int arg;
  lua_State *L1 = getthread(L, &arg);
  char buff[5];
  int mask = lua_gethookmask(L1);
  lua_Hook hook = lua_gethook(L1);
  if (hook == NULL) {  
    luaL_pushfail(L);
    return 1;
  }
  else if (hook != hookf)  
    lua_pushliteral(L, "external hook");
  else {  
    lua_getfield(L, LUA_REGISTRYINDEX, HOOKKEY);
    checkstack(L, L1, 1);
    lua_pushthread(L1); lua_xmove(L1, L, 1);
    lua_rawget(L, -2);   
    lua_remove(L, -2);  
  }
  lua_pushstring(L, unmakemask(mask, buff));  
  lua_pushinteger(L, lua_gethookcount(L1));  
  return 3;
}
static int db_traceback (lua_State *L) {
  int arg;
  lua_State *L1 = getthread(L, &arg);
  const char *msg = lua_tostring(L, arg + 1);
  if (msg == NULL && !lua_isnoneornil(L, arg + 1))  
    lua_pushvalue(L, arg + 1);  
  else {
    int level = (int)luaL_optinteger(L, arg + 2, (L == L1) ? 1 : 0);
    luaL_traceback(L, L1, msg, level);
  }
  return 1;
}
static const luaL_Reg dblib[] = {
 #if !LUA_NO_UNSAFE
  {"gethook", db_gethook},
  {"getmetatable", db_getmetatable},
  {"getupvalue", db_getupvalue},
  {"upvaluejoin", db_upvaluejoin},
  {"upvalueid", db_upvalueid},
  {"setmetatable", db_setmetatable},
  {"setupvalue", db_setupvalue},
 #endif
  {"getinfo", db_getinfo},
  {"sethook", db_sethook},
  {"traceback", db_traceback},
  {"getfname", db_getfname},
  {NULL, NULL}
};
LUAMOD_API int luaopen_debug (lua_State *L) {
  luaL_newlib(L, dblib);
  return 1;
}
// root include ./lualib/lmathlib.c
#include <float.h>
#include <limits.h>
//included "math.h" 
//included "stdlib.h" 
#include <time.h>
//included "string.h" 
#undef PI
#define PI (l_mathop(3.141592653589793238462643383279502884))
static int math_abs (lua_State *L) {
  if (lua_isinteger(L, 1)) {
    lua_Integer n = lua_tointeger(L, 1);
    if (n < 0) n = (lua_Integer)(0u - (lua_Unsigned)n);
    lua_pushinteger(L, n);
  }
  else
    lua_pushnumber(L, l_mathop(fabs)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_sin (lua_State *L) {
  lua_pushnumber(L, l_mathop(sin)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_cos (lua_State *L) {
  lua_pushnumber(L, l_mathop(cos)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_tan (lua_State *L) {
  lua_pushnumber(L, l_mathop(tan)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_asin (lua_State *L) {
  lua_pushnumber(L, l_mathop(asin)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_acos (lua_State *L) {
  lua_pushnumber(L, l_mathop(acos)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_atan (lua_State *L) {
  lua_Number y = luaL_checknumber(L, 1);
  lua_Number x = luaL_optnumber(L, 2, 1);
  lua_pushnumber(L, l_mathop(atan2)(y, x));
  return 1;
}
static int math_toint (lua_State *L) {
  int valid;
  lua_Integer n = lua_tointegerx(L, 1, &valid);
  if (l_likely(valid))
    lua_pushinteger(L, n);
  else {
    luaL_checkany(L, 1);
    luaL_pushfail(L);  
  }
  return 1;
}
static void pushnumint (lua_State *L, lua_Number d) {
  lua_Integer n;
  if (lua_numbertointeger(d, &n))  
    lua_pushinteger(L, n);  
  else
    lua_pushnumber(L, d);  
}
static int math_floor (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  
  else {
    lua_Number d = l_mathop(floor)(luaL_checknumber(L, 1));
    pushnumint(L, d);
  }
  return 1;
}
static int math_ceil (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  
  else {
    lua_Number d = l_mathop(ceil)(luaL_checknumber(L, 1));
    pushnumint(L, d);
  }
  return 1;
}
static int math_fmod (lua_State *L) {
  if (lua_isinteger(L, 1) && lua_isinteger(L, 2)) {
    lua_Integer d = lua_tointeger(L, 2);
    if ((lua_Unsigned)d + 1u <= 1u) {  
      luaL_argcheck(L, d != 0, 2, "zero");
      lua_pushinteger(L, 0);  
    }
    else
      lua_pushinteger(L, lua_tointeger(L, 1) % d);
  }
  else
    lua_pushnumber(L, l_mathop(fmod)(luaL_checknumber(L, 1),
                                     luaL_checknumber(L, 2)));
  return 1;
}
static int math_modf (lua_State *L) {
  if (lua_isinteger(L ,1)) {
    lua_settop(L, 1);  
    lua_pushnumber(L, 0);  
  }
  else {
    lua_Number n = luaL_checknumber(L, 1);
    
    lua_Number ip = (n < 0) ? l_mathop(ceil)(n) : l_mathop(floor)(n);
    pushnumint(L, ip);
    
    lua_pushnumber(L, (n == ip) ? l_mathop(0.0) : (n - ip));
  }
  return 2;
}
static int math_sqrt (lua_State *L) {
  lua_pushnumber(L, l_mathop(sqrt)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_ult (lua_State *L) {
  lua_Integer a = luaL_checkinteger(L, 1);
  lua_Integer b = luaL_checkinteger(L, 2);
  lua_pushboolean(L, (lua_Unsigned)a < (lua_Unsigned)b);
  return 1;
}
static int math_log (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number res;
  if (lua_isnoneornil(L, 2))
    res = l_mathop(log)(x);
  else {
    lua_Number base = luaL_checknumber(L, 2);
#if !defined(LUA_USE_C89)
    if (base == l_mathop(2.0))
      res = l_mathop(log2)(x);
    else
#endif
    if (base == l_mathop(10.0))
      res = l_mathop(log10)(x);
    else
      res = l_mathop(log)(x)/l_mathop(log)(base);
  }
  lua_pushnumber(L, res);
  return 1;
}
static int math_exp (lua_State *L) {
  lua_pushnumber(L, l_mathop(exp)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_deg (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (l_mathop(180.0) / PI));
  return 1;
}
static int math_rad (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (PI / l_mathop(180.0)));
  return 1;
}
static int math_min (lua_State *L) {
  int n = lua_gettop(L);  
  int imin = 1;  
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, i, imin, LUA_OPLT))
      imin = i;
  }
  lua_pushvalue(L, imin);
  return 1;
}
static int math_max (lua_State *L) {
  int n = lua_gettop(L);  
  int imax = 1;  
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, imax, i, LUA_OPLT))
      imax = i;
  }
  lua_pushvalue(L, imax);
  return 1;
}
static int math_type (lua_State *L) {
  if (lua_type(L, 1) == LUA_TNUMBER)
    lua_pushstring(L, (lua_isinteger(L, 1)) ? "integer" : "float");
  else {
    luaL_pushfail(L);
  }
  return 1;
}
#define FIGS l_floatatt(MANT_DIG)
#if FIGS > 64
#undef FIGS
#define FIGS 64
#endif
#if !defined(LUA_RAND32) && !defined(Rand64)
#if (ULONG_MAX >> 31 >> 31) >= 3
#define Rand64  unsigned long
#elif !defined(LUA_USE_C89) && defined(LLONG_MAX)
#define Rand64  unsigned long long
#elif (LUA_MAXUNSIGNED >> 31 >> 31) >= 3
#define Rand64  lua_Unsigned
#endif
#endif
#if defined(Rand64)  
#define trim64(x) ((x) & 0xffffffffffffffffu)
static Rand64 rotl (Rand64 x, int n) {
  return (x << n) | (trim64(x) >> (64 - n));
}
static Rand64 nextrand (Rand64 *state) {
  Rand64 state0 = state[0];
  Rand64 state1 = state[1];
  Rand64 state2 = state[2] ^ state0;
  Rand64 state3 = state[3] ^ state1;
  Rand64 res = rotl(state1 * 5, 7) * 9;
  state[0] = state0 ^ state3;
  state[1] = state1 ^ state2;
  state[2] = state2 ^ (state1 << 17);
  state[3] = rotl(state3, 45);
  return res;
}
#define shift64_FIG (64 - FIGS)
#define scaleFIG (l_mathop(0.5) / ((Rand64)1 << (FIGS - 1)))
static lua_Number I2d (Rand64 x) {
  return (lua_Number)(trim64(x) >> shift64_FIG) * scaleFIG;
}
#define I2UInt(x) ((lua_Unsigned)trim64(x))
#define Int2I(x) ((Rand64)(x))
#else 
#if LUAI_IS32INT
typedef unsigned int lu_int32;
#else
typedef unsigned long lu_int32;
#endif
typedef struct Rand64 {
  lu_int32 h;  
  lu_int32 l;  
} Rand64;
#define trim32(x) ((x) & 0xffffffffu)
static Rand64 packI (lu_int32 h, lu_int32 l) {
  Rand64 result;
  result.h = h;
  result.l = l;
  return result;
}
static Rand64 Ishl (Rand64 i, int n) {
  lua_assert(n > 0 && n < 32);
  return packI((i.h << n) | (trim32(i.l) >> (32 - n)), i.l << n);
}
static void Ixor (Rand64 *i1, Rand64 i2) {
  i1->h ^= i2.h;
  i1->l ^= i2.l;
}
static Rand64 Iadd (Rand64 i1, Rand64 i2) {
  Rand64 result = packI(i1.h + i2.h, i1.l + i2.l);
  if (trim32(result.l) < trim32(i1.l))  
    result.h++;
  return result;
}
static Rand64 times5 (Rand64 i) {
  return Iadd(Ishl(i, 2), i);  
}
static Rand64 times9 (Rand64 i) {
  return Iadd(Ishl(i, 3), i);  
}
static Rand64 rotl (Rand64 i, int n) {
  lua_assert(n > 0 && n < 32);
  return packI((i.h << n) | (trim32(i.l) >> (32 - n)),
               (trim32(i.h) >> (32 - n)) | (i.l << n));
}
static Rand64 rotl1 (Rand64 i, int n) {
  lua_assert(n > 32 && n < 64);
  n = 64 - n;
  return packI((trim32(i.h) >> n) | (i.l << (32 - n)),
               (i.h << (32 - n)) | (trim32(i.l) >> n));
}
static Rand64 nextrand (Rand64 *state) {
  Rand64 res = times9(rotl(times5(state[1]), 7));
  Rand64 t = Ishl(state[1], 17);
  Ixor(&state[2], state[0]);
  Ixor(&state[3], state[1]);
  Ixor(&state[1], state[2]);
  Ixor(&state[0], state[3]);
  Ixor(&state[2], t);
  state[3] = rotl1(state[3], 45);
  return res;
}
#define UONE  ((lu_int32)1)
#if FIGS <= 32
#define scaleFIG       (l_mathop(0.5) / (UONE << (FIGS - 1)))
static lua_Number I2d (Rand64 x) {
  lua_Number h = (lua_Number)(trim32(x.h) >> (32 - FIGS));
  return h * scaleFIG;
}
#else 
#define scaleFIG      (l_mathop(1.0) / (UONE << 30) / l_mathop(8.0) / (UONE << (FIGS - 33)))
#define shiftLOW (64 - FIGS)
#define shiftHI  ((lua_Number)(UONE << (FIGS - 33)) * l_mathop(2.0))
static lua_Number I2d (Rand64 x) {
  lua_Number h = (lua_Number)trim32(x.h) * shiftHI;
  lua_Number l = (lua_Number)(trim32(x.l) >> shiftLOW);
  return (h + l) * scaleFIG;
}
#endif
static lua_Unsigned I2UInt (Rand64 x) {
  return ((lua_Unsigned)trim32(x.h) << 31 << 1) | (lua_Unsigned)trim32(x.l);
}
static Rand64 Int2I (lua_Unsigned n) {
  return packI((lu_int32)(n >> 31 >> 1), (lu_int32)n);
}
#endif  
typedef struct {
  Rand64 s[4];
} RanState;
static lua_Unsigned project (lua_Unsigned ran, lua_Unsigned n,
                             RanState *state) {
  if ((n & (n + 1)) == 0)  
    return ran & n;  
  else {
    lua_Unsigned lim = n;
    
    lim |= (lim >> 1);
    lim |= (lim >> 2);
    lim |= (lim >> 4);
    lim |= (lim >> 8);
    lim |= (lim >> 16);
#if (LUA_MAXUNSIGNED >> 31) >= 3
    lim |= (lim >> 32);  
#endif
    lua_assert((lim & (lim + 1)) == 0  
      && lim >= n  
      && (lim >> 1) < n);  
    while ((ran &= lim) > n)  
      ran = I2UInt(nextrand(state->s));  
    return ran;
  }
}
static int math_random (lua_State *L) {
  lua_Integer low, up;
  lua_Unsigned p;
  RanState *state = (RanState *)lua_touserdata(L, lua_upvalueindex(1));
  Rand64 rv = nextrand(state->s);  
  switch (lua_gettop(L)) {  
    case 0: {  
      lua_pushnumber(L, I2d(rv));  
      return 1;
    }
    case 1: {  
      low = 1;
      up = luaL_checkinteger(L, 1);
      if (up == 0) {  
        lua_pushinteger(L, I2UInt(rv));  
        return 1;
      }
      break;
    }
    case 2: {  
      low = luaL_checkinteger(L, 1);
      up = luaL_checkinteger(L, 2);
      break;
    }
    default: return luaL_error(L, "wrong number of arguments");
  }
  
  luaL_argcheck(L, low <= up, 1, "interval is empty");
  
  p = project(I2UInt(rv), (lua_Unsigned)up - (lua_Unsigned)low, state);
  lua_pushinteger(L, p + (lua_Unsigned)low);
  return 1;
}
static void setseed (lua_State *L, Rand64 *state,
                     lua_Unsigned n1, lua_Unsigned n2) {
  int i;
  state[0] = Int2I(n1);
  state[1] = Int2I(0xff);  
  state[2] = Int2I(n2);
  state[3] = Int2I(0);
  for (i = 0; i < 16; i++)
    nextrand(state);  
  lua_pushinteger(L, n1);
  lua_pushinteger(L, n2);
}
static void randseed (lua_State *L, RanState *state) {
  lua_Unsigned seed1 = (lua_Unsigned)time(NULL);
  lua_Unsigned seed2 = (lua_Unsigned)(size_t)L;
  setseed(L, state->s, seed1, seed2);
}
static int math_randomseed (lua_State *L) {
  RanState *state = (RanState *)lua_touserdata(L, lua_upvalueindex(1));
  if (lua_isnone(L, 1)) {
    randseed(L, state);
  }
  else {
    lua_Integer n1 = luaL_checkinteger(L, 1);
    lua_Integer n2 = luaL_optinteger(L, 2, 0);
    setseed(L, state->s, n1, n2);
  }
  return 2;  
}
static const luaL_Reg randfuncs[] = {
  {"random", math_random},
  {"randomseed", math_randomseed},
  {NULL, NULL}
};
static void setrandfunc (lua_State *L) {
  RanState *state = (RanState *)lua_newuserdatauv(L, sizeof(RanState), 0);
  randseed(L, state);  
  lua_pop(L, 2);  
  luaL_setfuncs(L, randfuncs, 1);
}
#if defined(LUA_COMPAT_MATHLIB)
static int math_cosh (lua_State *L) {
  lua_pushnumber(L, l_mathop(cosh)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_sinh (lua_State *L) {
  lua_pushnumber(L, l_mathop(sinh)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_tanh (lua_State *L) {
  lua_pushnumber(L, l_mathop(tanh)(luaL_checknumber(L, 1)));
  return 1;
}
static int math_pow (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number y = luaL_checknumber(L, 2);
  lua_pushnumber(L, l_mathop(pow)(x, y));
  return 1;
}
static int math_frexp (lua_State *L) {
  int e;
  lua_pushnumber(L, l_mathop(frexp)(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}
static int math_ldexp (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  int ep = (int)luaL_checkinteger(L, 2);
  lua_pushnumber(L, l_mathop(ldexp)(x, ep));
  return 1;
}
static int math_log10 (lua_State *L) {
  lua_pushnumber(L, l_mathop(log10)(luaL_checknumber(L, 1)));
  return 1;
}
#endif
static const luaL_Reg mathlib[] = {
  {"abs",   math_abs},
  {"acos",  math_acos},
  {"asin",  math_asin},
  {"atan",  math_atan},
  {"ceil",  math_ceil},
  {"cos",   math_cos},
  {"deg",   math_deg},
  {"exp",   math_exp},
  {"tointeger", math_toint},
  {"floor", math_floor},
  {"fmod",   math_fmod},
  {"ult",   math_ult},
  {"log",   math_log},
  {"max",   math_max},
  {"min",   math_min},
  {"modf",   math_modf},
  {"rad",   math_rad},
  {"sin",   math_sin},
  {"sqrt",  math_sqrt},
  {"tan",   math_tan},
  {"type", math_type},
#if defined(LUA_COMPAT_MATHLIB)
  {"atan2", math_atan},
  {"cosh",   math_cosh},
  {"sinh",   math_sinh},
  {"tanh",   math_tanh},
  {"pow",   math_pow},
  {"frexp", math_frexp},
  {"ldexp", math_ldexp},
  {"log10", math_log10},
#endif
  
  {"random", NULL},
  {"randomseed", NULL},
  {"pi", NULL},
  {"huge", NULL},
  {"maxinteger", NULL},
  {"mininteger", NULL},
  {NULL, NULL}
};
LUAMOD_API int luaopen_math (lua_State *L) {
  luaL_newlib(L, mathlib);
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, (lua_Number)HUGE_VAL);
  lua_setfield(L, -2, "huge");
  lua_pushinteger(L, LUA_MAXINTEGER);
  lua_setfield(L, -2, "maxinteger");
  lua_pushinteger(L, LUA_MININTEGER);
  lua_setfield(L, -2, "mininteger");
  setrandfunc(L);
  return 1;
}
// root include ./lualib/lstrlib.c
//included "ctype.h" 
//included "float.h" 
//included "limits.h" 
//included "math.h" 
#include <stddef.h>
//included "stdio.h" 
//included "stdlib.h" 
//included "string.h" 
#if !defined(LUA_MAXCAPTURES)
#define LUA_MAXCAPTURES  32
#endif
#define uchar(c) ((unsigned char)(c))
#define MAX_SIZET ((size_t)(~(size_t)0))
#define MAXSIZE   (sizeof(size_t) < sizeof(int) ? MAX_SIZET : (size_t)(INT_MAX))
static int str_len (lua_State *L) {
  size_t l;
  luaL_checklstring(L, 1, &l);
  lua_pushinteger(L, (lua_Integer)l);
  return 1;
}
static size_t posrelatI (lua_Integer pos, size_t len) {
  if (pos > 0)
    return (size_t)pos;
  else if (pos == 0)
    return 1;
  else if (pos < -(lua_Integer)len)  
    return 1;  
  else return len + (size_t)pos + 1;
}
static size_t getendpos (lua_State *L, int arg, lua_Integer def,
                         size_t len) {
  lua_Integer pos = luaL_optinteger(L, arg, def);
  if (pos > (lua_Integer)len)
    return len;
  else if (pos >= 0)
    return (size_t)pos;
  else if (pos < -(lua_Integer)len)
    return 0;
  else return len + (size_t)pos + 1;
}
static int str_sub (lua_State *L) {
  size_t l;
  const char *s = luaL_checklstring(L, 1, &l);
  size_t start = posrelatI(luaL_checkinteger(L, 2), l);
  size_t end = getendpos(L, 3, -1, l);
  if (start <= end)
    lua_pushlstring(L, s + start - 1, (end - start) + 1);
  else lua_pushliteral(L, "");
  return 1;
}
static int str_reverse (lua_State *L) {
  size_t l, i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  char *p = luaL_buffinitsize(L, &b, l);
  for (i = 0; i < l; i++)
    p[i] = s[l - i - 1];
  luaL_pushresultsize(&b, l);
  return 1;
}
static int str_lower (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  char *p = luaL_buffinitsize(L, &b, l);
  for (i=0; i<l; i++)
    p[i] = tolower(uchar(s[i]));
  luaL_pushresultsize(&b, l);
  return 1;
}
static int str_upper (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  char *p = luaL_buffinitsize(L, &b, l);
  for (i=0; i<l; i++)
    p[i] = toupper(uchar(s[i]));
  luaL_pushresultsize(&b, l);
  return 1;
}
static int str_rep (lua_State *L) {
  size_t l, lsep;
  const char *s = luaL_checklstring(L, 1, &l);
  lua_Integer n = luaL_checkinteger(L, 2);
  const char *sep = luaL_optlstring(L, 3, "", &lsep);
  if (n <= 0)
    lua_pushliteral(L, "");
  else if (l_unlikely(l + lsep < l || l + lsep > MAXSIZE / n))
    return luaL_error(L, "resulting string too large");
  else {
    size_t totallen = (size_t)n * l + (size_t)(n - 1) * lsep;
    luaL_Buffer b;
    char *p = luaL_buffinitsize(L, &b, totallen);
    while (n-- > 1) {  
      memcpy(p, s, l * sizeof(char)); p += l;
      if (lsep > 0) {  
        memcpy(p, sep, lsep * sizeof(char));
        p += lsep;
      }
    }
    memcpy(p, s, l * sizeof(char));  
    luaL_pushresultsize(&b, totallen);
  }
  return 1;
}
static int str_byte (lua_State *L) {
  size_t l;
  const char *s = luaL_checklstring(L, 1, &l);
  lua_Integer pi = luaL_optinteger(L, 2, 1);
  size_t posi = posrelatI(pi, l);
  size_t pose = getendpos(L, 3, pi, l);
  int n, i;
  if (posi > pose) return 0;  
  if (l_unlikely(pose - posi >= (size_t)INT_MAX))  
    return luaL_error(L, "string slice too long");
  n = (int)(pose -  posi) + 1;
  luaL_checkstack(L, n, "string slice too long");
  for (i=0; i<n; i++)
    lua_pushinteger(L, uchar(s[posi+i-1]));
  return n;
}
static int str_char (lua_State *L) {
  int n = lua_gettop(L);  
  int i;
  luaL_Buffer b;
  char *p = luaL_buffinitsize(L, &b, n);
  for (i=1; i<=n; i++) {
    lua_Unsigned c = (lua_Unsigned)luaL_checkinteger(L, i);
    luaL_argcheck(L, c <= (lua_Unsigned)UCHAR_MAX, i, "value out of range");
    p[i - 1] = uchar(c);
  }
  luaL_pushresultsize(&b, n);
  return 1;
}
struct str_Writer {
  int init;  
  luaL_Buffer B;
};
static int writerbc (lua_State *L, const void *b, size_t size, void *ud) {
  struct str_Writer *state = (struct str_Writer *)ud;
  if (!state->init) {
    state->init = 1;
    luaL_buffinit(L, &state->B);
  }
  luaL_addlstring(&state->B, (const char *)b, size);
  return 0;
}
static int str_dump (lua_State *L) {
  struct str_Writer state = {0};
  int strip = lua_toboolean(L, 2);
  luaL_checktype(L, 1, LUA_TFUNCTION);
  lua_settop(L, 1);  
  state.init = 0;
  if (l_unlikely(lua_dump(L, writerbc, &state, strip) != 0))
    return luaL_error(L, "unable to dump given function");
  luaL_pushresult(&state.B);
  return 1;
}
#if defined(LUA_NOCVTS2N) 
static const luaL_Reg stringmetamethods[] = {
  {"__index", NULL},  
  {NULL, NULL}
};
#endif  
#define CAP_UNFINISHED (-1)
#define CAP_POSITION (-2)
typedef struct MatchState {
  const char *src_init;  
  const char *src_end;  
  const char *p_end;  
  lua_State *L;
  int matchdepth;  
  unsigned char level;  
  struct {
    const char *init;
    ptrdiff_t len;
  } capture[LUA_MAXCAPTURES];
} MatchState;
static const char *match (MatchState *ms, const char *s, const char *p);
#if !defined(MAXCCALLS)
#define MAXCCALLS 200
#endif
#define L_ESC  '%'
#define SPECIALS "^$*+?.([%-"
static int check_capture (MatchState *ms, int l) {
  l -= '1';
  if (l_unlikely(l < 0 || l >= ms->level ||
                 ms->capture[l].len == CAP_UNFINISHED))
    return luaL_error(ms->L, "invalid capture index %%%d", l + 1);
  return l;
}
static int capture_to_close (MatchState *ms) {
  int level = ms->level;
  for (level--; level>=0; level--)
    if (ms->capture[level].len == CAP_UNFINISHED) return level;
  return luaL_error(ms->L, "invalid pattern capture");
}
static const char *classend (MatchState *ms, const char *p) {
  switch (*p++) {
    case L_ESC: {
      if (l_unlikely(p == ms->p_end))
        luaL_error(ms->L, "malformed pattern (ends with '%%')");
      return p+1;
    }
    case '[': {
      if (*p == '^') p++;
      do {  
        if (l_unlikely(p == ms->p_end))
          luaL_error(ms->L, "malformed pattern (missing ']')");
        if (*(p++) == L_ESC && p < ms->p_end)
          p++;  
      } while (*p != ']');
      return p+1;
    }
    default: {
      return p;
    }
  }
}
static int match_class (int c, int cl) {
  int res;
  switch (tolower(cl)) {
    case 'a' : res = isalpha(c); break;
    case 'c' : res = iscntrl(c); break;
    case 'd' : res = isdigit(c); break;
    case 'g' : res = isgraph(c); break;
    case 'l' : res = islower(c); break;
    case 'p' : res = ispunct(c); break;
    case 's' : res = isspace(c); break;
    case 'u' : res = isupper(c); break;
    case 'w' : res = isalnum(c); break;
    case 'x' : res = isxdigit(c); break;
    case 'z' : res = (c == 0); break;  
    default: return (cl == c);
  }
  return (islower(cl) ? res : !res);
}
static int matchbracketclass (int c, const char *p, const char *ec) {
  int sig = 1;
  if (*(p+1) == '^') {
    sig = 0;
    p++;  
  }
  while (++p < ec) {
    if (*p == L_ESC) {
      p++;
      if (match_class(c, uchar(*p)))
        return sig;
    }
    else if ((*(p+1) == '-') && (p+2 < ec)) {
      p+=2;
      if (uchar(*(p-2)) <= c && c <= uchar(*p))
        return sig;
    }
    else if (uchar(*p) == c) return sig;
  }
  return !sig;
}
static int singlematch (MatchState *ms, const char *s, const char *p,
                        const char *ep) {
  if (s >= ms->src_end)
    return 0;
  else {
    int c = uchar(*s);
    switch (*p) {
      case '.': return 1;  
      case L_ESC: return match_class(c, uchar(*(p+1)));
      case '[': return matchbracketclass(c, p, ep-1);
      default:  return (uchar(*p) == c);
    }
  }
}
static const char *matchbalance (MatchState *ms, const char *s,
                                   const char *p) {
  if (l_unlikely(p >= ms->p_end - 1))
    luaL_error(ms->L, "malformed pattern (missing arguments to '%%b')");
  if (*s != *p) return NULL;
  else {
    int b = *p;
    int e = *(p+1);
    int cont = 1;
    while (++s < ms->src_end) {
      if (*s == e) {
        if (--cont == 0) return s+1;
      }
      else if (*s == b) cont++;
    }
  }
  return NULL;  
}
static const char *max_expand (MatchState *ms, const char *s,
                                 const char *p, const char *ep) {
  ptrdiff_t i = 0;  
  while (singlematch(ms, s + i, p, ep))
    i++;
  
  while (i>=0) {
    const char *res = match(ms, (s+i), ep+1);
    if (res) return res;
    i--;  
  }
  return NULL;
}
static const char *min_expand (MatchState *ms, const char *s,
                                 const char *p, const char *ep) {
  for (;;) {
    const char *res = match(ms, s, ep+1);
    if (res != NULL)
      return res;
    else if (singlematch(ms, s, p, ep))
      s++;  
    else return NULL;
  }
}
static const char *start_capture (MatchState *ms, const char *s,
                                    const char *p, int what) {
  const char *res;
  int level = ms->level;
  if (level >= LUA_MAXCAPTURES) luaL_error(ms->L, "too many captures");
  ms->capture[level].init = s;
  ms->capture[level].len = what;
  ms->level = level+1;
  if ((res=match(ms, s, p)) == NULL)  
    ms->level--;  
  return res;
}
static const char *end_capture (MatchState *ms, const char *s,
                                  const char *p) {
  int l = capture_to_close(ms);
  const char *res;
  ms->capture[l].len = s - ms->capture[l].init;  
  if ((res = match(ms, s, p)) == NULL)  
    ms->capture[l].len = CAP_UNFINISHED;  
  return res;
}
static const char *match_capture (MatchState *ms, const char *s, int l) {
  size_t len;
  l = check_capture(ms, l);
  len = ms->capture[l].len;
  if ((size_t)(ms->src_end-s) >= len &&
      memcmp(ms->capture[l].init, s, len) == 0)
    return s+len;
  else return NULL;
}
static const char *match (MatchState *ms, const char *s, const char *p) {
  if (l_unlikely(ms->matchdepth-- == 0))
    luaL_error(ms->L, "pattern too complex");
  init: 
  if (p != ms->p_end) {  
    switch (*p) {
      case '(': {  
        if (*(p + 1) == ')')  
          s = start_capture(ms, s, p + 2, CAP_POSITION);
        else
          s = start_capture(ms, s, p + 1, CAP_UNFINISHED);
        break;
      }
      case ')': {  
        s = end_capture(ms, s, p + 1);
        break;
      }
      case '$': {
        if ((p + 1) != ms->p_end)  
          goto dflt;  
        s = (s == ms->src_end) ? s : NULL;  
        break;
      }
      case L_ESC: {  
        switch (*(p + 1)) {
          case 'b': {  
            s = matchbalance(ms, s, p + 2);
            if (s != NULL) {
              p += 4; goto init;  
            }  
            break;
          }
          case 'f': {  
            const char *ep; char previous;
            p += 2;
            if (l_unlikely(*p != '['))
              luaL_error(ms->L, "missing '[' after '%%f' in pattern");
            ep = classend(ms, p);  
            previous = (s == ms->src_init) ? '\0' : *(s - 1);
            if (!matchbracketclass(uchar(previous), p, ep - 1) &&
               matchbracketclass(uchar(*s), p, ep - 1)) {
              p = ep; goto init;  
            }
            s = NULL;  
            break;
          }
          case '0': case '1': case '2': case '3':
          case '4': case '5': case '6': case '7':
          case '8': case '9': {  
            s = match_capture(ms, s, uchar(*(p + 1)));
            if (s != NULL) {
              p += 2; goto init;  
            }
            break;
          }
          default: goto dflt;
        }
        break;
      }
      default: dflt: {  
        const char *ep = classend(ms, p);  
        
        if (!singlematch(ms, s, p, ep)) {
          if (*ep == '*' || *ep == '?' || *ep == '-') {  
            p = ep + 1; goto init;  
          }
          else  
            s = NULL;  
        }
        else {  
          switch (*ep) {  
            case '?': {  
              const char *res;
              if ((res = match(ms, s + 1, ep + 1)) != NULL)
                s = res;
              else {
                p = ep + 1; goto init;  
              }
              break;
            }
            case '+':  
              s++;  
              s = max_expand(ms, s, p, ep);
              break;
            case '*':  
              s = max_expand(ms, s, p, ep);
              break;
            case '-':  
              s = min_expand(ms, s, p, ep);
              break;
            default:  
              s++; p = ep; goto init;  
          }
        }
        break;
      }
    }
  }
  ms->matchdepth++;
  return s;
}
static const char *lmemfind (const char *s1, size_t l1,
                               const char *s2, size_t l2) {
  if (l2 == 0) return s1;  
  else if (l2 > l1) return NULL;  
  else {
    const char *init;  
    l2--;  
    l1 = l1-l2;  
    while (l1 > 0 && (init = (const char *)memchr(s1, *s2, l1)) != NULL) {
      init++;   
      if (memcmp(init, s2+1, l2) == 0)
        return init-1;
      else {  
        l1 -= init-s1;
        s1 = init;
      }
    }
    return NULL;  
  }
}
static size_t get_onecapture (MatchState *ms, int i, const char *s,
                              const char *e, const char **cap) {
  if (i >= ms->level) {
    if (l_unlikely(i != 0))
      luaL_error(ms->L, "invalid capture index %%%d", i + 1);
    *cap = s;
    return e - s;
  }
  else {
    ptrdiff_t capl = ms->capture[i].len;
    *cap = ms->capture[i].init;
    if (l_unlikely(capl == CAP_UNFINISHED))
      luaL_error(ms->L, "unfinished capture");
    else if (capl == CAP_POSITION)
      lua_pushinteger(ms->L, (ms->capture[i].init - ms->src_init) + 1);
    return capl;
  }
}
static void push_onecapture (MatchState *ms, int i, const char *s,
                                                    const char *e) {
  const char *cap;
  ptrdiff_t l = get_onecapture(ms, i, s, e, &cap);
  if (l != CAP_POSITION)
    lua_pushlstring(ms->L, cap, l);
  
}
static int push_captures (MatchState *ms, const char *s, const char *e) {
  int i;
  int nlevels = (ms->level == 0 && s) ? 1 : ms->level;
  luaL_checkstack(ms->L, nlevels, "too many captures");
  for (i = 0; i < nlevels; i++)
    push_onecapture(ms, i, s, e);
  return nlevels;  
}
static int nospecials (const char *p, size_t l) {
  size_t upto = 0;
  do {
    if (strpbrk(p + upto, SPECIALS))
      return 0;  
    upto += strlen(p + upto) + 1;  
  } while (upto <= l);
  return 1;  
}
static void prepstate (MatchState *ms, lua_State *L,
                       const char *s, size_t ls, const char *p, size_t lp) {
  ms->L = L;
  ms->matchdepth = MAXCCALLS;
  ms->src_init = s;
  ms->src_end = s + ls;
  ms->p_end = p + lp;
}
static void reprepstate (MatchState *ms) {
  ms->level = 0;
  lua_assert(ms->matchdepth == MAXCCALLS);
}
static int str_find_aux (lua_State *L, int find) {
  size_t ls, lp;
  const char *s = luaL_checklstring(L, 1, &ls);
  const char *p = luaL_checklstring(L, 2, &lp);
  size_t init = posrelatI(luaL_optinteger(L, 3, 1), ls) - 1;
  if (init > ls) {  
    luaL_pushfail(L);  
    return 1;
  }
  
  if (find && (lua_toboolean(L, 4) || nospecials(p, lp))) {
    
    const char *s2 = lmemfind(s + init, ls - init, p, lp);
    if (s2) {
      lua_pushinteger(L, (s2 - s) + 1);
      lua_pushinteger(L, (s2 - s) + lp);
      return 2;
    }
  }
  else {
    MatchState ms;
    const char *s1 = s + init;
    int anchor = (*p == '^');
    if (anchor) {
      p++; lp--;  
    }
    prepstate(&ms, L, s, ls, p, lp);
    do {
      const char *res;
      reprepstate(&ms);
      if ((res=match(&ms, s1, p)) != NULL) {
        if (find) {
          lua_pushinteger(L, (s1 - s) + 1);  
          lua_pushinteger(L, res - s);   
          return push_captures(&ms, NULL, 0) + 2;
        }
        else
          return push_captures(&ms, s1, res);
      }
    } while (s1++ < ms.src_end && !anchor);
  }
  luaL_pushfail(L);  
  return 1;
}
static int str_find (lua_State *L) {
  return str_find_aux(L, 1);
}
static int str_match (lua_State *L) {
  return str_find_aux(L, 0);
}
typedef struct GMatchState {
  const char *src;  
  const char *p;  
  const char *lastmatch;  
  MatchState ms;  
} GMatchState;
static int gmatch_aux (lua_State *L) {
  GMatchState *gm = (GMatchState *)lua_touserdata(L, lua_upvalueindex(3));
  const char *src;
  gm->ms.L = L;
  for (src = gm->src; src <= gm->ms.src_end; src++) {
    const char *e;
    reprepstate(&gm->ms);
    if ((e = match(&gm->ms, src, gm->p)) != NULL && e != gm->lastmatch) {
      gm->src = gm->lastmatch = e;
      return push_captures(&gm->ms, src, e);
    }
  }
  return 0;  
}
static int gmatch (lua_State *L) {
  size_t ls, lp;
  const char *s = luaL_checklstring(L, 1, &ls);
  const char *p = luaL_checklstring(L, 2, &lp);
  size_t init = posrelatI(luaL_optinteger(L, 3, 1), ls) - 1;
  GMatchState *gm;
  lua_settop(L, 2);  
  gm = (GMatchState *)lua_newuserdatauv(L, sizeof(GMatchState), 0);
  if (init > ls)  
    init = ls + 1;  
  prepstate(&gm->ms, L, s, ls, p, lp);
  gm->src = s + init; gm->p = p; gm->lastmatch = NULL;
  lua_pushcclosure(L, gmatch_aux, 3);
  return 1;
}
static void add_s (MatchState *ms, luaL_Buffer *b, const char *s,
                                                   const char *e) {
  size_t l;
  lua_State *L = ms->L;
  const char *news = lua_tolstring(L, 3, &l);
  const char *p;
  while ((p = (char *)memchr(news, L_ESC, l)) != NULL) {
    luaL_addlstring(b, news, p - news);
    p++;  
    if (*p == L_ESC)  
      luaL_addchar(b, *p);
    else if (*p == '0')  
        luaL_addlstring(b, s, e - s);
    else if (isdigit(uchar(*p))) {  
      const char *cap;
      ptrdiff_t resl = get_onecapture(ms, *p - '1', s, e, &cap);
      if (resl == CAP_POSITION)
        luaL_addvalue(b);  
      else
        luaL_addlstring(b, cap, resl);
    }
    else
      luaL_error(L, "invalid use of '%c' in replacement string", L_ESC);
    l -= p + 1 - news;
    news = p + 1;
  }
  luaL_addlstring(b, news, l);
}
static int add_value (MatchState *ms, luaL_Buffer *b, const char *s,
                                      const char *e, int tr) {
  lua_State *L = ms->L;
  switch (tr) {
    case LUA_TFUNCTION: {  
      int n;
      lua_pushvalue(L, 3);  
      n = push_captures(ms, s, e);  
      lua_call(L, n, 1);  
      break;
    }
    case LUA_TTABLE: {  
      push_onecapture(ms, 0, s, e);  
      lua_gettable(L, 3);
      break;
    }
    default: {  
      add_s(ms, b, s, e);  
      return 1;  
    }
  }
  if (!lua_toboolean(L, -1)) {  
    lua_pop(L, 1);  
    luaL_addlstring(b, s, e - s);  
    return 0;  
  }
  else if (l_unlikely(!lua_isstring(L, -1)))
    return luaL_error(L, "invalid replacement value (a %s)",
                         luaL_typename(L, -1));
  else {
    luaL_addvalue(b);  
    return 1;  
  }
}
static int str_gsub (lua_State *L) {
  size_t srcl, lp;
  const char *src = luaL_checklstring(L, 1, &srcl);  
  const char *p = luaL_checklstring(L, 2, &lp);  
  const char *lastmatch = NULL;  
  int tr = lua_type(L, 3);  
  lua_Integer max_s = luaL_optinteger(L, 4, srcl + 1);  
  int anchor = (*p == '^');
  lua_Integer n = 0;  
  int changed = 0;  
  MatchState ms;
  luaL_Buffer b;
  luaL_argexpected(L, tr == LUA_TNUMBER || tr == LUA_TSTRING ||
                   tr == LUA_TFUNCTION || tr == LUA_TTABLE, 3,
                      "string/function/table");
  luaL_buffinit(L, &b);
  if (anchor) {
    p++; lp--;  
  }
  prepstate(&ms, L, src, srcl, p, lp);
  while (n < max_s) {
    const char *e;
    reprepstate(&ms);  
    if ((e = match(&ms, src, p)) != NULL && e != lastmatch) {  
      n++;
      changed = add_value(&ms, &b, src, e, tr) | changed;
      src = lastmatch = e;
    }
    else if (src < ms.src_end)  
      luaL_addchar(&b, *src++);
    else break;  
    if (anchor) break;
  }
  if (!changed)  
    lua_pushvalue(L, 1);  
  else {  
    luaL_addlstring(&b, src, ms.src_end-src);
    luaL_pushresult(&b);  
  }
  lua_pushinteger(L, n);  
  return 2;
}
#if !defined(lua_number2strx) 
#define SIZELENMOD (sizeof(LUA_NUMBER_FRMLEN)/sizeof(char))
#define L_NBFD  ((l_floatatt(MANT_DIG) - 1)%4 + 1)
static lua_Number adddigit (char *buff, int n, lua_Number x) {
  lua_Number dd = l_mathop(floor)(x);  
  int d = (int)dd;
  buff[n] = (d < 10 ? d + '0' : d - 10 + 'a');  
  return x - dd;  
}
static int num2straux (char *buff, int sz, lua_Number x) {
  
  if (x != x || x == (lua_Number)HUGE_VAL || x == -(lua_Number)HUGE_VAL)
    return l_sprintf(buff, sz, LUA_NUMBER_FMT, (LUAI_UACNUMBER)x);
  else if (x == 0) {  
    
    return l_sprintf(buff, sz, LUA_NUMBER_FMT "x0p+0", (LUAI_UACNUMBER)x);
  }
  else {
    int e;
    lua_Number m = l_mathop(frexp)(x, &e);  
    int n = 0;  
    if (m < 0) {  
      buff[n++] = '-';  
      m = -m;  
    }
    buff[n++] = '0'; buff[n++] = 'x';  
    m = adddigit(buff, n++, m * (1 << L_NBFD));  
    e -= L_NBFD;  
    if (m > 0) {  
      buff[n++] = lua_getlocaledecpoint();  
      do {  
        m = adddigit(buff, n++, m * 16);
      } while (m > 0);
    }
    n += l_sprintf(buff + n, sz - n, "p%+d", e);  
    lua_assert(n < sz);
    return n;
  }
}
static int lua_number2strx (lua_State *L, char *buff, int sz,
                            const char *fmt, lua_Number x) {
  int n = num2straux(buff, sz, x);
  if (fmt[SIZELENMOD] == 'A') {
    int i;
    for (i = 0; i < n; i++)
      buff[i] = toupper(uchar(buff[i]));
  }
  else if (l_unlikely(fmt[SIZELENMOD] != 'a'))
    return luaL_error(L, "modifiers for format '%%a'/'%%A' not implemented");
  return n;
}
#endif    
#define MAX_ITEMF (110 + l_floatatt(MAX_10_EXP))
#define MAX_ITEM 120
#if !defined(L_FMTFLAGSF)
#define L_FMTFLAGSF "-+#0 "
#define L_FMTFLAGSX "-#0"
#define L_FMTFLAGSI "-+0 "
#define L_FMTFLAGSU "-0"
#define L_FMTFLAGSC "-"
#endif
#define MAX_FORMAT 32
static void addquoted (luaL_Buffer *b, const char *s, size_t len) {
  luaL_addchar(b, '"');
  while (len--) {
    if (*s == '"' || *s == '\\' || *s == '\n') {
      luaL_addchar(b, '\\');
      luaL_addchar(b, *s);
    }
    else if (iscntrl(uchar(*s))) {
      char buff[10];
      if (!isdigit(uchar(*(s+1))))
        l_sprintf(buff, sizeof(buff), "\\%d", (int)uchar(*s));
      else
        l_sprintf(buff, sizeof(buff), "\\%03d", (int)uchar(*s));
      luaL_addstring(b, buff);
    }
    else
      luaL_addchar(b, *s);
    s++;
  }
  luaL_addchar(b, '"');
}
static int quotefloat (lua_State *L, char *buff, lua_Number n) {
  const char *s;  
  if (n == (lua_Number)HUGE_VAL)  
    s = "1e9999";
  else if (n == -(lua_Number)HUGE_VAL)  
    s = "-1e9999";
  else if (n != n)  
    s = "(0/0)";
  else {  
    int  nb = lua_number2strx(L, buff, MAX_ITEM,
                                 "%" LUA_NUMBER_FRMLEN "a", n);
    
    if (memchr(buff, '.', nb) == NULL) {  
      char point = lua_getlocaledecpoint();  
      char *ppoint = (char *)memchr(buff, point, nb);
      if (ppoint) *ppoint = '.';  
    }
    return nb;
  }
  
  return l_sprintf(buff, MAX_ITEM, "%s", s);
}
static void addliteral (lua_State *L, luaL_Buffer *b, int arg) {
  switch (lua_type(L, arg)) {
    case LUA_TSTRING: {
      size_t len;
      const char *s = lua_tolstring(L, arg, &len);
      addquoted(b, s, len);
      break;
    }
    case LUA_TNUMBER: {
      char *buff = luaL_prepbuffsize(b, MAX_ITEM);
      int nb;
      if (!lua_isinteger(L, arg))  
        nb = quotefloat(L, buff, lua_tonumber(L, arg));
      else {  
        lua_Integer n = lua_tointeger(L, arg);
        const char *format = (n == LUA_MININTEGER)  
                           ? "0x%" LUA_INTEGER_FRMLEN "x"  
                           : LUA_INTEGER_FMT;  
        nb = l_sprintf(buff, MAX_ITEM, format, (LUAI_UACINT)n);
      }
      luaL_addsize(b, nb);
      break;
    }
    case LUA_TNIL: case LUA_TBOOLEAN: {
      luaL_tolstring(L, arg, NULL);
      luaL_addvalue(b);
      break;
    }
    default: {
      luaL_argerror(L, arg, "value has no literal form");
    }
  }
}
static const char *get2digits (const char *s) {
  if (isdigit(uchar(*s))) {
    s++;
    if (isdigit(uchar(*s))) s++;  
  }
  return s;
}
static void checkformat (lua_State *L, const char *form, const char *flags,
                                       int precision) {
  const char *spec = form + 1;  
  spec += strspn(spec, flags);  
  if (*spec != '0') {  
    spec = get2digits(spec);  
    if (*spec == '.' && precision) {
      spec++;
      spec = get2digits(spec);  
    }
  }
  if (!isalpha(uchar(*spec)))  
    luaL_error(L, "invalid conversion specification: '%s'", form);
}
static const char *getformat (lua_State *L, const char *strfrmt,
                                            char *form) {
  
  size_t len = strspn(strfrmt, L_FMTFLAGSF "123456789.");
  len++;  
  
  if (len >= MAX_FORMAT - 10)
    luaL_error(L, "invalid format (too long)");
  *(form++) = '%';
  memcpy(form, strfrmt, len * sizeof(char));
  *(form + len) = '\0';
  return strfrmt + len - 1;
}
static void addlenmod (char *form, const char *lenmod) {
  size_t l = strlen(form);
  size_t lm = strlen(lenmod);
  char spec = form[l - 1];
  strcpy(form + l - 1, lenmod);
  form[l + lm - 1] = spec;
  form[l + lm] = '\0';
}
static int str_format (lua_State *L) {
  int top = lua_gettop(L);
  int arg = 1;
  size_t sfl;
  const char *strfrmt = luaL_checklstring(L, arg, &sfl);
  const char *strfrmt_end = strfrmt+sfl;
  const char *flags;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  while (strfrmt < strfrmt_end) {
    if (*strfrmt != L_ESC)
      luaL_addchar(&b, *strfrmt++);
    else if (*++strfrmt == L_ESC)
      luaL_addchar(&b, *strfrmt++);  
    else { 
      char form[MAX_FORMAT];  
      int maxitem = MAX_ITEM;  
      char *buff = luaL_prepbuffsize(&b, maxitem);  
      int nb = 0;  
      if (++arg > top)
        return luaL_argerror(L, arg, "no value");
      strfrmt = getformat(L, strfrmt, form);
      switch (*strfrmt++) {
        case 'c': {
          checkformat(L, form, L_FMTFLAGSC, 0);
          nb = l_sprintf(buff, maxitem, form, (int)luaL_checkinteger(L, arg));
          break;
        }
        case 'd': case 'i':
          flags = L_FMTFLAGSI;
          goto intcase;
        case 'u':
          flags = L_FMTFLAGSU;
          goto intcase;
        case 'o': case 'x': case 'X':
          flags = L_FMTFLAGSX;
         intcase: {
          lua_Integer n = luaL_checkinteger(L, arg);
          checkformat(L, form, flags, 1);
          addlenmod(form, LUA_INTEGER_FRMLEN);
          nb = l_sprintf(buff, maxitem, form, (LUAI_UACINT)n);
          break;
        }
        case 'a': case 'A':
          checkformat(L, form, L_FMTFLAGSF, 1);
          addlenmod(form, LUA_NUMBER_FRMLEN);
          nb = lua_number2strx(L, buff, maxitem, form,
                                  luaL_checknumber(L, arg));
          break;
        case 'f':
          maxitem = MAX_ITEMF;  
          buff = luaL_prepbuffsize(&b, maxitem);
     goto ft_3f;
        case 'e': case 'E': case 'g': case 'G': 
    ft_3f : {
          lua_Number n = luaL_checknumber(L, arg);
          checkformat(L, form, L_FMTFLAGSF, 1);
          addlenmod(form, LUA_NUMBER_FRMLEN);
          nb = l_sprintf(buff, maxitem, form, (LUAI_UACNUMBER)n);
          break;
        }
        case 'p': {
          const void *p = lua_topointer(L, arg);
          checkformat(L, form, L_FMTFLAGSC, 0);
          if (p == NULL) {  
            p = "(null)";  
            form[strlen(form) - 1] = 's';  
          }
          nb = l_sprintf(buff, maxitem, form, p);
          break;
        }
        case 'q': {
          if (form[2] != '\0')  
            return luaL_error(L, "specifier '%%q' cannot have modifiers");
          addliteral(L, &b, arg);
          break;
        }
        case 's': {
          size_t l;
          const char *s = luaL_tolstring(L, arg, &l);
          if (form[2] == '\0')  
            luaL_addvalue(&b);  
          else {
            luaL_argcheck(L, l == strlen(s), arg, "string contains zeros");
            checkformat(L, form, L_FMTFLAGSC, 1);
            if (strchr(form, '.') == NULL && l >= 100) {
              
              luaL_addvalue(&b);  
            }
            else {  
              nb = l_sprintf(buff, maxitem, form, s);
              lua_pop(L, 1);  
            }
          }
          break;
        }
        default: {  
          return luaL_error(L, "invalid conversion '%s' to 'format'", form);
        }
      }
      lua_assert(nb < maxitem);
      luaL_addsize(&b, nb);
    }
  }
  luaL_pushresult(&b);
  return 1;
}
static const luaL_Reg strlib[] = {
  {"byte", str_byte},
  {"char", str_char},
  {"dump", str_dump},
  {"find", str_find},
  {"format", str_format},
  {"gmatch", gmatch},
  {"gsub", str_gsub},
  {"len", str_len},
  {"lower", str_lower},
  {"match", str_match},
  {"rep", str_rep},
  {"reverse", str_reverse},
  {"sub", str_sub},
  {"upper", str_upper},
  
  {NULL, NULL}
};
static void createmetatable (lua_State *L) {
  
  luaL_newlibtable(L, stringmetamethods);
  luaL_setfuncs(L, stringmetamethods, 0);
  lua_pushliteral(L, "");  
  lua_pushvalue(L, -2);  
  lua_setmetatable(L, -2);  
  lua_pop(L, 1);  
  lua_pushvalue(L, -2);  
  lua_setfield(L, -2, "__index");  
  lua_pop(L, 1);  
}
LUAMOD_API int luaopen_string (lua_State *L) {
  luaL_newlib(L, strlib);
  createmetatable(L);
  return 1;
}
// root include ./lualib/ltablib.c
//included "limits.h" 
//included "stddef.h" 
//included "string.h" 
#define TAB_R 1   
#define TAB_W 2   
#define TAB_L 4   
#define TAB_RW (TAB_R | TAB_W)  
#define aux_getn(L,n,w) (checktab(L, n, (w) | TAB_L), luaL_len(L, n))
static int checkfield (lua_State *L, const char *key, int n) {
  lua_pushstring(L, key);
  return (lua_rawget(L, -n) != LUA_TNIL);
}
static void checktab (lua_State *L, int arg, int what) {
  if (lua_type(L, arg) != LUA_TTABLE) {  
    int n = 1;  
    if (lua_getmetatable(L, arg) &&  
        (!(what & TAB_R) || checkfield(L, "__index", ++n)) &&
        (!(what & TAB_W) || checkfield(L, "__newindex", ++n)) &&
        (!(what & TAB_L) || checkfield(L, "__len", ++n))) {
      lua_pop(L, n);  
    }
    else
      luaL_checktype(L, arg, LUA_TTABLE);  
  }
}
static int tinsert (lua_State *L) {
  lua_Integer pos;  
  lua_Integer e = aux_getn(L, 1, TAB_RW);
  e = luaL_intop(+, e, 1);  
  switch (lua_gettop(L)) {
    case 2: {  
      pos = e;  
      break;
    }
    case 3: {
      lua_Integer i;
      pos = luaL_checkinteger(L, 2);  
      
      luaL_argcheck(L, (lua_Unsigned)pos - 1u < (lua_Unsigned)e, 2,
                       "position out of bounds");
      for (i = e; i > pos; i--) {  
        lua_geti(L, 1, i - 1);
        lua_seti(L, 1, i);  
      }
      break;
    }
    default: {
      return luaL_error(L, "wrong number of arguments to 'insert'");
    }
  }
  lua_seti(L, 1, pos);  
  return 0;
}
static int tremove (lua_State *L) {
  lua_Integer size = aux_getn(L, 1, TAB_RW);
  lua_Integer pos = luaL_optinteger(L, 2, size);
  if (pos != size)  
    
    luaL_argcheck(L, (lua_Unsigned)pos - 1u <= (lua_Unsigned)size, 1,
                     "position out of bounds");
  lua_geti(L, 1, pos);  
  for ( ; pos < size; pos++) {
    lua_geti(L, 1, pos + 1);
    lua_seti(L, 1, pos);  
  }
  lua_pushnil(L);
  lua_seti(L, 1, pos);  
  return 1;
}
static int tmove (lua_State *L) {
  lua_Integer f = luaL_checkinteger(L, 2);
  lua_Integer e = luaL_checkinteger(L, 3);
  lua_Integer t = luaL_checkinteger(L, 4);
  int tt = !lua_isnoneornil(L, 5) ? 5 : 1;  
  checktab(L, 1, TAB_R);
  checktab(L, tt, TAB_W);
  if (e >= f) {  
    lua_Integer n, i;
    luaL_argcheck(L, f > 0 || e < LUA_MAXINTEGER + f, 3,
                  "too many elements to move");
    n = e - f + 1;  
    luaL_argcheck(L, t <= LUA_MAXINTEGER - n + 1, 4,
                  "destination wrap around");
    if (t > e || t <= f || (tt != 1 && !lua_compare(L, 1, tt, LUA_OPEQ))) {
      for (i = 0; i < n; i++) {
        lua_geti(L, 1, f + i);
        lua_seti(L, tt, t + i);
      }
    }
    else {
      for (i = n - 1; i >= 0; i--) {
        lua_geti(L, 1, f + i);
        lua_seti(L, tt, t + i);
      }
    }
  }
  lua_pushvalue(L, tt);  
  return 1;
}
static void addfield (lua_State *L, luaL_Buffer *b, lua_Integer i) {
  lua_geti(L, 1, i);
  if (l_unlikely(!lua_isstring(L, -1)))
    luaL_error(L, "invalid value (%s) at index %I in table for 'concat'",
                  luaL_typename(L, -1), (LUAI_UACINT)i);
  luaL_addvalue(b);
}
static int tconcat (lua_State *L) {
  luaL_Buffer b;
  lua_Integer last = aux_getn(L, 1, TAB_R);
  size_t lsep;
  const char *sep = luaL_optlstring(L, 2, "", &lsep);
  lua_Integer i = luaL_optinteger(L, 3, 1);
  last = luaL_optinteger(L, 4, last);
  luaL_buffinit(L, &b);
  for (; i < last; i++) {
    addfield(L, &b, i);
    luaL_addlstring(&b, sep, lsep);
  }
  if (i == last)  
    addfield(L, &b, i);
  luaL_pushresult(&b);
  return 1;
}
static int tpack (lua_State *L) {
  int i;
  int n = lua_gettop(L);  
  lua_createtable(L, n, 1);  
  lua_insert(L, 1);  
  for (i = n; i >= 1; i--)  
    lua_seti(L, 1, i);
  lua_pushinteger(L, n);
  lua_setfield(L, 1, "n");  
  return 1;  
}
static int tunpack (lua_State *L) {
  lua_Unsigned n;
  lua_Integer i = luaL_optinteger(L, 2, 1);
  lua_Integer e = luaL_opt(L, luaL_checkinteger, 3, luaL_len(L, 1));
  if (i > e) return 0;  
  n = (lua_Unsigned)e - i;  
  if (l_unlikely(n >= (unsigned int)INT_MAX  ||
                 !lua_checkstack(L, (int)(++n))))
    return luaL_error(L, "too many results to unpack");
  for (; i < e; i++) {  
    lua_geti(L, 1, i);
  }
  lua_geti(L, 1, e);  
  return (int)n;
}
typedef unsigned int IdxT;
#if !defined(l_randomizePivot)  //included "time.h" 
#define sof(e)  (sizeof(e) / sizeof(unsigned int))
static unsigned int l_randomizePivot (void) {
  clock_t c = clock();
  time_t t = time(NULL);
  unsigned int buff[sof(c) + sof(t)];
  unsigned int i, rnd = 0;
  memcpy(buff, &c, sof(c) * sizeof(unsigned int));
  memcpy(buff + sof(c), &t, sof(t) * sizeof(unsigned int));
  for (i = 0; i < sof(buff); i++)
    rnd += buff[i];
  return rnd;
}
#endif     
#define RANLIMIT 100u
static void set2 (lua_State *L, IdxT i, IdxT j) {
  lua_seti(L, 1, i);
  lua_seti(L, 1, j);
}
static int sort_comp (lua_State *L, int a, int b) {
  if (lua_isnil(L, 2))  
    return lua_compare(L, a, b, LUA_OPLT);  
  else {  
    int res;
    lua_pushvalue(L, 2);    
    lua_pushvalue(L, a-1);  
    lua_pushvalue(L, b-2);  
    lua_call(L, 2, 1);      
    res = lua_toboolean(L, -1);  
    lua_pop(L, 1);          
    return res;
  }
}
static IdxT partition (lua_State *L, IdxT lo, IdxT up) {
  IdxT i = lo;  
  IdxT j = up - 1;  
  
  for (;;) {
    
    while ((void)lua_geti(L, 1, ++i), sort_comp(L, -1, -2)) {
      if (l_unlikely(i == up - 1))  
        luaL_error(L, "invalid order function for sorting");
      lua_pop(L, 1);  
    }
    
    
    while ((void)lua_geti(L, 1, --j), sort_comp(L, -3, -1)) {
      if (l_unlikely(j < i))  
        luaL_error(L, "invalid order function for sorting");
      lua_pop(L, 1);  
    }
    
    if (j < i) {  
      
      lua_pop(L, 1);  
      
      set2(L, up - 1, i);
      return i;
    }
    
    set2(L, i, j);
  }
}
static IdxT choosePivot (IdxT lo, IdxT up, unsigned int rnd) {
  IdxT r4 = (up - lo) / 4;  
  IdxT p = rnd % (r4 * 2) + (lo + r4);
  lua_assert(lo + r4 <= p && p <= up - r4);
  return p;
}
static void auxsort (lua_State *L, IdxT lo, IdxT up,
                                   unsigned int rnd) {
  while (lo < up) {  
    IdxT p;  
    IdxT n;  
    
    lua_geti(L, 1, lo);
    lua_geti(L, 1, up);
    if (sort_comp(L, -1, -2))  
      set2(L, lo, up);  
    else
      lua_pop(L, 2);  
    if (up - lo == 1)  
      return;  
    if (up - lo < RANLIMIT || rnd == 0)  
      p = (lo + up)/2;  
    else  
      p = choosePivot(lo, up, rnd);
    lua_geti(L, 1, p);
    lua_geti(L, 1, lo);
    if (sort_comp(L, -2, -1))  
      set2(L, p, lo);  
    else {
      lua_pop(L, 1);  
      lua_geti(L, 1, up);
      if (sort_comp(L, -1, -2))  
        set2(L, p, up);  
      else
        lua_pop(L, 2);
    }
    if (up - lo == 2)  
      return;  
    lua_geti(L, 1, p);  
    lua_pushvalue(L, -1);  
    lua_geti(L, 1, up - 1);  
    set2(L, p, up - 1);  
    p = partition(L, lo, up);
    
    if (p - lo < up - p) {  
      auxsort(L, lo, p - 1, rnd);  
      n = p - lo;  
      lo = p + 1;  
    }
    else {
      auxsort(L, p + 1, up, rnd);  
      n = up - p;  
      up = p - 1;  
    }
    if ((up - lo) / 128 > n) 
      rnd = l_randomizePivot();  
  }  
}
static int sort (lua_State *L) {
  lua_Integer n = aux_getn(L, 1, TAB_RW);
  if (n > 1) {  
    luaL_argcheck(L, n < INT_MAX, 1, "array too big");
    if (!lua_isnoneornil(L, 2))  
      luaL_checktype(L, 2, LUA_TFUNCTION);  
    lua_settop(L, 2);  
    auxsort(L, 1, (IdxT)n, 0);
  }
  return 0;
}
static int tnew(lua_State* L) {
 int a = luaL_checkinteger(L, 1);
 int b = luaL_checkinteger(L, 2);
 if (a < 0) luaL_argerror(L, 1, "number of array elements cannot be negative");
 if (b < 0) luaL_argerror(L, 2, "number of hash table slots cannot be negative");
 lua_createtable(L, a, b);
 return 1;
}
static int tclear(lua_State* L) {
 luaL_checktype(L, 1, LUA_TTABLE);
 int keep = lua_toboolean(L, 2);
 lua_cleartable(L, 1, keep);
 return 0;
}
static const luaL_Reg tab_funcs[] = {
  {"concat", tconcat},
  {"insert", tinsert},
  {"pack", tpack},
  {"unpack", tunpack},
  {"remove", tremove},
  {"move", tmove},
  {"sort", sort},
 {"new", tnew},
 {"clear", tclear},
  {NULL, NULL}
};
LUAMOD_API int luaopen_table (lua_State *L) {
  luaL_newlib(L, tab_funcs);
  return 1;
}
// root include ./lualib/lutf8lib.c
#include <assert.h>
//included "limits.h" 
//included "stdlib.h" 
//included "string.h" 
#define MAXUNICODE 0x10FFFFu
#define MAXUTF  0x7FFFFFFFu
#if (UINT_MAX >> 30) >= 1
typedef unsigned int utfint;
#else
typedef unsigned long utfint;
#endif
#define iscont(p) ((*(p) & 0xC0) == 0x80)
static lua_Integer u_posrelat (lua_Integer pos, size_t len) {
  if (pos >= 0) return pos;
  else if (0u - (size_t)pos > len) return 0;
  else return (lua_Integer)len + pos + 1;
}
static const char *utf8_decode (const char *s, utfint *val, int strict) {
  static const utfint limits[] =
        {~(utfint)0, 0x80, 0x800, 0x10000u, 0x200000u, 0x4000000u};
  unsigned int c = (unsigned char)s[0];
  utfint res = 0;  
  if (c < 0x80)  
    res = c;
  else {
    int count = 0;  
    for (; c & 0x40; c <<= 1) {  
      unsigned int cc = (unsigned char)s[++count];  
      if ((cc & 0xC0) != 0x80)  
        return NULL;  
      res = (res << 6) | (cc & 0x3F);  
    }
    res |= ((utfint)(c & 0x7F) << (count * 5));  
    if (count > 5 || res > MAXUTF || res < limits[count])
      return NULL;  
    s += count;  
  }
  if (strict) {
    
    if (res > MAXUNICODE || (0xD800u <= res && res <= 0xDFFFu))
      return NULL;
  }
  if (val) *val = res;
  return s + 1;  
}
static int utflen (lua_State *L) {
  lua_Integer n = 0;  
  size_t len;  
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Integer posi = u_posrelat(luaL_optinteger(L, 2, 1), len);
  lua_Integer posj = u_posrelat(luaL_optinteger(L, 3, -1), len);
  int lax = lua_toboolean(L, 4);
  luaL_argcheck(L, 1 <= posi && --posi <= (lua_Integer)len, 2,
                   "initial position out of bounds");
  luaL_argcheck(L, --posj < (lua_Integer)len, 3,
                   "final position out of bounds");
  while (posi <= posj) {
    const char *s1 = utf8_decode(s + posi, NULL, !lax);
    if (s1 == NULL) {  
      luaL_pushfail(L);  
      lua_pushinteger(L, posi + 1);  
      return 2;
    }
    posi = s1 - s;
    n++;
  }
  lua_pushinteger(L, n);
  return 1;
}
static int codepoint (lua_State *L) {
  size_t len;
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Integer posi = u_posrelat(luaL_optinteger(L, 2, 1), len);
  lua_Integer pose = u_posrelat(luaL_optinteger(L, 3, posi), len);
  int lax = lua_toboolean(L, 4);
  int n;
  const char *se;
  luaL_argcheck(L, posi >= 1, 2, "out of bounds");
  luaL_argcheck(L, pose <= (lua_Integer)len, 3, "out of bounds");
  if (posi > pose) return 0;  
  if (pose - posi >= INT_MAX)  
    return luaL_error(L, "string slice too long");
  n = (int)(pose -  posi) + 1;  
  luaL_checkstack(L, n, "string slice too long");
  n = 0;  
  se = s + pose;  
  for (s += posi - 1; s < se;) {
    utfint code;
    s = utf8_decode(s, &code, !lax);
    if (s == NULL)
      return luaL_error(L, "invalid UTF-8 code");
    lua_pushinteger(L, code);
    n++;
  }
  return n;
}
static void pushutfchar (lua_State *L, int arg) {
  lua_Unsigned code = (lua_Unsigned)luaL_checkinteger(L, arg);
  luaL_argcheck(L, code <= MAXUTF, arg, "value out of range");
  lua_pushfstring(L, "%U", (long)code);
}
static int utfchar (lua_State *L) {
  int n = lua_gettop(L);  
  if (n == 1)  
    pushutfchar(L, 1);
  else {
    int i;
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    for (i = 1; i <= n; i++) {
      pushutfchar(L, i);
      luaL_addvalue(&b);
    }
    luaL_pushresult(&b);
  }
  return 1;
}
static int byteoffset (lua_State *L) {
  size_t len;
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Integer n  = luaL_checkinteger(L, 2);
  lua_Integer posi = (n >= 0) ? 1 : len + 1;
  posi = u_posrelat(luaL_optinteger(L, 3, posi), len);
  luaL_argcheck(L, 1 <= posi && --posi <= (lua_Integer)len, 3,
                   "position out of bounds");
  if (n == 0) {
    
    while (posi > 0 && iscont(s + posi)) posi--;
  }
  else {
    if (iscont(s + posi))
      return luaL_error(L, "initial position is a continuation byte");
    if (n < 0) {
       while (n < 0 && posi > 0) {  
         do {  
           posi--;
         } while (posi > 0 && iscont(s + posi));
         n++;
       }
     }
     else {
       n--;  
       while (n > 0 && posi < (lua_Integer)len) {
         do {  
           posi++;
         } while (iscont(s + posi));  
         n--;
       }
     }
  }
  if (n == 0)  
    lua_pushinteger(L, posi + 1);
  else  
    luaL_pushfail(L);
  return 1;
}
static int iter_aux (lua_State *L, int strict) {
  size_t len;
  const char *s = luaL_checklstring(L, 1, &len);
  lua_Unsigned n = (lua_Unsigned)lua_tointeger(L, 2);
  if (n < len) {
    while (iscont(s + n)) n++;  
  }
  if (n >= len)  
    return 0;  
  else {
    utfint code;
    const char *next = utf8_decode(s + n, &code, strict);
    if (next == NULL)
      return luaL_error(L, "invalid UTF-8 code");
    lua_pushinteger(L, n + 1);
    lua_pushinteger(L, code);
    return 2;
  }
}
static int iter_auxstrict (lua_State *L) {
  return iter_aux(L, 1);
}
static int iter_auxlax (lua_State *L) {
  return iter_aux(L, 0);
}
static int iter_codes (lua_State *L) {
  int lax = lua_toboolean(L, 2);
  luaL_checkstring(L, 1);
  lua_pushcfunction(L, lax ? iter_auxlax : iter_auxstrict);
  lua_pushvalue(L, 1);
  lua_pushinteger(L, 0);
  return 3;
}
#define UTF8PATT "[\0-\x7F\xC2-\xFD][\x80-\xBF]*"
static const luaL_Reg funcs[] = {
  {"offset", byteoffset},
  {"codepoint", codepoint},
  {"char", utfchar},
  {"len", utflen},
  {"codes", iter_codes},
  
  {"charpattern", NULL},
  {NULL, NULL}
};
LUAMOD_API int luaopen_utf8 (lua_State *L) {
  luaL_newlib(L, funcs);
  lua_pushlstring(L, UTF8PATT, sizeof(UTF8PATT)/sizeof(char) - 1);
  lua_setfield(L, -2, "charpattern");
  return 1;
}
// root include ./lualib/linit.c
//included "stddef.h" 
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
  
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);  
  }
}
// root include ./lualib/lbuffer.c
#include <stdint.h>
//included "stdarg.h" 
//included "stdio.h" 
//included "stdlib.h" 
//included "string.h" 
typedef struct UBox {
  void *box;
  size_t bsize;
} UBox;
static void *resizebox (lua_State *L, int idx, size_t newsize) {
  void *ud;
  lua_Alloc allocf = lua_getallocf(L, &ud);
  UBox *box = (UBox *)lua_touserdata(L, idx);
  void *temp = allocf(ud, box->box, box->bsize, newsize);
  if (l_unlikely(temp == NULL && newsize > 0)) {  
    lua_pushliteral(L, "not enough memory");
    lua_error(L);  
  }
  box->box = temp;
  box->bsize = newsize;
  return temp;
}
static int boxgc (lua_State *L) {
  resizebox(L, 1, 0);
  return 0;
}
static const luaL_Reg boxmt[] = {  
  {"__gc", boxgc},
  {"__close", boxgc},
  {NULL, NULL}
};
static void newbox (lua_State *L) {
  UBox *box = (UBox *)lua_newuserdatauv(L, sizeof(UBox), 0);
  box->box = NULL;
  box->bsize = 0;
  if (luaL_newmetatable(L, "_UBOX*"))  
    luaL_setfuncs(L, boxmt, 0);  
  lua_setmetatable(L, -2);
}
#define buffonstack(B) ((B)->b != (B)->init.b)
#define FUNCHANDLER 
#define checkbufferlevel(B, idx)   lua_assert(buffonstack(B) ? lua_touserdata(B->L, idx) != NULL : lua_touserdata(B->L, idx) == (void*)B);
static size_t newbuffsize (luaL_Buffer *B, size_t sz) {
 FUNCHANDLER
  size_t newsize = B->size * 2;  
  if (l_unlikely(MAX_SIZET - sz < B->n))  
    return luaL_error(B->L, "buffer too large");
  if (newsize < B->n + sz)  
    newsize = B->n + sz;
  return newsize;
}
static char *prepbuffsize (luaL_Buffer *B, size_t sz, int boxidx) {
 FUNCHANDLER
  checkbufferlevel(B, boxidx);
  if (B->size - B->n >= sz)  
    return B->b + B->n;
  else {
    lua_State *L = B->L;
    char *newbuff;
    size_t newsize = newbuffsize(B, sz);
    
    if (buffonstack(B))  
      newbuff = (char *)resizebox(L, boxidx, newsize);  
    else {  
      lua_remove(L, boxidx);  
      newbox(L);  
      lua_insert(L, boxidx);  
      lua_toclose(L, boxidx);
      newbuff = (char *)resizebox(L, boxidx, newsize);
      memcpy(newbuff, B->b, B->n * sizeof(char));  
    }
    B->b = newbuff;
    B->size = newsize;
    return newbuff + B->n;
  }
}
LUALIB_API char *luaL_prepbuffsize (luaL_Buffer *B, size_t sz) {
 FUNCHANDLER
  return prepbuffsize(B, sz, -1);
}
LUALIB_API void luaL_addlstring (luaL_Buffer *B, const char *s, size_t l) {
 FUNCHANDLER
  if (l > 0) {  
    char *b = prepbuffsize(B, l, -1);
    memcpy(b, s, l * sizeof(char));
    luaL_addsize(B, l);
  }
}
LUALIB_API void luaL_addstring (luaL_Buffer *B, const char *s) {
 FUNCHANDLER
  luaL_addlstring(B, s, strlen(s));
}
LUALIB_API void luaL_pushresult (luaL_Buffer *B) {
 FUNCHANDLER
  lua_State *L = B->L;
  checkbufferlevel(B, -1);
  lua_pushlstring(L, B->b, B->n);
  if (buffonstack(B))
    lua_closeslot(L, -2);  
  lua_remove(L, -2);  
}
LUALIB_API void luaL_pushresultsize (luaL_Buffer *B, size_t sz) {
 FUNCHANDLER
  luaL_addsize(B, sz);
  luaL_pushresult(B);
}
LUALIB_API void luaL_addvalue (luaL_Buffer *B) {
 FUNCHANDLER
  lua_State *L = B->L;
  size_t len;
  const char *s = luaL_tolstring(L, -1, &len);
 lua_remove(L, -2);
  char *b = prepbuffsize(B, len, -2);
  memcpy(b, s, len * sizeof(char));
  luaL_addsize(B, len);
  lua_pop(L, 1);  
}
LUALIB_API void luaL_buffinit (lua_State *L, luaL_Buffer *B) {
 FUNCHANDLER
  B->L = L;
  B->b = B->init.b;
  B->n = 0;
  B->size = LUAL_BUFFERSIZE;
  lua_pushlightuserdata(L, (void*)B);  
}
LUALIB_API char *luaL_buffinitsize (lua_State *L, luaL_Buffer *B, size_t sz) {
 FUNCHANDLER
  luaL_buffinit(L, B);
  return prepbuffsize(B, sz, -1);
}
LUALIB_API void luaL_addgsub (luaL_Buffer *b, const char *s,
                                     const char *p, const char *r) {
  const char *wild;
  size_t l = strlen(p);
  while ((wild = strstr(s, p)) != NULL) {
    luaL_addlstring(b, s, wild - s);  
    luaL_addstring(b, r);  
    s = wild + l;  
  }
  luaL_addstring(b, s);  
}
// root include ./lualib/lpackage.c
//included "assert.h" 
//included "limits.h" 
//included "stdlib.h" 
//included "string.h" 
// Wrapper this around lua source string in C file
// In case preprocessing is done, this will be inlined directly as precompiled bytecode :D
#define LUA_BCDEF(NAME, SRC) static const char BC_##NAME##_DATA[] = SRC;static const size_t BC_##NAME##_SIZE = sizeof(BC_##NAME##_DATA);
#define _TOKENIZE(A) #A
// cause source always has main chunk (except for precompiled ones)
// Loads and runs bytecode with one argument from the top of the stack
// (keeping it on the stack). Returns nothing
#ifdef NOPACK
#define LUA_BCLOAD(L, NAME) luaL_loadbufferx(L, BC_##NAME##_DATA, BC_##NAME##_SIZE, _TOKENIZE(NAME), "t") != LUA_OK ? lua_error(L) : 0;  lua_pushvalue(L, -2); lua_call(L, 1, 0);
#else
#define LUA_BCLOAD(L, NAME) luaL_loadbufferx(L, BC_##NAME##_DATA, BC_##NAME##_SIZE, _TOKENIZE(NAME), "b") != LUA_OK ? lua_error(L) : 0;  lua_pushvalue(L, -2); lua_call(L, 1, 0);
#endif
static const char BC_require_DATA[] = {
27, 76, 117, 97, 85, 0, 25, 
 147, 13, 10, 26, 10, 4, 8, 
 8, 120, 86, 0, 0, 0, 0, 
 0, 0, 0, 0, 0, 0, 0, 
 40, 119, 64, 1, 128, 128, 128, 
 0, 1, 2, 133, 81, 0, 0, 
 0, 80, 0, 0, 2, 207, 0, 
 0, 0, 15, 0, 0, 1, 198, 
 128, 1, 1, 129, 4, 136, 114, 
 101, 113, 117, 105, 114, 101, 129, 
 1, 0, 0, 129, 128, 129, 129, 
 1, 0, 8, 162, 136, 0, 1, 
 0, 139, 1, 0, 0, 12, 1, 
 3, 0, 66, 1, 0, 0, 56, 
 0, 0, 128, 72, 1, 2, 0, 
 139, 1, 0, 1, 12, 1, 3, 
 0, 66, 1, 0, 0, 56, 0, 
 0, 128, 184, 7, 0, 128, 139, 
 1, 1, 2, 0, 2, 0, 0, 
 131, 130, 1, 0, 53, 2, 2, 
 0, 196, 1, 2, 3, 128, 0, 
 4, 0, 0, 1, 3, 0, 66, 
 129, 0, 0, 56, 3, 0, 128, 
 139, 1, 1, 4, 3, 130, 2, 
 0, 128, 2, 0, 0, 3, 3, 
 3, 0, 128, 3, 1, 0, 53, 
 2, 4, 0, 196, 1, 2, 1, 
 128, 1, 2, 0, 196, 1, 1, 
 2, 0, 1, 3, 0, 139, 1, 
 0, 0, 144, 1, 0, 2, 72, 
 1, 2, 0, 199, 1, 1, 0, 
 135, 4, 135, 108, 111, 97, 100, 
 101, 100, 4, 136, 112, 114, 101, 
 108, 111, 97, 100, 4, 137, 108, 
 111, 97, 100, 102, 105, 108, 101, 
 4, 133, 46, 108, 117, 97, 4, 
 134, 101, 114, 114, 111, 114, 4, 
 134, 102, 105, 108, 101, 32, 4, 
 144, 32, 105, 115, 32, 110, 111, 
 116, 32, 102, 111, 117, 110, 100, 
 33, 10, 130, 1, 0, 0, 0, 
 0, 0, 128, 128, 128, 128, 128, 
 128, 128, 128, 128};
static const size_t BC_require_SIZE = 298;

LUAMOD_API int (luaopen_package) (lua_State *L) {
 // some compability hacks :o
 lua_createtable(L, 0, 1);
 lua_pushstring(L, "loaded");
 lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
 lua_rawset(L, -3);
 lua_createtable(L, 0, 0);
 lua_setfield(L, -2, "preload");
 LUA_BCLOAD(L, require);
 return 1;
}
// root include ./lualib/lstrpack.c
//included "ctype.h" 
//included "float.h" 
//included "limits.h" 
#include <locale.h>
//included "math.h" 
//included "stddef.h" 
//included "stdio.h" 
//included "stdlib.h" 
//included "string.h" 
#if !defined(LUAL_PACKPADBYTE)
#define LUAL_PACKPADBYTE  0x00
#endif
#define MAXINTSIZE 16
#define MAX_SIZET ((size_t)(~(size_t)0))
#define MAXSIZE   (sizeof(size_t) < sizeof(int) ? MAX_SIZET : (size_t)(INT_MAX))
static size_t posrelatI2 (lua_Integer pos, size_t len) {
  if (pos > 0)
    return (size_t)pos;
  else if (pos == 0)
    return 1;
  else if (pos < -(lua_Integer)len)  
    return 1;  
  else return len + (size_t)pos + 1;
}
#define NB CHAR_BIT
#define MC ((1 << NB) - 1)
#define SZINT ((int)sizeof(lua_Integer))
static const union {
  int dummy;
  char little;  
} nativeendian = {1};
typedef struct Header {
  lua_State *L;
  int islittle;
  int maxalign;
} Header;
typedef enum KOption {
  Kint,  
  Kuint, 
  Kfloat, 
  Knumber, 
  Kdouble, 
  Kchar, 
  Kstring, 
  Kzstr, 
  Kpadding, 
  Kpaddalign, 
  Knop  
} KOption;
static int digit (int c) { return '0' <= c && c <= '9'; }
static int getnum (const char **fmt, int df) {
  if (!digit(**fmt))  
    return df;  
  else {
    int a = 0;
    do {
      a = a*10 + (*((*fmt)++) - '0');
    } while (digit(**fmt) && a <= ((int)MAXSIZE - 9)/10);
    return a;
  }
}
static int getnumlimit (Header *h, const char **fmt, int df) {
  int sz = getnum(fmt, df);
  if (l_unlikely(sz > MAXINTSIZE || sz <= 0))
    return luaL_error(h->L, "integral size (%d) out of limits [1,%d]",
                            sz, MAXINTSIZE);
  return sz;
}
static void initheader (lua_State *L, Header *h) {
  h->L = L;
  h->islittle = nativeendian.little;
  h->maxalign = 1;
}
static KOption getoption (Header *h, const char **fmt, int *size) {
  
  struct cD { char c; union { LUAI_MAXALIGN; } u; };
  int opt = *((*fmt)++);
  *size = 0;  
  switch (opt) {
    case 'b': *size = sizeof(char); return Kint;
    case 'B': *size = sizeof(char); return Kuint;
    case 'h': *size = sizeof(short); return Kint;
    case 'H': *size = sizeof(short); return Kuint;
    case 'l': *size = sizeof(long); return Kint;
    case 'L': *size = sizeof(long); return Kuint;
    case 'j': *size = sizeof(lua_Integer); return Kint;
    case 'J': *size = sizeof(lua_Integer); return Kuint;
    case 'T': *size = sizeof(size_t); return Kuint;
    case 'f': *size = sizeof(float); return Kfloat;
    case 'n': *size = sizeof(lua_Number); return Knumber;
    case 'd': *size = sizeof(double); return Kdouble;
    case 'i': *size = getnumlimit(h, fmt, sizeof(int)); return Kint;
    case 'I': *size = getnumlimit(h, fmt, sizeof(int)); return Kuint;
    case 's': *size = getnumlimit(h, fmt, sizeof(size_t)); return Kstring;
    case 'c':
      *size = getnum(fmt, -1);
      if (l_unlikely(*size == -1))
        luaL_error(h->L, "missing size for format option 'c'");
      return Kchar;
    case 'z': return Kzstr;
    case 'x': *size = 1; return Kpadding;
    case 'X': return Kpaddalign;
    case ' ': break;
    case '<': h->islittle = 1; break;
    case '>': h->islittle = 0; break;
    case '=': h->islittle = nativeendian.little; break;
    case '!': {
      const int maxalign = offsetof(struct cD, u);
      h->maxalign = getnumlimit(h, fmt, maxalign);
      break;
    }
    default: luaL_error(h->L, "invalid format option '%c'", opt);
  }
  return Knop;
}
static KOption getdetails (Header *h, size_t totalsize,
                           const char **fmt, int *psize, int *ntoalign) {
  KOption opt = getoption(h, fmt, psize);
  int align = *psize;  
  if (opt == Kpaddalign) {  
    if (**fmt == '\0' || getoption(h, fmt, &align) == Kchar || align == 0)
      luaL_argerror(h->L, 1, "invalid next option for option 'X'");
  }
  if (align <= 1 || opt == Kchar)  
    *ntoalign = 0;
  else {
    if (align > h->maxalign)  
      align = h->maxalign;
    if (l_unlikely((align & (align - 1)) != 0))  
      luaL_argerror(h->L, 1, "format asks for alignment not power of 2");
    *ntoalign = (align - (int)(totalsize & (align - 1))) & (align - 1);
  }
  return opt;
}
static void packint (luaL_Buffer *b, lua_Unsigned n,
                     int islittle, int size, int neg) {
  char *buff = luaL_prepbuffsize(b, size);
  int i;
  buff[islittle ? 0 : size - 1] = (char)(n & MC);  
  for (i = 1; i < size; i++) {
    n >>= NB;
    buff[islittle ? i : size - 1 - i] = (char)(n & MC);
  }
  if (neg && size > SZINT) {  
    for (i = SZINT; i < size; i++)  
      buff[islittle ? i : size - 1 - i] = (char)MC;
  }
  luaL_addsize(b, size);  
}
static void copywithendian (char *dest, const char *src,
                            int size, int islittle) {
  if (islittle == nativeendian.little)
    memcpy(dest, src, size);
  else {
    dest += size - 1;
    while (size-- != 0)
      *(dest--) = *(src++);
  }
}
static int str_pack (lua_State *L) {
  luaL_Buffer b;
  Header h;
  const char *fmt = luaL_checkstring(L, 1);  
  int arg = 1;  
  size_t totalsize = 0;  
  initheader(L, &h);
  lua_pushnil(L);  
  luaL_buffinit(L, &b);
  while (*fmt != '\0') {
    int size, ntoalign;
    KOption opt = getdetails(&h, totalsize, &fmt, &size, &ntoalign);
    totalsize += ntoalign + size;
    while (ntoalign-- > 0)
     luaL_addchar(&b, LUAL_PACKPADBYTE);  
    arg++;
    switch (opt) {
      case Kint: {  
        lua_Integer n = luaL_checkinteger(L, arg);
        if (size < SZINT) {  
          lua_Integer lim = (lua_Integer)1 << ((size * NB) - 1);
          luaL_argcheck(L, -lim <= n && n < lim, arg, "integer overflow");
        }
        packint(&b, (lua_Unsigned)n, h.islittle, size, (n < 0));
        break;
      }
      case Kuint: {  
        lua_Integer n = luaL_checkinteger(L, arg);
        if (size < SZINT)  
          luaL_argcheck(L, (lua_Unsigned)n < ((lua_Unsigned)1 << (size * NB)),
                           arg, "unsigned overflow");
        packint(&b, (lua_Unsigned)n, h.islittle, size, 0);
        break;
      }
      case Kfloat: {  
        float f = (float)luaL_checknumber(L, arg);  
        char *buff = luaL_prepbuffsize(&b, sizeof(f));
        
        copywithendian(buff, (char *)&f, sizeof(f), h.islittle);
        luaL_addsize(&b, size);
        break;
      }
      case Knumber: {  
        lua_Number f = luaL_checknumber(L, arg);  
        char *buff = luaL_prepbuffsize(&b, sizeof(f));
        
        copywithendian(buff, (char *)&f, sizeof(f), h.islittle);
        luaL_addsize(&b, size);
        break;
      }
      case Kdouble: {  
        double f = (double)luaL_checknumber(L, arg);  
        char *buff = luaL_prepbuffsize(&b, sizeof(f));
        
        copywithendian(buff, (char *)&f, sizeof(f), h.islittle);
        luaL_addsize(&b, size);
        break;
      }
      case Kchar: {  
        size_t len;
        const char *s = luaL_checklstring(L, arg, &len);
        luaL_argcheck(L, len <= (size_t)size, arg,
                         "string longer than given size");
        luaL_addlstring(&b, s, len);  
        while (len++ < (size_t)size)  
          luaL_addchar(&b, LUAL_PACKPADBYTE);
        break;
      }
      case Kstring: {  
        size_t len;
        const char *s = luaL_checklstring(L, arg, &len);
        luaL_argcheck(L, size >= (int)sizeof(size_t) ||
                         len < ((size_t)1 << (size * NB)),
                         arg, "string length does not fit in given size");
        packint(&b, (lua_Unsigned)len, h.islittle, size, 0);  
        luaL_addlstring(&b, s, len);
        totalsize += len;
        break;
      }
      case Kzstr: {  
        size_t len;
        const char *s = luaL_checklstring(L, arg, &len);
        luaL_argcheck(L, strlen(s) == len, arg, "string contains zeros");
        luaL_addlstring(&b, s, len);
        luaL_addchar(&b, '\0');  
        totalsize += len + 1;
        break;
      }
      case Kpadding: 
    luaL_addchar(&b, LUAL_PACKPADBYTE);  
        arg--;  
        break;
      case Kpaddalign: case Knop:
        arg--;  
        break;
    }
  }
  luaL_pushresult(&b);
  return 1;
}
static int str_packsize (lua_State *L) {
  Header h;
  const char *fmt = luaL_checkstring(L, 1);  
  size_t totalsize = 0;  
  initheader(L, &h);
  while (*fmt != '\0') {
    int size, ntoalign;
    KOption opt = getdetails(&h, totalsize, &fmt, &size, &ntoalign);
    luaL_argcheck(L, opt != Kstring && opt != Kzstr, 1,
                     "variable-length format");
    size += ntoalign;  
    luaL_argcheck(L, totalsize <= MAXSIZE - size, 1,
                     "format result too large");
    totalsize += size;
  }
  lua_pushinteger(L, (lua_Integer)totalsize);
  return 1;
}
static lua_Integer unpackint (lua_State *L, const char *str,
                              int islittle, int size, int issigned) {
  lua_Unsigned res = 0;
  int i;
  int limit = (size  <= SZINT) ? size : SZINT;
  for (i = limit - 1; i >= 0; i--) {
    res <<= NB;
    res |= (lua_Unsigned)(unsigned char)str[islittle ? i : size - 1 - i];
  }
  if (size < SZINT) {  
    if (issigned) {  
      lua_Unsigned mask = (lua_Unsigned)1 << (size*NB - 1);
      res = ((res ^ mask) - mask);  
    }
  }
  else if (size > SZINT) {  
    int mask = (!issigned || (lua_Integer)res >= 0) ? 0 : MC;
    for (i = limit; i < size; i++) {
      if (l_unlikely((unsigned char)str[islittle ? i : size - 1 - i] != mask))
        luaL_error(L, "%d-byte integer does not fit into Lua Integer", size);
    }
  }
  return (lua_Integer)res;
}
static int str_unpack (lua_State *L) {
  Header h;
  const char *fmt = luaL_checkstring(L, 1);
  size_t ld;
  const char *data = luaL_checklstring(L, 2, &ld);
  size_t pos = posrelatI2(luaL_optinteger(L, 3, 1), ld) - 1;
  int n = 0;  
  luaL_argcheck(L, pos <= ld, 3, "initial position out of string");
  initheader(L, &h);
  while (*fmt != '\0') {
    int size, ntoalign;
    KOption opt = getdetails(&h, pos, &fmt, &size, &ntoalign);
    luaL_argcheck(L, (size_t)ntoalign + size <= ld - pos, 2,
                    "data string too short");
    pos += ntoalign;  
    
    luaL_checkstack(L, 2, "too many results");
    n++;
    switch (opt) {
      case Kint:
      case Kuint: {
        lua_Integer res = unpackint(L, data + pos, h.islittle, size,
                                       (opt == Kint));
        lua_pushinteger(L, res);
        break;
      }
      case Kfloat: {
        float f;
        copywithendian((char *)&f, data + pos, sizeof(f), h.islittle);
        lua_pushnumber(L, (lua_Number)f);
        break;
      }
      case Knumber: {
        lua_Number f;
        copywithendian((char *)&f, data + pos, sizeof(f), h.islittle);
        lua_pushnumber(L, f);
        break;
      }
      case Kdouble: {
        double f;
        copywithendian((char *)&f, data + pos, sizeof(f), h.islittle);
        lua_pushnumber(L, (lua_Number)f);
        break;
      }
      case Kchar: {
        lua_pushlstring(L, data + pos, size);
        break;
      }
      case Kstring: {
        size_t len = (size_t)unpackint(L, data + pos, h.islittle, size, 0);
        luaL_argcheck(L, len <= ld - pos - size, 2, "data string too short");
        lua_pushlstring(L, data + pos + size, len);
        pos += len;  
        break;
      }
      case Kzstr: {
        size_t len = strlen(data + pos);
        luaL_argcheck(L, pos + len < ld, 2,
                         "unfinished string for format 'z'");
        lua_pushlstring(L, data + pos, len);
        pos += len + 1;  
        break;
      }
      case Kpaddalign: case Kpadding: case Knop:
        n--;  
        break;
    }
    pos += size;
  }
  lua_pushinteger(L, pos + 1);  
  return n + 1;
}
static const luaL_Reg packlib[] = {
  {"pack", str_pack},
  {"packsize", str_packsize},
  {"unpack", str_unpack},
  {NULL, NULL}
};
LUAMOD_API int luaopen_strpack(lua_State* L) {
 luaL_newlib(L, packlib);
 return 1;
}
