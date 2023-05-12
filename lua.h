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
#ifndef lua_inc
#define lua_inc

#ifdef __cplusplus
extern C {
#endif

#ifndef lua_h
#include <stdarg.h>
#include <stddef.h>
#ifndef luaconf_h
#include <limits.h>
//included "stddef.h" 
#if !defined(LUA_USE_C89) && defined(_WIN32) && !defined(_WIN32_WCE)
#define LUA_USE_WINDOWS  
#endif
#if defined(LUA_USE_WINDOWS)
#define LUA_DL_DLL 
#define LUA_USE_C89 
#endif
#if defined(LUA_USE_LINUX)
#define LUA_USE_POSIX
#endif
#if defined(LUA_USE_MACOSX)
#define LUA_USE_POSIX
#endif
#define LUAI_IS32INT ((UINT_MAX >> 30) >= 3)
#define LUA_INT_INT  1
#define LUA_INT_LONG  2
#define LUA_INT_LONGLONG 3
#define LUA_FLOAT_FLOAT  1
#define LUA_FLOAT_DOUBLE 2
#define LUA_FLOAT_LONGDOUBLE 3
#define LUA_INT_DEFAULT  LUA_INT_LONGLONG
#define LUA_FLOAT_DEFAULT LUA_FLOAT_DOUBLE
#define LUA_32BITS 0
#if defined(LUA_USE_C89) && !defined(LUA_USE_WINDOWS)
#define LUA_C89_NUMBERS  1
#else
#define LUA_C89_NUMBERS  0
#endif
#if LUA_32BITS  
#if LUAI_IS32INT  
#define LUA_INT_TYPE LUA_INT_INT
#else  
#define LUA_INT_TYPE LUA_INT_LONG
#endif
#define LUA_FLOAT_TYPE LUA_FLOAT_FLOAT
#elif LUA_C89_NUMBERS 
#define LUA_INT_TYPE LUA_INT_LONG
#define LUA_FLOAT_TYPE LUA_FLOAT_DOUBLE
#else  
#define LUA_INT_TYPE LUA_INT_DEFAULT
#define LUA_FLOAT_TYPE LUA_FLOAT_DEFAULT
#endif    
#if !defined(LUA_DIRSEP)
#if defined(_WIN32)
#define LUA_DIRSEP "\\"
#else
#define LUA_DIRSEP "/"
#endif
#endif
#if defined(LUA_BUILD_AS_DLL) 
#if defined(LUA_CORE) || defined(LUA_LIB) 
#define LUA_API __declspec(dllexport)
#else      
#define LUA_API __declspec(dllimport)
#endif      
#else    
#define LUA_API  extern
#endif    
#define LUALIB_API LUA_API
#define LUAMOD_API LUA_API
#ifndef LUAI_FUNC
#define LUAI_FUNC static
#endif
#define LUAI_DDEC(dec) LUAI_FUNC dec
#define LUAI_DDEF 
#if defined(LUA_COMPAT_5_3) 
//define LUA_COMPAT_MATHLIB
//define LUA_COMPAT_APIINTCASTS
//define LUA_COMPAT_LT_LE
#define lua_strlen(L,i)  lua_rawlen(L, (i))
#define lua_objlen(L,i)  lua_rawlen(L, (i))
#define lua_equal(L,idx1,idx2)  lua_compare(L,(idx1),(idx2),LUA_OPEQ)
#define lua_lessthan(L,idx1,idx2) lua_compare(L,(idx1),(idx2),LUA_OPLT)
#endif    
#define l_floor(x)  (l_mathop(floor)(x))
#define lua_number2str(s,sz,n)   l_sprintf((s), sz, LUA_NUMBER_FMT, (LUAI_UACNUMBER)(n))
#define lua_numbertointeger(n,p)   ((n) >= (LUA_NUMBER)(LUA_MININTEGER) &&    (n) < -(LUA_NUMBER)(LUA_MININTEGER) &&       (*(p) = (LUA_INTEGER)(n), 1))
#if LUA_FLOAT_TYPE == LUA_FLOAT_FLOAT  
#define LUA_NUMBER float
#define l_floatatt(n)  (FLT_##n)
#define LUAI_UACNUMBER double
#define LUA_NUMBER_FRMLEN ""
#define LUA_NUMBER_FMT  "%.7g"
#define l_mathop(op)  op##f
#define lua_str2number(s,p) strtof((s), (p))
#elif LUA_FLOAT_TYPE == LUA_FLOAT_LONGDOUBLE 
#define LUA_NUMBER long double
#define l_floatatt(n)  (LDBL_##n)
#define LUAI_UACNUMBER long double
#define LUA_NUMBER_FRMLEN "L"
#define LUA_NUMBER_FMT  "%.19Lg"
#define l_mathop(op)  op##l
#define lua_str2number(s,p) strtold((s), (p))
#elif LUA_FLOAT_TYPE == LUA_FLOAT_DOUBLE 
#define LUA_NUMBER double
#define l_floatatt(n)  (DBL_##n)
#define LUAI_UACNUMBER double
#define LUA_NUMBER_FRMLEN ""
#define LUA_NUMBER_FMT  "%.14g"
#define l_mathop(op)  op
#define lua_str2number(s,p) strtod((s), (p))
#else      
#error "numeric float type not defined"
#endif     
#define LUA_INTEGER_FMT  "%" LUA_INTEGER_FRMLEN "d"
#define LUAI_UACINT  LUA_INTEGER
#define lua_integer2str(s,sz,n)   l_sprintf((s), sz, LUA_INTEGER_FMT, (LUAI_UACINT)(n))
#define LUA_UNSIGNED  unsigned LUAI_UACINT
#if LUA_INT_TYPE == LUA_INT_INT  
#define LUA_INTEGER  int
#define LUA_INTEGER_FRMLEN ""
#define LUA_MAXINTEGER  INT_MAX
#define LUA_MININTEGER  INT_MIN
#define LUA_MAXUNSIGNED  UINT_MAX
#elif LUA_INT_TYPE == LUA_INT_LONG 
#define LUA_INTEGER  long
#define LUA_INTEGER_FRMLEN "l"
#define LUA_MAXINTEGER  LONG_MAX
#define LUA_MININTEGER  LONG_MIN
#define LUA_MAXUNSIGNED  ULONG_MAX
#elif LUA_INT_TYPE == LUA_INT_LONGLONG 
#if defined(LLONG_MAX)  
#define LUA_INTEGER  long long
#define LUA_INTEGER_FRMLEN "ll"
#define LUA_MAXINTEGER  LLONG_MAX
#define LUA_MININTEGER  LLONG_MIN
#define LUA_MAXUNSIGNED  ULLONG_MAX
#elif defined(LUA_USE_WINDOWS) 
#define LUA_INTEGER  __int64
#define LUA_INTEGER_FRMLEN "I64"
#define LUA_MAXINTEGER  _I64_MAX
#define LUA_MININTEGER  _I64_MIN
#define LUA_MAXUNSIGNED  _UI64_MAX
#else    
#error "Compiler does not support 'long long'. Use option '-DLUA_32BITS'   or '-DLUA_C89_NUMBERS' (see file 'luaconf.h' for details)"
#endif    
#else    
#error "numeric integer type not defined"
#endif    
#if !defined(LUA_USE_C89)
#define l_sprintf(s,sz,f,i) snprintf(s,sz,f,i)
#else
#define l_sprintf(s,sz,f,i) ((void)(sz), sprintf(s,f,i))
#endif
#define lua_pointer2str(buff,sz,p) l_sprintf(buff,sz,"%p",p)
#if defined(LUA_USE_C89) || (defined(HUGE_VAL) && !defined(HUGE_VALF))
#undef l_mathop  
#undef lua_str2number
#define l_mathop(op)  (lua_Number)op  
#define lua_str2number(s,p) ((lua_Number)strtod((s), (p)))
#endif
#define LUA_KCONTEXT ptrdiff_t
#if !defined(LUA_USE_C89) && defined(__STDC_VERSION__) &&     __STDC_VERSION__ >= 199901L
#include <stdint.h>
#if defined(INTPTR_MAX)  
#undef LUA_KCONTEXT
#define LUA_KCONTEXT intptr_t
#endif
#endif
#if !defined(lua_getlocaledecpoint)
#define lua_getlocaledecpoint() '.'
#endif
#if !defined(luai_likely)
#if defined(__GNUC__) && !defined(LUA_NOBUILTIN)
#define luai_likely(x)  (__builtin_expect(((x) != 0), 1))
#define luai_unlikely(x) (__builtin_expect(((x) != 0), 0))
#define luai_unreachable(x) __builtin_unreachable()
#else
#define luai_likely(x)  (x)
#define luai_unlikely(x) (x)
#define luai_unreachable() lua_assert(0)
#endif
#endif
#if defined(LUA_CORE) || defined(LUA_LIB)
#define l_likely(x) luai_likely(x)
#define l_unlikely(x) luai_unlikely(x)
#endif
#define LUA_NOCVTS2N
#if defined(LUA_USE_APICHECK)
#include <assert.h>
#define luai_apicheck(l,e) (assert(e))
#endif
#if LUAI_IS32INT
#define LUAI_MAXSTACK  1000000
#else
#define LUAI_MAXSTACK  15000
#endif
#define LUA_EXTRASPACE  (sizeof(void *))
#define LUA_IDSIZE 60
#define LUAL_BUFFERSIZE   ((int)(16 * sizeof(void*) * sizeof(lua_Number)))
#define LUAI_MAXALIGN  lua_Number n; double u; void *s; lua_Integer i; long l
#define LUA_NO_UNSAFE 1
#endif
#define LUA_VERSION_MAJOR "5"
#define LUA_VERSION_MINOR "5"
#define LUA_VERSION_RELEASE "4"
#define LUA_VERSION_NUM   504
#define LUA_VERSION_RELEASE_NUM  (LUA_VERSION_NUM * 100 + 4)
#define LUA_VERSION "Lua " LUA_VERSION_MAJOR "." LUA_VERSION_MINOR
#define LUA_RELEASE LUA_VERSION "." LUA_VERSION_RELEASE
#define LUA_COPYRIGHT LUA_RELEASE "  Copyright (C) 1994-2022 Lua.org, PUC-Rio"
#define LUA_AUTHORS "R. Ierusalimschy, L. H. de Figueiredo, W. Celes"
#define LUA_SIGNATURE "\x1bLua"
#define LUA_MULTRET (-1)
#define LUA_REGISTRYINDEX (-LUAI_MAXSTACK - 1000)
#define lua_upvalueindex(i) (LUA_REGISTRYINDEX - (i))
#define LUA_OK  0
#define LUA_YIELD 1
#define LUA_ERRRUN 2
#define LUA_ERRSYNTAX 3
#define LUA_ERRMEM 4
#define LUA_ERRERR 5
typedef struct lua_State lua_State;
#define LUA_TNONE  (-1)
#define LUA_TNIL  0
#define LUA_TBOOLEAN  1
#define LUA_TLIGHTUSERDATA 2
#define LUA_TNUMBER  3
#define LUA_TSTRING  4
#define LUA_TTABLE  5
#define LUA_TFUNCTION  6
#define LUA_TUSERDATA  7
#define LUA_TTHREAD  8
#define LUA_NUMTYPES  9
#define LUA_MINSTACK 20
#define LUA_RIDX_MAINTHREAD 1
#define LUA_RIDX_GLOBALS 2
#define LUA_RIDX_LAST  LUA_RIDX_GLOBALS
typedef LUA_NUMBER lua_Number;
typedef LUA_INTEGER lua_Integer;
typedef LUA_UNSIGNED lua_Unsigned;
typedef LUA_KCONTEXT lua_KContext;
typedef int (*lua_CFunction) (lua_State *L);
typedef int (*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);
typedef const char * (*lua_Reader) (lua_State *L, void *ud, size_t *sz);
typedef int (*lua_Writer) (lua_State *L, const void *p, size_t sz, void *ud);
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);
typedef void (*lua_WarnFunction) (void *ud, const char *msg, int tocont);
#if defined(LUA_USER_H)
#include LUA_USER_H
#endif
extern const char lua_ident[];
LUA_API lua_State *(lua_newstate) (lua_Alloc f, void *ud);
LUA_API void       (lua_close) (lua_State *L);
LUA_API lua_State *(lua_newthread) (lua_State *L);
LUA_API int        (lua_resetthread) (lua_State *L);
LUA_API lua_CFunction (lua_atpanic) (lua_State *L, lua_CFunction panicf);
LUA_API lua_Number (lua_version) (lua_State *L);
LUA_API int   (lua_absindex) (lua_State *L, int idx);
LUA_API int   (lua_gettop) (lua_State *L);
LUA_API void  (lua_settop) (lua_State *L, int idx);
LUA_API void  (lua_pushvalue) (lua_State *L, int idx);
LUA_API void  (lua_rotate) (lua_State *L, int idx, int n);
LUA_API void  (lua_copy) (lua_State *L, int fromidx, int toidx);
LUA_API int   (lua_checkstack) (lua_State *L, int n);
LUA_API void  (lua_xmove) (lua_State *from, lua_State *to, int n);
LUA_API int             (lua_isnumber) (lua_State *L, int idx);
LUA_API int             (lua_isstring) (lua_State *L, int idx);
LUA_API int             (lua_iscfunction) (lua_State *L, int idx);
LUA_API int             (lua_isinteger) (lua_State *L, int idx);
LUA_API int             (lua_isuserdata) (lua_State *L, int idx);
LUA_API int             (lua_type) (lua_State *L, int idx);
LUA_API const char     *(lua_typename) (lua_State *L, int tp);
LUA_API const char     *(lua_objtypename)(lua_State *L, int idx);
LUA_API lua_Number      (lua_tonumberx) (lua_State *L, int idx, int *isnum);
LUA_API lua_Integer     (lua_tointegerx) (lua_State *L, int idx, int *isnum);
LUA_API int             (lua_toboolean) (lua_State *L, int idx);
LUA_API const char     *(lua_tolstring) (lua_State *L, int idx, size_t *len);
LUA_API lua_Unsigned    (lua_rawlen) (lua_State *L, int idx);
LUA_API lua_CFunction   (lua_tocfunction) (lua_State *L, int idx);
LUA_API void        *(lua_touserdata) (lua_State *L, int idx);
LUA_API lua_State      *(lua_tothread) (lua_State *L, int idx);
LUA_API const void     *(lua_topointer) (lua_State *L, int idx);
#define LUA_OPADD 0 
#define LUA_OPSUB 1
#define LUA_OPMUL 2
#define LUA_OPMOD 3
#define LUA_OPPOW 4
#define LUA_OPDIV 5
#define LUA_OPIDIV 6
#define LUA_OPBAND 7
#define LUA_OPBOR 8
#define LUA_OPBXOR 9
#define LUA_OPSHL 10
#define LUA_OPSHR 11
#define LUA_OPUNM 12
#define LUA_OPBNOT 13
LUA_API void  (lua_arith) (lua_State *L, int op);
#define LUA_OPEQ 0
#define LUA_OPLT 1
#define LUA_OPLE 2
LUA_API int   (lua_rawequal) (lua_State *L, int idx1, int idx2);
LUA_API int   (lua_compare) (lua_State *L, int idx1, int idx2, int op);
LUA_API void        (lua_pushnil) (lua_State *L);
LUA_API void        (lua_pushnumber) (lua_State *L, lua_Number n);
LUA_API void        (lua_pushinteger) (lua_State *L, lua_Integer n);
LUA_API const char *(lua_pushlstring) (lua_State *L, const char *s, size_t len);
LUA_API const char *(lua_pushstring) (lua_State *L, const char *s);
LUA_API const char *(lua_pushvfstring) (lua_State *L, const char *fmt,
                                                      va_list argp);
