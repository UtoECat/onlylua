/*
** $Id: lauxlib.h $
** Auxiliary functions for building Lua libraries
** See Copyright Notice in lua.h
*/


#ifndef lauxlib_h
#define lauxlib_h


#include <stddef.h>
#include <stdio.h>

#include "luaconf.h"
#include "lua.h"


/* global table */
#define LUA_GNAME	"_G"

#include "lbuffer.h"

/* key, in the registry, for table of loaded modules */
#define LUA_LOADED_TABLE	"_LOADED"

typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;

#define LUAL_NUMSIZES	(sizeof(lua_Integer)*16 + sizeof(lua_Number))

LUALIB_API void (luaL_checkversion_) (lua_State *L, lua_Number ver, size_t sz);
#define luaL_checkversion(L)  \
	  luaL_checkversion_(L, LUA_VERSION_NUM, LUAL_NUMSIZES)

/*
 * Micellanious functions
 */
LUALIB_API void (luaL_traceback)   (lua_State *L, lua_State *L1, const char *msg, int level);

#define luaL_typename(L,i)	lua_typename(L, lua_type(L,(i)))
LUALIB_API const char *(luaL_tolstring) (lua_State *L, int idx, size_t *len);

LUALIB_API lua_State *(luaL_newstate) (void);
LUALIB_API lua_Integer (luaL_len) (lua_State *L, int idx);
LUALIB_API const char *(luaL_gsub) (lua_State *L, const char *s,
                                    const char *p, const char *r);

LUALIB_API void (luaL_setfuncs)    (lua_State *L, const luaL_Reg *l, int nup);

LUALIB_API int  (luaL_getsubtable) (lua_State *L, int idx, const char *fname);

/*
 * Type checks
 */
#define luaL_opt(L,f,n,d)	(lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

#define luaL_argcheck(L, cond,arg,extramsg)	\
	((void)(luai_likely(cond) || luaL_argerror(L, (arg), (extramsg))))

#define luaL_argexpected(L,cond,arg,tname)	\
	((void)(luai_likely(cond) || luaL_typeerror(L, (arg), (tname))))

LUALIB_API const char *(luaL_checklstring) (lua_State *L, int arg, size_t *l);
LUALIB_API const char *(luaL_optlstring) (lua_State *L, int arg, const char *def, size_t *l);

#define luaL_checkstring(L,n)	(luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d)	(luaL_optlstring(L, (n), (d), NULL))

LUALIB_API lua_Number (luaL_checknumber) (lua_State *L, int arg);
LUALIB_API lua_Number (luaL_optnumber) (lua_State *L, int arg, lua_Number def);

LUALIB_API lua_Integer (luaL_checkinteger) (lua_State *L, int arg);
LUALIB_API lua_Integer (luaL_optinteger) (lua_State *L, int arg, lua_Integer def);

LUALIB_API void (luaL_checkstack) (lua_State *L, int sz, const char *msg);
LUALIB_API void (luaL_checktype) (lua_State *L, int arg, int t);
LUALIB_API void (luaL_checkany) (lua_State *L, int arg);
LUALIB_API int  (luaL_checkoption) (lua_State *L, int arg, const char *def, const char *const lst[]);

/*
 * "Classes" API
 */
#define luaL_getmetatable(L,n) (lua_getfield(L, LUA_REGISTRYINDEX, (n)))

LUALIB_API int   (luaL_newmetatable) (lua_State *L, const char *tname);
LUALIB_API void  (luaL_setmetatable) (lua_State *L, const char *tname);
LUALIB_API void *(luaL_testudata)    (lua_State *L, int ud, const char *tname);
LUALIB_API void *(luaL_checkudata)   (lua_State *L, int ud, const char *tname);
LUALIB_API int  (luaL_getmetafield)  (lua_State *L, int obj, const char *e);
LUALIB_API int  (luaL_callmeta)      (lua_State *L, int obj, const char *e);

/*
 * Error messages
 */
LUALIB_API void (luaL_where) (lua_State *L, int lvl);
LUALIB_API int  (luaL_error) (lua_State *L, const char *fmt, ...);
LUALIB_API int (luaL_argerror)  (lua_State *L, int arg, const char *extramsg);
LUALIB_API int (luaL_typeerror) (lua_State *L, int arg, const char *tname);


/* REMOVED
LUALIB_API int (luaL_fileresult) (lua_State *L, int stat, const char *fname);
LUALIB_API int (luaL_execresult) (lua_State *L, int stat);
*/

/*
 * Reference System
 */
#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)

LUALIB_API int (luaL_ref) (lua_State *L, int t);
LUALIB_API void (luaL_unref) (lua_State *L, int t, int ref);

/*
 * Load code/bytecode (from buffers and strings ONLY!)
 */
LUALIB_API int (luaL_loadbufferx) (lua_State *L, const char *buff, size_t sz, const char *name, const char *mode);
#define luaL_loadbuffer(L,s,sz,n)	luaL_loadbufferx(L,s,sz,n,NULL)

LUALIB_API int (luaL_loadstring) (lua_State *L, const char *s);
#define luaL_dostring(L, s) \
	(luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))

/*
 * Module system
 */
LUALIB_API void (luaL_requiref) (lua_State *L, const char *modname, lua_CFunction openf, int glb);

/*
** ===============================================================
** some useful macros
** ===============================================================
*/

#define luaL_newlibtable(L,l)	\
  lua_createtable(L, 0, sizeof(l)/sizeof((l)[0]) - 1)

#define luaL_newlib(L,l)  \
  (luaL_checkversion(L), luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))

/*
** Perform arithmetic operations on lua_Integer values with wrap-around
** semantics, as the Lua core does.
*/
#define luaL_intop(op,v1,v2)  \
	((lua_Integer)((lua_Unsigned)(v1) op (lua_Unsigned)(v2)))

/* push the value used to represent failure/error */
#define luaL_pushfail(L)	lua_pushnil(L)

/*
** Internal assertions for in-house debugging
*/
#if !defined(lua_assert)

#if defined LUAI_ASSERT
  #include <assert.h>
  #define lua_assert(c)		assert(c)
#else
  #define lua_assert(c)		((void)0)
#endif

#endif

/*
** {==================================================================
** "Abstraction Layer" for basic report of messages and errors
** ===================================================================
*/

/* print a string */
#if !defined(lua_writestring)
#define lua_writestring(s,l)   fwrite((s), sizeof(char), (l), stdout)
#endif

/* print a newline and flush the output */
#if !defined(lua_writeline)
#define lua_writeline()        (lua_writestring("\n", 1), fflush(stdout))
#endif

/* print an error message */
#if !defined(lua_writestringerror)
#define lua_writestringerror(s,p) \
        (fprintf(stderr, (s), (p)))
#endif

/* }================================================================== */

#endif