LUA_API const char *(lua_pushfstring) (lua_State *L, const char *fmt, ...);
LUA_API void  (lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);
LUA_API void  (lua_pushboolean) (lua_State *L, int b);
LUA_API void  (lua_pushlightuserdata) (lua_State *L, void *p);
LUA_API int   (lua_pushthread) (lua_State *L);
LUA_API int (lua_getglobal) (lua_State *L, const char *name);
LUA_API int (lua_gettable) (lua_State *L, int idx);
LUA_API int (lua_getfield) (lua_State *L, int idx, const char *k);
LUA_API int (lua_geti) (lua_State *L, int idx, lua_Integer n);
LUA_API int (lua_rawget) (lua_State *L, int idx);
LUA_API int (lua_rawgeti) (lua_State *L, int idx, lua_Integer n);
LUA_API int (lua_rawgetp) (lua_State *L, int idx, const void *p);
LUA_API void  (lua_createtable) (lua_State *L, int narr, int nrec);
LUA_API void *(lua_newuserdatauv) (lua_State *L, size_t sz, int nuvalue);
LUA_API int   (lua_getmetatable) (lua_State *L, int objindex);
LUA_API int  (lua_getiuservalue) (lua_State *L, int idx, int n);
LUA_API void  (lua_setglobal) (lua_State *L, const char *name);
LUA_API void  (lua_settable) (lua_State *L, int idx);
LUA_API void  (lua_setfield) (lua_State *L, int idx, const char *k);
LUA_API void  (lua_seti) (lua_State *L, int idx, lua_Integer n);
LUA_API void  (lua_rawset) (lua_State *L, int idx);
LUA_API void  (lua_rawseti) (lua_State *L, int idx, lua_Integer n);
LUA_API void  (lua_rawsetp) (lua_State *L, int idx, const void *p);
LUA_API int   (lua_setmetatable) (lua_State *L, int objindex);
LUA_API int   (lua_setiuservalue) (lua_State *L, int idx, int n);
LUA_API void  (lua_callk) (lua_State *L, int nargs, int nresults,
                           lua_KContext ctx, lua_KFunction k);
#define lua_call(L,n,r)  lua_callk(L, (n), (r), 0, NULL)
LUA_API int   (lua_pcallk) (lua_State *L, int nargs, int nresults, int errfunc,
                            lua_KContext ctx, lua_KFunction k);
#define lua_pcall(L,n,r,f) lua_pcallk(L, (n), (r), (f), 0, NULL)
LUA_API int   (lua_load) (lua_State *L, lua_Reader reader, void *dt,
                          const char *chunkname, const char *mode);
LUA_API int (lua_dump) (lua_State *L, lua_Writer writer, void *data, int strip);
LUA_API int  (lua_yieldk)     (lua_State *L, int nresults, lua_KContext ctx,
                               lua_KFunction k);
LUA_API int  (lua_resume)     (lua_State *L, lua_State *from, int narg,
                               int *nres);
LUA_API int  (lua_status)     (lua_State *L);
LUA_API int (lua_isyieldable) (lua_State *L);
#define lua_yield(L,n)  lua_yieldk(L, (n), 0, NULL)
LUA_API void (lua_setwarnf) (lua_State *L, lua_WarnFunction f, void *ud);
LUA_API void (lua_warning)  (lua_State *L, const char *msg, int tocont);
#define LUA_GCSTOP  0
#define LUA_GCRESTART  1
#define LUA_GCCOLLECT  2
#define LUA_GCCOUNT  3
#define LUA_GCCOUNTB  4
#define LUA_GCSTEP  5
#define LUA_GCSETPAUSE  6
#define LUA_GCSETSTEPMUL 7
#define LUA_GCISRUNNING  9
#define LUA_GCGEN  10
#define LUA_GCINC  11
LUA_API int (lua_gc) (lua_State *L, int what, ...);
LUA_API int   (lua_error) (lua_State *L);
LUA_API int   (lua_next) (lua_State *L, int idx);
LUA_API void  (lua_concat) (lua_State *L, int n);
LUA_API void  (lua_len)    (lua_State *L, int idx);
LUA_API size_t   (lua_stringtonumber) (lua_State *L, const char *s);
LUA_API lua_Alloc (lua_getallocf) (lua_State *L, void **ud);
LUA_API void      (lua_setallocf) (lua_State *L, lua_Alloc f, void *ud);
LUA_API void (lua_toclose) (lua_State *L, int idx);
LUA_API void (lua_closeslot) (lua_State *L, int idx);
LUA_API void (lua_pushobjtype)(lua_State* L, int idx, int meta);
LUA_API void (lua_cleartable) (lua_State* L, int idx, int keep);
#define lua_getextraspace(L) ((void *)((char *)(L) - LUA_EXTRASPACE))
#define lua_tonumber(L,i) lua_tonumberx(L,(i),NULL)
#define lua_tointeger(L,i) lua_tointegerx(L,(i),NULL)
#define lua_pop(L,n)  lua_settop(L, -(n)-1)
#define lua_newtable(L)  lua_createtable(L, 0, 0)
#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))
#define lua_pushcfunction(L,f) lua_pushcclosure(L, (f), 0)
#define lua_isfunction(L,n) (lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n) (lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n) (lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)  (lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n) (lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n) (lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)  (lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n) (lua_type(L, (n)) <= 0)
#define lua_pushliteral(L, s) lua_pushstring(L, "" s)
#define lua_pushglobaltable(L)   ((void)lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS))
#define lua_tostring(L,i) lua_tolstring(L, (i), NULL)
#define lua_insert(L,idx) lua_rotate(L, (idx), 1)
#define lua_remove(L,idx) (lua_rotate(L, (idx), -1), lua_pop(L, 1))
#define lua_replace(L,idx) (lua_copy(L, -1, (idx)), lua_pop(L, 1))
#if defined(LUA_COMPAT_APIINTCASTS)
#define lua_pushunsigned(L,n) lua_pushinteger(L, (lua_Integer)(n))
#define lua_tounsignedx(L,i,is) ((lua_Unsigned)lua_tointegerx(L,i,is))
#define lua_tounsigned(L,i) lua_tounsignedx(L,(i),NULL)
#endif
#define lua_newuserdata(L,s) lua_newuserdatauv(L,s,1)
#define lua_getuservalue(L,idx) lua_getiuservalue(L,idx,1)
#define lua_setuservalue(L,idx) lua_setiuservalue(L,idx,1)
#define LUA_NUMTAGS  LUA_NUMTYPES
#define LUA_HOOKCALL 0
#define LUA_HOOKRET 1
#define LUA_HOOKLINE 2
#define LUA_HOOKCOUNT 3
#define LUA_HOOKTAILCALL 4
#define LUA_MASKCALL (1 << LUA_HOOKCALL)
#define LUA_MASKRET (1 << LUA_HOOKRET)
#define LUA_MASKLINE (1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT (1 << LUA_HOOKCOUNT)
typedef struct lua_Debug lua_Debug;  
typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);
LUA_API int (lua_getstack) (lua_State *L, int level, lua_Debug *ar);
LUA_API int (lua_getinfo) (lua_State *L, const char *what, lua_Debug *ar);
LUA_API const char *(lua_getlocal) (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *(lua_setlocal) (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *(lua_getupvalue) (lua_State *L, int funcindex, int n);
LUA_API const char *(lua_setupvalue) (lua_State *L, int funcindex, int n);
LUA_API void *(lua_upvalueid) (lua_State *L, int fidx, int n);
LUA_API void  (lua_upvaluejoin) (lua_State *L, int fidx1, int n1,
                                               int fidx2, int n2);
LUA_API void (lua_sethook) (lua_State *L, lua_Hook func, int mask, int count);
LUA_API lua_Hook (lua_gethook) (lua_State *L);
LUA_API int (lua_gethookmask) (lua_State *L);
LUA_API int (lua_gethookcount) (lua_State *L);
LUA_API int (lua_setcstacklimit) (lua_State *L, unsigned int limit);
struct lua_Debug {
  int event;
  const char *name; 
  const char *namewhat; 
  const char *what; 
  const char *source; 
  size_t srclen; 
  int currentline; 
  int linedefined; 
  int lastlinedefined; 
  unsigned char nups; 
  unsigned char nparams;
  char isvararg;        
  char istailcall; 
  unsigned short ftransfer;   
  unsigned short ntransfer;   
  char short_src[LUA_IDSIZE]; 
  
  struct CallInfo *i_ci;  
};
#endif
#ifndef lualib_h
#define LUA_VERSUFFIX          "_" LUA_VERSION_MAJOR "_" LUA_VERSION_MINOR
LUAMOD_API int (luaopen_base) (lua_State *L);
#define LUA_COLIBNAME "coroutine"
LUAMOD_API int (luaopen_coroutine) (lua_State *L);
#define LUA_TABLIBNAME "table"
LUAMOD_API int (luaopen_table) (lua_State *L);
#define LUA_STRLIBNAME "string"
LUAMOD_API int (luaopen_string) (lua_State *L);
#define LUA_UTF8LIBNAME "utf8"
LUAMOD_API int (luaopen_utf8) (lua_State *L);
#define LUA_MATHLIBNAME "math"
LUAMOD_API int (luaopen_math) (lua_State *L);
#define LUA_PACKAGELIBNAME "package"
LUAMOD_API int (luaopen_package) (lua_State *L);
#define LUA_STRPACKLIBNAME "strpack"
LUAMOD_API int luaopen_strpack(lua_State* L);
// WARNING: debug library is NOT LOADED, and even PRELOADED!
#define LUA_DBLIBNAME "debug"
LUAMOD_API int (luaopen_debug) (lua_State *L);
LUALIB_API void (luaL_openlibs) (lua_State *L);
#endif
#ifndef lauxlib_h
#include <stdio.h>
//included "luaconf.h" 
#define LUA_GNAME "_G"
#ifndef lbuffer_h
typedef struct luaL_Buffer luaL_Buffer;
struct luaL_Buffer {
  char *b;  
  size_t size;  
  size_t n;  
  lua_State *L;
  union {
    LUAI_MAXALIGN;  
    char b[LUAL_BUFFERSIZE];  
  } init;
};
#define luaL_bufflen(bf) ((bf)->n)
#define luaL_buffaddr(bf) ((bf)->b)
#define luaL_addchar(B,c)   ((void)((B)->n < (B)->size || luaL_prepbuffsize((B), 1)),    ((B)->b[(B)->n++] = (c)))
#define luaL_addsize(B,s) ((B)->n += (s))
#define luaL_buffsub(B,s) ((B)->n -= (s))
LUALIB_API char *(luaL_prepbuffsize) (luaL_Buffer *B, size_t sz);
LUALIB_API void (luaL_addlstring) (luaL_Buffer *B, const char *s, size_t l);
LUALIB_API void (luaL_addstring) (luaL_Buffer *B, const char *s);
LUALIB_API void (luaL_pushresult) (luaL_Buffer *B);
LUALIB_API void (luaL_pushresultsize) (luaL_Buffer *B, size_t sz);
LUALIB_API void (luaL_addvalue) (luaL_Buffer *B);
LUALIB_API void (luaL_buffinit) (lua_State *L, luaL_Buffer *B);
LUALIB_API char *(luaL_buffinitsize) (lua_State *L, luaL_Buffer *B, size_t sz);
LUALIB_API void (luaL_addgsub) (luaL_Buffer *b, const char *s,
                                     const char *p, const char *r);
#define luaL_prepbuffer(B) luaL_prepbuffsize(B, LUAL_BUFFERSIZE)
#endif
#define LUA_LOADED_TABLE "_LOADED"
typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;
#define LUAL_NUMSIZES (sizeof(lua_Integer)*16 + sizeof(lua_Number))
LUALIB_API void (luaL_checkversion_) (lua_State *L, lua_Number ver, size_t sz);
#define luaL_checkversion(L)     luaL_checkversion_(L, LUA_VERSION_NUM, LUAL_NUMSIZES)
LUALIB_API void (luaL_traceback)   (lua_State *L, lua_State *L1, const char *msg, int level);
#define luaL_typename(L,i) lua_typename(L, lua_type(L,(i)))
LUALIB_API const char *(luaL_tolstring) (lua_State *L, int idx, size_t *len);
LUALIB_API lua_State *(luaL_newstate) (void);
LUALIB_API lua_Integer (luaL_len) (lua_State *L, int idx);
LUALIB_API const char *(luaL_gsub) (lua_State *L, const char *s,
                                    const char *p, const char *r);
LUALIB_API void (luaL_setfuncs)    (lua_State *L, const luaL_Reg *l, int nup);
LUALIB_API int  (luaL_getsubtable) (lua_State *L, int idx, const char *fname);
#define luaL_opt(L,f,n,d) (lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))
#define luaL_argcheck(L, cond,arg,extramsg)  ((void)(luai_likely(cond) || luaL_argerror(L, (arg), (extramsg))))
#define luaL_argexpected(L,cond,arg,tname)  ((void)(luai_likely(cond) || luaL_typeerror(L, (arg), (tname))))
LUALIB_API const char *(luaL_checklstring) (lua_State *L, int arg, size_t *l);
LUALIB_API const char *(luaL_optlstring) (lua_State *L, int arg, const char *def, size_t *l);
#define luaL_checkstring(L,n) (luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d) (luaL_optlstring(L, (n), (d), NULL))
LUALIB_API lua_Number (luaL_checknumber) (lua_State *L, int arg);
LUALIB_API lua_Number (luaL_optnumber) (lua_State *L, int arg, lua_Number def);
LUALIB_API lua_Integer (luaL_checkinteger) (lua_State *L, int arg);
LUALIB_API lua_Integer (luaL_optinteger) (lua_State *L, int arg, lua_Integer def);
LUALIB_API void (luaL_checkstack) (lua_State *L, int sz, const char *msg);
LUALIB_API void (luaL_checktype) (lua_State *L, int arg, int t);
LUALIB_API void (luaL_checkany) (lua_State *L, int arg);
LUALIB_API int  (luaL_checkoption) (lua_State *L, int arg, const char *def, const char *const lst[]);
#define luaL_getmetatable(L,n) (lua_getfield(L, LUA_REGISTRYINDEX, (n)))
LUALIB_API int   (luaL_newmetatable) (lua_State *L, const char *tname);
LUALIB_API void  (luaL_setmetatable) (lua_State *L, const char *tname);
LUALIB_API void *(luaL_testudata)    (lua_State *L, int ud, const char *tname);
LUALIB_API void *(luaL_checkudata)   (lua_State *L, int ud, const char *tname);
LUALIB_API int  (luaL_getmetafield)  (lua_State *L, int obj, const char *e);
LUALIB_API int  (luaL_callmeta)      (lua_State *L, int obj, const char *e);
LUALIB_API void (luaL_where) (lua_State *L, int lvl);
LUALIB_API int  (luaL_error) (lua_State *L, const char *fmt, ...);
LUALIB_API int (luaL_argerror)  (lua_State *L, int arg, const char *extramsg);
LUALIB_API int (luaL_typeerror) (lua_State *L, int arg, const char *tname);
#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)
LUALIB_API int (luaL_ref) (lua_State *L, int t);
LUALIB_API void (luaL_unref) (lua_State *L, int t, int ref);
LUALIB_API int (luaL_loadbufferx) (lua_State *L, const char *buff, size_t sz, const char *name, const char *mode);
#define luaL_loadbuffer(L,s,sz,n) luaL_loadbufferx(L,s,sz,n,NULL)
LUALIB_API int (luaL_loadstring) (lua_State *L, const char *s);
#define luaL_dostring(L, s)  (luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
LUALIB_API void (luaL_requiref) (lua_State *L, const char *modname, lua_CFunction openf, int glb);
#define luaL_newlibtable(L,l)   lua_createtable(L, 0, sizeof(l)/sizeof((l)[0]) - 1)
#define luaL_newlib(L,l)    (luaL_checkversion(L), luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))
#define luaL_intop(op,v1,v2)   ((lua_Integer)((lua_Unsigned)(v1) op (lua_Unsigned)(v2)))
#define luaL_pushfail(L) lua_pushnil(L)
#if !defined(lua_assert)
#if defined LUAI_ASSERT//included "assert.h" 
  #define lua_assert(c)  assert(c)
#else
  #define lua_assert(c)  ((void)0)
#endif
#endif
#if !defined(lua_writestring)
#define lua_writestring(s,l)   fwrite((s), sizeof(char), (l), stdout)
#endif
#if !defined(lua_writeline)
#define lua_writeline()        (lua_writestring("\n", 1), fflush(stdout))
#endif
#if !defined(lua_writestringerror)
#define lua_writestringerror(s,p)         (fprintf(stderr, (s), (p)))
#endif
#endif
#ifndef luapolicy_h
#define LUAPOLICY_REGISTRY  LUAPOLICY_REGISTRY
enum {
 LUAPOLICY_REGISTRY = 1,
 LUAPOLICY_BYTECODE = 2,
 LUAPOLICY_CONTROLGC = 4,
 LUAPOLICY_CANRUNGC = 8,
 LUAPOLICY_FILESYSTEM = 16,
 LUAPOLICY_EXTRADEBUG = 32,
 LUAPOLICY_POLICYCTL = 65536
};
#define LUAPOLICY_BYTECODE  LUAPOLICY_BYTECODE
#define LUAPOLICY_CONTROLGC LUAPOLICY_CONTROLGC
#define LUAPOLICY_CANRUNGC  LUAPOLICY_CANRUNGC
#define LUAPOLICY_FILESYSTEM LUAPOLICY_FILESYSTEM
#define LUAPOLICY_EXTRADEBUG LUAPOLICY_EXTRADEBUG
#define LUAPOLICY_POLICYCTL LUAPOLICY_POLICYCTL
#define LUAPOLICY_DEFAULT LUAPOLICY_CANRUNGC | LUAPOLICY_CONTROLGC 
LUA_API int lua_getpolicy(lua_State* L); 
#define lua_getsubpolicy(L, FLAG) (lua_getpolicy(L) & FLAG)
LUA_API void lua_setpolicy(lua_State* L, int flags);
LUALIB_API const char* luaL_policyname(lua_State* L, int flag);
LUALIB_API void luaL_checkpolicy(lua_State *L, int flag);
#endif
#ifdef __cplusplus
};
#endif
#endif /* lua_inc */
