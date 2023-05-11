#define LUA_CORE
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
// root include lapi.c
#ifndef lprefix_h
#if !defined(LUA_USE_C89)	
#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE           600
#elif _XOPEN_SOURCE == 0
#undef _XOPEN_SOURCE  
#endif
#if !defined(LUA_32BITS) && !defined(_FILE_OFFSET_BITS)
#define _LARGEFILE_SOURCE       1
#define _FILE_OFFSET_BITS       64
#endif
#endif				
#if defined(_WIN32)	
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS  
#endif
#endif			
#endif
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#ifndef lapi_h
#ifndef llimits_h
#include <stddef.h>
#if defined(LUAI_MEM)		
typedef LUAI_UMEM lu_mem;
typedef LUAI_MEM l_mem;
#elif LUAI_IS32INT	
typedef size_t lu_mem;
typedef ptrdiff_t l_mem;
#else  	
typedef unsigned long lu_mem;
typedef long l_mem;
#endif				
typedef unsigned char lu_byte;
typedef signed char ls_byte;
#define MAX_SIZET	((size_t)(~(size_t)0))
#define MAX_SIZE	(sizeof(size_t) < sizeof(lua_Integer) ? MAX_SIZET                           : (size_t)(LUA_MAXINTEGER))
#define MAX_LUMEM	((lu_mem)(~(lu_mem)0))
#define MAX_LMEM	((l_mem)(MAX_LUMEM >> 1))
#define MAX_INT		INT_MAX  
#define log2maxs(t)	(sizeof(t) * 8 - 2)
#define ispow2(x)	(((x) & ((x) - 1)) == 0)
#define LL(x)   (sizeof(x)/sizeof(char) - 1)
#define point2uint(p)	((unsigned int)((size_t)(p) & UINT_MAX))
typedef LUAI_UACNUMBER l_uacNumber;
typedef LUAI_UACINT l_uacInt;
#if defined LUAI_ASSERT
#undef NDEBUG
#include <assert.h>
#define lua_assert(c)           assert(c)
#endif
#if defined(lua_assert)
#define check_exp(c,e)		(lua_assert(c), (e))
#define lua_longassert(c)	((c) ? (void)0 : lua_assert(0))
#else
#define lua_assert(c)		((void)0)
#define check_exp(c,e)		(e)
#define lua_longassert(c)	((void)0)
#endif
#if !defined(luai_apicheck)
#define luai_apicheck(l,e)	((void)l, lua_assert(e))
#endif
#define api_check(l,e,msg)	luai_apicheck(l,(e) && msg)
#if !defined(UNUSED)
#define UNUSED(x)	((void)(x))
#endif
#define cast(t, exp)	((t)(exp))
#define cast_void(i)	cast(void, (i))
#define cast_voidp(i)	cast(void *, (i))
#define cast_num(i)	cast(lua_Number, (i))
#define cast_int(i)	cast(int, (i))
#define cast_uint(i)	cast(unsigned int, (i))
#define cast_byte(i)	cast(lu_byte, (i))
#define cast_uchar(i)	cast(unsigned char, (i))
#define cast_char(i)	cast(char, (i))
#define cast_charp(i)	cast(char *, (i))
#define cast_sizet(i)	cast(size_t, (i))
#if !defined(l_castS2U)
#define l_castS2U(i)	((lua_Unsigned)(i))
#endif
#if !defined(l_castU2S)
#define l_castU2S(i)	((lua_Integer)(i))
#endif
#if !defined(l_noret)
#if defined(__GNUC__)
#define l_noret		void __attribute__((noreturn))
#elif defined(_MSC_VER) && _MSC_VER >= 1200
#define l_noret		void __declspec(noreturn)
#else
#define l_noret		void
#endif
#endif
#if !defined(LUA_USE_C89)
#define l_inline	inline
#elif defined(__GNUC__)
#define l_inline	__inline__
#else
#define l_inline	
#endif
#define l_sinline	static l_inline
#if LUAI_IS32INT
typedef unsigned int l_uint32;
#else
typedef unsigned long l_uint32;
#endif
typedef l_uint32 Instruction;
#if !defined(LUAI_MAXSHORTLEN)
#define LUAI_MAXSHORTLEN	40
#endif
#if !defined(MINSTRTABSIZE)
#define MINSTRTABSIZE	128
#endif
#if !defined(STRCACHE_N)
#define STRCACHE_N		53
#define STRCACHE_M		2
#endif
#if !defined(LUA_MINBUFFER)
#define LUA_MINBUFFER	32
#endif
#if !defined(LUAI_MAXCCALLS)
#define LUAI_MAXCCALLS		200
#endif
#if !defined(lua_lock)
#define lua_lock(L)	((void) 0)
#define lua_unlock(L)	((void) 0)
#endif
#if !defined(luai_threadyield)
#define luai_threadyield(L)	{lua_unlock(L); lua_lock(L);}
#endif
#if !defined(luai_userstateopen)
#define luai_userstateopen(L)		((void)L)
#endif
#if !defined(luai_userstateclose)
#define luai_userstateclose(L)		((void)L)
#endif
#if !defined(luai_userstatethread)
#define luai_userstatethread(L,L1)	((void)L)
#endif
#if !defined(luai_userstatefree)
#define luai_userstatefree(L,L1)	((void)L)
#endif
#if !defined(luai_userstateresume)
#define luai_userstateresume(L,n)	((void)L)
#endif
#if !defined(luai_userstateyield)
#define luai_userstateyield(L,n)	((void)L)
#endif
#if !defined(luai_numidiv)
#define luai_numidiv(L,a,b)     ((void)L, l_floor(luai_numdiv(L,a,b)))
#endif
#if !defined(luai_numdiv)
#define luai_numdiv(L,a,b)      ((a)/(b))
#endif
#if !defined(luai_nummod)
#define luai_nummod(L,a,b,m)    { (void)L; (m) = l_mathop(fmod)(a,b);     if (((m) > 0) ? (b) < 0 : ((m) < 0 && (b) > 0)) (m) += (b); }
#endif
#if !defined(luai_numpow)
#define luai_numpow(L,a,b)    ((void)L, (b == 2) ? (a)*(a) : l_mathop(pow)(a,b))
#endif
#if !defined(luai_numadd)
#define luai_numadd(L,a,b)      ((a)+(b))
#define luai_numsub(L,a,b)      ((a)-(b))
#define luai_nummul(L,a,b)      ((a)*(b))
#define luai_numunm(L,a)        (-(a))
#define luai_numeq(a,b)         ((a)==(b))
#define luai_numlt(a,b)         ((a)<(b))
#define luai_numle(a,b)         ((a)<=(b))
#define luai_numgt(a,b)         ((a)>(b))
#define luai_numge(a,b)         ((a)>=(b))
#define luai_numisnan(a)        (!luai_numeq((a), (a)))
#endif
#if !defined(HARDSTACKTESTS)
#define condmovestack(L,pre,pos)	((void)0)
#else
#define condmovestack(L,pre,pos)    { int sz_ = stacksize(L); pre; luaD_reallocstack((L), sz_, 0); pos; }
#endif
#if !defined(HARDMEMTESTS)
#define condchangemem(L,pre,pos)	((void)0)
#else
#define condchangemem(L,pre,pos)  	{ if (gcrunning(G(L))) { pre; luaC_fullgc(L, 0); pos; } }
#endif
#endif
#ifndef lstate_h
#ifndef lobject_h
//included "llimits.h" 
#define LUA_TUPVAL	LUA_NUMTYPES  
#define LUA_TPROTO	(LUA_NUMTYPES+1)  
#define LUA_TDEADKEY	(LUA_NUMTYPES+2)  
#define LUA_TOTALTYPES		(LUA_TPROTO + 2)
#define makevariant(t,v)	((t) | ((v) << 4))
typedef union Value {
  struct GCObject *gc;    
  void *p;         
  lua_CFunction f; 
  lua_Integer i;   
  lua_Number n;    
} Value;
#define TValuefields	Value value_; lu_byte tt_
typedef struct TValue {
  TValuefields;
} TValue;
#define val_(o)		((o)->value_)
#define valraw(o)	(val_(o))
#define rawtt(o)	((o)->tt_)
#define novariant(t)	((t) & 0x0F)
#define withvariant(t)	((t) & 0x3F)
#define ttypetag(o)	withvariant(rawtt(o))
#define ttype(o)	(novariant(rawtt(o)))
#define checktag(o,t)		(rawtt(o) == (t))
#define checktype(o,t)		(ttype(o) == (t))
#define righttt(obj)		(ttypetag(obj) == gcvalue(obj)->tt)
#define checkliveness(L,obj) 	((void)L, lua_longassert(!iscollectable(obj) || 		(righttt(obj) && (L == NULL || !isdead(G(L),gcvalue(obj))))))
#define settt_(o,t)	((o)->tt_=(t))
#define setobj(L,obj1,obj2) 	{ TValue *io1=(obj1); const TValue *io2=(obj2);           io1->value_ = io2->value_; settt_(io1, io2->tt_); 	  checkliveness(L,io1); lua_assert(!isnonstrictnil(io1)); }
#define setobjs2s(L,o1,o2)	setobj(L,s2v(o1),s2v(o2))
#define setobj2s(L,o1,o2)	setobj(L,s2v(o1),o2)
#define setobjt2t	setobj
#define setobj2n	setobj
#define setobj2t	setobj
typedef union StackValue {
  TValue val;
  struct {
    TValuefields;
    unsigned short delta;
  } tbclist;
} StackValue;
typedef StackValue *StkId;
#define s2v(o)	(&(o)->val)
#define LUA_VNIL	makevariant(LUA_TNIL, 0)
#define LUA_VEMPTY	makevariant(LUA_TNIL, 1)
#define LUA_VABSTKEY	makevariant(LUA_TNIL, 2)
#define ttisnil(v)		checktype((v), LUA_TNIL)
#define ttisstrictnil(o)	checktag((o), LUA_VNIL)
#define setnilvalue(obj) settt_(obj, LUA_VNIL)
#define isabstkey(v)		checktag((v), LUA_VABSTKEY)
#define isnonstrictnil(v)	(ttisnil(v) && !ttisstrictnil(v))
#define isempty(v)		ttisnil(v)
#define ABSTKEYCONSTANT		{NULL}, LUA_VABSTKEY
#define setempty(v)		settt_(v, LUA_VEMPTY)
#define LUA_VFALSE	makevariant(LUA_TBOOLEAN, 0)
#define LUA_VTRUE	makevariant(LUA_TBOOLEAN, 1)
#define ttisboolean(o)		checktype((o), LUA_TBOOLEAN)
#define ttisfalse(o)		checktag((o), LUA_VFALSE)
#define ttistrue(o)		checktag((o), LUA_VTRUE)
#define l_isfalse(o)	(ttisfalse(o) || ttisnil(o))
#define setbfvalue(obj)		settt_(obj, LUA_VFALSE)
#define setbtvalue(obj)		settt_(obj, LUA_VTRUE)
#define LUA_VTHREAD		makevariant(LUA_TTHREAD, 0)
#define ttisthread(o)		checktag((o), ctb(LUA_VTHREAD))
#define thvalue(o)	check_exp(ttisthread(o), gco2th(val_(o).gc))
#define setthvalue(L,obj,x)   { TValue *io = (obj); lua_State *x_ = (x);     val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VTHREAD));     checkliveness(L,io); }
#define setthvalue2s(L,o,t)	setthvalue(L,s2v(o),t)
#define CommonHeader	struct GCObject *next; lu_byte tt; lu_byte marked
typedef struct GCObject {
  CommonHeader;
} GCObject;
#define BIT_ISCOLLECTABLE	(1 << 6)
#define iscollectable(o)	(rawtt(o) & BIT_ISCOLLECTABLE)
#define ctb(t)			((t) | BIT_ISCOLLECTABLE)
#define gcvalue(o)	check_exp(iscollectable(o), val_(o).gc)
#define gcvalueraw(v)	((v).gc)
#define setgcovalue(L,obj,x)   { TValue *io = (obj); GCObject *i_g=(x);     val_(io).gc = i_g; settt_(io, ctb(i_g->tt)); }
#define LUA_VNUMINT	makevariant(LUA_TNUMBER, 0)  
#define LUA_VNUMFLT	makevariant(LUA_TNUMBER, 1)  
#define ttisnumber(o)		checktype((o), LUA_TNUMBER)
#define ttisfloat(o)		checktag((o), LUA_VNUMFLT)
#define ttisinteger(o)		checktag((o), LUA_VNUMINT)
#define nvalue(o)	check_exp(ttisnumber(o), 	(ttisinteger(o) ? cast_num(ivalue(o)) : fltvalue(o)))
#define fltvalue(o)	check_exp(ttisfloat(o), val_(o).n)
#define ivalue(o)	check_exp(ttisinteger(o), val_(o).i)
#define fltvalueraw(v)	((v).n)
#define ivalueraw(v)	((v).i)
#define setfltvalue(obj,x)   { TValue *io=(obj); val_(io).n=(x); settt_(io, LUA_VNUMFLT); }
#define chgfltvalue(obj,x)   { TValue *io=(obj); lua_assert(ttisfloat(io)); val_(io).n=(x); }
#define setivalue(obj,x)   { TValue *io=(obj); val_(io).i=(x); settt_(io, LUA_VNUMINT); }
#define chgivalue(obj,x)   { TValue *io=(obj); lua_assert(ttisinteger(io)); val_(io).i=(x); }
#define LUA_VSHRSTR	makevariant(LUA_TSTRING, 0)  
#define LUA_VLNGSTR	makevariant(LUA_TSTRING, 1)  
#define ttisstring(o)		checktype((o), LUA_TSTRING)
#define ttisshrstring(o)	checktag((o), ctb(LUA_VSHRSTR))
#define ttislngstring(o)	checktag((o), ctb(LUA_VLNGSTR))
#define tsvalueraw(v)	(gco2ts((v).gc))
#define tsvalue(o)	check_exp(ttisstring(o), gco2ts(val_(o).gc))
#define setsvalue(L,obj,x)   { TValue *io = (obj); TString *x_ = (x);     val_(io).gc = obj2gco(x_); settt_(io, ctb(x_->tt));     checkliveness(L,io); }
#define setsvalue2s(L,o,s)	setsvalue(L,s2v(o),s)
#define setsvalue2n	setsvalue
typedef struct TString {
  CommonHeader;
  lu_byte extra;  
  lu_byte shrlen;  
  unsigned int hash;
  union {
    size_t lnglen;  
    struct TString *hnext;  
  } u;
  char contents[1];
} TString;
#define getstr(ts)  ((ts)->contents)
#define svalue(o)       getstr(tsvalue(o))
#define tsslen(s)	((s)->tt == LUA_VSHRSTR ? (s)->shrlen : (s)->u.lnglen)
#define vslen(o)	tsslen(tsvalue(o))
#define LUA_VLIGHTUSERDATA	makevariant(LUA_TLIGHTUSERDATA, 0)
#define LUA_VUSERDATA		makevariant(LUA_TUSERDATA, 0)
#define ttislightuserdata(o)	checktag((o), LUA_VLIGHTUSERDATA)
#define ttisfulluserdata(o)	checktag((o), ctb(LUA_VUSERDATA))
#define pvalue(o)	check_exp(ttislightuserdata(o), val_(o).p)
#define uvalue(o)	check_exp(ttisfulluserdata(o), gco2u(val_(o).gc))
#define pvalueraw(v)	((v).p)
#define setpvalue(obj,x)   { TValue *io=(obj); val_(io).p=(x); settt_(io, LUA_VLIGHTUSERDATA); }
#define setuvalue(L,obj,x)   { TValue *io = (obj); Udata *x_ = (x);     val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VUSERDATA));     checkliveness(L,io); }
typedef union UValue {
  TValue uv;
  LUAI_MAXALIGN;  
} UValue;
typedef struct Udata {
  CommonHeader;
  unsigned short nuvalue;  
  size_t len;  
  struct Table *metatable;
  GCObject *gclist;
  UValue uv[1];  
} Udata;
typedef struct Udata0 {
  CommonHeader;
  unsigned short nuvalue;  
  size_t len;  
  struct Table *metatable;
  union {LUAI_MAXALIGN;} bindata;
} Udata0;
#define udatamemoffset(nuv) 	((nuv) == 0 ? offsetof(Udata0, bindata)                      : offsetof(Udata, uv) + (sizeof(UValue) * (nuv)))
#define getudatamem(u)	(cast_charp(u) + udatamemoffset((u)->nuvalue))
#define sizeudata(nuv,nb)	(udatamemoffset(nuv) + (nb))
#define LUA_VPROTO	makevariant(LUA_TPROTO, 0)
typedef struct Upvaldesc {
  TString *name;  
  lu_byte instack;  
  lu_byte idx;  
  lu_byte kind;  
} Upvaldesc;
typedef struct LocVar {
  TString *varname;
  int startpc;  
  int endpc;    
} LocVar;
typedef struct AbsLineInfo {
  int pc;
  int line;
} AbsLineInfo;
typedef struct Proto {
  CommonHeader;
  lu_byte numparams;  
  lu_byte is_vararg;
  lu_byte maxstacksize;  
  int sizeupvalues;  
  int sizek;  
  int sizecode; 
  int sizep;  
  Upvaldesc *upvalues;  
  TValue *k;  
  Instruction *code;  
  struct Proto **p;  
	
  ls_byte *lineinfo;  
  int sizelineinfo;
  int sizelocvars;
  int sizeabslineinfo;  
  int linedefined;  
  int lastlinedefined;  
  AbsLineInfo *abslineinfo;  
  LocVar *locvars;  
  TString  *source;  
	
  GCObject *gclist;
} Proto;
#define LUA_VUPVAL	makevariant(LUA_TUPVAL, 0)
#define LUA_VLCL	makevariant(LUA_TFUNCTION, 0)  
#define LUA_VLCF	makevariant(LUA_TFUNCTION, 1)  
#define LUA_VCCL	makevariant(LUA_TFUNCTION, 2)  
#define ttisfunction(o)		checktype(o, LUA_TFUNCTION)
#define ttisLclosure(o)		checktag((o), ctb(LUA_VLCL))
#define ttislcf(o)		checktag((o), LUA_VLCF)
#define ttisCclosure(o)		checktag((o), ctb(LUA_VCCL))
#define ttisclosure(o)         (ttisLclosure(o) || ttisCclosure(o))
#define isLfunction(o)	ttisLclosure(o)
#define clvalue(o)	check_exp(ttisclosure(o), gco2cl(val_(o).gc))
#define clLvalue(o)	check_exp(ttisLclosure(o), gco2lcl(val_(o).gc))
#define fvalue(o)	check_exp(ttislcf(o), val_(o).f)
#define clCvalue(o)	check_exp(ttisCclosure(o), gco2ccl(val_(o).gc))
#define fvalueraw(v)	((v).f)
#define setclLvalue(L,obj,x)   { TValue *io = (obj); LClosure *x_ = (x);     val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VLCL));     checkliveness(L,io); }
#define setclLvalue2s(L,o,cl)	setclLvalue(L,s2v(o),cl)
#define setfvalue(obj,x)   { TValue *io=(obj); val_(io).f=(x); settt_(io, LUA_VLCF); }
#define setclCvalue(L,obj,x)   { TValue *io = (obj); CClosure *x_ = (x);     val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VCCL));     checkliveness(L,io); }
typedef struct UpVal {
  CommonHeader;
  lu_byte tbc;  
  TValue *v;  
  union {
    struct {  
      struct UpVal *next;  
      struct UpVal **previous;
    } open;
    TValue value;  
  } u;
} UpVal;
#define ClosureHeader 	CommonHeader; lu_byte nupvalues; GCObject *gclist
typedef struct CClosure {
  ClosureHeader;
  lua_CFunction f;
  TValue upvalue[1];  
} CClosure;
typedef struct LClosure {
  ClosureHeader;
  struct Proto *p;
  UpVal *upvals[1];  
} LClosure;
typedef union Closure {
  CClosure c;
  LClosure l;
} Closure;
#define getproto(o)	(clLvalue(o)->p)
#define LUA_VTABLE	makevariant(LUA_TTABLE, 0)
#define ttistable(o)		checktag((o), ctb(LUA_VTABLE))
#define hvalue(o)	check_exp(ttistable(o), gco2t(val_(o).gc))
#define sethvalue(L,obj,x)   { TValue *io = (obj); Table *x_ = (x);     val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VTABLE));     checkliveness(L,io); }
#define sethvalue2s(L,o,h)	sethvalue(L,s2v(o),h)
typedef union Node {
  struct NodeKey {
    TValuefields;  
    lu_byte key_tt;  
    int next;  
    Value key_val;  
  } u;
  TValue i_val;  
} Node;
#define setnodekey(L,node,obj) 	{ Node *n_=(node); const TValue *io_=(obj); 	  n_->u.key_val = io_->value_; n_->u.key_tt = io_->tt_; 	  checkliveness(L,io_); }
#define getnodekey(L,obj,node) 	{ TValue *io_=(obj); const Node *n_=(node); 	  io_->value_ = n_->u.key_val; io_->tt_ = n_->u.key_tt; 	  checkliveness(L,io_); }
#define BITRAS		(1 << 7)
#define isrealasize(t)		(!((t)->flags & BITRAS))
#define setrealasize(t)		((t)->flags &= cast_byte(~BITRAS))
#define setnorealasize(t)	((t)->flags |= BITRAS)
typedef struct Table {
  CommonHeader;
  lu_byte flags;  
  lu_byte lsizenode;  
  unsigned int alimit;  
  TValue *array;  
  Node *node;
  Node *lastfree;  
  struct Table *metatable;
  GCObject *gclist;
} Table;
#define keytt(node)		((node)->u.key_tt)
#define keyval(node)		((node)->u.key_val)
#define keyisnil(node)		(keytt(node) == LUA_TNIL)
#define keyisinteger(node)	(keytt(node) == LUA_VNUMINT)
#define keyival(node)		(keyval(node).i)
#define keyisshrstr(node)	(keytt(node) == ctb(LUA_VSHRSTR))
#define keystrval(node)		(gco2ts(keyval(node).gc))
#define setnilkey(node)		(keytt(node) = LUA_TNIL)
#define keyiscollectable(n)	(keytt(n) & BIT_ISCOLLECTABLE)
#define gckey(n)	(keyval(n).gc)
#define gckeyN(n)	(keyiscollectable(n) ? gckey(n) : NULL)
#define setdeadkey(node)	(keytt(node) = LUA_TDEADKEY)
#define keyisdead(node)		(keytt(node) == LUA_TDEADKEY)
#define lmod(s,size) 	(check_exp((size&(size-1))==0, (cast_int((s) & ((size)-1)))))
#define twoto(x)	(1<<(x))
#define sizenode(t)	(twoto((t)->lsizenode))
#include <stdio.h>
static inline unsigned int fibonacci_hash(unsigned int s, unsigned int size)
{
	//assert(size < 32);
	unsigned int res = size ?
		(s * 11400714819323198485u) >> (64 - size) : 0;
	//fprintf(stderr, "%li = %li hashed by %li\n", (long)res, (long)s, (long)twoto(size));
	//assert(res <= (unsigned int)twoto(size));
	return res;
}
#define UTF8BUFFSZ	8
LUAI_FUNC int luaO_utf8esc (char *buff, unsigned long x);
LUAI_FUNC int luaO_ceillog2 (unsigned int x);
LUAI_FUNC int luaO_rawarith (lua_State *L, int op, const TValue *p1,
                             const TValue *p2, TValue *res);
LUAI_FUNC void luaO_arith (lua_State *L, int op, const TValue *p1,
                           const TValue *p2, StkId res);
LUAI_FUNC size_t luaO_str2num (const char *s, TValue *o);
LUAI_FUNC int luaO_hexavalue (int c);
LUAI_FUNC void luaO_tostring (lua_State *L, TValue *obj);
LUAI_FUNC const char *luaO_pushvfstring (lua_State *L, const char *fmt,
                                                       va_list argp);
LUAI_FUNC const char *luaO_pushfstring (lua_State *L, const char *fmt, ...);
LUAI_FUNC void luaO_chunkid (char *out, const char *source, size_t srclen);
#endif
#ifndef ltm_h
typedef enum {
  TM_INDEX,
  TM_NEWINDEX,
  TM_GC,
  TM_MODE,
  TM_LEN,
  TM_EQ,  
  TM_ADD,
  TM_SUB,
  TM_MUL,
  TM_MOD,
  TM_POW,
  TM_DIV,
  TM_IDIV,
  TM_BAND,
  TM_BOR,
  TM_BXOR,
  TM_SHL,
  TM_SHR,
  TM_UNM,
  TM_BNOT,
  TM_LT,
  TM_LE,
  TM_CONCAT,
  TM_CALL,
  TM_CLOSE,
  TM_N		
} TMS;
#define maskflags	(~(~0u << (TM_EQ + 1)))
#define notm(tm)	ttisnil(tm)
#define gfasttm(g,et,e) ((et) == NULL ? NULL :   ((et)->flags & (1u<<(e))) ? NULL : luaT_gettm(et, e, (g)->tmname[e]))
#define fasttm(l,et,e)	gfasttm(G(l), et, e)
#define ttypename(x)	luaT_typenames_[(x) + 1]
static const char *const luaT_typenames_[LUA_TOTALTYPES] = {
  "no value",
  "nil", "boolean", "lightuserdata", "number",
  "string", "table", "function", "userdata", "thread",
  "upvalue", "proto" 
};
LUAI_FUNC const char *luaT_objtypename (lua_State *L, const TValue *o);
// Extension
LUAI_FUNC TString* luaT_objtypestr (lua_State *L, const TValue *o);
LUAI_FUNC const TValue *luaT_gettm (Table *events, TMS event, TString *ename);
LUAI_FUNC const TValue *luaT_gettmbyobj (lua_State *L, const TValue *o,
                                                       TMS event);
LUAI_FUNC void luaT_init (lua_State *L);
LUAI_FUNC void luaT_callTM (lua_State *L, const TValue *f, const TValue *p1,
                            const TValue *p2, const TValue *p3);
LUAI_FUNC void luaT_callTMres (lua_State *L, const TValue *f,
                            const TValue *p1, const TValue *p2, StkId p3);
LUAI_FUNC void luaT_trybinTM (lua_State *L, const TValue *p1, const TValue *p2,
                              StkId res, TMS event);
LUAI_FUNC void luaT_tryconcatTM (lua_State *L);
LUAI_FUNC void luaT_trybinassocTM (lua_State *L, const TValue *p1,
       const TValue *p2, int inv, StkId res, TMS event);
LUAI_FUNC void luaT_trybiniTM (lua_State *L, const TValue *p1, lua_Integer i2,
                               int inv, StkId res, TMS event);
LUAI_FUNC int luaT_callorderTM (lua_State *L, const TValue *p1,
                                const TValue *p2, TMS event);
LUAI_FUNC int luaT_callorderiTM (lua_State *L, const TValue *p1, int v2,
                                 int inv, int isfloat, TMS event);
LUAI_FUNC void luaT_adjustvarargs (lua_State *L, int nfixparams,
                                   struct CallInfo *ci, const Proto *p);
LUAI_FUNC void luaT_getvarargs (lua_State *L, struct CallInfo *ci,
                                              StkId where, int wanted);
#endif
#ifndef lzio_h
#ifndef lmem_h
#define luaM_error(L)	luaD_throw(L, LUA_ERRMEM)
#define luaM_testsize(n,e)  	(sizeof(n) >= sizeof(size_t) && cast_sizet((n)) + 1 > MAX_SIZET/(e))
#define luaM_checksize(L,n,e)  	(luaM_testsize(n,e) ? luaM_toobig(L) : cast_void(0))
#define luaM_limitN(n,t)    ((cast_sizet(n) <= MAX_SIZET/sizeof(t)) ? (n) :       cast_uint((MAX_SIZET/sizeof(t))))
#define luaM_reallocvchar(L,b,on,n)    cast_charp(luaM_saferealloc_(L, (b), (on)*sizeof(char), (n)*sizeof(char)))
#define luaM_freemem(L, b, s)	luaM_free_(L, (b), (s))
#define luaM_free(L, b)		luaM_free_(L, (b), sizeof(*(b)))
#define luaM_freearray(L, b, n)   luaM_free_(L, (b), (n)*sizeof(*(b)))
#define luaM_new(L,t)		cast(t*, luaM_malloc_(L, sizeof(t), 0))
#define luaM_newvector(L,n,t)	cast(t*, luaM_malloc_(L, (n)*sizeof(t), 0))
#define luaM_newvectorchecked(L,n,t)   (luaM_checksize(L,n,sizeof(t)), luaM_newvector(L,n,t))
#define luaM_newobject(L,tag,s)	luaM_malloc_(L, (s), tag)
#define luaM_growvector(L,v,nelems,size,t,limit,e) 	((v)=cast(t *, luaM_growaux_(L,v,nelems,&(size),sizeof(t),                          luaM_limitN(limit,t),e)))
#define luaM_reallocvector(L, v,oldn,n,t)    (cast(t *, luaM_realloc_(L, v, cast_sizet(oldn) * sizeof(t),                                   cast_sizet(n) * sizeof(t))))
#define luaM_shrinkvector(L,v,size,fs,t)    ((v)=cast(t *, luaM_shrinkvector_(L, v, &(size), fs, sizeof(t))))
LUAI_FUNC l_noret luaM_toobig (lua_State *L);
LUAI_FUNC void *luaM_realloc_ (lua_State *L, void *block, size_t oldsize,
                                                          size_t size);
LUAI_FUNC void *luaM_saferealloc_ (lua_State *L, void *block, size_t oldsize,
                                                              size_t size);
LUAI_FUNC void luaM_free_ (lua_State *L, void *block, size_t osize);
LUAI_FUNC void *luaM_growaux_ (lua_State *L, void *block, int nelems,
                               int *size, int size_elem, int limit,
                               const char *what);
LUAI_FUNC void *luaM_shrinkvector_ (lua_State *L, void *block, int *nelem,
                                    int final_n, int size_elem);
LUAI_FUNC void *luaM_malloc_ (lua_State *L, size_t size, int tag);
#endif
#define EOZ	(-1)			
typedef struct Zio ZIO;
#define zgetc(z)  (((z)->n--)>0 ?  cast_uchar(*(z)->p++) : luaZ_fill(z))
typedef struct Mbuffer {
  char *buffer;
  size_t n;
  size_t buffsize;
} Mbuffer;
#define luaZ_initbuffer(L, buff) ((buff)->buffer = NULL, (buff)->buffsize = 0)
#define luaZ_buffer(buff)	((buff)->buffer)
#define luaZ_sizebuffer(buff)	((buff)->buffsize)
#define luaZ_bufflen(buff)	((buff)->n)
#define luaZ_buffremove(buff,i)	((buff)->n -= (i))
#define luaZ_resetbuffer(buff) ((buff)->n = 0)
#define luaZ_resizebuffer(L, buff, size) 	((buff)->buffer = luaM_reallocvchar(L, (buff)->buffer, 				(buff)->buffsize, size), 	(buff)->buffsize = size)
#define luaZ_freebuffer(L, buff)	luaZ_resizebuffer(L, buff, 0)
LUAI_FUNC void luaZ_init (lua_State *L, ZIO *z, lua_Reader reader,
                                        void *data);
LUAI_FUNC size_t luaZ_read (ZIO* z, void *b, size_t n);	
struct Zio {
  size_t n;			
  const char *p;		
  lua_Reader reader;		
  void *data;			
  lua_State *L;			
};
LUAI_FUNC int luaZ_fill (ZIO *z);
#endif
#define yieldable(L)		(((L)->nCcalls & 0xffff0000) == 0)
#define getCcalls(L)	((L)->nCcalls & 0xffff)
#define incnny(L)	((L)->nCcalls += 0x10000)
#define decnny(L)	((L)->nCcalls -= 0x10000)
#define nyci	(0x10000 | 1)
struct lua_longjmp;  
#if !defined(l_signalT)
#include <signal.h>
#define l_signalT	sig_atomic_t
#endif
#define EXTRA_STACK   5
#define BASIC_STACK_SIZE        (2*LUA_MINSTACK)
#define stacksize(th)	cast_int((th)->stack_last - (th)->stack)
#define KGC_INC		0	
#define KGC_GEN		1	
typedef struct stringtable {
  TString **hash;
  int nuse;  
  int size;
} stringtable;
typedef struct CallInfo {
  StkId func;  
  StkId	top;  
  struct CallInfo *previous, *next;  
  union {
    struct {  
      const Instruction *savedpc;
      volatile l_signalT trap;
      int nextraargs;  
    } l;
    struct {  
      lua_KFunction k;  
      ptrdiff_t old_errfunc;
      lua_KContext ctx;  
    } c;
  } u;
  union {
    int funcidx;  
    int nyield;  
    int nres;  
    struct {  
      unsigned short ftransfer;  
      unsigned short ntransfer;  
    } transferinfo;
  } u2;
  short nresults;  
  unsigned short callstatus;
} CallInfo;
#define CIST_OAH	(1<<0)	
#define CIST_C		(1<<1)	
#define CIST_FRESH	(1<<2)	
#define CIST_HOOKED	(1<<3)	
#define CIST_YPCALL	(1<<4)	
#define CIST_TAIL	(1<<5)	
#define CIST_HOOKYIELD	(1<<6)	
#define CIST_FIN	(1<<7)	
#define CIST_TRAN	(1<<8)	
#define CIST_CLSRET	(1<<9)  
#define CIST_RECST	10
#if defined(LUA_COMPAT_LT_LE)
#define CIST_LEQ	(1<<13)  
#endif
#define getcistrecst(ci)     (((ci)->callstatus >> CIST_RECST) & 7)
#define setcistrecst(ci,st)    check_exp(((st) & 7) == (st),                 ((ci)->callstatus = ((ci)->callstatus & ~(7 << CIST_RECST))                                                    | ((st) << CIST_RECST)))
#define isLua(ci)	(!((ci)->callstatus & CIST_C))
#define isLuacode(ci)	(!((ci)->callstatus & (CIST_C | CIST_HOOKED)))
#define setoah(st,v)	((st) = ((st) & ~CIST_OAH) | (v))
#define getoah(st)	((st) & CIST_OAH)
typedef struct global_State {
  lua_Alloc frealloc;  
  void *ud;         
  l_mem totalbytes;  
  l_mem GCdebt;  
  lu_mem GCestimate;  
  lu_mem lastatomic;  
  stringtable strt;  
  TValue l_registry;
  TValue nilvalue;  
  unsigned int seed;  
  lu_byte currentwhite;
  lu_byte gcstate;  
  lu_byte gckind;  
  lu_byte gcstopem;  
  lu_byte genminormul;  
  lu_byte genmajormul;  
  lu_byte gcstp;  
  lu_byte gcemergency;  
  lu_byte gcpause;  
  lu_byte gcstepmul;  
  lu_byte gcstepsize;  
  GCObject *allgc;  
  GCObject **sweepgc;  
  GCObject *finobj;  
  GCObject *gray;  
  GCObject *grayagain;  
  GCObject *weak;  
  GCObject *ephemeron;  
  GCObject *allweak;  
  GCObject *tobefnz;  
  GCObject *fixedgc;  
  
  GCObject *survival;  
  GCObject *old1;  
  GCObject *reallyold;  
  GCObject *firstold1;  
  GCObject *finobjsur;  
  GCObject *finobjold1;  
  GCObject *finobjrold;  
  struct lua_State *twups;  
  lua_CFunction panic;  
  struct lua_State *mainthread;
  TString *memerrmsg;  
  TString *tmname[TM_N];  
  TString *typenames[LUA_TOTALTYPES + 1];  
  struct Table *mt[LUA_NUMTAGS];  
  TString *strcache[STRCACHE_N][STRCACHE_M];  
  lua_WarnFunction warnf;  
  void *ud_warn;         
} global_State;
struct lua_State {
  CommonHeader;
  lu_byte status;
  lu_byte allowhook;
  unsigned short nci;  
  StkId top;  
  global_State *l_G;
  CallInfo *ci;  
  StkId stack_last;  
  StkId stack;  
  UpVal *openupval;  
  StkId tbclist;  
  GCObject *gclist;
  struct lua_State *twups;  
  struct lua_longjmp *errorJmp;  
  CallInfo base_ci;  
  volatile lua_Hook hook;
  ptrdiff_t errfunc;  
  l_uint32 nCcalls;  
  int oldpc;  
  int basehookcount;
  int hookcount;
  volatile l_signalT hookmask;
};
#define G(L)	(L->l_G)
#define completestate(g)	ttisnil(&g->nilvalue)
union GCUnion {
  GCObject gc;  
  struct TString ts;
  struct Udata u;
  union Closure cl;
  struct Table h;
  struct Proto p;
  struct lua_State th;  
  struct UpVal upv;
};
#define cast_u(o)	cast(union GCUnion *, (o))
#define gco2ts(o)  	check_exp(novariant((o)->tt) == LUA_TSTRING, &((cast_u(o))->ts))
#define gco2u(o)  check_exp((o)->tt == LUA_VUSERDATA, &((cast_u(o))->u))
#define gco2lcl(o)  check_exp((o)->tt == LUA_VLCL, &((cast_u(o))->cl.l))
#define gco2ccl(o)  check_exp((o)->tt == LUA_VCCL, &((cast_u(o))->cl.c))
#define gco2cl(o)  	check_exp(novariant((o)->tt) == LUA_TFUNCTION, &((cast_u(o))->cl))
#define gco2t(o)  check_exp((o)->tt == LUA_VTABLE, &((cast_u(o))->h))
#define gco2p(o)  check_exp((o)->tt == LUA_VPROTO, &((cast_u(o))->p))
#define gco2th(o)  check_exp((o)->tt == LUA_VTHREAD, &((cast_u(o))->th))
#define gco2upv(o)	check_exp((o)->tt == LUA_VUPVAL, &((cast_u(o))->upv))
#define obj2gco(v)	check_exp((v)->tt >= LUA_TSTRING, &(cast_u(v)->gc))
#define gettotalbytes(g)	cast(lu_mem, (g)->totalbytes + (g)->GCdebt)
LUAI_FUNC void luaE_setdebt (global_State *g, l_mem debt);
LUAI_FUNC void luaE_freethread (lua_State *L, lua_State *L1);
LUAI_FUNC CallInfo *luaE_extendCI (lua_State *L);
LUAI_FUNC void luaE_freeCI (lua_State *L);
LUAI_FUNC void luaE_shrinkCI (lua_State *L);
LUAI_FUNC void luaE_checkcstack (lua_State *L);
LUAI_FUNC void luaE_incCstack (lua_State *L);
LUAI_FUNC void luaE_warning (lua_State *L, const char *msg, int tocont);
LUAI_FUNC void luaE_warnerror (lua_State *L, const char *where);
LUAI_FUNC int luaE_resetthread (lua_State *L, int status);
#endif
#define api_incr_top(L)   {L->top++; api_check(L, L->top <= L->ci->top, 				"stack overflow");}
#define adjustresults(L,nres)     { if ((nres) <= LUA_MULTRET && L->ci->top < L->top) L->ci->top = L->top; }
#define api_checknelems(L,n)	api_check(L, (n) < (L->top - L->ci->func), 				  "not enough elements in the stack")
#define hastocloseCfunc(n)	((n) < LUA_MULTRET)
#define codeNresults(n)		(-(n) - 3)
#define decodeNresults(n)	(-(n) - 3)
#endif
#ifndef ldebug_h
#define pcRel(pc, p)	(cast_int((pc) - (p)->code) - 1)
#define ci_func(ci)		(clLvalue(s2v((ci)->func)))
#define resethookcount(L)	(L->hookcount = L->basehookcount)
#define ABSLINEINFO	(-0x80)
#if !defined(MAXIWTHABS)
#define MAXIWTHABS	128
#endif
LUAI_FUNC int luaG_getfuncline (const Proto *f, int pc);
LUAI_FUNC const char *luaG_findlocal (lua_State *L, CallInfo *ci, int n,
                                                    StkId *pos);
LUAI_FUNC l_noret luaG_typeerror (lua_State *L, const TValue *o,
                                                const char *opname);
LUAI_FUNC l_noret luaG_callerror (lua_State *L, const TValue *o);
LUAI_FUNC l_noret luaG_forerror (lua_State *L, const TValue *o,
                                               const char *what);
LUAI_FUNC l_noret luaG_concaterror (lua_State *L, const TValue *p1,
                                                  const TValue *p2);
LUAI_FUNC l_noret luaG_opinterror (lua_State *L, const TValue *p1,
                                                 const TValue *p2,
                                                 const char *msg);
LUAI_FUNC l_noret luaG_tointerror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
LUAI_FUNC l_noret luaG_ordererror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
LUAI_FUNC l_noret luaG_runerror (lua_State *L, const char *fmt, ...);
LUAI_FUNC const char *luaG_addinfo (lua_State *L, const char *msg,
                                                  TString *src, int line);
LUAI_FUNC l_noret luaG_errormsg (lua_State *L);
LUAI_FUNC int luaG_traceexec (lua_State *L, const Instruction *pc);
#endif
#ifndef ldo_h
//included "lzio.h" 
#define luaD_checkstackaux(L,n,pre,pos)  	if (l_unlikely(L->stack_last - L->top <= (n))) 	  { pre; luaD_growstack(L, n, 1); pos; }         else { condmovestack(L,pre,pos); }
#define luaD_checkstack(L,n)	luaD_checkstackaux(L,n,(void)0,(void)0)
#define savestack(L,p)		((char *)(p) - (char *)L->stack)
#define restorestack(L,n)	((StkId)((char *)L->stack + (n)))
#define checkstackGCp(L,n,p)    luaD_checkstackaux(L, n,     ptrdiff_t t__ = savestack(L, p);       luaC_checkGC(L),       p = restorestack(L, t__))  
#define checkstackGC(L,fsize)  	luaD_checkstackaux(L, (fsize), luaC_checkGC(L), (void)0)
typedef void (*Pfunc) (lua_State *L, void *ud);
LUAI_FUNC void luaD_seterrorobj (lua_State *L, int errcode, StkId oldtop);
LUAI_FUNC int luaD_protectedparser (lua_State *L, ZIO *z, const char *name,
                                                  const char *mode);
LUAI_FUNC void luaD_hook (lua_State *L, int event, int line,
                                        int fTransfer, int nTransfer);
LUAI_FUNC void luaD_hookcall (lua_State *L, CallInfo *ci);
LUAI_FUNC int luaD_pretailcall (lua_State *L, CallInfo *ci, StkId func, int narg1, int delta);
LUAI_FUNC CallInfo *luaD_precall (lua_State *L, StkId func, int nResults);
LUAI_FUNC void luaD_call (lua_State *L, StkId func, int nResults);
LUAI_FUNC void luaD_callnoyield (lua_State *L, StkId func, int nResults);
LUAI_FUNC StkId luaD_tryfuncTM (lua_State *L, StkId func);
LUAI_FUNC int luaD_closeprotected (lua_State *L, ptrdiff_t level, int status);
LUAI_FUNC int luaD_pcall (lua_State *L, Pfunc func, void *u,
                                        ptrdiff_t oldtop, ptrdiff_t ef);
LUAI_FUNC void luaD_poscall (lua_State *L, CallInfo *ci, int nres);
LUAI_FUNC int luaD_reallocstack (lua_State *L, int newsize, int raiseerror);
LUAI_FUNC int luaD_growstack (lua_State *L, int n, int raiseerror);
LUAI_FUNC void luaD_shrinkstack (lua_State *L);
LUAI_FUNC void luaD_inctop (lua_State *L);
LUAI_FUNC l_noret luaD_throw (lua_State *L, int errcode);
LUAI_FUNC int luaD_rawrunprotected (lua_State *L, Pfunc f, void *ud);
#endif
#ifndef lfunc_h
#define sizeCclosure(n)	(cast_int(offsetof(CClosure, upvalue)) +                          cast_int(sizeof(TValue)) * (n))
#define sizeLclosure(n)	(cast_int(offsetof(LClosure, upvals)) +                          cast_int(sizeof(TValue *)) * (n))
#define isintwups(L)	(L->twups != L)
#define MAXUPVAL	255
#define upisopen(up)	((up)->v != &(up)->u.value)
#define uplevel(up)	check_exp(upisopen(up), cast(StkId, (up)->v))
#define MAXMISS		10
#define CLOSEKTOP	(-1)
LUAI_FUNC Proto *luaF_newproto (lua_State *L);
LUAI_FUNC CClosure *luaF_newCclosure (lua_State *L, int nupvals);
LUAI_FUNC LClosure *luaF_newLclosure (lua_State *L, int nupvals);
LUAI_FUNC void luaF_initupvals (lua_State *L, LClosure *cl);
LUAI_FUNC UpVal *luaF_findupval (lua_State *L, StkId level);
LUAI_FUNC void luaF_newtbcupval (lua_State *L, StkId level);
LUAI_FUNC void luaF_closeupval (lua_State *L, StkId level);
LUAI_FUNC void luaF_close (lua_State *L, StkId level, int status, int yy);
LUAI_FUNC void luaF_unlinkupval (UpVal *uv);
LUAI_FUNC void luaF_freeproto (lua_State *L, Proto *f);
LUAI_FUNC const char *luaF_getlocalname (const Proto *func, int local_number, int pc);
#endif
#ifndef lgc_h
#define GCSpropagate	0
#define GCSenteratomic	1
#define GCSatomic	2
#define GCSswpallgc	3
#define GCSswpfinobj	4
#define GCSswptobefnz	5
#define GCSswpend	6
#define GCScallfin	7
#define GCSpause	8
#define issweepphase(g)  	(GCSswpallgc <= (g)->gcstate && (g)->gcstate <= GCSswpend)
#define keepinvariant(g)	((g)->gcstate <= GCSatomic)
#define resetbits(x,m)		((x) &= cast_byte(~(m)))
#define setbits(x,m)		((x) |= (m))
#define testbits(x,m)		((x) & (m))
#define bitmask(b)		(1<<(b))
#define bit2mask(b1,b2)		(bitmask(b1) | bitmask(b2))
#define l_setbit(x,b)		setbits(x, bitmask(b))
#define resetbit(x,b)		resetbits(x, bitmask(b))
#define testbit(x,b)		testbits(x, bitmask(b))
#define WHITE0BIT	3  
#define WHITE1BIT	4  
#define BLACKBIT	5  
#define FINALIZEDBIT	6  
#define TESTBIT		7
#define WHITEBITS	bit2mask(WHITE0BIT, WHITE1BIT)
#define iswhite(x)      testbits((x)->marked, WHITEBITS)
#define isblack(x)      testbit((x)->marked, BLACKBIT)
#define isgray(x)    	(!testbits((x)->marked, WHITEBITS | bitmask(BLACKBIT)))
#define tofinalize(x)	testbit((x)->marked, FINALIZEDBIT)
#define otherwhite(g)	((g)->currentwhite ^ WHITEBITS)
#define isdeadm(ow,m)	((m) & (ow))
#define isdead(g,v)	isdeadm(otherwhite(g), (v)->marked)
#define changewhite(x)	((x)->marked ^= WHITEBITS)
#define nw2black(x)  	check_exp(!iswhite(x), l_setbit((x)->marked, BLACKBIT))
#define luaC_white(g)	cast_byte((g)->currentwhite & WHITEBITS)
#define G_NEW		0	
#define G_SURVIVAL	1	
#define G_OLD0		2	
#define G_OLD1		3	
#define G_OLD		4	
#define G_TOUCHED1	5	
#define G_TOUCHED2	6	
#define AGEBITS		7  
#define getage(o)	((o)->marked & AGEBITS)
#define setage(o,a)  ((o)->marked = cast_byte(((o)->marked & (~AGEBITS)) | a))
#define isold(o)	(getage(o) > G_SURVIVAL)
#define changeage(o,f,t)  	check_exp(getage(o) == (f), (o)->marked ^= ((f)^(t)))
#define LUAI_GENMAJORMUL         100
#define LUAI_GENMINORMUL         20
#define LUAI_GCPAUSE    200
#define getgcparam(p)	((p) * 4)
#define setgcparam(p,v)	((p) = (v) / 4)
#define LUAI_GCMUL      100
#define LUAI_GCSTEPSIZE 13      
#define isdecGCmodegen(g)	(g->gckind == KGC_GEN || g->lastatomic != 0)
#define GCSTPUSR	1  
#define GCSTPGC		2  
#define GCSTPCLS	4  
#define gcrunning(g)	((g)->gcstp == 0)
#define luaC_condGC(L,pre,pos) 	{ if (G(L)->GCdebt > 0) { pre; luaC_step(L); pos;}; 	  condchangemem(L,pre,pos); }
#define luaC_checkGC(L)		luaC_condGC(L,(void)0,(void)0)
#define luaC_barrier(L,p,v) (  	(iscollectable(v) && isblack(p) && iswhite(gcvalue(v))) ?  	luaC_barrier_(L,obj2gco(p),gcvalue(v)) : cast_void(0))
#define luaC_barrierback(L,p,v) (  	(iscollectable(v) && isblack(p) && iswhite(gcvalue(v))) ? 	luaC_barrierback_(L,p) : cast_void(0))
#define luaC_objbarrier(L,p,o) (  	(isblack(p) && iswhite(o)) ? 	luaC_barrier_(L,obj2gco(p),obj2gco(o)) : cast_void(0))
LUAI_FUNC void luaC_fix (lua_State *L, GCObject *o);
LUAI_FUNC void luaC_freeallobjects (lua_State *L);
LUAI_FUNC void luaC_step (lua_State *L);
LUAI_FUNC void luaC_runtilstate (lua_State *L, int statesmask);
LUAI_FUNC void luaC_fullgc (lua_State *L, int isemergency);
LUAI_FUNC GCObject *luaC_newobj (lua_State *L, int tt, size_t sz);
LUAI_FUNC void luaC_barrier_ (lua_State *L, GCObject *o, GCObject *v);
LUAI_FUNC void luaC_barrierback_ (lua_State *L, GCObject *o);
LUAI_FUNC void luaC_checkfinalizer (lua_State *L, GCObject *o, Table *mt);
LUAI_FUNC void luaC_changemode (lua_State *L, int newmode);
#endif
//included "lmem.h" 
#ifndef lstring_h
#define MEMERRMSG       "not enough memory"
#define sizelstring(l)  (offsetof(TString, contents) + ((l) + 1) * sizeof(char))
#define luaS_newliteral(L, s)	(luaS_newlstr(L, "" s,                                  (sizeof(s)/sizeof(char))-1))
#define isreserved(s)	((s)->tt == LUA_VSHRSTR && (s)->extra > 0)
#define eqshrstr(a,b)	check_exp((a)->tt == LUA_VSHRSTR, (a) == (b))
LUAI_FUNC unsigned int luaS_hash (const char *str, size_t l, unsigned int seed);
LUAI_FUNC unsigned int luaS_hashlongstr (TString *ts);
LUAI_FUNC int luaS_eqlngstr (TString *a, TString *b);
LUAI_FUNC void luaS_resize (lua_State *L, int newsize);
LUAI_FUNC void luaS_clearcache (global_State *g);
LUAI_FUNC void luaS_init (lua_State *L);
LUAI_FUNC void luaS_remove (lua_State *L, TString *ts);
LUAI_FUNC Udata *luaS_newudata (lua_State *L, size_t s, int nuvalue);
LUAI_FUNC TString *luaS_newlstr (lua_State *L, const char *str, size_t l);
LUAI_FUNC TString *luaS_new (lua_State *L, const char *str);
LUAI_FUNC TString *luaS_createlngstrobj (lua_State *L, size_t l);
#endif
#ifndef ltable_h
#define gnode(t,i)	(&(t)->node[i])
#define gval(n)		(&(n)->i_val)
#define gnext(n)	((n)->u.next)
#define invalidateTMcache(t)	((t)->flags &= ~maskflags)
#define isdummy(t)		((t)->lastfree == NULL)
#define allocsizenode(t)	(isdummy(t) ? 0 : sizenode(t))
#define nodefromval(v)	cast(Node *, (v))
LUAI_FUNC const TValue *luaH_getint (Table *t, lua_Integer key);
LUAI_FUNC void luaH_setint (lua_State *L, Table *t, lua_Integer key,
                                                    TValue *value);
LUAI_FUNC const TValue *luaH_getshortstr (Table *t, TString *key);
LUAI_FUNC const TValue *luaH_getstr (Table *t, TString *key);
LUAI_FUNC const TValue *luaH_get (Table *t, const TValue *key);
LUAI_FUNC void luaH_newkey (lua_State *L, Table *t, const TValue *key,
                                                    TValue *value);
LUAI_FUNC void luaH_set (lua_State *L, Table *t, const TValue *key,
                                                 TValue *value);
LUAI_FUNC void luaH_finishset (lua_State *L, Table *t, const TValue *key,
                                       const TValue *slot, TValue *value);
LUAI_FUNC Table *luaH_new (lua_State *L);
LUAI_FUNC void luaH_resize (lua_State *L, Table *t, unsigned int nasize,
                                                    unsigned int nhsize);
LUAI_FUNC void luaH_resizearray (lua_State *L, Table *t, unsigned int nasize);
LUAI_FUNC void luaH_free (lua_State *L, Table *t);
LUAI_FUNC int luaH_next (lua_State *L, Table *t, StkId key);
LUAI_FUNC lua_Unsigned luaH_getn (Table *t);
LUAI_FUNC unsigned int luaH_realasize (const Table *t);
#if defined(LUA_DEBUG)
LUAI_FUNC Node *luaH_mainposition (const Table *t, const TValue *key);
LUAI_FUNC int luaH_isdummy (const Table *t);
#endif
#endif
//included "ltm.h" 
#ifndef lundump_h
#define LUAC_DATA	"\x19\x93\r\n\x1a\n"
#define LUAC_INT	0x5678
#define LUAC_NUM	cast_num(370.5)
#define MYINT(s)	(s[0]-'0')  
#define LUAC_VERSION	(MYINT(LUA_VERSION_MAJOR)*16+MYINT(LUA_VERSION_MINOR))
#define LUAC_FORMAT	0	
LUAI_FUNC LClosure* luaU_undump (lua_State* L, ZIO* Z, const char* name);
LUAI_FUNC int luaU_dump (lua_State* L, const Proto* f, lua_Writer w,
                         void* data, int strip);
#endif
#ifndef lvm_h
#if !defined(LUA_NOCVTN2S)
#define cvt2str(o)	ttisnumber(o)
#else
#define cvt2str(o)	0	
#endif
#if !defined(LUA_NOCVTS2N)
#define cvt2num(o)	ttisstring(o)
#else
#define cvt2num(o)	0	
#endif
#if !defined(LUA_FLOORN2I)
#define LUA_FLOORN2I		F2Ieq
#endif
typedef enum {
  F2Ieq,     
  F2Ifloor,  
  F2Iceil    
} F2Imod;
#define tonumber(o,n) 	(ttisfloat(o) ? (*(n) = fltvalue(o), 1) : luaV_tonumber_(o,n))
#define tonumberns(o,n) 	(ttisfloat(o) ? ((n) = fltvalue(o), 1) : 	(ttisinteger(o) ? ((n) = cast_num(ivalue(o)), 1) : 0))
#define tointeger(o,i)   (l_likely(ttisinteger(o)) ? (*(i) = ivalue(o), 1)                           : luaV_tointeger(o,i,LUA_FLOORN2I))
#define tointegerns(o,i)   (l_likely(ttisinteger(o)) ? (*(i) = ivalue(o), 1)                           : luaV_tointegerns(o,i,LUA_FLOORN2I))
#define intop(op,v1,v2) l_castU2S(l_castS2U(v1) op l_castS2U(v2))
#define luaV_rawequalobj(t1,t2)		luaV_equalobj(NULL,t1,t2)
#define luaV_fastget(L,t,k,slot,f)   (!ttistable(t)     ? (slot = NULL, 0)       : (slot = f(hvalue(t), k),          !isempty(slot)))  
#define luaV_fastgeti(L,t,k,slot)   (!ttistable(t)     ? (slot = NULL, 0)       : (slot = (l_castS2U(k) - 1u < hvalue(t)->alimit)               ? &hvalue(t)->array[k - 1] : luaH_getint(hvalue(t), k),       !isempty(slot)))  
#define luaV_finishfastset(L,t,slot,v)     { setobj2t(L, cast(TValue *,slot), v);       luaC_barrierback(L, gcvalue(t), v); }
LUAI_FUNC int luaV_equalobj (lua_State *L, const TValue *t1, const TValue *t2);
LUAI_FUNC int luaV_lessthan (lua_State *L, const TValue *l, const TValue *r);
LUAI_FUNC int luaV_lessequal (lua_State *L, const TValue *l, const TValue *r);
LUAI_FUNC int luaV_tonumber_ (const TValue *obj, lua_Number *n);
LUAI_FUNC int luaV_tointeger (const TValue *obj, lua_Integer *p, F2Imod mode);
LUAI_FUNC int luaV_tointegerns (const TValue *obj, lua_Integer *p,
                                F2Imod mode);
LUAI_FUNC int luaV_flttointeger (lua_Number n, lua_Integer *p, F2Imod mode);
LUAI_FUNC void luaV_finishget (lua_State *L, const TValue *t, TValue *key,
                               StkId val, const TValue *slot);
LUAI_FUNC void luaV_finishset (lua_State *L, const TValue *t, TValue *key,
                               TValue *val, const TValue *slot);
LUAI_FUNC void luaV_finishOp (lua_State *L);
LUAI_FUNC void luaV_execute (lua_State *L, CallInfo *ci);
LUAI_FUNC void luaV_concat (lua_State *L, int total);
LUAI_FUNC lua_Integer luaV_idiv (lua_State *L, lua_Integer x, lua_Integer y);
LUAI_FUNC lua_Integer luaV_mod (lua_State *L, lua_Integer x, lua_Integer y);
LUAI_FUNC lua_Number luaV_modf (lua_State *L, lua_Number x, lua_Number y);
LUAI_FUNC lua_Integer luaV_shiftl (lua_Integer x, lua_Integer y);
LUAI_FUNC void luaV_objlen (lua_State *L, StkId ra, const TValue *rb);
#endif
const char lua_ident[] =
  "$LuaVersion: " LUA_COPYRIGHT " $"
  "$LuaAuthors: " LUA_AUTHORS " $";
#define isvalid(L, o)	(!ttisnil(o) || o != &G(L)->nilvalue)
#define ispseudo(i)		((i) <= LUA_REGISTRYINDEX)
#define isupvalue(i)		((i) < LUA_REGISTRYINDEX)
static TValue *index2value (lua_State *L, int idx) {
  CallInfo *ci = L->ci;
  if (idx > 0) {
    StkId o = ci->func + idx;
    api_check(L, idx <= L->ci->top - (ci->func + 1), "unacceptable index");
    if (o >= L->top) return &G(L)->nilvalue;
    else return s2v(o);
  }
  else if (!ispseudo(idx)) {  
    api_check(L, idx != 0 && -idx <= L->top - (ci->func + 1), "invalid index");
    return s2v(L->top + idx);
  }
  else if (idx == LUA_REGISTRYINDEX)
    return &G(L)->l_registry;
  else {  
    idx = LUA_REGISTRYINDEX - idx;
    api_check(L, idx <= MAXUPVAL + 1, "upvalue index too large");
    if (ttisCclosure(s2v(ci->func))) {  
      CClosure *func = clCvalue(s2v(ci->func));
      return (idx <= func->nupvalues) ? &func->upvalue[idx-1]
                                      : &G(L)->nilvalue;
    }
    else {  
      api_check(L, ttislcf(s2v(ci->func)), "caller not a C function");
      return &G(L)->nilvalue;  
    }
  }
}
l_sinline StkId index2stack (lua_State *L, int idx) {
  CallInfo *ci = L->ci;
  if (idx > 0) {
    StkId o = ci->func + idx;
    api_check(L, o < L->top, "invalid index");
    return o;
  }
  else {    
    api_check(L, idx != 0 && -idx <= L->top - (ci->func + 1), "invalid index");
    api_check(L, !ispseudo(idx), "invalid index");
    return L->top + idx;
  }
}
LUA_API int lua_checkstack (lua_State *L, int n) {
  int res;
  CallInfo *ci;
  lua_lock(L);
  ci = L->ci;
  api_check(L, n >= 0, "negative 'n'");
  if (L->stack_last - L->top > n)  
    res = 1;  
  else {  
    int inuse = cast_int(L->top - L->stack) + EXTRA_STACK;
    if (inuse > LUAI_MAXSTACK - n)  
      res = 0;  
    else  
      res = luaD_growstack(L, n, 0);
  }
  if (res && ci->top < L->top + n)
    ci->top = L->top + n;  
  lua_unlock(L);
  return res;
}
LUA_API void lua_xmove (lua_State *from, lua_State *to, int n) {
  int i;
  if (from == to) return;
  lua_lock(to);
  api_checknelems(from, n);
  api_check(from, G(from) == G(to), "moving among independent states");
  api_check(from, to->ci->top - to->top >= n, "stack overflow");
  from->top -= n;
  for (i = 0; i < n; i++) {
    setobjs2s(to, to->top, from->top + i);
    to->top++;  
  }
  lua_unlock(to);
}
LUA_API lua_CFunction lua_atpanic (lua_State *L, lua_CFunction panicf) {
  lua_CFunction old;
  lua_lock(L);
  old = G(L)->panic;
  G(L)->panic = panicf;
  lua_unlock(L);
  return old;
}
LUA_API lua_Number lua_version (lua_State* L) {
  UNUSED(L);
  return LUA_VERSION_NUM;
}
LUA_API int lua_absindex (lua_State *L, int idx) {
  return (idx > 0 || ispseudo(idx))
         ? idx
         : cast_int(L->top - L->ci->func) + idx;
}
LUA_API int lua_gettop (lua_State *L) {
  return cast_int(L->top - (L->ci->func + 1));
}
LUA_API void lua_settop (lua_State *L, int idx) {
  CallInfo *ci;
  StkId func, newtop;
  ptrdiff_t diff;  
  lua_lock(L);
  ci = L->ci;
  func = ci->func;
  if (idx >= 0) {
    api_check(L, idx <= ci->top - (func + 1), "new top too large");
    diff = ((func + 1) + idx) - L->top;
    for (; diff > 0; diff--)
      setnilvalue(s2v(L->top++));  
  }
  else {
    api_check(L, -(idx+1) <= (L->top - (func + 1)), "invalid new top");
    diff = idx + 1;  
  }
  api_check(L, L->tbclist < L->top, "previous pop of an unclosed slot");
  newtop = L->top + diff;
  if (diff < 0 && L->tbclist >= newtop) {
    lua_assert(hastocloseCfunc(ci->nresults));
    luaF_close(L, newtop, CLOSEKTOP, 0);
  }
  L->top = newtop;  
  lua_unlock(L);
}
LUA_API void lua_closeslot (lua_State *L, int idx) {
  StkId level;
  lua_lock(L);
  level = index2stack(L, idx);
  api_check(L, hastocloseCfunc(L->ci->nresults) && L->tbclist == level,
     "no variable to close at given level");
  luaF_close(L, level, CLOSEKTOP, 0);
  level = index2stack(L, idx);  
  setnilvalue(s2v(level));
  lua_unlock(L);
}
l_sinline void reverse (lua_State *L, StkId from, StkId to) {
  for (; from < to; from++, to--) {
    TValue temp;
    setobj(L, &temp, s2v(from));
    setobjs2s(L, from, to);
    setobj2s(L, to, &temp);
  }
}
LUA_API void lua_rotate (lua_State *L, int idx, int n) {
  StkId p, t, m;
  lua_lock(L);
  t = L->top - 1;  
  p = index2stack(L, idx);  
  api_check(L, (n >= 0 ? n : -n) <= (t - p + 1), "invalid 'n'");
  m = (n >= 0 ? t - n : p - n - 1);  
  reverse(L, p, m);  
  reverse(L, m + 1, t);  
  reverse(L, p, t);  
  lua_unlock(L);
}
LUA_API void lua_copy (lua_State *L, int fromidx, int toidx) {
  TValue *fr, *to;
  lua_lock(L);
  fr = index2value(L, fromidx);
  to = index2value(L, toidx);
  api_check(L, isvalid(L, to), "invalid index");
  setobj(L, to, fr);
  if (isupvalue(toidx))  
    luaC_barrier(L, clCvalue(s2v(L->ci->func)), fr);
  
  lua_unlock(L);
}
LUA_API void lua_pushvalue (lua_State *L, int idx) {
  lua_lock(L);
  setobj2s(L, L->top, index2value(L, idx));
  api_incr_top(L);
  lua_unlock(L);
}
LUA_API void lua_pushobjtype(lua_State* L, int idx) {
	const TValue *o = index2value(L, idx);
	api_checknelems(L, 1);
  setsvalue2s(L, L->top, luaT_objtypestr(L, o));
	api_incr_top(L);
  return;
}
LUA_API int lua_type (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (isvalid(L, o) ? ttype(o) : LUA_TNONE);
}
LUA_API const char *lua_typename (lua_State *L, int t) {
  UNUSED(L);
  api_check(L, LUA_TNONE <= t && t < LUA_NUMTYPES, "invalid type");
  return ttypename(t);
}
LUA_API const char* lua_objtypename(lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return luaT_objtypename(L, o);
}
LUA_API int lua_iscfunction (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (ttislcf(o) || (ttisCclosure(o)));
}
LUA_API int lua_isinteger (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return ttisinteger(o);
}
LUA_API int lua_isnumber (lua_State *L, int idx) {
  lua_Number n;
  const TValue *o = index2value(L, idx);
  return tonumber(o, &n);
}
LUA_API int lua_isstring (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (ttisstring(o) || cvt2str(o));
}
LUA_API int lua_isuserdata (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (ttisfulluserdata(o) || ttislightuserdata(o));
}
LUA_API int lua_rawequal (lua_State *L, int index1, int index2) {
  const TValue *o1 = index2value(L, index1);
  const TValue *o2 = index2value(L, index2);
  return (isvalid(L, o1) && isvalid(L, o2)) ? luaV_rawequalobj(o1, o2) : 0;
}
LUA_API void lua_arith (lua_State *L, int op) {
  lua_lock(L);
  if (op != LUA_OPUNM && op != LUA_OPBNOT)
    api_checknelems(L, 2);  
  else {  
    api_checknelems(L, 1);
    setobjs2s(L, L->top, L->top - 1);
    api_incr_top(L);
  }
  
  luaO_arith(L, op, s2v(L->top - 2), s2v(L->top - 1), L->top - 2);
  L->top--;  
  lua_unlock(L);
}
LUA_API int lua_compare (lua_State *L, int index1, int index2, int op) {
  const TValue *o1;
  const TValue *o2;
  int i = 0;
  lua_lock(L);  
  o1 = index2value(L, index1);
  o2 = index2value(L, index2);
  if (isvalid(L, o1) && isvalid(L, o2)) {
    switch (op) {
      case LUA_OPEQ: i = luaV_equalobj(L, o1, o2); break;
      case LUA_OPLT: i = luaV_lessthan(L, o1, o2); break;
      case LUA_OPLE: i = luaV_lessequal(L, o1, o2); break;
      default: api_check(L, 0, "invalid option");
    }
  }
  lua_unlock(L);
  return i;
}
LUA_API size_t lua_stringtonumber (lua_State *L, const char *s) {
  size_t sz = luaO_str2num(s, s2v(L->top));
  if (sz != 0)
    api_incr_top(L);
  return sz;
}
LUA_API lua_Number lua_tonumberx (lua_State *L, int idx, int *pisnum) {
  lua_Number n = 0;
  const TValue *o = index2value(L, idx);
  int isnum = tonumber(o, &n);
  if (pisnum)
    *pisnum = isnum;
  return n;
}
LUA_API lua_Integer lua_tointegerx (lua_State *L, int idx, int *pisnum) {
  lua_Integer res = 0;
  const TValue *o = index2value(L, idx);
  int isnum = tointeger(o, &res);
  if (pisnum)
    *pisnum = isnum;
  return res;
}
LUA_API int lua_toboolean (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return !l_isfalse(o);
}
LUA_API const char *lua_tolstring (lua_State *L, int idx, size_t *len) {
  TValue *o;
  lua_lock(L);
  o = index2value(L, idx);
  if (!ttisstring(o)) {
    if (!cvt2str(o)) {  
      if (len != NULL) *len = 0;
      lua_unlock(L);
      return NULL;
    }
    luaO_tostring(L, o);
    luaC_checkGC(L);
    o = index2value(L, idx);  
  }
  if (len != NULL)
    *len = vslen(o);
  lua_unlock(L);
  return svalue(o);
}
LUA_API lua_Unsigned lua_rawlen (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  switch (ttypetag(o)) {
    case LUA_VSHRSTR: return tsvalue(o)->shrlen;
    case LUA_VLNGSTR: return tsvalue(o)->u.lnglen;
    case LUA_VUSERDATA: return uvalue(o)->len;
    case LUA_VTABLE: return luaH_getn(hvalue(o));
    default: return 0;
  }
}
LUA_API lua_CFunction lua_tocfunction (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  if (ttislcf(o)) return fvalue(o);
  else if (ttisCclosure(o))
    return clCvalue(o)->f;
  else return NULL;  
}
l_sinline void *touserdata (const TValue *o) {
  switch (ttype(o)) {
    case LUA_TUSERDATA: return getudatamem(uvalue(o));
    case LUA_TLIGHTUSERDATA: return pvalue(o);
    default: return NULL;
  }
}
LUA_API void *lua_touserdata (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return touserdata(o);
}
LUA_API lua_State *lua_tothread (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  return (!ttisthread(o)) ? NULL : thvalue(o);
}
LUA_API const void *lua_topointer (lua_State *L, int idx) {
  const TValue *o = index2value(L, idx);
  switch (ttypetag(o)) {
    case LUA_VLCF: return cast_voidp(cast_sizet(fvalue(o)));
    case LUA_VUSERDATA: case LUA_VLIGHTUSERDATA:
      return touserdata(o);
    default: {
      if (iscollectable(o))
        return gcvalue(o);
      else
        return NULL;
    }
  }
}
LUA_API void lua_pushnil (lua_State *L) {
  lua_lock(L);
  setnilvalue(s2v(L->top));
  api_incr_top(L);
  lua_unlock(L);
}
LUA_API void lua_pushnumber (lua_State *L, lua_Number n) {
  lua_lock(L);
  setfltvalue(s2v(L->top), n);
  api_incr_top(L);
  lua_unlock(L);
}
LUA_API void lua_pushinteger (lua_State *L, lua_Integer n) {
  lua_lock(L);
  setivalue(s2v(L->top), n);
  api_incr_top(L);
  lua_unlock(L);
}
LUA_API const char *lua_pushlstring (lua_State *L, const char *s, size_t len) {
  TString *ts;
  lua_lock(L);
  ts = (len == 0) ? luaS_new(L, "") : luaS_newlstr(L, s, len);
  setsvalue2s(L, L->top, ts);
  api_incr_top(L);
  luaC_checkGC(L);
  lua_unlock(L);
  return getstr(ts);
}
LUA_API const char *lua_pushstring (lua_State *L, const char *s) {
  lua_lock(L);
  if (s == NULL)
    setnilvalue(s2v(L->top));
  else {
    TString *ts;
    ts = luaS_new(L, s);
    setsvalue2s(L, L->top, ts);
    s = getstr(ts);  
  }
  api_incr_top(L);
  luaC_checkGC(L);
  lua_unlock(L);
  return s;
}
LUA_API const char *lua_pushvfstring (lua_State *L, const char *fmt,
                                      va_list argp) {
  const char *ret;
  lua_lock(L);
  ret = luaO_pushvfstring(L, fmt, argp);
  luaC_checkGC(L);
  lua_unlock(L);
  return ret;
}
LUA_API const char *lua_pushfstring (lua_State *L, const char *fmt, ...) {
  const char *ret;
  va_list argp;
  lua_lock(L);
  va_start(argp, fmt);
  ret = luaO_pushvfstring(L, fmt, argp);
  va_end(argp);
  luaC_checkGC(L);
  lua_unlock(L);
  return ret;
}
LUA_API void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n) {
  lua_lock(L);
  if (n == 0) {
    setfvalue(s2v(L->top), fn);
    api_incr_top(L);
  }
  else {
    CClosure *cl;
    api_checknelems(L, n);
    api_check(L, n <= MAXUPVAL, "upvalue index too large");
    cl = luaF_newCclosure(L, n);
    cl->f = fn;
    L->top -= n;
    while (n--) {
      setobj2n(L, &cl->upvalue[n], s2v(L->top + n));
      
      lua_assert(iswhite(cl));
    }
    setclCvalue(L, s2v(L->top), cl);
    api_incr_top(L);
    luaC_checkGC(L);
  }
  lua_unlock(L);
}
LUA_API void lua_pushboolean (lua_State *L, int b) {
  lua_lock(L);
  if (b)
    setbtvalue(s2v(L->top));
  else
    setbfvalue(s2v(L->top));
  api_incr_top(L);
  lua_unlock(L);
}
LUA_API void lua_pushlightuserdata (lua_State *L, void *p) {
  lua_lock(L);
  setpvalue(s2v(L->top), p);
  api_incr_top(L);
  lua_unlock(L);
}
LUA_API int lua_pushthread (lua_State *L) {
  lua_lock(L);
  setthvalue(L, s2v(L->top), L);
  api_incr_top(L);
  lua_unlock(L);
  return (G(L)->mainthread == L);
}
l_sinline int auxgetstr (lua_State *L, const TValue *t, const char *k) {
  const TValue *slot;
  TString *str = luaS_new(L, k);
  if (luaV_fastget(L, t, str, slot, luaH_getstr)) {
    setobj2s(L, L->top, slot);
    api_incr_top(L);
  }
  else {
    setsvalue2s(L, L->top, str);
    api_incr_top(L);
    luaV_finishget(L, t, s2v(L->top - 1), L->top - 1, slot);
  }
  lua_unlock(L);
  return ttype(s2v(L->top - 1));
}
#define getGtable(L)  	(&hvalue(&G(L)->l_registry)->array[LUA_RIDX_GLOBALS - 1])
LUA_API int lua_getglobal (lua_State *L, const char *name) {
  const TValue *G;
  lua_lock(L);
  G = getGtable(L);
  return auxgetstr(L, G, name);
}
LUA_API int lua_gettable (lua_State *L, int idx) {
  const TValue *slot;
  TValue *t;
  lua_lock(L);
  t = index2value(L, idx);
  if (luaV_fastget(L, t, s2v(L->top - 1), slot, luaH_get)) {
    setobj2s(L, L->top - 1, slot);
  }
  else
    luaV_finishget(L, t, s2v(L->top - 1), L->top - 1, slot);
  lua_unlock(L);
  return ttype(s2v(L->top - 1));
}
LUA_API int lua_getfield (lua_State *L, int idx, const char *k) {
  lua_lock(L);
  return auxgetstr(L, index2value(L, idx), k);
}
LUA_API int lua_geti (lua_State *L, int idx, lua_Integer n) {
  TValue *t;
  const TValue *slot;
  lua_lock(L);
  t = index2value(L, idx);
  if (luaV_fastgeti(L, t, n, slot)) {
    setobj2s(L, L->top, slot);
  }
  else {
    TValue aux;
    setivalue(&aux, n);
    luaV_finishget(L, t, &aux, L->top, slot);
  }
  api_incr_top(L);
  lua_unlock(L);
  return ttype(s2v(L->top - 1));
}
l_sinline int finishrawget (lua_State *L, const TValue *val) {
  if (isempty(val))  
    setnilvalue(s2v(L->top));
  else
    setobj2s(L, L->top, val);
  api_incr_top(L);
  lua_unlock(L);
  return ttype(s2v(L->top - 1));
}
static Table *gettable (lua_State *L, int idx) {
  TValue *t = index2value(L, idx);
  api_check(L, ttistable(t), "table expected");
  return hvalue(t);
}
LUA_API int lua_rawget (lua_State *L, int idx) {
  Table *t;
  const TValue *val;
  lua_lock(L);
  api_checknelems(L, 1);
  t = gettable(L, idx);
  val = luaH_get(t, s2v(L->top - 1));
  L->top--;  
  return finishrawget(L, val);
}
LUA_API int lua_rawgeti (lua_State *L, int idx, lua_Integer n) {
  Table *t;
  lua_lock(L);
  t = gettable(L, idx);
  return finishrawget(L, luaH_getint(t, n));
}
LUA_API int lua_rawgetp (lua_State *L, int idx, const void *p) {
  Table *t;
  TValue k;
  lua_lock(L);
  t = gettable(L, idx);
  setpvalue(&k, cast_voidp(p));
  return finishrawget(L, luaH_get(t, &k));
}
LUA_API void lua_createtable (lua_State *L, int narray, int nrec) {
  Table *t;
  lua_lock(L);
  t = luaH_new(L);
  sethvalue2s(L, L->top, t);
  api_incr_top(L);
  if (narray > 0 || nrec > 0)
    luaH_resize(L, t, narray, nrec);
  luaC_checkGC(L);
  lua_unlock(L);
}
LUA_API int lua_getmetatable (lua_State *L, int objindex) {
  const TValue *obj;
  Table *mt;
  int res = 0;
  lua_lock(L);
  obj = index2value(L, objindex);
  switch (ttype(obj)) {
    case LUA_TTABLE:
      mt = hvalue(obj)->metatable;
      break;
    case LUA_TUSERDATA:
      mt = uvalue(obj)->metatable;
      break;
    default:
      mt = G(L)->mt[ttype(obj)];
      break;
  }
  if (mt != NULL) {
    sethvalue2s(L, L->top, mt);
    api_incr_top(L);
    res = 1;
  }
  lua_unlock(L);
  return res;
}
LUA_API int lua_getiuservalue (lua_State *L, int idx, int n) {
  TValue *o;
  int t;
  lua_lock(L);
  o = index2value(L, idx);
  api_check(L, ttisfulluserdata(o), "full userdata expected");
  if (n <= 0 || n > uvalue(o)->nuvalue) {
    setnilvalue(s2v(L->top));
    t = LUA_TNONE;
  }
  else {
    setobj2s(L, L->top, &uvalue(o)->uv[n - 1].uv);
    t = ttype(s2v(L->top));
  }
  api_incr_top(L);
  lua_unlock(L);
  return t;
}
static void auxsetstr (lua_State *L, const TValue *t, const char *k) {
  const TValue *slot;
  TString *str = luaS_new(L, k);
  api_checknelems(L, 1);
  if (luaV_fastget(L, t, str, slot, luaH_getstr)) {
    luaV_finishfastset(L, t, slot, s2v(L->top - 1));
    L->top--;  
  }
  else {
    setsvalue2s(L, L->top, str);  
    api_incr_top(L);
    luaV_finishset(L, t, s2v(L->top - 1), s2v(L->top - 2), slot);
    L->top -= 2;  
  }
  lua_unlock(L);  
}
LUA_API void lua_setglobal (lua_State *L, const char *name) {
  const TValue *G;
  lua_lock(L);  
  G = getGtable(L);
  auxsetstr(L, G, name);
}
LUA_API void lua_settable (lua_State *L, int idx) {
  TValue *t;
  const TValue *slot;
  lua_lock(L);
  api_checknelems(L, 2);
  t = index2value(L, idx);
  if (luaV_fastget(L, t, s2v(L->top - 2), slot, luaH_get)) {
    luaV_finishfastset(L, t, slot, s2v(L->top - 1));
  }
  else
    luaV_finishset(L, t, s2v(L->top - 2), s2v(L->top - 1), slot);
  L->top -= 2;  
  lua_unlock(L);
}
LUA_API void lua_setfield (lua_State *L, int idx, const char *k) {
  lua_lock(L);  
  auxsetstr(L, index2value(L, idx), k);
}
LUA_API void lua_seti (lua_State *L, int idx, lua_Integer n) {
  TValue *t;
  const TValue *slot;
  lua_lock(L);
  api_checknelems(L, 1);
  t = index2value(L, idx);
  if (luaV_fastgeti(L, t, n, slot)) {
    luaV_finishfastset(L, t, slot, s2v(L->top - 1));
  }
  else {
    TValue aux;
    setivalue(&aux, n);
    luaV_finishset(L, t, &aux, s2v(L->top - 1), slot);
  }
  L->top--;  
  lua_unlock(L);
}
static void aux_rawset (lua_State *L, int idx, TValue *key, int n) {
  Table *t;
  lua_lock(L);
  api_checknelems(L, n);
  t = gettable(L, idx);
  luaH_set(L, t, key, s2v(L->top - 1));
  invalidateTMcache(t);
  luaC_barrierback(L, obj2gco(t), s2v(L->top - 1));
  L->top -= n;
  lua_unlock(L);
}
LUA_API void lua_rawset (lua_State *L, int idx) {
  aux_rawset(L, idx, s2v(L->top - 2), 2);
}
LUA_API void lua_rawsetp (lua_State *L, int idx, const void *p) {
  TValue k;
  setpvalue(&k, cast_voidp(p));
  aux_rawset(L, idx, &k, 1);
}
LUA_API void lua_rawseti (lua_State *L, int idx, lua_Integer n) {
  Table *t;
  lua_lock(L);
  api_checknelems(L, 1);
  t = gettable(L, idx);
  luaH_setint(L, t, n, s2v(L->top - 1));
  luaC_barrierback(L, obj2gco(t), s2v(L->top - 1));
  L->top--;
  lua_unlock(L);
}
LUA_API int lua_setmetatable (lua_State *L, int objindex) {
  TValue *obj;
  Table *mt;
  lua_lock(L);
  api_checknelems(L, 1);
  obj = index2value(L, objindex);
  if (ttisnil(s2v(L->top - 1)))
    mt = NULL;
  else {
    api_check(L, ttistable(s2v(L->top - 1)), "table expected");
    mt = hvalue(s2v(L->top - 1));
  }
  switch (ttype(obj)) {
    case LUA_TTABLE: {
      hvalue(obj)->metatable = mt;
      if (mt) {
        luaC_objbarrier(L, gcvalue(obj), mt);
        luaC_checkfinalizer(L, gcvalue(obj), mt);
      }
      break;
    }
    case LUA_TUSERDATA: {
      uvalue(obj)->metatable = mt;
      if (mt) {
        luaC_objbarrier(L, uvalue(obj), mt);
        luaC_checkfinalizer(L, gcvalue(obj), mt);
      }
      break;
    }
    default: {
      G(L)->mt[ttype(obj)] = mt;
      break;
    }
  }
  L->top--;
  lua_unlock(L);
  return 1;
}
LUA_API int lua_setiuservalue (lua_State *L, int idx, int n) {
  TValue *o;
  int res;
  lua_lock(L);
  api_checknelems(L, 1);
  o = index2value(L, idx);
  api_check(L, ttisfulluserdata(o), "full userdata expected");
  if (!(cast_uint(n) - 1u < cast_uint(uvalue(o)->nuvalue)))
    res = 0;  
  else {
    setobj(L, &uvalue(o)->uv[n - 1].uv, s2v(L->top - 1));
    luaC_barrierback(L, gcvalue(o), s2v(L->top - 1));
    res = 1;
  }
  L->top--;
  lua_unlock(L);
  return res;
}
#define checkresults(L,na,nr)      api_check(L, (nr) == LUA_MULTRET || (L->ci->top - L->top >= (nr) - (na)), 	"results from function overflow current stack size")
LUA_API void lua_callk (lua_State *L, int nargs, int nresults,
                        lua_KContext ctx, lua_KFunction k) {
  StkId func;
  lua_lock(L);
  api_check(L, k == NULL || !isLua(L->ci),
    "cannot use continuations inside hooks");
  api_checknelems(L, nargs+1);
  api_check(L, L->status == LUA_OK, "cannot do calls on non-normal thread");
  checkresults(L, nargs, nresults);
  func = L->top - (nargs+1);
  if (k != NULL && yieldable(L)) {  
    L->ci->u.c.k = k;  
    L->ci->u.c.ctx = ctx;  
    luaD_call(L, func, nresults);  
  }
  else  
    luaD_callnoyield(L, func, nresults);  
  adjustresults(L, nresults);
  lua_unlock(L);
}
struct CallS {  
  StkId func;
  int nresults;
};
static void f_call (lua_State *L, void *ud) {
  struct CallS *c = cast(struct CallS *, ud);
  luaD_callnoyield(L, c->func, c->nresults);
}
LUA_API int lua_pcallk (lua_State *L, int nargs, int nresults, int errfunc,
                        lua_KContext ctx, lua_KFunction k) {
  struct CallS c;
  int status;
  ptrdiff_t func;
  lua_lock(L);
  api_check(L, k == NULL || !isLua(L->ci),
    "cannot use continuations inside hooks");
  api_checknelems(L, nargs+1);
  api_check(L, L->status == LUA_OK, "cannot do calls on non-normal thread");
  checkresults(L, nargs, nresults);
  if (errfunc == 0)
    func = 0;
  else {
    StkId o = index2stack(L, errfunc);
    api_check(L, ttisfunction(s2v(o)), "error handler must be a function");
    func = savestack(L, o);
  }
  c.func = L->top - (nargs+1);  
  if (k == NULL || !yieldable(L)) {  
    c.nresults = nresults;  
    status = luaD_pcall(L, f_call, &c, savestack(L, c.func), func);
  }
  else {  
    CallInfo *ci = L->ci;
    ci->u.c.k = k;  
    ci->u.c.ctx = ctx;  
    
    ci->u2.funcidx = cast_int(savestack(L, c.func));
    ci->u.c.old_errfunc = L->errfunc;
    L->errfunc = func;
    setoah(ci->callstatus, L->allowhook);  
    ci->callstatus |= CIST_YPCALL;  
    luaD_call(L, c.func, nresults);  
    ci->callstatus &= ~CIST_YPCALL;
    L->errfunc = ci->u.c.old_errfunc;
    status = LUA_OK;  
  }
  adjustresults(L, nresults);
  lua_unlock(L);
  return status;
}
LUA_API int lua_load (lua_State *L, lua_Reader reader, void *data,
                      const char *chunkname, const char *mode) {
  ZIO z;
  int status;
  lua_lock(L);
  if (!chunkname) chunkname = "?";
  luaZ_init(L, &z, reader, data);
  status = luaD_protectedparser(L, &z, chunkname, mode);
  if (status == LUA_OK) {  
    LClosure *f = clLvalue(s2v(L->top - 1));  
    if (f->nupvalues >= 1) {  
      
      const TValue *gt = getGtable(L);
      
      setobj(L, f->upvals[0]->v, gt);
      luaC_barrier(L, f->upvals[0], gt);
    }
  }
  lua_unlock(L);
  return status;
}
LUA_API int lua_dump (lua_State *L, lua_Writer writer, void *data, int strip) {
  int status;
  TValue *o;
  lua_lock(L);
  api_checknelems(L, 1);
  o = s2v(L->top - 1);
  if (isLfunction(o))
    status = luaU_dump(L, getproto(o), writer, data, strip);
  else
    status = 1;
  lua_unlock(L);
  return status;
}
LUA_API int lua_status (lua_State *L) {
  return L->status;
}
LUA_API int lua_gc (lua_State *L, int what, ...) {
  va_list argp;
  int res = 0;
  global_State *g = G(L);
  if (g->gcstp & GCSTPGC)  
    return -1;  
  lua_lock(L);
  va_start(argp, what);
  switch (what) {
    case LUA_GCSTOP: {
      g->gcstp = GCSTPUSR;  
      break;
    }
    case LUA_GCRESTART: {
      luaE_setdebt(g, 0);
      g->gcstp = 0;  
      break;
    }
    case LUA_GCCOLLECT: {
      luaC_fullgc(L, 0);
      break;
    }
    case LUA_GCCOUNT: {
      
      res = cast_int(gettotalbytes(g) >> 10);
      break;
    }
    case LUA_GCCOUNTB: {
      res = cast_int(gettotalbytes(g) & 0x3ff);
      break;
    }
    case LUA_GCSTEP: {
      int data = va_arg(argp, int);
      l_mem debt = 1;  
      lu_byte oldstp = g->gcstp;
      g->gcstp = 0;  
      if (data == 0) {
        luaE_setdebt(g, 0);  
        luaC_step(L);
      }
      else {  
        debt = cast(l_mem, data) * 1024 + g->GCdebt;
        luaE_setdebt(g, debt);
        luaC_checkGC(L);
      }
      g->gcstp = oldstp;  
      if (debt > 0 && g->gcstate == GCSpause)  
        res = 1;  
      break;
    }
    case LUA_GCSETPAUSE: {
      int data = va_arg(argp, int);
      res = getgcparam(g->gcpause);
      setgcparam(g->gcpause, data);
      break;
    }
    case LUA_GCSETSTEPMUL: {
      int data = va_arg(argp, int);
      res = getgcparam(g->gcstepmul);
      setgcparam(g->gcstepmul, data);
      break;
    }
    case LUA_GCISRUNNING: {
      res = gcrunning(g);
      break;
    }
    case LUA_GCGEN: {
      int minormul = va_arg(argp, int);
      int majormul = va_arg(argp, int);
      res = isdecGCmodegen(g) ? LUA_GCGEN : LUA_GCINC;
      if (minormul != 0)
        g->genminormul = minormul;
      if (majormul != 0)
        setgcparam(g->genmajormul, majormul);
      luaC_changemode(L, KGC_GEN);
      break;
    }
    case LUA_GCINC: {
      int pause = va_arg(argp, int);
      int stepmul = va_arg(argp, int);
      int stepsize = va_arg(argp, int);
      res = isdecGCmodegen(g) ? LUA_GCGEN : LUA_GCINC;
      if (pause != 0)
        setgcparam(g->gcpause, pause);
      if (stepmul != 0)
        setgcparam(g->gcstepmul, stepmul);
      if (stepsize != 0)
        g->gcstepsize = stepsize;
      luaC_changemode(L, KGC_INC);
      break;
    }
    default: res = -1;  
  }
  va_end(argp);
  lua_unlock(L);
  return res;
}
LUA_API int lua_error (lua_State *L) {
  TValue *errobj;
  lua_lock(L);
  errobj = s2v(L->top - 1);
  api_checknelems(L, 1);
  
  if (ttisshrstring(errobj) && eqshrstr(tsvalue(errobj), G(L)->memerrmsg))
    luaM_error(L);  
  else
    luaG_errormsg(L);  
  
  return 0;  
}
LUA_API int lua_next (lua_State *L, int idx) {
  Table *t;
  int more;
  lua_lock(L);
  api_checknelems(L, 1);
  t = gettable(L, idx);
  more = luaH_next(L, t, L->top - 1);
  if (more) {
    api_incr_top(L);
  }
  else  
    L->top -= 1;  
  lua_unlock(L);
  return more;
}
LUA_API void lua_toclose (lua_State *L, int idx) {
  int nresults;
  StkId o;
  lua_lock(L);
  o = index2stack(L, idx);
  nresults = L->ci->nresults;
  api_check(L, L->tbclist < o, "given index below or equal a marked one");
  luaF_newtbcupval(L, o);  
  if (!hastocloseCfunc(nresults))  
    L->ci->nresults = codeNresults(nresults);  
  lua_assert(hastocloseCfunc(L->ci->nresults));
  lua_unlock(L);
}
LUA_API void lua_concat (lua_State *L, int n) {
  lua_lock(L);
  api_checknelems(L, n);
  if (n > 0)
    luaV_concat(L, n);
  else {  
    setsvalue2s(L, L->top, luaS_newlstr(L, "", 0));  
    api_incr_top(L);
  }
  luaC_checkGC(L);
  lua_unlock(L);
}
LUA_API void lua_len (lua_State *L, int idx) {
  TValue *t;
  lua_lock(L);
  t = index2value(L, idx);
  luaV_objlen(L, L->top, t);
  api_incr_top(L);
  lua_unlock(L);
}
LUA_API lua_Alloc lua_getallocf (lua_State *L, void **ud) {
  lua_Alloc f;
  lua_lock(L);
  if (ud) *ud = G(L)->ud;
  f = G(L)->frealloc;
  lua_unlock(L);
  return f;
}
LUA_API void lua_setallocf (lua_State *L, lua_Alloc f, void *ud) {
  lua_lock(L);
  G(L)->ud = ud;
  G(L)->frealloc = f;
  lua_unlock(L);
}
void lua_setwarnf (lua_State *L, lua_WarnFunction f, void *ud) {
  lua_lock(L);
  G(L)->ud_warn = ud;
  G(L)->warnf = f;
  lua_unlock(L);
}
void lua_warning (lua_State *L, const char *msg, int tocont) {
  lua_lock(L);
  luaE_warning(L, msg, tocont);
  lua_unlock(L);
}
LUA_API void *lua_newuserdatauv (lua_State *L, size_t size, int nuvalue) {
  Udata *u;
  lua_lock(L);
  api_check(L, 0 <= nuvalue && nuvalue < USHRT_MAX, "invalid value");
  u = luaS_newudata(L, size, nuvalue);
  setuvalue(L, s2v(L->top), u);
  api_incr_top(L);
  luaC_checkGC(L);
  lua_unlock(L);
  return getudatamem(u);
}
static const char *aux_upvalue (TValue *fi, int n, TValue **val,
                                GCObject **owner) {
  switch (ttypetag(fi)) {
    case LUA_VCCL: {  
      CClosure *f = clCvalue(fi);
      if (!(cast_uint(n) - 1u < cast_uint(f->nupvalues)))
        return NULL;  
      *val = &f->upvalue[n-1];
      if (owner) *owner = obj2gco(f);
      return "";
    }
    case LUA_VLCL: {  
      LClosure *f = clLvalue(fi);
      TString *name;
      Proto *p = f->p;
      if (!(cast_uint(n) - 1u  < cast_uint(p->sizeupvalues)))
        return NULL;  
      *val = f->upvals[n-1]->v;
      if (owner) *owner = obj2gco(f->upvals[n - 1]);
      name = p->upvalues[n-1].name;
      return (name == NULL) ? "(no name)" : getstr(name);
    }
    default: return NULL;  
  }
}
LUA_API const char *lua_getupvalue (lua_State *L, int funcindex, int n) {
  const char *name;
  TValue *val = NULL;  
  lua_lock(L);
  name = aux_upvalue(index2value(L, funcindex), n, &val, NULL);
  if (name) {
    setobj2s(L, L->top, val);
    api_incr_top(L);
  }
  lua_unlock(L);
  return name;
}
LUA_API const char *lua_setupvalue (lua_State *L, int funcindex, int n) {
  const char *name;
  TValue *val = NULL;  
  GCObject *owner = NULL;  
  TValue *fi;
  lua_lock(L);
  fi = index2value(L, funcindex);
  api_checknelems(L, 1);
  name = aux_upvalue(fi, n, &val, &owner);
  if (name) {
    L->top--;
    setobj(L, val, s2v(L->top));
    luaC_barrier(L, owner, val);
  }
  lua_unlock(L);
  return name;
}
static UpVal **getupvalref (lua_State *L, int fidx, int n, LClosure **pf) {
  static const UpVal *const nullup = NULL;
  LClosure *f;
  TValue *fi = index2value(L, fidx);
  api_check(L, ttisLclosure(fi), "Lua function expected");
  f = clLvalue(fi);
  if (pf) *pf = f;
  if (1 <= n && n <= f->p->sizeupvalues)
    return &f->upvals[n - 1];  
  else
    return (UpVal**)&nullup;
}
LUA_API void *lua_upvalueid (lua_State *L, int fidx, int n) {
  TValue *fi = index2value(L, fidx);
  switch (ttypetag(fi)) {
    case LUA_VLCL: {  
      return *getupvalref(L, fidx, n, NULL);
    }
    case LUA_VCCL: {  
      CClosure *f = clCvalue(fi);
      if (1 <= n && n <= f->nupvalues)
        return &f->upvalue[n - 1];
      return NULL;
    }
    case LUA_VLCF:
      return NULL;  
    default: {
      api_check(L, 0, "function expected");
      return NULL;
    }
  }
}
LUA_API void lua_upvaluejoin (lua_State *L, int fidx1, int n1,
                                            int fidx2, int n2) {
  LClosure *f1;
  UpVal **up1 = getupvalref(L, fidx1, n1, &f1);
  UpVal **up2 = getupvalref(L, fidx2, n2, NULL);
  api_check(L, *up1 != NULL && *up2 != NULL, "invalid upvalue index");
  *up1 = *up2;
  luaC_objbarrier(L, f1, *up1);
}
// root include lcode.c
#include <float.h>
//included "limits.h" 
#include <math.h>
#include <stdlib.h>
#ifndef lcode_h
#ifndef llex_h
#define FIRST_RESERVED	(UCHAR_MAX + 1)
#if !defined(LUA_ENV)
#define LUA_ENV		"_ENV"
#endif
enum RESERVED {
  
  TK_AND = FIRST_RESERVED, TK_BREAK,
  TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FALSE, TK_FOR, TK_FUNCTION,
  TK_GOTO, TK_IF, TK_IN, TK_LOCAL, TK_NIL, TK_NOT, TK_OR, TK_REPEAT,
  TK_RETURN, TK_THEN, TK_TRUE, TK_UNTIL, TK_WHILE,
  
  TK_IDIV, TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE,
  TK_SHL, TK_SHR,
  TK_DBCOLON, TK_EOS,
  TK_FLT, TK_INT, TK_NAME, TK_STRING
};
#define NUM_RESERVED	(cast_int(TK_WHILE-FIRST_RESERVED + 1))
typedef union {
  lua_Number r;
  lua_Integer i;
  TString *ts;
} SemInfo;  
typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;
typedef struct LexState {
  int current;  
  int linenumber;  
  int lastline;  
  Token t;  
  Token lookahead;  
  struct FuncState *fs;  
  struct lua_State *L;
  ZIO *z;  
  Mbuffer *buff;  
  Table *h;  
  struct Dyndata *dyd;  
  TString *source;  
  TString *envn;  
} LexState;
LUAI_FUNC void luaX_init (lua_State *L);
LUAI_FUNC void luaX_setinput (lua_State *L, LexState *ls, ZIO *z,
                              TString *source, int firstchar);
LUAI_FUNC TString *luaX_newstring (LexState *ls, const char *str, size_t l);
LUAI_FUNC void luaX_next (LexState *ls);
LUAI_FUNC int luaX_lookahead (LexState *ls);
LUAI_FUNC l_noret luaX_syntaxerror (LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str (LexState *ls, int token);
#endif
#ifndef lopcodes_h
enum OpMode {iABC, iABx, iAsBx, iAx, isJ};  
#define SIZE_C		8
#define SIZE_B		8
#define SIZE_Bx		(SIZE_C + SIZE_B + 1)
#define SIZE_A		8
#define SIZE_Ax		(SIZE_Bx + SIZE_A)
#define SIZE_sJ		(SIZE_Bx + SIZE_A)
#define SIZE_OP		7
#define POS_OP		0
#define POS_A		(POS_OP + SIZE_OP)
#define POS_k		(POS_A + SIZE_A)
#define POS_B		(POS_k + 1)
#define POS_C		(POS_B + SIZE_B)
#define POS_Bx		POS_k
#define POS_Ax		POS_A
#define POS_sJ		POS_A
#define L_INTHASBITS(b)		((UINT_MAX >> ((b) - 1)) >= 1)
#if L_INTHASBITS(SIZE_Bx)
#define MAXARG_Bx	((1<<SIZE_Bx)-1)
#else
#define MAXARG_Bx	MAX_INT
#endif
#define OFFSET_sBx	(MAXARG_Bx>>1)         
#if L_INTHASBITS(SIZE_Ax)
#define MAXARG_Ax	((1<<SIZE_Ax)-1)
#else
#define MAXARG_Ax	MAX_INT
#endif
#if L_INTHASBITS(SIZE_sJ)
#define MAXARG_sJ	((1 << SIZE_sJ) - 1)
#else
#define MAXARG_sJ	MAX_INT
#endif
#define OFFSET_sJ	(MAXARG_sJ >> 1)
#define MAXARG_A	((1<<SIZE_A)-1)
#define MAXARG_B	((1<<SIZE_B)-1)
#define MAXARG_C	((1<<SIZE_C)-1)
#define OFFSET_sC	(MAXARG_C >> 1)
#define int2sC(i)	((i) + OFFSET_sC)
#define sC2int(i)	((i) - OFFSET_sC)
#define MASK1(n,p)	((~((~(Instruction)0)<<(n)))<<(p))
#define MASK0(n,p)	(~MASK1(n,p))
#define GET_OPCODE(i)	(cast(OpCode, ((i)>>POS_OP) & MASK1(SIZE_OP,0)))
#define SET_OPCODE(i,o)	((i) = (((i)&MASK0(SIZE_OP,POS_OP)) | 		((cast(Instruction, o)<<POS_OP)&MASK1(SIZE_OP,POS_OP))))
#define checkopm(i,m)	(getOpMode(GET_OPCODE(i)) == m)
#define getarg(i,pos,size)	(cast_int(((i)>>(pos)) & MASK1(size,0)))
#define setarg(i,v,pos,size)	((i) = (((i)&MASK0(size,pos)) |                 ((cast(Instruction, v)<<pos)&MASK1(size,pos))))
#define GETARG_A(i)	getarg(i, POS_A, SIZE_A)
#define SETARG_A(i,v)	setarg(i, v, POS_A, SIZE_A)
#define GETARG_B(i)	check_exp(checkopm(i, iABC), getarg(i, POS_B, SIZE_B))
#define GETARG_sB(i)	sC2int(GETARG_B(i))
#define SETARG_B(i,v)	setarg(i, v, POS_B, SIZE_B)
#define GETARG_C(i)	check_exp(checkopm(i, iABC), getarg(i, POS_C, SIZE_C))
#define GETARG_sC(i)	sC2int(GETARG_C(i))
#define SETARG_C(i,v)	setarg(i, v, POS_C, SIZE_C)
#define TESTARG_k(i)	check_exp(checkopm(i, iABC), (cast_int(((i) & (1u << POS_k)))))
#define GETARG_k(i)	check_exp(checkopm(i, iABC), getarg(i, POS_k, 1))
#define SETARG_k(i,v)	setarg(i, v, POS_k, 1)
#define GETARG_Bx(i)	check_exp(checkopm(i, iABx), getarg(i, POS_Bx, SIZE_Bx))
#define SETARG_Bx(i,v)	setarg(i, v, POS_Bx, SIZE_Bx)
#define GETARG_Ax(i)	check_exp(checkopm(i, iAx), getarg(i, POS_Ax, SIZE_Ax))
#define SETARG_Ax(i,v)	setarg(i, v, POS_Ax, SIZE_Ax)
#define GETARG_sBx(i)  	check_exp(checkopm(i, iAsBx), getarg(i, POS_Bx, SIZE_Bx) - OFFSET_sBx)
#define SETARG_sBx(i,b)	SETARG_Bx((i),cast_uint((b)+OFFSET_sBx))
#define GETARG_sJ(i)  	check_exp(checkopm(i, isJ), getarg(i, POS_sJ, SIZE_sJ) - OFFSET_sJ)
#define SETARG_sJ(i,j) 	setarg(i, cast_uint((j)+OFFSET_sJ), POS_sJ, SIZE_sJ)
#define CREATE_ABCk(o,a,b,c,k)	((cast(Instruction, o)<<POS_OP) 			| (cast(Instruction, a)<<POS_A) 			| (cast(Instruction, b)<<POS_B) 			| (cast(Instruction, c)<<POS_C) 			| (cast(Instruction, k)<<POS_k))
#define CREATE_ABx(o,a,bc)	((cast(Instruction, o)<<POS_OP) 			| (cast(Instruction, a)<<POS_A) 			| (cast(Instruction, bc)<<POS_Bx))
#define CREATE_Ax(o,a)		((cast(Instruction, o)<<POS_OP) 			| (cast(Instruction, a)<<POS_Ax))
#define CREATE_sJ(o,j,k)	((cast(Instruction, o) << POS_OP) 			| (cast(Instruction, j) << POS_sJ) 			| (cast(Instruction, k) << POS_k))
#if !defined(MAXINDEXRK)  
#define MAXINDEXRK	MAXARG_B
#endif
#define NO_REG		MAXARG_A
typedef enum {
OP_MOVE,
OP_LOADI,
OP_LOADF,
OP_LOADK,
OP_LOADKX,
OP_LOADFALSE,
OP_LFALSESKIP,
OP_LOADTRUE,
OP_LOADNIL,
OP_GETUPVAL,
OP_SETUPVAL,
OP_GETTABUP,
OP_GETTABLE,
OP_GETI,
OP_GETFIELD,
OP_SETTABUP,
OP_SETTABLE,
OP_SETI,
OP_SETFIELD,
OP_NEWTABLE,
OP_SELF,
OP_ADDI,
OP_ADDK,
OP_SUBK,
OP_MULK,
OP_MODK,
OP_POWK,
OP_DIVK,
OP_IDIVK,
OP_BANDK,
OP_BORK,
OP_BXORK,
OP_SHRI,
OP_SHLI,
OP_ADD,
OP_SUB,
OP_MUL,
OP_MOD,
OP_POW,
OP_DIV,
OP_IDIV,
OP_BAND,
OP_BOR,
OP_BXOR,
OP_SHL,
OP_SHR,
OP_MMBIN,
OP_MMBINI,
OP_MMBINK,
OP_UNM,
OP_BNOT,
OP_NOT,
OP_LEN,
OP_CONCAT,
OP_CLOSE,
OP_TBC,
OP_JMP,
OP_EQ,
OP_LT,
OP_LE,
OP_EQK,
OP_EQI,
OP_LTI,
OP_LEI,
OP_GTI,
OP_GEI,
OP_TEST,
OP_TESTSET,
OP_CALL,
OP_TAILCALL,
OP_RETURN,
OP_RETURN0,
OP_RETURN1,
OP_FORLOOP,
OP_FORPREP,
OP_TFORPREP,
OP_TFORCALL,
OP_TFORLOOP,
OP_SETLIST,
OP_CLOSURE,
OP_VARARG,
OP_VARARGPREP,
OP_EXTRAARG
} OpCode;
#define NUM_OPCODES	((int)(OP_EXTRAARG) + 1)
#define getOpMode(m)	(cast(enum OpMode, luaP_opmodes[m] & 7))
#define testAMode(m)	(luaP_opmodes[m] & (1 << 3))
#define testTMode(m)	(luaP_opmodes[m] & (1 << 4))
#define testITMode(m)	(luaP_opmodes[m] & (1 << 5))
#define testOTMode(m)	(luaP_opmodes[m] & (1 << 6))
#define testMMMode(m)	(luaP_opmodes[m] & (1 << 7))
#define isOT(i)  	((testOTMode(GET_OPCODE(i)) && GETARG_C(i) == 0) ||           GET_OPCODE(i) == OP_TAILCALL)
#define isIT(i)		(testITMode(GET_OPCODE(i)) && GETARG_B(i) == 0)
#define opmode(mm,ot,it,t,a,m)      (((mm) << 7) | ((ot) << 6) | ((it) << 5) | ((t) << 4) | ((a) << 3) | (m))
#define LFIELDS_PER_FLUSH	50
static const lu_byte luaP_opmodes[NUM_OPCODES] = {
  opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iAsBx)		
 ,opmode(0, 0, 0, 0, 1, iAsBx)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(1, 0, 0, 0, 0, iABC)		
 ,opmode(1, 0, 0, 0, 0, iABC)		
 ,opmode(1, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, isJ)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 0, iABC)		
 ,opmode(0, 0, 0, 1, 1, iABC)		
 ,opmode(0, 1, 1, 0, 1, iABC)		
 ,opmode(0, 1, 1, 0, 1, iABC)		
 ,opmode(0, 0, 1, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 0, 0, 0, iABx)		
 ,opmode(0, 0, 0, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 0, 1, 0, 0, iABC)		
 ,opmode(0, 0, 0, 0, 1, iABx)		
 ,opmode(0, 1, 0, 0, 1, iABC)		
 ,opmode(0, 0, 1, 0, 1, iABC)		
 ,opmode(0, 0, 0, 0, 0, iAx)		
};
#endif
#ifndef lparser_h
typedef enum {
  VVOID,  
  VNIL,  
  VTRUE,  
  VFALSE,  
  VK,  
  VKFLT,  
  VKINT,  
  VKSTR,  
  VNONRELOC,  
  VLOCAL,  
  VUPVAL,  
  VCONST,  
  VINDEXED,  
  VINDEXUP,  
  VINDEXI, 
  VINDEXSTR, 
  VJMP,  
  VRELOC,  
  VCALL,  
  VVARARG  
} expkind;
#define vkisvar(k)	(VLOCAL <= (k) && (k) <= VINDEXSTR)
#define vkisindexed(k)	(VINDEXED <= (k) && (k) <= VINDEXSTR)
typedef struct expdesc {
  expkind k;
  union {
    lua_Integer ival;    
    lua_Number nval;  
    TString *strval;  
    int info;  
    struct {  
      short idx;  
      lu_byte t;  
    } ind;
    struct {  
      lu_byte ridx;  
      unsigned short vidx;  
    } var;
  } u;
  int t;  
  int f;  
} expdesc;
#define VDKREG		0   
#define RDKCONST	1   
#define RDKTOCLOSE	2   
#define RDKCTC		3   
typedef union Vardesc {
  struct {
    TValuefields;  
    lu_byte kind;
    lu_byte ridx;  
    short pidx;  
    TString *name;  
  } vd;
  TValue k;  
} Vardesc;
typedef struct Labeldesc {
  TString *name;  
  int pc;  
  int line;  
  lu_byte nactvar;  
  lu_byte close;  
} Labeldesc;
typedef struct Labellist {
  Labeldesc *arr;  
  int n;  
  int size;  
} Labellist;
typedef struct Dyndata {
  struct {  
    Vardesc *arr;
    int n;
    int size;
  } actvar;
  Labellist gt;  
  Labellist label;   
} Dyndata;
struct BlockCnt;  
typedef struct FuncState {
  Proto *f;  
  struct FuncState *prev;  
  struct LexState *ls;  
  struct BlockCnt *bl;  
  int pc;  
  int lasttarget;   
  int previousline;  
  int nk;  
  int np;  
  int nabslineinfo;  
  int firstlocal;  
  int firstlabel;  
  short ndebugvars;  
  lu_byte nactvar;  
  lu_byte nups;  
  lu_byte freereg;  
  lu_byte iwthabs;  
  lu_byte needclose;  
} FuncState;
LUAI_FUNC int luaY_nvarstack (FuncState *fs);
LUAI_FUNC LClosure *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff,
                                 Dyndata *dyd, const char *name, int firstchar);
#endif
#define NO_JUMP (-1)
typedef enum BinOpr {
  
  OPR_ADD, OPR_SUB, OPR_MUL, OPR_MOD, OPR_POW,
  OPR_DIV, OPR_IDIV,
  
  OPR_BAND, OPR_BOR, OPR_BXOR,
  OPR_SHL, OPR_SHR,
  
  OPR_CONCAT,
  
  OPR_EQ, OPR_LT, OPR_LE,
  OPR_NE, OPR_GT, OPR_GE,
  
  OPR_AND, OPR_OR,
  OPR_NOBINOPR
} BinOpr;
#define foldbinop(op)	((op) <= OPR_SHR)
#define luaK_codeABC(fs,o,a,b,c)	luaK_codeABCk(fs,o,a,b,c,0)
typedef enum UnOpr { OPR_MINUS, OPR_BNOT, OPR_NOT, OPR_LEN, OPR_NOUNOPR } UnOpr;
#define getinstruction(fs,e)	((fs)->f->code[(e)->u.info])
#define luaK_setmultret(fs,e)	luaK_setreturns(fs, e, LUA_MULTRET)
#define luaK_jumpto(fs,t)	luaK_patchlist(fs, luaK_jump(fs), t)
LUAI_FUNC int luaK_code (FuncState *fs, Instruction i);
LUAI_FUNC int luaK_codeABx (FuncState *fs, OpCode o, int A, unsigned int Bx);
LUAI_FUNC int luaK_codeAsBx (FuncState *fs, OpCode o, int A, int Bx);
LUAI_FUNC int luaK_codeABCk (FuncState *fs, OpCode o, int A,
                                            int B, int C, int k);
LUAI_FUNC int luaK_isKint (expdesc *e);
LUAI_FUNC int luaK_exp2const (FuncState *fs, const expdesc *e, TValue *v);
LUAI_FUNC void luaK_fixline (FuncState *fs, int line);
LUAI_FUNC void luaK_nil (FuncState *fs, int from, int n);
LUAI_FUNC void luaK_reserveregs (FuncState *fs, int n);
LUAI_FUNC void luaK_checkstack (FuncState *fs, int n);
LUAI_FUNC void luaK_int (FuncState *fs, int reg, lua_Integer n);
LUAI_FUNC void luaK_dischargevars (FuncState *fs, expdesc *e);
LUAI_FUNC int luaK_exp2anyreg (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_exp2anyregup (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_exp2nextreg (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_exp2val (FuncState *fs, expdesc *e);
LUAI_FUNC int luaK_exp2RK (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_self (FuncState *fs, expdesc *e, expdesc *key);
LUAI_FUNC void luaK_indexed (FuncState *fs, expdesc *t, expdesc *k);
LUAI_FUNC void luaK_goiftrue (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_goiffalse (FuncState *fs, expdesc *e);
LUAI_FUNC void luaK_storevar (FuncState *fs, expdesc *var, expdesc *e);
LUAI_FUNC void luaK_setreturns (FuncState *fs, expdesc *e, int nresults);
LUAI_FUNC void luaK_setoneret (FuncState *fs, expdesc *e);
LUAI_FUNC int luaK_jump (FuncState *fs);
LUAI_FUNC void luaK_ret (FuncState *fs, int first, int nret);
LUAI_FUNC void luaK_patchlist (FuncState *fs, int list, int target);
LUAI_FUNC void luaK_patchtohere (FuncState *fs, int list);
LUAI_FUNC void luaK_concat (FuncState *fs, int *l1, int l2);
LUAI_FUNC int luaK_getlabel (FuncState *fs);
LUAI_FUNC void luaK_prefix (FuncState *fs, UnOpr op, expdesc *v, int line);
LUAI_FUNC void luaK_infix (FuncState *fs, BinOpr op, expdesc *v);
LUAI_FUNC void luaK_posfix (FuncState *fs, BinOpr op, expdesc *v1,
                            expdesc *v2, int line);
LUAI_FUNC void luaK_settablesize (FuncState *fs, int pc,
                                  int ra, int asize, int hsize);
LUAI_FUNC void luaK_setlist (FuncState *fs, int base, int nelems, int tostore);
LUAI_FUNC void luaK_finish (FuncState *fs);
LUAI_FUNC l_noret luaK_semerror (LexState *ls, const char *msg);
#endif
//included "ldebug.h" 
//included "llex.h" 
//included "lopcodes.h" 
//included "lparser.h" 
//included "lstring.h" 
//included "ltable.h" 
//included "lvm.h" 
#define MAXREGS		255
#define hasjumps(e)	((e)->t != (e)->f)
static int codesJ (FuncState *fs, OpCode o, int sj, int k);
l_noret luaK_semerror (LexState *ls, const char *msg) {
  ls->t.token = 0;  
  luaX_syntaxerror(ls, msg);
}
static int tonumeral (const expdesc *e, TValue *v) {
  if (hasjumps(e))
    return 0;  
  switch (e->k) {
    case VKINT:
      if (v) setivalue(v, e->u.ival);
      return 1;
    case VKFLT:
      if (v) setfltvalue(v, e->u.nval);
      return 1;
    default: return 0;
  }
}
static TValue *const2val (FuncState *fs, const expdesc *e) {
  lua_assert(e->k == VCONST);
  return &fs->ls->dyd->actvar.arr[e->u.info].k;
}
int luaK_exp2const (FuncState *fs, const expdesc *e, TValue *v) {
  if (hasjumps(e))
    return 0;  
  switch (e->k) {
    case VFALSE:
      setbfvalue(v);
      return 1;
    case VTRUE:
      setbtvalue(v);
      return 1;
    case VNIL:
      setnilvalue(v);
      return 1;
    case VKSTR: {
      setsvalue(fs->ls->L, v, e->u.strval);
      return 1;
    }
    case VCONST: {
      setobj(fs->ls->L, v, const2val(fs, e));
      return 1;
    }
    default: return tonumeral(e, v);
  }
}
static Instruction *previousinstruction (FuncState *fs) {
  static const Instruction invalidinstruction = ~(Instruction)0;
  if (fs->pc > fs->lasttarget)
    return &fs->f->code[fs->pc - 1];  
  else
    return cast(Instruction*, &invalidinstruction);
}
void luaK_nil (FuncState *fs, int from, int n) {
  int l = from + n - 1;  
  Instruction *previous = previousinstruction(fs);
  if (GET_OPCODE(*previous) == OP_LOADNIL) {  
    int pfrom = GETARG_A(*previous);  
    int pl = pfrom + GETARG_B(*previous);
    if ((pfrom <= from && from <= pl + 1) ||
        (from <= pfrom && pfrom <= l + 1)) {  
      if (pfrom < from) from = pfrom;  
      if (pl > l) l = pl;  
      SETARG_A(*previous, from);
      SETARG_B(*previous, l - from);
      return;
    }  
  }
  luaK_codeABC(fs, OP_LOADNIL, from, n - 1, 0);  
}
static int getjump (FuncState *fs, int pc) {
  int offset = GETARG_sJ(fs->f->code[pc]);
  if (offset == NO_JUMP)  
    return NO_JUMP;  
  else
    return (pc+1)+offset;  
}
static void fixjump (FuncState *fs, int pc, int dest) {
  Instruction *jmp = &fs->f->code[pc];
  int offset = dest - (pc + 1);
  lua_assert(dest != NO_JUMP);
  if (!(-OFFSET_sJ <= offset && offset <= MAXARG_sJ - OFFSET_sJ))
    luaX_syntaxerror(fs->ls, "control structure too long");
  lua_assert(GET_OPCODE(*jmp) == OP_JMP);
  SETARG_sJ(*jmp, offset);
}
void luaK_concat (FuncState *fs, int *l1, int l2) {
  if (l2 == NO_JUMP) return;  
  else if (*l1 == NO_JUMP)  
    *l1 = l2;  
  else {
    int list = *l1;
    int next;
    while ((next = getjump(fs, list)) != NO_JUMP)  
      list = next;
    fixjump(fs, list, l2);  
  }
}
int luaK_jump (FuncState *fs) {
  return codesJ(fs, OP_JMP, NO_JUMP, 0);
}
void luaK_ret (FuncState *fs, int first, int nret) {
  OpCode op;
  switch (nret) {
    case 0: op = OP_RETURN0; break;
    case 1: op = OP_RETURN1; break;
    default: op = OP_RETURN; break;
  }
  luaK_codeABC(fs, op, first, nret + 1, 0);
}
static int condjump (FuncState *fs, OpCode op, int A, int B, int C, int k) {
  luaK_codeABCk(fs, op, A, B, C, k);
  return luaK_jump(fs);
}
int luaK_getlabel (FuncState *fs) {
  fs->lasttarget = fs->pc;
  return fs->pc;
}
static Instruction *getjumpcontrol (FuncState *fs, int pc) {
  Instruction *pi = &fs->f->code[pc];
  if (pc >= 1 && testTMode(GET_OPCODE(*(pi-1))))
    return pi-1;
  else
    return pi;
}
static int patchtestreg (FuncState *fs, int node, int reg) {
  Instruction *i = getjumpcontrol(fs, node);
  if (GET_OPCODE(*i) != OP_TESTSET)
    return 0;  
  if (reg != NO_REG && reg != GETARG_B(*i))
    SETARG_A(*i, reg);
  else {
     
    *i = CREATE_ABCk(OP_TEST, GETARG_B(*i), 0, 0, GETARG_k(*i));
  }
  return 1;
}
static void removevalues (FuncState *fs, int list) {
  for (; list != NO_JUMP; list = getjump(fs, list))
      patchtestreg(fs, list, NO_REG);
}
static void patchlistaux (FuncState *fs, int list, int vtarget, int reg,
                          int dtarget) {
  while (list != NO_JUMP) {
    int next = getjump(fs, list);
    if (patchtestreg(fs, list, reg))
      fixjump(fs, list, vtarget);
    else
      fixjump(fs, list, dtarget);  
    list = next;
  }
}
void luaK_patchlist (FuncState *fs, int list, int target) {
  lua_assert(target <= fs->pc);
  patchlistaux(fs, list, target, NO_REG, target);
}
void luaK_patchtohere (FuncState *fs, int list) {
  int hr = luaK_getlabel(fs);  
  luaK_patchlist(fs, list, hr);
}
#define LIMLINEDIFF	0x80
static void savelineinfo (FuncState *fs, Proto *f, int line) {
  int linedif = line - fs->previousline;
  int pc = fs->pc - 1;  
  if (abs(linedif) >= LIMLINEDIFF || fs->iwthabs++ >= MAXIWTHABS) {
    luaM_growvector(fs->ls->L, f->abslineinfo, fs->nabslineinfo,
                    f->sizeabslineinfo, AbsLineInfo, MAX_INT, "lines");
    f->abslineinfo[fs->nabslineinfo].pc = pc;
    f->abslineinfo[fs->nabslineinfo++].line = line;
    linedif = ABSLINEINFO;  
    fs->iwthabs = 1;  
  }
  luaM_growvector(fs->ls->L, f->lineinfo, pc, f->sizelineinfo, ls_byte,
                  MAX_INT, "opcodes");
  f->lineinfo[pc] = linedif;
  fs->previousline = line;  
}
static void removelastlineinfo (FuncState *fs) {
  Proto *f = fs->f;
  int pc = fs->pc - 1;  
  if (f->lineinfo[pc] != ABSLINEINFO) {  
    fs->previousline -= f->lineinfo[pc];  
    fs->iwthabs--;  
  }
  else {  
    lua_assert(f->abslineinfo[fs->nabslineinfo - 1].pc == pc);
    fs->nabslineinfo--;  
    fs->iwthabs = MAXIWTHABS + 1;  
  }
}
static void removelastinstruction (FuncState *fs) {
  removelastlineinfo(fs);
  fs->pc--;
}
int luaK_code (FuncState *fs, Instruction i) {
  Proto *f = fs->f;
  
  luaM_growvector(fs->ls->L, f->code, fs->pc, f->sizecode, Instruction,
                  MAX_INT, "opcodes");
  f->code[fs->pc++] = i;
  savelineinfo(fs, f, fs->ls->lastline);
  return fs->pc - 1;  
}
int luaK_codeABCk (FuncState *fs, OpCode o, int a, int b, int c, int k) {
  lua_assert(getOpMode(o) == iABC);
  lua_assert(a <= MAXARG_A && b <= MAXARG_B &&
             c <= MAXARG_C && (k & ~1) == 0);
  return luaK_code(fs, CREATE_ABCk(o, a, b, c, k));
}
int luaK_codeABx (FuncState *fs, OpCode o, int a, unsigned int bc) {
  lua_assert(getOpMode(o) == iABx);
  lua_assert(a <= MAXARG_A && bc <= MAXARG_Bx);
  return luaK_code(fs, CREATE_ABx(o, a, bc));
}
int luaK_codeAsBx (FuncState *fs, OpCode o, int a, int bc) {
  unsigned int b = bc + OFFSET_sBx;
  lua_assert(getOpMode(o) == iAsBx);
  lua_assert(a <= MAXARG_A && b <= MAXARG_Bx);
  return luaK_code(fs, CREATE_ABx(o, a, b));
}
static int codesJ (FuncState *fs, OpCode o, int sj, int k) {
  unsigned int j = sj + OFFSET_sJ;
  lua_assert(getOpMode(o) == isJ);
  lua_assert(j <= MAXARG_sJ && (k & ~1) == 0);
  return luaK_code(fs, CREATE_sJ(o, j, k));
}
static int codeextraarg (FuncState *fs, int a) {
  lua_assert(a <= MAXARG_Ax);
  return luaK_code(fs, CREATE_Ax(OP_EXTRAARG, a));
}
static int luaK_codek (FuncState *fs, int reg, int k) {
  if (k <= MAXARG_Bx)
    return luaK_codeABx(fs, OP_LOADK, reg, k);
  else {
    int p = luaK_codeABx(fs, OP_LOADKX, reg, 0);
    codeextraarg(fs, k);
    return p;
  }
}
void luaK_checkstack (FuncState *fs, int n) {
  int newstack = fs->freereg + n;
  if (newstack > fs->f->maxstacksize) {
    if (newstack >= MAXREGS)
      luaX_syntaxerror(fs->ls,
        "function or expression needs too many registers");
    fs->f->maxstacksize = cast_byte(newstack);
  }
}
void luaK_reserveregs (FuncState *fs, int n) {
  luaK_checkstack(fs, n);
  fs->freereg += n;
}
static void freereg (FuncState *fs, int reg) {
  if (reg >= luaY_nvarstack(fs)) {
    fs->freereg--;
    lua_assert(reg == fs->freereg);
  }
}
static void freeregs (FuncState *fs, int r1, int r2) {
  if (r1 > r2) {
    freereg(fs, r1);
    freereg(fs, r2);
  }
  else {
    freereg(fs, r2);
    freereg(fs, r1);
  }
}
static void freeexp (FuncState *fs, expdesc *e) {
  if (e->k == VNONRELOC)
    freereg(fs, e->u.info);
}
static void freeexps (FuncState *fs, expdesc *e1, expdesc *e2) {
  int r1 = (e1->k == VNONRELOC) ? e1->u.info : -1;
  int r2 = (e2->k == VNONRELOC) ? e2->u.info : -1;
  freeregs(fs, r1, r2);
}
static int addk (FuncState *fs, TValue *key, TValue *v) {
  TValue val;
  lua_State *L = fs->ls->L;
  Proto *f = fs->f;
  const TValue *idx = luaH_get(fs->ls->h, key);  
  int k, oldsize;
  if (ttisinteger(idx)) {  
    k = cast_int(ivalue(idx));
    
    if (k < fs->nk && ttypetag(&f->k[k]) == ttypetag(v) &&
                      luaV_rawequalobj(&f->k[k], v))
      return k;  
  }
  
  oldsize = f->sizek;
  k = fs->nk;
  
  setivalue(&val, k);
  luaH_finishset(L, fs->ls->h, key, idx, &val);
  luaM_growvector(L, f->k, k, f->sizek, TValue, MAXARG_Ax, "constants");
  while (oldsize < f->sizek) setnilvalue(&f->k[oldsize++]);
  setobj(L, &f->k[k], v);
  fs->nk++;
  luaC_barrier(L, f, v);
  return k;
}
static int stringK (FuncState *fs, TString *s) {
  TValue o;
  setsvalue(fs->ls->L, &o, s);
  return addk(fs, &o, &o);  
}
static int luaK_intK (FuncState *fs, lua_Integer n) {
  TValue o;
  setivalue(&o, n);
  return addk(fs, &o, &o);  
}
static int luaK_numberK (FuncState *fs, lua_Number r) {
  TValue o;
  lua_Integer ik;
  setfltvalue(&o, r);
  if (!luaV_flttointeger(r, &ik, F2Ieq))  
    return addk(fs, &o, &o);  
  else {  
    const int nbm = l_floatatt(MANT_DIG);
    const lua_Number q = l_mathop(ldexp)(l_mathop(1.0), -nbm + 1);
    const lua_Number k = (ik == 0) ? q : r + r*q;  
    TValue kv;
    setfltvalue(&kv, k);
    
    lua_assert(!luaV_flttointeger(k, &ik, F2Ieq) ||
                l_mathop(fabs)(r) >= l_mathop(1e6));
    return addk(fs, &kv, &o);
  }
}
static int boolF (FuncState *fs) {
  TValue o;
  setbfvalue(&o);
  return addk(fs, &o, &o);  
}
static int boolT (FuncState *fs) {
  TValue o;
  setbtvalue(&o);
  return addk(fs, &o, &o);  
}
static int nilK (FuncState *fs) {
  TValue k, v;
  setnilvalue(&v);
  
  sethvalue(fs->ls->L, &k, fs->ls->h);
  return addk(fs, &k, &v);
}
static int fitsC (lua_Integer i) {
  return (l_castS2U(i) + OFFSET_sC <= cast_uint(MAXARG_C));
}
static int fitsBx (lua_Integer i) {
  return (-OFFSET_sBx <= i && i <= MAXARG_Bx - OFFSET_sBx);
}
void luaK_int (FuncState *fs, int reg, lua_Integer i) {
  if (fitsBx(i))
    luaK_codeAsBx(fs, OP_LOADI, reg, cast_int(i));
  else
    luaK_codek(fs, reg, luaK_intK(fs, i));
}
static void luaK_float (FuncState *fs, int reg, lua_Number f) {
  lua_Integer fi;
  if (luaV_flttointeger(f, &fi, F2Ieq) && fitsBx(fi))
    luaK_codeAsBx(fs, OP_LOADF, reg, cast_int(fi));
  else
    luaK_codek(fs, reg, luaK_numberK(fs, f));
}
static void const2exp (TValue *v, expdesc *e) {
  switch (ttypetag(v)) {
    case LUA_VNUMINT:
      e->k = VKINT; e->u.ival = ivalue(v);
      break;
    case LUA_VNUMFLT:
      e->k = VKFLT; e->u.nval = fltvalue(v);
      break;
    case LUA_VFALSE:
      e->k = VFALSE;
      break;
    case LUA_VTRUE:
      e->k = VTRUE;
      break;
    case LUA_VNIL:
      e->k = VNIL;
      break;
    case LUA_VSHRSTR:  case LUA_VLNGSTR:
      e->k = VKSTR; e->u.strval = tsvalue(v);
      break;
    default: luai_unreachable();
  }
}
void luaK_setreturns (FuncState *fs, expdesc *e, int nresults) {
  Instruction *pc = &getinstruction(fs, e);
  if (e->k == VCALL)  
    SETARG_C(*pc, nresults + 1);
  else {
    lua_assert(e->k == VVARARG);
    SETARG_C(*pc, nresults + 1);
    SETARG_A(*pc, fs->freereg);
    luaK_reserveregs(fs, 1);
  }
}
static void str2K (FuncState *fs, expdesc *e) {
  lua_assert(e->k == VKSTR);
  e->u.info = stringK(fs, e->u.strval);
  e->k = VK;
}
void luaK_setoneret (FuncState *fs, expdesc *e) {
  if (e->k == VCALL) {  
    
    lua_assert(GETARG_C(getinstruction(fs, e)) == 2);
    e->k = VNONRELOC;  
    e->u.info = GETARG_A(getinstruction(fs, e));
  }
  else if (e->k == VVARARG) {
    SETARG_C(getinstruction(fs, e), 2);
    e->k = VRELOC;  
  }
}
void luaK_dischargevars (FuncState *fs, expdesc *e) {
  switch (e->k) {
    case VCONST: {
      const2exp(const2val(fs, e), e);
      break;
    }
    case VLOCAL: {  
      e->u.info = e->u.var.ridx;
      e->k = VNONRELOC;  
      break;
    }
    case VUPVAL: {  
      e->u.info = luaK_codeABC(fs, OP_GETUPVAL, 0, e->u.info, 0);
      e->k = VRELOC;
      break;
    }
    case VINDEXUP: {
      e->u.info = luaK_codeABC(fs, OP_GETTABUP, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VINDEXI: {
      freereg(fs, e->u.ind.t);
      e->u.info = luaK_codeABC(fs, OP_GETI, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VINDEXSTR: {
      freereg(fs, e->u.ind.t);
      e->u.info = luaK_codeABC(fs, OP_GETFIELD, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VINDEXED: {
      freeregs(fs, e->u.ind.t, e->u.ind.idx);
      e->u.info = luaK_codeABC(fs, OP_GETTABLE, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOC;
      break;
    }
    case VVARARG: case VCALL: {
      luaK_setoneret(fs, e);
      break;
    }
    default: break;  
  }
}
static void discharge2reg (FuncState *fs, expdesc *e, int reg) {
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VNIL: {
      luaK_nil(fs, reg, 1);
      break;
    }
    case VFALSE: {
      luaK_codeABC(fs, OP_LOADFALSE, reg, 0, 0);
      break;
    }
    case VTRUE: {
      luaK_codeABC(fs, OP_LOADTRUE, reg, 0, 0);
      break;
    }
    case VKSTR: {
      str2K(fs, e);
			luaK_codek(fs, reg, e->u.info);
      break;
    }
    case VK: {
      luaK_codek(fs, reg, e->u.info);
      break;
    }
    case VKFLT: {
      luaK_float(fs, reg, e->u.nval);
      break;
    }
    case VKINT: {
      luaK_int(fs, reg, e->u.ival);
      break;
    }
    case VRELOC: {
      Instruction *pc = &getinstruction(fs, e);
      SETARG_A(*pc, reg);  
      break;
    }
    case VNONRELOC: {
      if (reg != e->u.info)
        luaK_codeABC(fs, OP_MOVE, reg, e->u.info, 0);
      break;
    }
    default: {
      lua_assert(e->k == VJMP);
      return;  
    }
  }
  e->u.info = reg;
  e->k = VNONRELOC;
}
static void discharge2anyreg (FuncState *fs, expdesc *e) {
  if (e->k != VNONRELOC) {  
    luaK_reserveregs(fs, 1);  
    discharge2reg(fs, e, fs->freereg-1);  
  }
}
static int code_loadbool (FuncState *fs, int A, OpCode op) {
  luaK_getlabel(fs);  
  return luaK_codeABC(fs, op, A, 0, 0);
}
static int need_value (FuncState *fs, int list) {
  for (; list != NO_JUMP; list = getjump(fs, list)) {
    Instruction i = *getjumpcontrol(fs, list);
    if (GET_OPCODE(i) != OP_TESTSET) return 1;
  }
  return 0;  
}
static void exp2reg (FuncState *fs, expdesc *e, int reg) {
  discharge2reg(fs, e, reg);
  if (e->k == VJMP)  
    luaK_concat(fs, &e->t, e->u.info);  
  if (hasjumps(e)) {
    int final;  
    int p_f = NO_JUMP;  
    int p_t = NO_JUMP;  
    if (need_value(fs, e->t) || need_value(fs, e->f)) {
      int fj = (e->k == VJMP) ? NO_JUMP : luaK_jump(fs);
      p_f = code_loadbool(fs, reg, OP_LFALSESKIP);  
      p_t = code_loadbool(fs, reg, OP_LOADTRUE);
      
      luaK_patchtohere(fs, fj);
    }
    final = luaK_getlabel(fs);
    patchlistaux(fs, e->f, final, reg, p_f);
    patchlistaux(fs, e->t, final, reg, p_t);
  }
  e->f = e->t = NO_JUMP;
  e->u.info = reg;
  e->k = VNONRELOC;
}
void luaK_exp2nextreg (FuncState *fs, expdesc *e) {
  luaK_dischargevars(fs, e);
  freeexp(fs, e);
  luaK_reserveregs(fs, 1);
  exp2reg(fs, e, fs->freereg - 1);
}
int luaK_exp2anyreg (FuncState *fs, expdesc *e) {
  luaK_dischargevars(fs, e);
  if (e->k == VNONRELOC) {  
    if (!hasjumps(e))  
      return e->u.info;  
    if (e->u.info >= luaY_nvarstack(fs)) {  
      exp2reg(fs, e, e->u.info);  
      return e->u.info;
    }
    
  }
  luaK_exp2nextreg(fs, e);  
  return e->u.info;
}
void luaK_exp2anyregup (FuncState *fs, expdesc *e) {
  if (e->k != VUPVAL || hasjumps(e))
    luaK_exp2anyreg(fs, e);
}
void luaK_exp2val (FuncState *fs, expdesc *e) {
  if (hasjumps(e))
    luaK_exp2anyreg(fs, e);
  else
    luaK_dischargevars(fs, e);
}
static int luaK_exp2K (FuncState *fs, expdesc *e) {
  if (!hasjumps(e)) {
    int info;
    switch (e->k) {  
      case VTRUE: info = boolT(fs); break;
      case VFALSE: info = boolF(fs); break;
      case VNIL: info = nilK(fs); break;
      case VKINT: info = luaK_intK(fs, e->u.ival); break;
      case VKFLT: info = luaK_numberK(fs, e->u.nval); break;
      case VKSTR: info = stringK(fs, e->u.strval); break;
      case VK: info = e->u.info; break;
      default: return 0;  
    }
    if (info <= MAXINDEXRK) {  
      e->k = VK;  
      e->u.info = info;
      return 1;
    }
  }
  
  return 0;
}
int luaK_exp2RK (FuncState *fs, expdesc *e) {
  if (luaK_exp2K(fs, e))
    return 1;
  else {  
    luaK_exp2anyreg(fs, e);
    return 0;
  }
}
static void codeABRK (FuncState *fs, OpCode o, int a, int b,
                      expdesc *ec) {
  int k = luaK_exp2RK(fs, ec);
  luaK_codeABCk(fs, o, a, b, ec->u.info, k);
}
void luaK_storevar (FuncState *fs, expdesc *var, expdesc *ex) {
  switch (var->k) {
    case VLOCAL: {
      freeexp(fs, ex);
      exp2reg(fs, ex, var->u.var.ridx);  
      return;
    }
    case VUPVAL: {
      int e = luaK_exp2anyreg(fs, ex);
      luaK_codeABC(fs, OP_SETUPVAL, e, var->u.info, 0);
      break;
    }
    case VINDEXUP: {
      codeABRK(fs, OP_SETTABUP, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    case VINDEXI: {
      codeABRK(fs, OP_SETI, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    case VINDEXSTR: {
      codeABRK(fs, OP_SETFIELD, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    case VINDEXED: {
      codeABRK(fs, OP_SETTABLE, var->u.ind.t, var->u.ind.idx, ex);
      break;
    }
    default: luai_unreachable();  
  }
  freeexp(fs, ex);
}
void luaK_self (FuncState *fs, expdesc *e, expdesc *key) {
  int ereg;
  luaK_exp2anyreg(fs, e);
  ereg = e->u.info;  
  freeexp(fs, e);
  e->u.info = fs->freereg;  
  e->k = VNONRELOC;  
  luaK_reserveregs(fs, 2);  
  codeABRK(fs, OP_SELF, e->u.info, ereg, key);
  freeexp(fs, key);
}
static void negatecondition (FuncState *fs, expdesc *e) {
  Instruction *pc = getjumpcontrol(fs, e->u.info);
  lua_assert(testTMode(GET_OPCODE(*pc)) && GET_OPCODE(*pc) != OP_TESTSET &&
                                           GET_OPCODE(*pc) != OP_TEST);
  SETARG_k(*pc, (GETARG_k(*pc) ^ 1));
}
static int jumponcond (FuncState *fs, expdesc *e, int cond) {
  if (e->k == VRELOC) {
    Instruction ie = getinstruction(fs, e);
    if (GET_OPCODE(ie) == OP_NOT) {
      removelastinstruction(fs);  
      return condjump(fs, OP_TEST, GETARG_B(ie), 0, 0, !cond);
    }
    
  }
  discharge2anyreg(fs, e);
  freeexp(fs, e);
  return condjump(fs, OP_TESTSET, NO_REG, e->u.info, 0, cond);
}
void luaK_goiftrue (FuncState *fs, expdesc *e) {
  int pc;  
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VJMP: {  
      negatecondition(fs, e);  
      pc = e->u.info;  
      break;
    }
    case VK: case VKFLT: case VKINT: case VKSTR: case VTRUE: {
      pc = NO_JUMP;  
      break;
    }
    default: {
      pc = jumponcond(fs, e, 0);  
      break;
    }
  }
  luaK_concat(fs, &e->f, pc);  
  luaK_patchtohere(fs, e->t);  
  e->t = NO_JUMP;
}
void luaK_goiffalse (FuncState *fs, expdesc *e) {
  int pc;  
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VJMP: {
      pc = e->u.info;  
      break;
    }
    case VNIL: case VFALSE: {
      pc = NO_JUMP;  
      break;
    }
    default: {
      pc = jumponcond(fs, e, 1);  
      break;
    }
  }
  luaK_concat(fs, &e->t, pc);  
  luaK_patchtohere(fs, e->f);  
  e->f = NO_JUMP;
}
static void codenot (FuncState *fs, expdesc *e) {
  switch (e->k) {
    case VNIL: case VFALSE: {
      e->k = VTRUE;  
      break;
    }
    case VK: case VKFLT: case VKINT: case VKSTR: case VTRUE: {
      e->k = VFALSE;  
      break;
    }
    case VJMP: {
      negatecondition(fs, e);
      break;
    }
    case VRELOC:
    case VNONRELOC: {
      discharge2anyreg(fs, e);
      freeexp(fs, e);
      e->u.info = luaK_codeABC(fs, OP_NOT, 0, e->u.info, 0);
      e->k = VRELOC;
      break;
    }
    default: luai_unreachable();  
  }
  
  { int temp = e->f; e->f = e->t; e->t = temp; }
  removevalues(fs, e->f);  
  removevalues(fs, e->t);
}
static int isKstr (FuncState *fs, expdesc *e) {
  return (e->k == VK && !hasjumps(e) && e->u.info <= MAXARG_B &&
          ttisshrstring(&fs->f->k[e->u.info]));
}
int luaK_isKint (expdesc *e) {
  return (e->k == VKINT && !hasjumps(e));
}
static int isCint (expdesc *e) {
  return luaK_isKint(e) && (l_castS2U(e->u.ival) <= l_castS2U(MAXARG_C));
}
static int isSCint (expdesc *e) {
  return luaK_isKint(e) && fitsC(e->u.ival);
}
static int isSCnumber (expdesc *e, int *pi, int *isfloat) {
  lua_Integer i;
  if (e->k == VKINT)
    i = e->u.ival;
  else if (e->k == VKFLT && luaV_flttointeger(e->u.nval, &i, F2Ieq))
    *isfloat = 1;
  else
    return 0;  
  if (!hasjumps(e) && fitsC(i)) {
    *pi = int2sC(cast_int(i));
    return 1;
  }
  else
    return 0;
}
void luaK_indexed (FuncState *fs, expdesc *t, expdesc *k) {
  if (k->k == VKSTR)
    str2K(fs, k);
  lua_assert(!hasjumps(t) &&
             (t->k == VLOCAL || t->k == VNONRELOC || t->k == VUPVAL));
  if (t->k == VUPVAL && !isKstr(fs, k))  
    luaK_exp2anyreg(fs, t);  
  if (t->k == VUPVAL) {
    t->u.ind.t = t->u.info;  
    t->u.ind.idx = k->u.info;  
    t->k = VINDEXUP;
  }
  else {
    
    t->u.ind.t = (t->k == VLOCAL) ? t->u.var.ridx: t->u.info;
    if (isKstr(fs, k)) {
      t->u.ind.idx = k->u.info;  
      t->k = VINDEXSTR;
    }
    else if (isCint(k)) {
      t->u.ind.idx = cast_int(k->u.ival);  
      t->k = VINDEXI;
    }
    else {
      t->u.ind.idx = luaK_exp2anyreg(fs, k);  
      t->k = VINDEXED;
    }
  }
}
static int validop (int op, TValue *v1, TValue *v2) {
  switch (op) {
    case LUA_OPBAND: case LUA_OPBOR: case LUA_OPBXOR:
    case LUA_OPSHL: case LUA_OPSHR: case LUA_OPBNOT: {  
      lua_Integer i;
      return (luaV_tointegerns(v1, &i, LUA_FLOORN2I) &&
              luaV_tointegerns(v2, &i, LUA_FLOORN2I));
    }
    case LUA_OPDIV: case LUA_OPIDIV: case LUA_OPMOD:  
      return (nvalue(v2) != 0);
    default: return 1;  
  }
}
static int constfolding (FuncState *fs, int op, expdesc *e1,
                                        const expdesc *e2) {
  TValue v1, v2, res;
  if (!tonumeral(e1, &v1) || !tonumeral(e2, &v2) || !validop(op, &v1, &v2))
    return 0;  
  luaO_rawarith(fs->ls->L, op, &v1, &v2, &res);  
  if (ttisinteger(&res)) {
    e1->k = VKINT;
    e1->u.ival = ivalue(&res);
  }
  else {  
    lua_Number n = fltvalue(&res);
    if (luai_numisnan(n) || n == 0)
      return 0;
    e1->k = VKFLT;
    e1->u.nval = n;
  }
  return 1;
}
static void codeunexpval (FuncState *fs, OpCode op, expdesc *e, int line) {
  int r = luaK_exp2anyreg(fs, e);  
  freeexp(fs, e);
  e->u.info = luaK_codeABC(fs, op, 0, r, 0);  
  e->k = VRELOC;  
  luaK_fixline(fs, line);
}
static void finishbinexpval (FuncState *fs, expdesc *e1, expdesc *e2,
                             OpCode op, int v2, int flip, int line,
                             OpCode mmop, TMS event) {
  int v1 = luaK_exp2anyreg(fs, e1);
  int pc = luaK_codeABCk(fs, op, 0, v1, v2, 0);
  freeexps(fs, e1, e2);
  e1->u.info = pc;
  e1->k = VRELOC;  
  luaK_fixline(fs, line);
  luaK_codeABCk(fs, mmop, v1, v2, event, flip);  
  luaK_fixline(fs, line);
}
static void codebinexpval (FuncState *fs, OpCode op,
                           expdesc *e1, expdesc *e2, int line) {
  int v2 = luaK_exp2anyreg(fs, e2);  
  lua_assert(OP_ADD <= op && op <= OP_SHR);
  finishbinexpval(fs, e1, e2, op, v2, 0, line, OP_MMBIN,
                  cast(TMS, (op - OP_ADD) + TM_ADD));
}
static void codebini (FuncState *fs, OpCode op,
                       expdesc *e1, expdesc *e2, int flip, int line,
                       TMS event) {
  int v2 = int2sC(cast_int(e2->u.ival));  
  lua_assert(e2->k == VKINT);
  finishbinexpval(fs, e1, e2, op, v2, flip, line, OP_MMBINI, event);
}
static int finishbinexpneg (FuncState *fs, expdesc *e1, expdesc *e2,
                             OpCode op, int line, TMS event) {
  if (!luaK_isKint(e2))
    return 0;  
  else {
    lua_Integer i2 = e2->u.ival;
    if (!(fitsC(i2) && fitsC(-i2)))
      return 0;  
    else {  
      int v2 = cast_int(i2);
      finishbinexpval(fs, e1, e2, op, int2sC(-v2), 0, line, OP_MMBINI, event);
      
      SETARG_B(fs->f->code[fs->pc - 1], int2sC(v2));
      return 1;  
    }
  }
}
static void swapexps (expdesc *e1, expdesc *e2) {
  expdesc temp = *e1; *e1 = *e2; *e2 = temp;  
}
static void codearith (FuncState *fs, BinOpr opr,
                       expdesc *e1, expdesc *e2, int flip, int line) {
  TMS event = cast(TMS, opr + TM_ADD);
  if (tonumeral(e2, NULL) && luaK_exp2K(fs, e2)) {  
    int v2 = e2->u.info;  
    OpCode op = cast(OpCode, opr + OP_ADDK);
    finishbinexpval(fs, e1, e2, op, v2, flip, line, OP_MMBINK, event);
  }
  else {  
    OpCode op = cast(OpCode, opr + OP_ADD);
    if (flip)
      swapexps(e1, e2);  
    codebinexpval(fs, op, e1, e2, line);  
  }
}
static void codecommutative (FuncState *fs, BinOpr op,
                             expdesc *e1, expdesc *e2, int line) {
  int flip = 0;
  if (tonumeral(e1, NULL)) {  
    swapexps(e1, e2);  
    flip = 1;
  }
  if (op == OPR_ADD && isSCint(e2))  
    codebini(fs, cast(OpCode, OP_ADDI), e1, e2, flip, line, TM_ADD);
  else
    codearith(fs, op, e1, e2, flip, line);
}
static void codebitwise (FuncState *fs, BinOpr opr,
                         expdesc *e1, expdesc *e2, int line) {
  int flip = 0;
  int v2;
  OpCode op;
  if (e1->k == VKINT && luaK_exp2RK(fs, e1)) {
    swapexps(e1, e2);  
    flip = 1;
  }
  else if (!(e2->k == VKINT && luaK_exp2RK(fs, e2))) {  
    op = cast(OpCode, opr + OP_ADD);
    codebinexpval(fs, op, e1, e2, line);  
    return;
  }
  v2 = e2->u.info;  
  op = cast(OpCode, opr + OP_ADDK);
  lua_assert(ttisinteger(&fs->f->k[v2]));
  finishbinexpval(fs, e1, e2, op, v2, flip, line, OP_MMBINK,
                  cast(TMS, opr + TM_ADD));
}
static void codeorder (FuncState *fs, OpCode op, expdesc *e1, expdesc *e2) {
  int r1, r2;
  int im;
  int isfloat = 0;
  if (isSCnumber(e2, &im, &isfloat)) {
    
    r1 = luaK_exp2anyreg(fs, e1);
    r2 = im;
    op = cast(OpCode, (op - OP_LT) + OP_LTI);
  }
  else if (isSCnumber(e1, &im, &isfloat)) {
    
    r1 = luaK_exp2anyreg(fs, e2);
    r2 = im;
    op = (op == OP_LT) ? OP_GTI : OP_GEI;
  }
  else {  
    r1 = luaK_exp2anyreg(fs, e1);
    r2 = luaK_exp2anyreg(fs, e2);
  }
  freeexps(fs, e1, e2);
  e1->u.info = condjump(fs, op, r1, r2, isfloat, 1);
  e1->k = VJMP;
}
static void codeeq (FuncState *fs, BinOpr opr, expdesc *e1, expdesc *e2) {
  int r1, r2;
  int im;
  int isfloat = 0;  
  OpCode op;
  if (e1->k != VNONRELOC) {
    lua_assert(e1->k == VK || e1->k == VKINT || e1->k == VKFLT);
    swapexps(e1, e2);
  }
  r1 = luaK_exp2anyreg(fs, e1);  
  if (isSCnumber(e2, &im, &isfloat)) {
    op = OP_EQI;
    r2 = im;  
  }
  else if (luaK_exp2RK(fs, e2)) {  
    op = OP_EQK;
    r2 = e2->u.info;  
  }
  else {
    op = OP_EQ;  
    r2 = luaK_exp2anyreg(fs, e2);
  }
  freeexps(fs, e1, e2);
  e1->u.info = condjump(fs, op, r1, r2, isfloat, (opr == OPR_EQ));
  e1->k = VJMP;
}
void luaK_prefix (FuncState *fs, UnOpr op, expdesc *e, int line) {
  static const expdesc ef = {VKINT, {0}, NO_JUMP, NO_JUMP};
  luaK_dischargevars(fs, e);
  switch (op) {
    case OPR_MINUS: case OPR_BNOT:  
      if (constfolding(fs, op + LUA_OPUNM, e, &ef))
        break;
      codeunexpval(fs, cast(OpCode, op + OP_UNM), e, line);
      break;
    case OPR_LEN:
      codeunexpval(fs, cast(OpCode, op + OP_UNM), e, line);
      break;
    case OPR_NOT: codenot(fs, e); break;
    default: luai_unreachable();
  }
}
void luaK_infix (FuncState *fs, BinOpr op, expdesc *v) {
  luaK_dischargevars(fs, v);
  switch (op) {
    case OPR_AND: {
      luaK_goiftrue(fs, v);  
      break;
    }
    case OPR_OR: {
      luaK_goiffalse(fs, v);  
      break;
    }
    case OPR_CONCAT: {
      luaK_exp2nextreg(fs, v);  
      break;
    }
    case OPR_ADD: case OPR_SUB:
    case OPR_MUL: case OPR_DIV: case OPR_IDIV:
    case OPR_MOD: case OPR_POW:
    case OPR_BAND: case OPR_BOR: case OPR_BXOR:
    case OPR_SHL: case OPR_SHR: {
      if (!tonumeral(v, NULL))
        luaK_exp2anyreg(fs, v);
      
      break;
    }
    case OPR_EQ: case OPR_NE: {
      if (!tonumeral(v, NULL))
        luaK_exp2RK(fs, v);
      
      break;
    }
    case OPR_LT: case OPR_LE:
    case OPR_GT: case OPR_GE: {
      int dummy, dummy2;
      if (!isSCnumber(v, &dummy, &dummy2))
        luaK_exp2anyreg(fs, v);
      
      break;
    }
    default: luai_unreachable();
  }
}
static void codeconcat (FuncState *fs, expdesc *e1, expdesc *e2, int line) {
  Instruction *ie2 = previousinstruction(fs);
  if (GET_OPCODE(*ie2) == OP_CONCAT) {  
    int n = GETARG_B(*ie2);  
    lua_assert(e1->u.info + 1 == GETARG_A(*ie2));
    freeexp(fs, e2);
    SETARG_A(*ie2, e1->u.info);  
    SETARG_B(*ie2, n + 1);  
  }
  else {  
    luaK_codeABC(fs, OP_CONCAT, e1->u.info, 2, 0);  
    freeexp(fs, e2);
    luaK_fixline(fs, line);
  }
}
void luaK_posfix (FuncState *fs, BinOpr opr,
                  expdesc *e1, expdesc *e2, int line) {
  luaK_dischargevars(fs, e2);
  if (foldbinop(opr) && constfolding(fs, opr + LUA_OPADD, e1, e2))
    return;  
  switch (opr) {
    case OPR_AND: {
      lua_assert(e1->t == NO_JUMP);  
      luaK_concat(fs, &e2->f, e1->f);
      *e1 = *e2;
      break;
    }
    case OPR_OR: {
      lua_assert(e1->f == NO_JUMP);  
      luaK_concat(fs, &e2->t, e1->t);
      *e1 = *e2;
      break;
    }
    case OPR_CONCAT: {  
      luaK_exp2nextreg(fs, e2);
      codeconcat(fs, e1, e2, line);
      break;
    }
    case OPR_ADD: case OPR_MUL: {
      codecommutative(fs, opr, e1, e2, line);
      break;
    }
    case OPR_SUB: {
      if (finishbinexpneg(fs, e1, e2, OP_ADDI, line, TM_SUB))
        break; 
      
			goto lb_ft1;
    }  
    case OPR_DIV: case OPR_IDIV: case OPR_MOD: case OPR_POW:
		lb_ft1:	{
      codearith(fs, opr, e1, e2, 0, line);
      break;
    }
    case OPR_BAND: case OPR_BOR: case OPR_BXOR: {
      codebitwise(fs, opr, e1, e2, line);
      break;
    }
    case OPR_SHL: {
      if (isSCint(e1)) {
        swapexps(e1, e2);
        codebini(fs, OP_SHLI, e1, e2, 1, line, TM_SHL);  
      }
      else if (finishbinexpneg(fs, e1, e2, OP_SHRI, line, TM_SHL)) {
        ;
      }
      else  
       codebinexpval(fs, OP_SHL, e1, e2, line);
      break;
    }
    case OPR_SHR: {
      if (isSCint(e2))
        codebini(fs, OP_SHRI, e1, e2, 0, line, TM_SHR);  
      else  
        codebinexpval(fs, OP_SHR, e1, e2, line);
      break;
    }
    case OPR_EQ: case OPR_NE: {
      codeeq(fs, opr, e1, e2);
      break;
    }
    case OPR_LT: case OPR_LE: {
      OpCode op = cast(OpCode, (opr - OPR_EQ) + OP_EQ);
      codeorder(fs, op, e1, e2);
      break;
    }
    case OPR_GT: case OPR_GE: {
      
      OpCode op = cast(OpCode, (opr - OPR_NE) + OP_EQ);
      swapexps(e1, e2);
      codeorder(fs, op, e1, e2);
      break;
    }
    default: luai_unreachable();
  }
}
void luaK_fixline (FuncState *fs, int line) {
  removelastlineinfo(fs);
  savelineinfo(fs, fs->f, line);
}
void luaK_settablesize (FuncState *fs, int pc, int ra, int asize, int hsize) {
  Instruction *inst = &fs->f->code[pc];
  int rb = (hsize != 0) ? luaO_ceillog2(hsize) + 1 : 0;  
  int extra = asize / (MAXARG_C + 1);  
  int rc = asize % (MAXARG_C + 1);  
  int k = (extra > 0);  
  *inst = CREATE_ABCk(OP_NEWTABLE, ra, rb, rc, k);
  *(inst + 1) = CREATE_Ax(OP_EXTRAARG, extra);
}
void luaK_setlist (FuncState *fs, int base, int nelems, int tostore) {
  lua_assert(tostore != 0 && tostore <= LFIELDS_PER_FLUSH);
  if (tostore == LUA_MULTRET)
    tostore = 0;
  if (nelems <= MAXARG_C)
    luaK_codeABC(fs, OP_SETLIST, base, tostore, nelems);
  else {
    int extra = nelems / (MAXARG_C + 1);
    nelems %= (MAXARG_C + 1);
    luaK_codeABCk(fs, OP_SETLIST, base, tostore, nelems, 1);
    codeextraarg(fs, extra);
  }
  fs->freereg = base + 1;  
}
static int finaltarget (Instruction *code, int i) {
  int count;
  for (count = 0; count < 100; count++) {  
    Instruction pc = code[i];
    if (GET_OPCODE(pc) != OP_JMP)
      break;
     else
       i += GETARG_sJ(pc) + 1;
  }
  return i;
}
void luaK_finish (FuncState *fs) {
  int i;
  Proto *p = fs->f;
  for (i = 0; i < fs->pc; i++) {
    Instruction *pc = &p->code[i];
    lua_assert(i == 0 || isOT(*(pc - 1)) == isIT(*pc));
    switch (GET_OPCODE(*pc)) {
      case OP_RETURN0: case OP_RETURN1: {
        if (!(fs->needclose || p->is_vararg))
          break;  
        
        SET_OPCODE(*pc, OP_RETURN);
				goto ft_2;
      }  
			case OP_RETURN: case OP_TAILCALL: 
			ft_2: {
        if (fs->needclose)
          SETARG_k(*pc, 1);  
        if (p->is_vararg)
          SETARG_C(*pc, p->numparams + 1);  
        break;
      }
      case OP_JMP: {
        int target = finaltarget(p->code, i);
        fixjump(fs, i, target);
        break;
      }
      default: break;
    }
  }
}
// root include lctype.c
// root include ldebug.c
//included "stdarg.h" 
//included "stddef.h" 
//included "string.h" 
//included "lapi.h" 
//included "lcode.h" 
//included "lfunc.h" 
#define noLuaClosure(f)		((f) == NULL || (f)->c.tt == LUA_VCCL)
static const char *funcnamefromcall (lua_State *L, CallInfo *ci,
                                                   const char **name);
static int currentpc (CallInfo *ci) {
  lua_assert(isLua(ci));
  return pcRel(ci->u.l.savedpc, ci_func(ci)->p);
}
static int getbaseline (const Proto *f, int pc, int *basepc) {
  if (f->sizeabslineinfo == 0 || pc < f->abslineinfo[0].pc) {
    *basepc = -1;  
    return f->linedefined;
  }
  else {
    int i = cast_uint(pc) / MAXIWTHABS - 1;  
    
    lua_assert(i < 0 ||
              (i < f->sizeabslineinfo && f->abslineinfo[i].pc <= pc));
    while (i + 1 < f->sizeabslineinfo && pc >= f->abslineinfo[i + 1].pc)
      i++;  
    *basepc = f->abslineinfo[i].pc;
    return f->abslineinfo[i].line;
  }
}
int luaG_getfuncline (const Proto *f, int pc) {
  if (f->lineinfo == NULL)  
    return -1;
  else {
    int basepc;
    int baseline = getbaseline(f, pc, &basepc);
    while (basepc++ < pc) {  
      lua_assert(f->lineinfo[basepc] != ABSLINEINFO);
      baseline += f->lineinfo[basepc];  
    }
    return baseline;
  }
}
static int getcurrentline (CallInfo *ci) {
  return luaG_getfuncline(ci_func(ci)->p, currentpc(ci));
}
static void settraps (CallInfo *ci) {
  for (; ci != NULL; ci = ci->previous)
    if (isLua(ci))
      ci->u.l.trap = 1;
}
LUA_API void lua_sethook (lua_State *L, lua_Hook func, int mask, int count) {
  if (func == NULL || mask == 0) {  
    mask = 0;
    func = NULL;
  }
  L->hook = func;
  L->basehookcount = count;
  resethookcount(L);
  L->hookmask = cast_byte(mask);
  if (mask)
    settraps(L->ci);  
}
LUA_API lua_Hook lua_gethook (lua_State *L) {
  return L->hook;
}
LUA_API int lua_gethookmask (lua_State *L) {
  return L->hookmask;
}
LUA_API int lua_gethookcount (lua_State *L) {
  return L->basehookcount;
}
LUA_API int lua_getstack (lua_State *L, int level, lua_Debug *ar) {
  int status;
  CallInfo *ci;
  if (level < 0) return 0;  
  lua_lock(L);
  for (ci = L->ci; level > 0 && ci != &L->base_ci; ci = ci->previous)
    level--;
  if (level == 0 && ci != &L->base_ci) {  
    status = 1;
    ar->i_ci = ci;
  }
  else status = 0;  
  lua_unlock(L);
  return status;
}
static const char *upvalname (const Proto *p, int uv) {
  TString *s = check_exp(uv < p->sizeupvalues, p->upvalues[uv].name);
  if (s == NULL) return "?";
  else return getstr(s);
}
static const char *findvararg (CallInfo *ci, int n, StkId *pos) {
  if (clLvalue(s2v(ci->func))->p->is_vararg) {
    int nextra = ci->u.l.nextraargs;
    if (n >= -nextra) {  
      *pos = ci->func - nextra - (n + 1);
      return "(vararg)";  
    }
  }
  return NULL;  
}
const char *luaG_findlocal (lua_State *L, CallInfo *ci, int n, StkId *pos) {
  StkId base = ci->func + 1;
  const char *name = NULL;
  if (isLua(ci)) {
    if (n < 0)  
      return findvararg(ci, n, pos);
    else
      name = luaF_getlocalname(ci_func(ci)->p, n, currentpc(ci));
  }
  if (name == NULL) {  
    StkId limit = (ci == L->ci) ? L->top : ci->next->func;
    if (limit - base >= n && n > 0) {  
      
      name = isLua(ci) ? "(temporary)" : "(C temporary)";
    }
    else
      return NULL;  
  }
  if (pos)
    *pos = base + (n - 1);
  return name;
}
LUA_API const char *lua_getlocal (lua_State *L, const lua_Debug *ar, int n) {
  const char *name;
  lua_lock(L);
  if (ar == NULL) {  
    if (!isLfunction(s2v(L->top - 1)))  
      name = NULL;
    else  
      name = luaF_getlocalname(clLvalue(s2v(L->top - 1))->p, n, 0);
  }
  else {  
    StkId pos = NULL;  
    name = luaG_findlocal(L, ar->i_ci, n, &pos);
    if (name) {
      setobjs2s(L, L->top, pos);
      api_incr_top(L);
    }
  }
  lua_unlock(L);
  return name;
}
LUA_API const char *lua_setlocal (lua_State *L, const lua_Debug *ar, int n) {
  StkId pos = NULL;  
  const char *name;
  lua_lock(L);
  name = luaG_findlocal(L, ar->i_ci, n, &pos);
  if (name) {
    setobjs2s(L, pos, L->top - 1);
    L->top--;  
  }
  lua_unlock(L);
  return name;
}
static void funcinfo (lua_Debug *ar, Closure *cl) {
  if (noLuaClosure(cl)) {
    ar->source = "=[C]";
    ar->srclen = LL("=[C]");
    ar->linedefined = -1;
    ar->lastlinedefined = -1;
    ar->what = "C";
  }
  else {
    const Proto *p = cl->l.p;
    if (p->source) {
      ar->source = getstr(p->source);
      ar->srclen = tsslen(p->source);
    }
    else {
      ar->source = "=?";
      ar->srclen = LL("=?");
    }
    ar->linedefined = p->linedefined;
    ar->lastlinedefined = p->lastlinedefined;
    ar->what = (ar->linedefined == 0) ? "main" : "Lua";
  }
  luaO_chunkid(ar->short_src, ar->source, ar->srclen);
}
static int nextline (const Proto *p, int currentline, int pc) {
  if (p->lineinfo[pc] != ABSLINEINFO)
    return currentline + p->lineinfo[pc];
  else
    return luaG_getfuncline(p, pc);
}
static void collectvalidlines (lua_State *L, Closure *f) {
  if (noLuaClosure(f)) {
    setnilvalue(s2v(L->top));
    api_incr_top(L);
  }
  else {
    int i;
    TValue v;
    const Proto *p = f->l.p;
    int currentline = p->linedefined;
    Table *t = luaH_new(L);  
    sethvalue2s(L, L->top, t);  
    api_incr_top(L);
    setbtvalue(&v);  
    if (!p->is_vararg)  
      i = 0;  
    else {  
      lua_assert(GET_OPCODE(p->code[0]) == OP_VARARGPREP);
      currentline = nextline(p, currentline, 0);
      i = 1;  
    }
    for (; i < p->sizelineinfo; i++) {  
      currentline = nextline(p, currentline, i);  
      luaH_setint(L, t, currentline, &v);  
    }
  }
}
static const char *getfuncname (lua_State *L, CallInfo *ci, const char **name) {
  
  if (ci != NULL && !(ci->callstatus & CIST_TAIL))
    return funcnamefromcall(L, ci->previous, name);
  else return NULL;  
}
static int auxgetinfo (lua_State *L, const char *what, lua_Debug *ar,
                       Closure *f, CallInfo *ci) {
  int status = 1;
  for (; *what; what++) {
    switch (*what) {
      case 'S': {
        funcinfo(ar, f);
        break;
      }
      case 'l': {
        ar->currentline = (ci && isLua(ci)) ? getcurrentline(ci) : -1;
        break;
      }
      case 'u': {
        ar->nups = (f == NULL) ? 0 : f->c.nupvalues;
        if (noLuaClosure(f)) {
          ar->isvararg = 1;
          ar->nparams = 0;
        }
        else {
          ar->isvararg = f->l.p->is_vararg;
          ar->nparams = f->l.p->numparams;
        }
        break;
      }
      case 't': {
        ar->istailcall = (ci) ? ci->callstatus & CIST_TAIL : 0;
        break;
      }
      case 'n': {
        ar->namewhat = getfuncname(L, ci, &ar->name);
        if (ar->namewhat == NULL) {
          ar->namewhat = "";  
          ar->name = NULL;
        }
        break;
      }
      case 'r': {
        if (ci == NULL || !(ci->callstatus & CIST_TRAN))
          ar->ftransfer = ar->ntransfer = 0;
        else {
          ar->ftransfer = ci->u2.transferinfo.ftransfer;
          ar->ntransfer = ci->u2.transferinfo.ntransfer;
        }
        break;
      }
      case 'L':
      case 'f':  
        break;
      default: status = 0;  
    }
  }
  return status;
}
LUA_API int lua_getinfo (lua_State *L, const char *what, lua_Debug *ar) {
  int status;
  Closure *cl;
  CallInfo *ci;
  TValue *func;
  lua_lock(L);
  if (*what == '>') {
    ci = NULL;
    func = s2v(L->top - 1);
    api_check(L, ttisfunction(func), "function expected");
    what++;  
    L->top--;  
  }
  else {
    ci = ar->i_ci;
    func = s2v(ci->func);
    lua_assert(ttisfunction(func));
  }
  cl = ttisclosure(func) ? clvalue(func) : NULL;
  status = auxgetinfo(L, what, ar, cl, ci);
  if (strchr(what, 'f')) {
    setobj2s(L, L->top, func);
    api_incr_top(L);
  }
  if (strchr(what, 'L'))
    collectvalidlines(L, cl);
  lua_unlock(L);
  return status;
}
static const char *getobjname (const Proto *p, int lastpc, int reg,
                               const char **name);
static void kname (const Proto *p, int c, const char **name) {
  TValue *kvalue = &p->k[c];
  *name = (ttisstring(kvalue)) ? svalue(kvalue) : "?";
}
static void rname (const Proto *p, int pc, int c, const char **name) {
  const char *what = getobjname(p, pc, c, name); 
  if (!(what && *what == 'c'))  
    *name = "?";
}
static void rkname (const Proto *p, int pc, Instruction i, const char **name) {
  int c = GETARG_C(i);  
  if (GETARG_k(i))  
    kname(p, c, name);
  else  
    rname(p, pc, c, name);
}
static int filterpc (int pc, int jmptarget) {
  if (pc < jmptarget)  
    return -1;  
  else return pc;  
}
static int findsetreg (const Proto *p, int lastpc, int reg) {
  int pc;
  int setreg = -1;  
  int jmptarget = 0;  
  if (testMMMode(GET_OPCODE(p->code[lastpc])))
    lastpc--;  
  for (pc = 0; pc < lastpc; pc++) {
    Instruction i = p->code[pc];
    OpCode op = GET_OPCODE(i);
    int a = GETARG_A(i);
    int change;  
    switch (op) {
      case OP_LOADNIL: {  
        int b = GETARG_B(i);
        change = (a <= reg && reg <= a + b);
        break;
      }
      case OP_TFORCALL: {  
        change = (reg >= a + 2);
        break;
      }
      case OP_CALL:
      case OP_TAILCALL: {  
        change = (reg >= a);
        break;
      }
      case OP_JMP: {  
        int b = GETARG_sJ(i);
        int dest = pc + 1 + b;
        
        if (dest <= lastpc && dest > jmptarget)
          jmptarget = dest;  
        change = 0;
        break;
      }
      default:  
        change = (testAMode(op) && reg == a);
        break;
    }
    if (change)
      setreg = filterpc(pc, jmptarget);
  }
  return setreg;
}
static const char *gxf (const Proto *p, int pc, Instruction i, int isup) {
  int t = GETARG_B(i);  
  const char *name;  
  if (isup)  
    name = upvalname(p, t);
  else
    getobjname(p, pc, t, &name);
  return (name && strcmp(name, LUA_ENV) == 0) ? "global" : "field";
}
static const char *getobjname (const Proto *p, int lastpc, int reg,
                               const char **name) {
  int pc;
  *name = luaF_getlocalname(p, reg + 1, lastpc);
  if (*name)  
    return "local";
  
  pc = findsetreg(p, lastpc, reg);
  if (pc != -1) {  
    Instruction i = p->code[pc];
    OpCode op = GET_OPCODE(i);
    switch (op) {
      case OP_MOVE: {
        int b = GETARG_B(i);  
        if (b < GETARG_A(i))
          return getobjname(p, pc, b, name);  
        break;
      }
      case OP_GETTABUP: {
        int k = GETARG_C(i);  
        kname(p, k, name);
        return gxf(p, pc, i, 1);
      }
      case OP_GETTABLE: {
        int k = GETARG_C(i);  
        rname(p, pc, k, name);
        return gxf(p, pc, i, 0);
      }
      case OP_GETI: {
        *name = "integer index";
        return "field";
      }
      case OP_GETFIELD: {
        int k = GETARG_C(i);  
        kname(p, k, name);
        return gxf(p, pc, i, 0);
      }
      case OP_GETUPVAL: {
        *name = upvalname(p, GETARG_B(i));
        return "upvalue";
      }
      case OP_LOADK:
      case OP_LOADKX: {
        int b = (op == OP_LOADK) ? GETARG_Bx(i)
                                 : GETARG_Ax(p->code[pc + 1]);
        if (ttisstring(&p->k[b])) {
          *name = svalue(&p->k[b]);
          return "constant";
        }
        break;
      }
      case OP_SELF: {
        rkname(p, pc, i, name);
        return "method";
      }
      default: break;  
    }
  }
  return NULL;  
}
static const char *funcnamefromcode (lua_State *L, const Proto *p,
                                     int pc, const char **name) {
  TMS tm = (TMS)0;  
  Instruction i = p->code[pc];  
  switch (GET_OPCODE(i)) {
    case OP_CALL:
    case OP_TAILCALL:
      return getobjname(p, pc, GETARG_A(i), name);  
    case OP_TFORCALL: {  
      *name = "for iterator";
       return "for iterator";
    }
    
    case OP_SELF: case OP_GETTABUP: case OP_GETTABLE:
    case OP_GETI: case OP_GETFIELD:
      tm = TM_INDEX;
      break;
    case OP_SETTABUP: case OP_SETTABLE: case OP_SETI: case OP_SETFIELD:
      tm = TM_NEWINDEX;
      break;
    case OP_MMBIN: case OP_MMBINI: case OP_MMBINK: {
      tm = cast(TMS, GETARG_C(i));
      break;
    }
    case OP_UNM: tm = TM_UNM; break;
    case OP_BNOT: tm = TM_BNOT; break;
    case OP_LEN: tm = TM_LEN; break;
    case OP_CONCAT: tm = TM_CONCAT; break;
    case OP_EQ: tm = TM_EQ; break;
    
    case OP_LT: case OP_LTI: case OP_GTI: tm = TM_LT; break;
    case OP_LE: case OP_LEI: case OP_GEI: tm = TM_LE; break;
    case OP_CLOSE: case OP_RETURN: tm = TM_CLOSE; break;
    default:
      return NULL;  
  }
  *name = getstr(G(L)->tmname[tm]) + 2;
  return "metamethod";
}
static const char *funcnamefromcall (lua_State *L, CallInfo *ci,
                                                   const char **name) {
  if (ci->callstatus & CIST_HOOKED) {  
    *name = "?";
    return "hook";
  }
  else if (ci->callstatus & CIST_FIN) {  
    *name = "__gc";
    return "metamethod";  
  }
  else if (isLua(ci))
    return funcnamefromcode(L, ci_func(ci)->p, currentpc(ci), name);
  else
    return NULL;
}
static int isinstack (CallInfo *ci, const TValue *o) {
  StkId pos;
  for (pos = ci->func + 1; pos < ci->top; pos++) {
    if (o == s2v(pos))
      return 1;
  }
  return 0;  
}
static const char *getupvalname (CallInfo *ci, const TValue *o,
                                 const char **name) {
  LClosure *c = ci_func(ci);
  int i;
  for (i = 0; i < c->nupvalues; i++) {
    if (c->upvals[i]->v == o) {
      *name = upvalname(c->p, i);
      return "upvalue";
    }
  }
  return NULL;
}
static const char *formatvarinfo (lua_State *L, const char *kind,
                                                const char *name) {
  if (kind == NULL)
    return "";  
  else
    return luaO_pushfstring(L, " (%s '%s')", kind, name);
}
static const char *varinfo (lua_State *L, const TValue *o) {
  CallInfo *ci = L->ci;
  const char *name = NULL;  
  const char *kind = NULL;
  if (isLua(ci)) {
    kind = getupvalname(ci, o, &name);  
    if (!kind && isinstack(ci, o))  
      kind = getobjname(ci_func(ci)->p, currentpc(ci),
                        cast_int(cast(StkId, o) - (ci->func + 1)), &name);
  }
  return formatvarinfo(L, kind, name);
}
static l_noret typeerror (lua_State *L, const TValue *o, const char *op,
                          const char *extra) {
  const char *t = luaT_objtypename(L, o);
  luaG_runerror(L, "attempt to %s a %s value%s", op, t, extra);
}
l_noret luaG_typeerror (lua_State *L, const TValue *o, const char *op) {
  typeerror(L, o, op, varinfo(L, o));
}
l_noret luaG_callerror (lua_State *L, const TValue *o) {
  CallInfo *ci = L->ci;
  const char *name = NULL;  
  const char *kind = funcnamefromcall(L, ci, &name);
  const char *extra = kind ? formatvarinfo(L, kind, name) : varinfo(L, o);
  typeerror(L, o, "call", extra);
}
l_noret luaG_forerror (lua_State *L, const TValue *o, const char *what) {
  luaG_runerror(L, "bad 'for' %s (number expected, got %s)",
                   what, luaT_objtypename(L, o));
}
l_noret luaG_concaterror (lua_State *L, const TValue *p1, const TValue *p2) {
  if (ttisstring(p1) || cvt2str(p1)) p1 = p2;
  luaG_typeerror(L, p1, "concatenate");
}
l_noret luaG_opinterror (lua_State *L, const TValue *p1,
                         const TValue *p2, const char *msg) {
  if (!ttisnumber(p1))  
    p2 = p1;  
  luaG_typeerror(L, p2, msg);
}
l_noret luaG_tointerror (lua_State *L, const TValue *p1, const TValue *p2) {
  lua_Integer temp;
  if (!luaV_tointegerns(p1, &temp, LUA_FLOORN2I))
    p2 = p1;
  luaG_runerror(L, "number%s has no integer representation", varinfo(L, p2));
}
l_noret luaG_ordererror (lua_State *L, const TValue *p1, const TValue *p2) {
  const char *t1 = luaT_objtypename(L, p1);
  const char *t2 = luaT_objtypename(L, p2);
  if (strcmp(t1, t2) == 0)
    luaG_runerror(L, "attempt to compare two %s values", t1);
  else
    luaG_runerror(L, "attempt to compare %s with %s", t1, t2);
}
const char *luaG_addinfo (lua_State *L, const char *msg, TString *src,
                                        int line) {
  char buff[LUA_IDSIZE];
  if (src)
    luaO_chunkid(buff, getstr(src), tsslen(src));
  else {  
    buff[0] = '?'; buff[1] = '\0';
  }
  return luaO_pushfstring(L, "%s:%d: %s", buff, line, msg);
}
l_noret luaG_errormsg (lua_State *L) {
  if (L->errfunc != 0) {  
    StkId errfunc = restorestack(L, L->errfunc);
    lua_assert(ttisfunction(s2v(errfunc)));
    setobjs2s(L, L->top, L->top - 1);  
    setobjs2s(L, L->top - 1, errfunc);  
    L->top++;  
    luaD_callnoyield(L, L->top - 2, 1);  
  }
  luaD_throw(L, LUA_ERRRUN);
}
l_noret luaG_runerror (lua_State *L, const char *fmt, ...) {
  CallInfo *ci = L->ci;
  const char *msg;
  va_list argp;
  luaC_checkGC(L);  
  va_start(argp, fmt);
  msg = luaO_pushvfstring(L, fmt, argp);  
  va_end(argp);
  if (isLua(ci))  
    luaG_addinfo(L, msg, ci_func(ci)->p->source, getcurrentline(ci));
  luaG_errormsg(L);
}
static int changedline (const Proto *p, int oldpc, int newpc) {
  if (p->lineinfo == NULL)  
    return 0;
  if (newpc - oldpc < MAXIWTHABS / 2) {  
    int delta = 0;  
    int pc = oldpc;
    for (;;) {
      int lineinfo = p->lineinfo[++pc];
      if (lineinfo == ABSLINEINFO)
        break;  
      delta += lineinfo;
      if (pc == newpc)
        return (delta != 0);  
    }
  }
  
  return (luaG_getfuncline(p, oldpc) != luaG_getfuncline(p, newpc));
}
int luaG_traceexec (lua_State *L, const Instruction *pc) {
  CallInfo *ci = L->ci;
  lu_byte mask = L->hookmask;
  const Proto *p = ci_func(ci)->p;
  int counthook;
  if (!(mask & (LUA_MASKLINE | LUA_MASKCOUNT))) {  
    ci->u.l.trap = 0;  
    return 0;  
  }
  pc++;  
  ci->u.l.savedpc = pc;  
  counthook = (--L->hookcount == 0 && (mask & LUA_MASKCOUNT));
  if (counthook)
    resethookcount(L);  
  else if (!(mask & LUA_MASKLINE))
    return 1;  
  if (ci->callstatus & CIST_HOOKYIELD) {  
    ci->callstatus &= ~CIST_HOOKYIELD;  
    return 1;  
  }
  if (!isIT(*(ci->u.l.savedpc - 1)))  
    L->top = ci->top;  
  if (counthook)
    luaD_hook(L, LUA_HOOKCOUNT, -1, 0, 0);  
  if (mask & LUA_MASKLINE) {
    
    int oldpc = (L->oldpc < p->sizecode) ? L->oldpc : 0;
    int npci = pcRel(pc, p);
    if (npci <= oldpc ||  
        changedline(p, oldpc, npci)) {  
      int newline = luaG_getfuncline(p, npci);
      luaD_hook(L, LUA_HOOKLINE, newline, 0, 0);  
    }
    L->oldpc = npci;  
  }
  if (L->status == LUA_YIELD) {  
    if (counthook)
      L->hookcount = 1;  
    ci->u.l.savedpc--;  
    ci->callstatus |= CIST_HOOKYIELD;  
    luaD_throw(L, LUA_YIELD);
  }
  return 1;  
}
// root include ldo.c
#include <setjmp.h>
//included "stdlib.h" 
//included "string.h" 
//included "lundump.h" 
#define errorstatus(s)	((s) > LUA_YIELD)
#if !defined(LUAI_THROW)				
#if defined(__cplusplus) && !defined(LUA_USE_LONGJMP)	
#define LUAI_THROW(L,c)		throw(c)
#define LUAI_TRY(L,c,a) 	try { a } catch(...) { if ((c)->status == 0) (c)->status = -1; }
#define luai_jmpbuf		int  
#elif defined(LUA_USE_POSIX)				
#define LUAI_THROW(L,c)		_longjmp((c)->b, 1)
#define LUAI_TRY(L,c,a)		if (_setjmp((c)->b) == 0) { a }
#define luai_jmpbuf		jmp_buf
#else							
#define LUAI_THROW(L,c)		longjmp((c)->b, 1)
#define LUAI_TRY(L,c,a)		if (setjmp((c)->b) == 0) { a }
#define luai_jmpbuf		jmp_buf
#endif							
#endif							
struct lua_longjmp {
  struct lua_longjmp *previous;
  luai_jmpbuf b;
  volatile int status;  
};
void luaD_seterrorobj (lua_State *L, int errcode, StkId oldtop) {
  switch (errcode) {
    case LUA_ERRMEM: {  
      setsvalue2s(L, oldtop, G(L)->memerrmsg); 
      break;
    }
    case LUA_ERRERR: {
      setsvalue2s(L, oldtop, luaS_newliteral(L, "error in error handling"));
      break;
    }
    case LUA_OK: {  
      setnilvalue(s2v(oldtop));  
      break;
    }
    default: {
      lua_assert(errorstatus(errcode));  
      setobjs2s(L, oldtop, L->top - 1);  
      break;
    }
  }
  L->top = oldtop + 1;
}
l_noret luaD_throw (lua_State *L, int errcode) {
  if (L->errorJmp) {  
    L->errorJmp->status = errcode;  
    LUAI_THROW(L, L->errorJmp);  
  }
  else {  
    global_State *g = G(L);
    errcode = luaE_resetthread(L, errcode);  
    if (g->mainthread->errorJmp) {  
      setobjs2s(L, g->mainthread->top++, L->top - 1);  
      luaD_throw(g->mainthread, errcode);  
    }
    else {  
      if (g->panic) {  
        lua_unlock(L);
        g->panic(L);  
      }
      abort();
    }
  }
}
int luaD_rawrunprotected (lua_State *L, Pfunc f, void *ud) {
  l_uint32 oldnCcalls = L->nCcalls;
  struct lua_longjmp lj;
  lj.status = LUA_OK;
  lj.previous = L->errorJmp;  
  L->errorJmp = &lj;
  LUAI_TRY(L, &lj,
    (*f)(L, ud);
  );
  L->errorJmp = lj.previous;  
  L->nCcalls = oldnCcalls;
  return lj.status;
}
static void correctstack (lua_State *L, StkId oldstack, StkId newstack) {
  CallInfo *ci;
  UpVal *up;
  L->top = (L->top - oldstack) + newstack;
  L->tbclist = (L->tbclist - oldstack) + newstack;
  for (up = L->openupval; up != NULL; up = up->u.open.next)
    up->v = s2v((uplevel(up) - oldstack) + newstack);
  for (ci = L->ci; ci != NULL; ci = ci->previous) {
    ci->top = (ci->top - oldstack) + newstack;
    ci->func = (ci->func - oldstack) + newstack;
    if (isLua(ci))
      ci->u.l.trap = 1;  
  }
}
#define ERRORSTACKSIZE	(LUAI_MAXSTACK + 200)
int luaD_reallocstack (lua_State *L, int newsize, int raiseerror) {
  int oldsize = stacksize(L);
  int i;
  StkId newstack = luaM_reallocvector(L, NULL, 0,
                                      newsize + EXTRA_STACK, StackValue);
  lua_assert(newsize <= LUAI_MAXSTACK || newsize == ERRORSTACKSIZE);
  if (l_unlikely(newstack == NULL)) {  
    if (raiseerror)
      luaM_error(L);
    else return 0;  
  }
  
  i = ((oldsize <= newsize) ? oldsize : newsize) + EXTRA_STACK;
  memcpy(newstack, L->stack, i * sizeof(StackValue));
  for (; i < newsize + EXTRA_STACK; i++)
    setnilvalue(s2v(newstack + i)); 
  correctstack(L, L->stack, newstack);
  luaM_freearray(L, L->stack, oldsize + EXTRA_STACK);
  L->stack = newstack;
  L->stack_last = L->stack + newsize;
  return 1;
}
int luaD_growstack (lua_State *L, int n, int raiseerror) {
  int size = stacksize(L);
  if (l_unlikely(size > LUAI_MAXSTACK)) {
    
    lua_assert(stacksize(L) == ERRORSTACKSIZE);
    if (raiseerror)
      luaD_throw(L, LUA_ERRERR);  
    return 0;  
  }
  else {
    int newsize = 2 * size;  
    int needed = cast_int(L->top - L->stack) + n;
    if (newsize > LUAI_MAXSTACK)  
      newsize = LUAI_MAXSTACK;
    if (newsize < needed)  
      newsize = needed;
    if (l_likely(newsize <= LUAI_MAXSTACK))
      return luaD_reallocstack(L, newsize, raiseerror);
    else {  
      
      luaD_reallocstack(L, ERRORSTACKSIZE, raiseerror);
      if (raiseerror)
        luaG_runerror(L, "stack overflow");
      return 0;
    }
  }
}
static int stackinuse (lua_State *L) {
  CallInfo *ci;
  int res;
  StkId lim = L->top;
  for (ci = L->ci; ci != NULL; ci = ci->previous) {
    if (lim < ci->top) lim = ci->top;
  }
  lua_assert(lim <= L->stack_last);
  res = cast_int(lim - L->stack) + 1;  
  if (res < LUA_MINSTACK)
    res = LUA_MINSTACK;  
  return res;
}
void luaD_shrinkstack (lua_State *L) {
  int inuse = stackinuse(L);
  int nsize = inuse * 2;  
  int max = inuse * 3;  
  if (max > LUAI_MAXSTACK) {
    max = LUAI_MAXSTACK;  
    if (nsize > LUAI_MAXSTACK)
      nsize = LUAI_MAXSTACK;
  }
  
  if (inuse <= LUAI_MAXSTACK && stacksize(L) > max)
    luaD_reallocstack(L, nsize, 0);  
  else  
    condmovestack(L,{},{});  
  luaE_shrinkCI(L);  
}
void luaD_inctop (lua_State *L) {
  luaD_checkstack(L, 1);
  L->top++;
}
void luaD_hook (lua_State *L, int event, int line,
                              int ftransfer, int ntransfer) {
  lua_Hook hook = L->hook;
  if (hook && L->allowhook) {  
    int mask = CIST_HOOKED;
    CallInfo *ci = L->ci;
    ptrdiff_t top = savestack(L, L->top);  
    ptrdiff_t ci_top = savestack(L, ci->top);  
    lua_Debug ar;
    ar.event = event;
    ar.currentline = line;
    ar.i_ci = ci;
    if (ntransfer != 0) {
      mask |= CIST_TRAN;  
      ci->u2.transferinfo.ftransfer = ftransfer;
      ci->u2.transferinfo.ntransfer = ntransfer;
    }
    if (isLua(ci) && L->top < ci->top)
      L->top = ci->top;  
    luaD_checkstack(L, LUA_MINSTACK);  
    if (ci->top < L->top + LUA_MINSTACK)
      ci->top = L->top + LUA_MINSTACK;
    L->allowhook = 0;  
    ci->callstatus |= mask;
    lua_unlock(L);
    (*hook)(L, &ar);
    lua_lock(L);
    lua_assert(!L->allowhook);
    L->allowhook = 1;
    ci->top = restorestack(L, ci_top);
    L->top = restorestack(L, top);
    ci->callstatus &= ~mask;
  }
}
void luaD_hookcall (lua_State *L, CallInfo *ci) {
  L->oldpc = 0;  
  if (L->hookmask & LUA_MASKCALL) {  
    int event = (ci->callstatus & CIST_TAIL) ? LUA_HOOKTAILCALL
                                             : LUA_HOOKCALL;
    Proto *p = ci_func(ci)->p;
    ci->u.l.savedpc++;  
    luaD_hook(L, event, -1, 1, p->numparams);
    ci->u.l.savedpc--;  
  }
}
static void rethook (lua_State *L, CallInfo *ci, int nres) {
  if (L->hookmask & LUA_MASKRET) {  
    StkId firstres = L->top - nres;  
    int delta = 0;  
    int ftransfer;
    if (isLua(ci)) {
      Proto *p = ci_func(ci)->p;
      if (p->is_vararg)
        delta = ci->u.l.nextraargs + p->numparams + 1;
    }
    ci->func += delta;  
    ftransfer = cast(unsigned short, firstres - ci->func);
    luaD_hook(L, LUA_HOOKRET, -1, ftransfer, nres);  
    ci->func -= delta;
  }
  if (isLua(ci = ci->previous))
    L->oldpc = pcRel(ci->u.l.savedpc, ci_func(ci)->p);  
}
StkId luaD_tryfuncTM (lua_State *L, StkId func) {
  const TValue *tm;
  StkId p;
  checkstackGCp(L, 1, func);  
  tm = luaT_gettmbyobj(L, s2v(func), TM_CALL);  
  if (l_unlikely(ttisnil(tm)))
    luaG_callerror(L, s2v(func));  
  for (p = L->top; p > func; p--)  
    setobjs2s(L, p, p-1);
  L->top++;  
  setobj2s(L, func, tm);  
  return func;
}
l_sinline void moveresults (lua_State *L, StkId res, int nres, int wanted) {
  StkId firstresult;
  int i;
  switch (wanted) {  
    case 0:  
      L->top = res;
      return;
    case 1:  
      if (nres == 0)   
        setnilvalue(s2v(res));  
      else  
        setobjs2s(L, res, L->top - nres);  
      L->top = res + 1;
      return;
    case LUA_MULTRET:
      wanted = nres;  
      break;
    default:  
      if (hastocloseCfunc(wanted)) {  
        ptrdiff_t savedres = savestack(L, res);
        L->ci->callstatus |= CIST_CLSRET;  
        L->ci->u2.nres = nres;
        luaF_close(L, res, CLOSEKTOP, 1);
        L->ci->callstatus &= ~CIST_CLSRET;
        if (L->hookmask)  
          rethook(L, L->ci, nres);
        res = restorestack(L, savedres);  
        wanted = decodeNresults(wanted);
        if (wanted == LUA_MULTRET)
          wanted = nres;  
      }
      break;
  }
  
  firstresult = L->top - nres;  
  if (nres > wanted)  
    nres = wanted;  
  for (i = 0; i < nres; i++)  
    setobjs2s(L, res + i, firstresult + i);
  for (; i < wanted; i++)  
    setnilvalue(s2v(res + i));
  L->top = res + wanted;  
}
void luaD_poscall (lua_State *L, CallInfo *ci, int nres) {
  int wanted = ci->nresults;
  if (l_unlikely(L->hookmask && !hastocloseCfunc(wanted)))
    rethook(L, ci, nres);
  
  moveresults(L, ci->func, nres, wanted);
  
  lua_assert(!(ci->callstatus &
        (CIST_HOOKED | CIST_YPCALL | CIST_FIN | CIST_TRAN | CIST_CLSRET)));
  L->ci = ci->previous;  
}
#define next_ci(L)  (L->ci->next ? L->ci->next : luaE_extendCI(L))
l_sinline CallInfo *prepCallInfo (lua_State *L, StkId func, int nret,
                                                int mask, StkId top) {
  CallInfo *ci = L->ci = next_ci(L);  
  ci->func = func;
  ci->nresults = nret;
  ci->callstatus = mask;
  ci->top = top;
  return ci;
}
l_sinline int precallC (lua_State *L, StkId func, int nresults,
                                            lua_CFunction f) {
  int n;  
  CallInfo *ci;
  checkstackGCp(L, LUA_MINSTACK, func);  
  L->ci = ci = prepCallInfo(L, func, nresults, CIST_C,
                               L->top + LUA_MINSTACK);
  lua_assert(ci->top <= L->stack_last);
  if (l_unlikely(L->hookmask & LUA_MASKCALL)) {
    int narg = cast_int(L->top - func) - 1;
    luaD_hook(L, LUA_HOOKCALL, -1, 1, narg);
  }
  lua_unlock(L);
  n = (*f)(L);  
  lua_lock(L);
  api_checknelems(L, n);
  luaD_poscall(L, ci, n);
  return n;
}
int luaD_pretailcall (lua_State *L, CallInfo *ci, StkId func,
                                    int narg1, int delta) {
 retry:
  switch (ttypetag(s2v(func))) {
    case LUA_VCCL:  
      return precallC(L, func, LUA_MULTRET, clCvalue(s2v(func))->f);
    case LUA_VLCF:  
      return precallC(L, func, LUA_MULTRET, fvalue(s2v(func)));
    case LUA_VLCL: {  
      Proto *p = clLvalue(s2v(func))->p;
      int fsize = p->maxstacksize;  
      int nfixparams = p->numparams;
      int i;
      checkstackGCp(L, fsize - delta, func);
      ci->func -= delta;  
      for (i = 0; i < narg1; i++)  
        setobjs2s(L, ci->func + i, func + i);
      func = ci->func;  
      for (; narg1 <= nfixparams; narg1++)
        setnilvalue(s2v(func + narg1));  
      ci->top = func + 1 + fsize;  
      lua_assert(ci->top <= L->stack_last);
      ci->u.l.savedpc = p->code;  
      ci->callstatus |= CIST_TAIL;
      L->top = func + narg1;  
      return -1;
    }
    default: {  
      func = luaD_tryfuncTM(L, func);  
      
      narg1++;
      goto retry;  
    }
  }
}
CallInfo *luaD_precall (lua_State *L, StkId func, int nresults) {
 retry:
  switch (ttypetag(s2v(func))) {
    case LUA_VCCL:  
      precallC(L, func, nresults, clCvalue(s2v(func))->f);
      return NULL;
    case LUA_VLCF:  
      precallC(L, func, nresults, fvalue(s2v(func)));
      return NULL;
    case LUA_VLCL: {  
      CallInfo *ci;
      Proto *p = clLvalue(s2v(func))->p;
      int narg = cast_int(L->top - func) - 1;  
      int nfixparams = p->numparams;
      int fsize = p->maxstacksize;  
      checkstackGCp(L, fsize, func);
      L->ci = ci = prepCallInfo(L, func, nresults, 0, func + 1 + fsize);
      ci->u.l.savedpc = p->code;  
      for (; narg < nfixparams; narg++)
        setnilvalue(s2v(L->top++));  
      lua_assert(ci->top <= L->stack_last);
      return ci;
    }
    default: {  
      func = luaD_tryfuncTM(L, func);  
      
      goto retry;  
    }
  }
}
l_sinline void ccall (lua_State *L, StkId func, int nResults, int inc) {
  CallInfo *ci;
  L->nCcalls += inc;
  if (l_unlikely(getCcalls(L) >= LUAI_MAXCCALLS))
    luaE_checkcstack(L);
  if ((ci = luaD_precall(L, func, nResults)) != NULL) {  
    ci->callstatus = CIST_FRESH;  
    luaV_execute(L, ci);  
  }
  L->nCcalls -= inc;
}
void luaD_call (lua_State *L, StkId func, int nResults) {
  ccall(L, func, nResults, 1);
}
void luaD_callnoyield (lua_State *L, StkId func, int nResults) {
  ccall(L, func, nResults, nyci);
}
static int finishpcallk (lua_State *L,  CallInfo *ci) {
  int status = getcistrecst(ci);  
  if (l_likely(status == LUA_OK))  
    status = LUA_YIELD;  
  else {  
    StkId func = restorestack(L, ci->u2.funcidx);
    L->allowhook = getoah(ci->callstatus);  
    luaF_close(L, func, status, 1);  
    func = restorestack(L, ci->u2.funcidx);  
    luaD_seterrorobj(L, status, func);
    luaD_shrinkstack(L);   
    setcistrecst(ci, LUA_OK);  
  }
  ci->callstatus &= ~CIST_YPCALL;
  L->errfunc = ci->u.c.old_errfunc;
  
  return status;
}
static void finishCcall (lua_State *L, CallInfo *ci) {
  int n;  
  if (ci->callstatus & CIST_CLSRET) {  
    lua_assert(hastocloseCfunc(ci->nresults));
    n = ci->u2.nres;  
    
  }
  else {
    int status = LUA_YIELD;  
    
    lua_assert(ci->u.c.k != NULL && yieldable(L));
    if (ci->callstatus & CIST_YPCALL)   
      status = finishpcallk(L, ci);  
    adjustresults(L, LUA_MULTRET);  
    lua_unlock(L);
    n = (*ci->u.c.k)(L, status, ci->u.c.ctx);  
    lua_lock(L);
    api_checknelems(L, n);
  }
  luaD_poscall(L, ci, n);  
}
static void unroll (lua_State *L, void *ud) {
  CallInfo *ci;
  UNUSED(ud);
  while ((ci = L->ci) != &L->base_ci) {  
    if (!isLua(ci))  
      finishCcall(L, ci);  
    else {  
      luaV_finishOp(L);  
      luaV_execute(L, ci);  
    }
  }
}
static CallInfo *findpcall (lua_State *L) {
  CallInfo *ci;
  for (ci = L->ci; ci != NULL; ci = ci->previous) {  
    if (ci->callstatus & CIST_YPCALL)
      return ci;
  }
  return NULL;  
}
static int resume_error (lua_State *L, const char *msg, int narg) {
  L->top -= narg;  
  setsvalue2s(L, L->top, luaS_new(L, msg));  
  api_incr_top(L);
  lua_unlock(L);
  return LUA_ERRRUN;
}
static void resume (lua_State *L, void *ud) {
  int n = *(cast(int*, ud));  
  StkId firstArg = L->top - n;  
  CallInfo *ci = L->ci;
  if (L->status == LUA_OK)  
    ccall(L, firstArg - 1, LUA_MULTRET, 0);  
  else {  
    lua_assert(L->status == LUA_YIELD);
    L->status = LUA_OK;  
    if (isLua(ci)) {  
      L->top = firstArg;  
      luaV_execute(L, ci);  
    }
    else {  
      if (ci->u.c.k != NULL) {  
        lua_unlock(L);
        n = (*ci->u.c.k)(L, LUA_YIELD, ci->u.c.ctx); 
        lua_lock(L);
        api_checknelems(L, n);
      }
      luaD_poscall(L, ci, n);  
    }
    unroll(L, NULL);  
  }
}
static int precover (lua_State *L, int status) {
  CallInfo *ci;
  while (errorstatus(status) && (ci = findpcall(L)) != NULL) {
    L->ci = ci;  
    setcistrecst(ci, status);  
    status = luaD_rawrunprotected(L, unroll, NULL);
  }
  return status;
}
LUA_API int lua_resume (lua_State *L, lua_State *from, int nargs,
                                      int *nresults) {
  int status;
  lua_lock(L);
  if (L->status == LUA_OK) {  
    if (L->ci != &L->base_ci)  
      return resume_error(L, "cannot resume non-suspended coroutine", nargs);
    else if (L->top - (L->ci->func + 1) == nargs)  
      return resume_error(L, "cannot resume dead coroutine", nargs);
  }
  else if (L->status != LUA_YIELD)  
    return resume_error(L, "cannot resume dead coroutine", nargs);
  L->nCcalls = (from) ? getCcalls(from) : 0;
  if (getCcalls(L) >= LUAI_MAXCCALLS)
    return resume_error(L, "C stack overflow", nargs);
  L->nCcalls++;
  luai_userstateresume(L, nargs);
  api_checknelems(L, (L->status == LUA_OK) ? nargs + 1 : nargs);
  status = luaD_rawrunprotected(L, resume, &nargs);
   
  status = precover(L, status);
  if (l_likely(!errorstatus(status)))
    lua_assert(status == L->status);  
  else {  
    L->status = cast_byte(status);  
    luaD_seterrorobj(L, status, L->top);  
    L->ci->top = L->top;
  }
  *nresults = (status == LUA_YIELD) ? L->ci->u2.nyield
                                    : cast_int(L->top - (L->ci->func + 1));
  lua_unlock(L);
  return status;
}
LUA_API int lua_isyieldable (lua_State *L) {
  return yieldable(L);
}
LUA_API int lua_yieldk (lua_State *L, int nresults, lua_KContext ctx,
                        lua_KFunction k) {
  CallInfo *ci;
  luai_userstateyield(L, nresults);
  lua_lock(L);
  ci = L->ci;
  api_checknelems(L, nresults);
  if (l_unlikely(!yieldable(L))) {
    if (L != G(L)->mainthread)
      luaG_runerror(L, "attempt to yield across a C-call boundary");
    else
      luaG_runerror(L, "attempt to yield from outside a coroutine");
  }
  L->status = LUA_YIELD;
  ci->u2.nyield = nresults;  
  if (isLua(ci)) {  
    lua_assert(!isLuacode(ci));
    api_check(L, nresults == 0, "hooks cannot yield values");
    api_check(L, k == NULL, "hooks cannot continue after yielding");
  }
  else {
    if ((ci->u.c.k = k) != NULL)  
      ci->u.c.ctx = ctx;  
    luaD_throw(L, LUA_YIELD);
  }
  lua_assert(ci->callstatus & CIST_HOOKED);  
  lua_unlock(L);
  return 0;  
}
struct CloseP {
  StkId level;
  int status;
};
static void closepaux (lua_State *L, void *ud) {
  struct CloseP *pcl = cast(struct CloseP *, ud);
  luaF_close(L, pcl->level, pcl->status, 0);
}
int luaD_closeprotected (lua_State *L, ptrdiff_t level, int status) {
  CallInfo *old_ci = L->ci;
  lu_byte old_allowhooks = L->allowhook;
  for (;;) {  
    struct CloseP pcl;
    pcl.level = restorestack(L, level); pcl.status = status;
    status = luaD_rawrunprotected(L, &closepaux, &pcl);
    if (l_likely(status == LUA_OK))  
      return pcl.status;
    else {  
      L->ci = old_ci;
      L->allowhook = old_allowhooks;
    }
  }
}
int luaD_pcall (lua_State *L, Pfunc func, void *u,
                ptrdiff_t old_top, ptrdiff_t ef) {
  int status;
  CallInfo *old_ci = L->ci;
  lu_byte old_allowhooks = L->allowhook;
  ptrdiff_t old_errfunc = L->errfunc;
  L->errfunc = ef;
  status = luaD_rawrunprotected(L, func, u);
  if (l_unlikely(status != LUA_OK)) {  
    L->ci = old_ci;
    L->allowhook = old_allowhooks;
    status = luaD_closeprotected(L, old_top, status);
    luaD_seterrorobj(L, status, restorestack(L, old_top));
    luaD_shrinkstack(L);   
  }
  L->errfunc = old_errfunc;
  return status;
}
struct SParser {  
  ZIO *z;
  Mbuffer buff;  
  Dyndata dyd;  
  const char *mode;
  const char *name;
};
static void checkmode (lua_State *L, const char *mode, const char *x) {
  if (mode && strchr(mode, x[0]) == NULL) {
    luaO_pushfstring(L,
       "attempt to load a %s chunk (mode is '%s')", x, mode);
    luaD_throw(L, LUA_ERRSYNTAX);
  }
}
static void f_parser (lua_State *L, void *ud) {
  LClosure *cl;
  struct SParser *p = cast(struct SParser *, ud);
  int c = zgetc(p->z);  
  if (c == LUA_SIGNATURE[0]) {
    checkmode(L, p->mode, "binary");
    cl = luaU_undump(L, p->z, p->name);
  }
  else {
    checkmode(L, p->mode, "text");
    cl = luaY_parser(L, p->z, &p->buff, &p->dyd, p->name, c);
  }
  lua_assert(cl->nupvalues == cl->p->sizeupvalues);
  luaF_initupvals(L, cl);
}
int luaD_protectedparser (lua_State *L, ZIO *z, const char *name,
                                        const char *mode) {
  struct SParser p;
  int status;
  incnny(L);  
  p.z = z; p.name = name; p.mode = mode;
  p.dyd.actvar.arr = NULL; p.dyd.actvar.size = 0;
  p.dyd.gt.arr = NULL; p.dyd.gt.size = 0;
  p.dyd.label.arr = NULL; p.dyd.label.size = 0;
  luaZ_initbuffer(L, &p.buff);
  status = luaD_pcall(L, f_parser, &p, savestack(L, L->top), L->errfunc);
  luaZ_freebuffer(L, &p.buff);
  luaM_freearray(L, p.dyd.actvar.arr, p.dyd.actvar.size);
  luaM_freearray(L, p.dyd.gt.arr, p.dyd.gt.size);
  luaM_freearray(L, p.dyd.label.arr, p.dyd.label.size);
  decnny(L);
  return status;
}
// root include ldump.c
//included "stddef.h" 
typedef struct {
  lua_State *L;
  lua_Writer writer;
  void *data;
  int strip;
  int status;
} DumpState;
#define dumpVector(D,v,n)	dumpBlock(D,v,(n)*sizeof((v)[0]))
#define dumpLiteral(D, s)	dumpBlock(D,s,sizeof(s) - sizeof(char))
static void dumpBlock (DumpState *D, const void *b, size_t size) {
  if (D->status == 0 && size > 0) {
    lua_unlock(D->L);
    D->status = (*D->writer)(D->L, b, size, D->data);
    lua_lock(D->L);
  }
}
#define dumpVar(D,x)		dumpVector(D,&x,1)
static void dumpByte (DumpState *D, int y) {
  lu_byte x = (lu_byte)y;
  dumpVar(D, x);
}
#define DIBS    ((sizeof(size_t) * 8 / 7) + 1)
static void dumpSize (DumpState *D, size_t x) {
  lu_byte buff[DIBS];
  int n = 0;
  do {
    buff[DIBS - (++n)] = x & 0x7f;  
    x >>= 7;
  } while (x != 0);
  buff[DIBS - 1] |= 0x80;  
  dumpVector(D, buff + DIBS - n, n);
}
static void dumpInt (DumpState *D, int x) {
  dumpSize(D, x);
}
static void dumpNumber (DumpState *D, lua_Number x) {
  dumpVar(D, x);
}
static void dumpInteger (DumpState *D, lua_Integer x) {
  dumpVar(D, x);
}
static void dumpString (DumpState *D, const TString *s) {
  if (s == NULL)
    dumpSize(D, 0);
  else {
    size_t size = tsslen(s);
    const char *str = getstr(s);
    dumpSize(D, size + 1);
    dumpVector(D, str, size);
  }
}
static void dumpCode (DumpState *D, const Proto *f) {
  dumpInt(D, f->sizecode);
  dumpVector(D, f->code, f->sizecode);
}
static void dumpFunction(DumpState *D, const Proto *f, TString *psource);
static void dumpConstants (DumpState *D, const Proto *f) {
  int i;
  int n = f->sizek;
  dumpInt(D, n);
  for (i = 0; i < n; i++) {
    const TValue *o = &f->k[i];
    int tt = ttypetag(o);
    dumpByte(D, tt);
    switch (tt) {
      case LUA_VNUMFLT:
        dumpNumber(D, fltvalue(o));
        break;
      case LUA_VNUMINT:
        dumpInteger(D, ivalue(o));
        break;
      case LUA_VSHRSTR:
      case LUA_VLNGSTR:
        dumpString(D, tsvalue(o));
        break;
      default:
        lua_assert(tt == LUA_VNIL || tt == LUA_VFALSE || tt == LUA_VTRUE);
    }
  }
}
static void dumpProtos (DumpState *D, const Proto *f) {
  int i;
  int n = f->sizep;
  dumpInt(D, n);
  for (i = 0; i < n; i++)
    dumpFunction(D, f->p[i], f->source);
}
static void dumpUpvalues (DumpState *D, const Proto *f) {
  int i, n = f->sizeupvalues;
  dumpInt(D, n);
  for (i = 0; i < n; i++) {
    dumpByte(D, f->upvalues[i].instack);
    dumpByte(D, f->upvalues[i].idx);
    dumpByte(D, f->upvalues[i].kind);
  }
}
static void dumpDebug (DumpState *D, const Proto *f) {
  int i, n;
  n = (D->strip) ? 0 : f->sizelineinfo;
  dumpInt(D, n);
  dumpVector(D, f->lineinfo, n);
  n = (D->strip) ? 0 : f->sizeabslineinfo;
  dumpInt(D, n);
  for (i = 0; i < n; i++) {
    dumpInt(D, f->abslineinfo[i].pc);
    dumpInt(D, f->abslineinfo[i].line);
  }
  n = (D->strip) ? 0 : f->sizelocvars;
  dumpInt(D, n);
  for (i = 0; i < n; i++) {
    dumpString(D, f->locvars[i].varname);
    dumpInt(D, f->locvars[i].startpc);
    dumpInt(D, f->locvars[i].endpc);
  }
  n = (D->strip) ? 0 : f->sizeupvalues;
  dumpInt(D, n);
  for (i = 0; i < n; i++)
    dumpString(D, f->upvalues[i].name);
}
static void dumpFunction (DumpState *D, const Proto *f, TString *psource) {
  if (D->strip || f->source == psource)
    dumpString(D, NULL);  
  else
    dumpString(D, f->source);
  dumpInt(D, f->linedefined);
  dumpInt(D, f->lastlinedefined);
  dumpByte(D, f->numparams);
  dumpByte(D, f->is_vararg);
  dumpByte(D, f->maxstacksize);
  dumpCode(D, f);
  dumpConstants(D, f);
  dumpUpvalues(D, f);
  dumpProtos(D, f);
  dumpDebug(D, f);
}
static void dumpHeader (DumpState *D) {
  dumpLiteral(D, LUA_SIGNATURE);
  dumpByte(D, LUAC_VERSION);
  dumpByte(D, LUAC_FORMAT);
  dumpLiteral(D, LUAC_DATA);
  dumpByte(D, sizeof(Instruction));
  dumpByte(D, sizeof(lua_Integer));
  dumpByte(D, sizeof(lua_Number));
  dumpInteger(D, LUAC_INT);
  dumpNumber(D, LUAC_NUM);
}
int luaU_dump(lua_State *L, const Proto *f, lua_Writer w, void *data,
              int strip) {
  DumpState D;
  D.L = L;
  D.writer = w;
  D.data = data;
  D.strip = strip;
  D.status = 0;
  dumpHeader(&D);
  dumpByte(&D, f->sizeupvalues);
  dumpFunction(&D, f, NULL);
  return D.status;
}
// root include lfunc.c
//included "stddef.h" 
CClosure *luaF_newCclosure (lua_State *L, int nupvals) {
  GCObject *o = luaC_newobj(L, LUA_VCCL, sizeCclosure(nupvals));
  CClosure *c = gco2ccl(o);
  c->nupvalues = cast_byte(nupvals);
  return c;
}
LClosure *luaF_newLclosure (lua_State *L, int nupvals) {
  GCObject *o = luaC_newobj(L, LUA_VLCL, sizeLclosure(nupvals));
  LClosure *c = gco2lcl(o);
  c->p = NULL;
  c->nupvalues = cast_byte(nupvals);
  while (nupvals--) c->upvals[nupvals] = NULL;
  return c;
}
void luaF_initupvals (lua_State *L, LClosure *cl) {
  int i;
  for (i = 0; i < cl->nupvalues; i++) {
    GCObject *o = luaC_newobj(L, LUA_VUPVAL, sizeof(UpVal));
    UpVal *uv = gco2upv(o);
    uv->v = &uv->u.value;  
    setnilvalue(uv->v);
    cl->upvals[i] = uv;
    luaC_objbarrier(L, cl, uv);
  }
}
static UpVal *newupval (lua_State *L, int tbc, StkId level, UpVal **prev) {
  GCObject *o = luaC_newobj(L, LUA_VUPVAL, sizeof(UpVal));
  UpVal *uv = gco2upv(o);
  UpVal *next = *prev;
  uv->v = s2v(level);  
  uv->tbc = tbc;
  uv->u.open.next = next;  
  uv->u.open.previous = prev;
  if (next)
    next->u.open.previous = &uv->u.open.next;
  *prev = uv;
  if (!isintwups(L)) {  
    L->twups = G(L)->twups;  
    G(L)->twups = L;
  }
  return uv;
}
UpVal *luaF_findupval (lua_State *L, StkId level) {
  UpVal **pp = &L->openupval;
  UpVal *p;
  lua_assert(isintwups(L) || L->openupval == NULL);
  while ((p = *pp) != NULL && uplevel(p) >= level) {  
    lua_assert(!isdead(G(L), p));
    if (uplevel(p) == level)  
      return p;  
    pp = &p->u.open.next;
  }
  
  return newupval(L, 0, level, pp);
}
static void callclosemethod (lua_State *L, TValue *obj, TValue *err, int yy) {
  StkId top = L->top;
  const TValue *tm = luaT_gettmbyobj(L, obj, TM_CLOSE);
  setobj2s(L, top, tm);  
  setobj2s(L, top + 1, obj);  
  setobj2s(L, top + 2, err);  
  L->top = top + 3;  
  if (yy)
    luaD_call(L, top, 0);
  else
    luaD_callnoyield(L, top, 0);
}
static void checkclosemth (lua_State *L, StkId level) {
  const TValue *tm = luaT_gettmbyobj(L, s2v(level), TM_CLOSE);
  if (ttisnil(tm)) {  
    int idx = cast_int(level - L->ci->func);  
    const char *vname = luaG_findlocal(L, L->ci, idx, NULL);
    if (vname == NULL) vname = "?";
    luaG_runerror(L, "variable '%s' got a non-closable value", vname);
  }
}
static void prepcallclosemth (lua_State *L, StkId level, int status, int yy) {
  TValue *uv = s2v(level);  
  TValue *errobj;
  if (status == CLOSEKTOP)
    errobj = &G(L)->nilvalue;  
  else {  
    errobj = s2v(level + 1);  
    luaD_seterrorobj(L, status, level + 1);  
  }
  callclosemethod(L, uv, errobj, yy);
}
#define MAXDELTA  	((256ul << ((sizeof(L->stack->tbclist.delta) - 1) * 8)) - 1)
void luaF_newtbcupval (lua_State *L, StkId level) {
  lua_assert(level > L->tbclist);
  if (l_isfalse(s2v(level)))
    return;  
  checkclosemth(L, level);  
  while (cast_uint(level - L->tbclist) > MAXDELTA) {
    L->tbclist += MAXDELTA;  
    L->tbclist->tbclist.delta = 0;
  }
  level->tbclist.delta = cast(unsigned short, level - L->tbclist);
  L->tbclist = level;
}
void luaF_unlinkupval (UpVal *uv) {
  lua_assert(upisopen(uv));
  *uv->u.open.previous = uv->u.open.next;
  if (uv->u.open.next)
    uv->u.open.next->u.open.previous = uv->u.open.previous;
}
void luaF_closeupval (lua_State *L, StkId level) {
  UpVal *uv;
  StkId upl;  
  while ((uv = L->openupval) != NULL && (upl = uplevel(uv)) >= level) {
    TValue *slot = &uv->u.value;  
    lua_assert(uplevel(uv) < L->top);
    luaF_unlinkupval(uv);  
    setobj(L, slot, uv->v);  
    uv->v = slot;  
    if (!iswhite(uv)) {  
      nw2black(uv);  
      luaC_barrier(L, uv, slot);
    }
  }
}
static void poptbclist (lua_State *L) {
  StkId tbc = L->tbclist;
  lua_assert(tbc->tbclist.delta > 0);  
  tbc -= tbc->tbclist.delta;
  while (tbc > L->stack && tbc->tbclist.delta == 0)
    tbc -= MAXDELTA;  
  L->tbclist = tbc;
}
void luaF_close (lua_State *L, StkId level, int status, int yy) {
  ptrdiff_t levelrel = savestack(L, level);
  luaF_closeupval(L, level);  
  while (L->tbclist >= level) {  
    StkId tbc = L->tbclist;  
    poptbclist(L);  
    prepcallclosemth(L, tbc, status, yy);  
    level = restorestack(L, levelrel);
  }
}
Proto *luaF_newproto (lua_State *L) {
  GCObject *o = luaC_newobj(L, LUA_VPROTO, sizeof(Proto));
  Proto *f = gco2p(o);
  f->k = NULL;
  f->sizek = 0;
  f->p = NULL;
  f->sizep = 0;
  f->code = NULL;
  f->sizecode = 0;
  f->lineinfo = NULL;
  f->sizelineinfo = 0;
  f->abslineinfo = NULL;
  f->sizeabslineinfo = 0;
  f->upvalues = NULL;
  f->sizeupvalues = 0;
  f->numparams = 0;
  f->is_vararg = 0;
  f->maxstacksize = 0;
  f->locvars = NULL;
  f->sizelocvars = 0;
  f->linedefined = 0;
  f->lastlinedefined = 0;
  f->source = NULL;
  return f;
}
void luaF_freeproto (lua_State *L, Proto *f) {
  luaM_freearray(L, f->code, f->sizecode);
  luaM_freearray(L, f->p, f->sizep);
  luaM_freearray(L, f->k, f->sizek);
  luaM_freearray(L, f->lineinfo, f->sizelineinfo);
  luaM_freearray(L, f->abslineinfo, f->sizeabslineinfo);
  luaM_freearray(L, f->locvars, f->sizelocvars);
  luaM_freearray(L, f->upvalues, f->sizeupvalues);
  luaM_free(L, f);
}
const char *luaF_getlocalname (const Proto *f, int local_number, int pc) {
  int i;
  for (i = 0; i<f->sizelocvars && f->locvars[i].startpc <= pc; i++) {
    if (pc < f->locvars[i].endpc) {  
      local_number--;
      if (local_number == 0)
        return getstr(f->locvars[i].varname);
    }
  }
  return NULL;  
}
// root include lgc.c
//included "stdio.h" 
//included "string.h" 
#define GCSWEEPMAX	100
#define GCFINMAX	10
#define GCFINALIZECOST	50
#define WORK2MEM	sizeof(TValue)
#define PAUSEADJ		100
#define maskcolors	(bitmask(BLACKBIT) | WHITEBITS)
#define maskgcbits      (maskcolors | AGEBITS)
#define makewhite(g,x)	  (x->marked = cast_byte((x->marked & ~maskcolors) | luaC_white(g)))
#define set2gray(x)	resetbits(x->marked, maskcolors)
#define set2black(x)    (x->marked = cast_byte((x->marked & ~WHITEBITS) | bitmask(BLACKBIT)))
#define valiswhite(x)   (iscollectable(x) && iswhite(gcvalue(x)))
#define keyiswhite(n)   (keyiscollectable(n) && iswhite(gckey(n)))
#define gcvalueN(o)     (iscollectable(o) ? gcvalue(o) : NULL)
#define markvalue(g,o) { checkliveness(g->mainthread,o);   if (valiswhite(o)) reallymarkobject(g,gcvalue(o)); }
#define markkey(g, n)	{ if keyiswhite(n) reallymarkobject(g,gckey(n)); }
#define markobject(g,t)	{ if (iswhite(t)) reallymarkobject(g, obj2gco(t)); }
#define markobjectN(g,t)	{ if (t) markobject(g,t); }
static void reallymarkobject (global_State *g, GCObject *o);
static lu_mem atomic (lua_State *L);
static void entersweep (lua_State *L);
#define gnodelast(h)	gnode(h, cast_sizet(sizenode(h)))
static GCObject **getgclist (GCObject *o) {
  switch (o->tt) {
    case LUA_VTABLE: return &gco2t(o)->gclist;
    case LUA_VLCL: return &gco2lcl(o)->gclist;
    case LUA_VCCL: return &gco2ccl(o)->gclist;
    case LUA_VTHREAD: return &gco2th(o)->gclist;
    case LUA_VPROTO: return &gco2p(o)->gclist;
    case LUA_VUSERDATA: {
      Udata *u = gco2u(o);
      lua_assert(u->nuvalue > 0);
      return &u->gclist;
    }
    default: luai_unreachable(); return 0;
  }
}
#define linkgclist(o,p)	linkgclist_(obj2gco(o), &(o)->gclist, &(p))
static void linkgclist_ (GCObject *o, GCObject **pnext, GCObject **list) {
  lua_assert(!isgray(o));  
  *pnext = *list;
  *list = o;
  set2gray(o);  
}
#define linkobjgclist(o,p) linkgclist_(obj2gco(o), getgclist(o), &(p))
static void clearkey (Node *n) {
  lua_assert(isempty(gval(n)));
  if (keyiscollectable(n))
    setdeadkey(n);  
}
static int iscleared (global_State *g, const GCObject *o) {
  if (o == NULL) return 0;  
  else if (novariant(o->tt) == LUA_TSTRING) {
    markobject(g, o);  
    return 0;
  }
  else return iswhite(o);
}
void luaC_barrier_ (lua_State *L, GCObject *o, GCObject *v) {
  global_State *g = G(L);
  lua_assert(isblack(o) && iswhite(v) && !isdead(g, v) && !isdead(g, o));
  if (keepinvariant(g)) {  
    reallymarkobject(g, v);  
    if (isold(o)) {
      lua_assert(!isold(v));  
      setage(v, G_OLD0);  
    }
  }
  else {  
    lua_assert(issweepphase(g));
    if (g->gckind == KGC_INC)  
      makewhite(g, o);  
  }
}
void luaC_barrierback_ (lua_State *L, GCObject *o) {
  global_State *g = G(L);
  lua_assert(isblack(o) && !isdead(g, o));
  lua_assert((g->gckind == KGC_GEN) == (isold(o) && getage(o) != G_TOUCHED1));
  if (getage(o) == G_TOUCHED2)  
    set2gray(o);  
  else  
    linkobjgclist(o, g->grayagain);
  if (isold(o))  
    setage(o, G_TOUCHED1);  
}
void luaC_fix (lua_State *L, GCObject *o) {
  global_State *g = G(L);
	#if 0//included "stdio.h" 
	if (novariant((o)->tt) == LUA_TSTRING) {
		TString* s = &((cast_u(o))->ts);
		fprintf(stderr, "'%s' is fixed now!\n", getstr(s));
	}
	#endif
  lua_assert(g->allgc == o);  
  set2gray(o);  
  setage(o, G_OLD);  
  g->allgc = o->next;  
  o->next = g->fixedgc;  
  g->fixedgc = o;
}
#if 1//included "stdio.h" 
#define __incr_top(L)   {L->top++; api_check(L, L->top <= L->ci->top, 				"stack overflow");}
int luaC_getfixed(lua_State* L) {
  global_State *g = G(L);
	GCObject* o = g->fixedgc;
	lua_createtable(L, 0, 0);
	int oldgcstp  = g->gcstp;
	g->gcstp |= GCSTPGC;  
	int i = 1;
	while (o) {
		setgcovalue(L, s2v(L->top), o);
		__incr_top(L);
		lua_seti(L, -2, i++);
		o = o->next;
	}
	g->gcstp = oldgcstp; 
	return 1;
}
#endif
GCObject *luaC_newobj (lua_State *L, int tt, size_t sz) {
  global_State *g = G(L);
  GCObject *o = cast(GCObject *, luaM_newobject(L, novariant(tt), sz));
  o->marked = luaC_white(g);
  o->tt = tt;
  o->next = g->allgc;
  g->allgc = o;
  return o;
}
static void reallymarkobject (global_State *g, GCObject *o) {
  switch (o->tt) {
    case LUA_VSHRSTR:
    case LUA_VLNGSTR: {
      set2black(o);  
      break;
    }
    case LUA_VUPVAL: {
      UpVal *uv = gco2upv(o);
      if (upisopen(uv))
        set2gray(uv);  
      else
        set2black(uv);  
      markvalue(g, uv->v);  
      break;
    }
    case LUA_VUSERDATA: {
      Udata *u = gco2u(o);
      if (u->nuvalue == 0) {  
        markobjectN(g, u->metatable);  
        set2black(u);  
        break;
      }
      
			goto ft_udata;
    }  
    case LUA_VLCL: case LUA_VCCL: case LUA_VTABLE:
		case LUA_VTHREAD: case LUA_VPROTO: 
		ft_udata: {
      linkobjgclist(o, g->gray);  
      break;
    }
    default: luai_unreachable(); break;
  }
}
static void markmt (global_State *g) {
  int i;
  for (i=0; i < LUA_NUMTAGS; i++)
    markobjectN(g, g->mt[i]);
}
static lu_mem markbeingfnz (global_State *g) {
  GCObject *o;
  lu_mem count = 0;
  for (o = g->tobefnz; o != NULL; o = o->next) {
    count++;
    markobject(g, o);
  }
  return count;
}
static int remarkupvals (global_State *g) {
  lua_State *thread;
  lua_State **p = &g->twups;
  int work = 0;  
  while ((thread = *p) != NULL) {
    work++;
    if (!iswhite(thread) && thread->openupval != NULL)
      p = &thread->twups;  
    else {  
      UpVal *uv;
      lua_assert(!isold(thread) || thread->openupval == NULL);
      *p = thread->twups;  
      thread->twups = thread;  
      for (uv = thread->openupval; uv != NULL; uv = uv->u.open.next) {
        lua_assert(getage(uv) <= getage(thread));
        work++;
        if (!iswhite(uv)) {  
          lua_assert(upisopen(uv) && isgray(uv));
          markvalue(g, uv->v);  
        }
      }
    }
  }
  return work;
}
static void cleargraylists (global_State *g) {
  g->gray = g->grayagain = NULL;
  g->weak = g->allweak = g->ephemeron = NULL;
}
static void restartcollection (global_State *g) {
  cleargraylists(g);
  markobject(g, g->mainthread);
  markvalue(g, &g->l_registry);
  markmt(g);
  markbeingfnz(g);  
}
static void genlink (global_State *g, GCObject *o) {
  lua_assert(isblack(o));
  if (getage(o) == G_TOUCHED1) {  
    linkobjgclist(o, g->grayagain);  
  }  
  else if (getage(o) == G_TOUCHED2)
    changeage(o, G_TOUCHED2, G_OLD);  
}
static void traverseweakvalue (global_State *g, Table *h) {
  Node *n, *limit = gnodelast(h);
  
  int hasclears = (h->alimit > 0);
  for (n = gnode(h, 0); n < limit; n++) {  
    if (isempty(gval(n)))  
      clearkey(n);  
    else {
      lua_assert(!keyisnil(n));
      markkey(g, n);
      if (!hasclears && iscleared(g, gcvalueN(gval(n))))  
        hasclears = 1;  
    }
  }
  if (g->gcstate == GCSatomic && hasclears)
    linkgclist(h, g->weak);  
  else
    linkgclist(h, g->grayagain);  
}
static int traverseephemeron (global_State *g, Table *h, int inv) {
  int marked = 0;  
  int hasclears = 0;  
  int hasww = 0;  
  unsigned int i;
  unsigned int asize = luaH_realasize(h);
  unsigned int nsize = sizenode(h);
  
  for (i = 0; i < asize; i++) {
    if (valiswhite(&h->array[i])) {
      marked = 1;
      reallymarkobject(g, gcvalue(&h->array[i]));
    }
  }
  
  for (i = 0; i < nsize; i++) {
    Node *n = inv ? gnode(h, nsize - 1 - i) : gnode(h, i);
    if (isempty(gval(n)))  
      clearkey(n);  
    else if (iscleared(g, gckeyN(n))) {  
      hasclears = 1;  
      if (valiswhite(gval(n)))  
        hasww = 1;  
    }
    else if (valiswhite(gval(n))) {  
      marked = 1;
      reallymarkobject(g, gcvalue(gval(n)));  
    }
  }
  
  if (g->gcstate == GCSpropagate)
    linkgclist(h, g->grayagain);  
  else if (hasww)  
    linkgclist(h, g->ephemeron);  
  else if (hasclears)  
    linkgclist(h, g->allweak);  
  else
    genlink(g, obj2gco(h));  
  return marked;
}
static void traversestrongtable (global_State *g, Table *h) {
  Node *n, *limit = gnodelast(h);
  unsigned int i;
  unsigned int asize = luaH_realasize(h);
  for (i = 0; i < asize; i++)  
    markvalue(g, &h->array[i]);
  for (n = gnode(h, 0); n < limit; n++) {  
    if (isempty(gval(n)))  
      clearkey(n);  
    else {
      lua_assert(!keyisnil(n));
      markkey(g, n);
      markvalue(g, gval(n));
    }
  }
  genlink(g, obj2gco(h));
}
static lu_mem traversetable (global_State *g, Table *h) {
  const char *weakkey, *weakvalue;
  const TValue *mode = gfasttm(g, h->metatable, TM_MODE);
  markobjectN(g, h->metatable);
  if (mode && ttisstring(mode) &&  
      (cast_void(weakkey = strchr(svalue(mode), 'k')),
       cast_void(weakvalue = strchr(svalue(mode), 'v')),
       (weakkey || weakvalue))) {  
    if (!weakkey)  
      traverseweakvalue(g, h);
    else if (!weakvalue)  
      traverseephemeron(g, h, 0);
    else  
      linkgclist(h, g->allweak);  
  }
  else  
    traversestrongtable(g, h);
  return 1 + h->alimit + 2 * allocsizenode(h);
}
static int traverseudata (global_State *g, Udata *u) {
  int i;
  markobjectN(g, u->metatable);  
  for (i = 0; i < u->nuvalue; i++)
    markvalue(g, &u->uv[i].uv);
  genlink(g, obj2gco(u));
  return 1 + u->nuvalue;
}
static int traverseproto (global_State *g, Proto *f) {
  int i;
  markobjectN(g, f->source);
  for (i = 0; i < f->sizek; i++)  
    markvalue(g, &f->k[i]);
  for (i = 0; i < f->sizeupvalues; i++)  
    markobjectN(g, f->upvalues[i].name);
  for (i = 0; i < f->sizep; i++)  
    markobjectN(g, f->p[i]);
  for (i = 0; i < f->sizelocvars; i++)  
    markobjectN(g, f->locvars[i].varname);
  return 1 + f->sizek + f->sizeupvalues + f->sizep + f->sizelocvars;
}
static int traverseCclosure (global_State *g, CClosure *cl) {
  int i;
  for (i = 0; i < cl->nupvalues; i++)  
    markvalue(g, &cl->upvalue[i]);
  return 1 + cl->nupvalues;
}
static int traverseLclosure (global_State *g, LClosure *cl) {
  int i;
  markobjectN(g, cl->p);  
  for (i = 0; i < cl->nupvalues; i++) {  
    UpVal *uv = cl->upvals[i];
    markobjectN(g, uv);  
  }
  return 1 + cl->nupvalues;
}
static int traversethread (global_State *g, lua_State *th) {
  UpVal *uv;
  StkId o = th->stack;
  if (isold(th) || g->gcstate == GCSpropagate)
    linkgclist(th, g->grayagain);  
  if (o == NULL)
    return 1;  
  lua_assert(g->gcstate == GCSatomic ||
             th->openupval == NULL || isintwups(th));
  for (; o < th->top; o++)  
    markvalue(g, s2v(o));
  for (uv = th->openupval; uv != NULL; uv = uv->u.open.next)
    markobject(g, uv);  
  if (g->gcstate == GCSatomic) {  
    for (; o < th->stack_last + EXTRA_STACK; o++)
      setnilvalue(s2v(o));  
    
    if (!isintwups(th) && th->openupval != NULL) {
      th->twups = g->twups;  
      g->twups = th;
    }
  }
  else if (!g->gcemergency)
    luaD_shrinkstack(th); 
  return 1 + stacksize(th);
}
static lu_mem propagatemark (global_State *g) {
  GCObject *o = g->gray;
  nw2black(o);
  g->gray = *getgclist(o);  
  switch (o->tt) {
    case LUA_VTABLE: return traversetable(g, gco2t(o));
    case LUA_VUSERDATA: return traverseudata(g, gco2u(o));
    case LUA_VLCL: return traverseLclosure(g, gco2lcl(o));
    case LUA_VCCL: return traverseCclosure(g, gco2ccl(o));
    case LUA_VPROTO: return traverseproto(g, gco2p(o));
    case LUA_VTHREAD: return traversethread(g, gco2th(o));
    default: luai_unreachable(); return 0;
  }
}
static lu_mem propagateall (global_State *g) {
  lu_mem tot = 0;
  while (g->gray)
    tot += propagatemark(g);
  return tot;
}
static void convergeephemerons (global_State *g) {
  int changed;
  int dir = 0;
  do {
    GCObject *w;
    GCObject *next = g->ephemeron;  
    g->ephemeron = NULL;  
    changed = 0;
    while ((w = next) != NULL) {  
      Table *h = gco2t(w);
      next = h->gclist;  
      nw2black(h);  
      if (traverseephemeron(g, h, dir)) {  
        propagateall(g);  
        changed = 1;  
      }
    }
    dir = !dir;  
  } while (changed);  
}
static void clearbykeys (global_State *g, GCObject *l) {
  for (; l; l = gco2t(l)->gclist) {
    Table *h = gco2t(l);
    Node *limit = gnodelast(h);
    Node *n;
    for (n = gnode(h, 0); n < limit; n++) {
      if (iscleared(g, gckeyN(n)))  
        setempty(gval(n));  
      if (isempty(gval(n)))  
        clearkey(n);  
    }
  }
}
static void clearbyvalues (global_State *g, GCObject *l, GCObject *f) {
  for (; l != f; l = gco2t(l)->gclist) {
    Table *h = gco2t(l);
    Node *n, *limit = gnodelast(h);
    unsigned int i;
    unsigned int asize = luaH_realasize(h);
    for (i = 0; i < asize; i++) {
      TValue *o = &h->array[i];
      if (iscleared(g, gcvalueN(o)))  
        setempty(o);  
    }
    for (n = gnode(h, 0); n < limit; n++) {
      if (iscleared(g, gcvalueN(gval(n))))  
        setempty(gval(n));  
      if (isempty(gval(n)))  
        clearkey(n);  
    }
  }
}
static void freeupval (lua_State *L, UpVal *uv) {
  if (upisopen(uv))
    luaF_unlinkupval(uv);
  luaM_free(L, uv);
}
static void freeobj (lua_State *L, GCObject *o) {
  switch (o->tt) {
    case LUA_VPROTO:
      luaF_freeproto(L, gco2p(o));
      break;
    case LUA_VUPVAL:
      freeupval(L, gco2upv(o));
      break;
    case LUA_VLCL: {
      LClosure *cl = gco2lcl(o);
      luaM_freemem(L, cl, sizeLclosure(cl->nupvalues));
      break;
    }
    case LUA_VCCL: {
      CClosure *cl = gco2ccl(o);
      luaM_freemem(L, cl, sizeCclosure(cl->nupvalues));
      break;
    }
    case LUA_VTABLE:
      luaH_free(L, gco2t(o));
      break;
    case LUA_VTHREAD:
      luaE_freethread(L, gco2th(o));
      break;
    case LUA_VUSERDATA: {
      Udata *u = gco2u(o);
      luaM_freemem(L, o, sizeudata(u->nuvalue, u->len));
      break;
    }
    case LUA_VSHRSTR: {
      TString *ts = gco2ts(o);
      luaS_remove(L, ts);  
      luaM_freemem(L, ts, sizelstring(ts->shrlen));
      break;
    }
    case LUA_VLNGSTR: {
      TString *ts = gco2ts(o);
      luaM_freemem(L, ts, sizelstring(ts->u.lnglen));
      break;
    }
    default: luai_unreachable();
  }
}
static GCObject **sweeplist (lua_State *L, GCObject **p, int countin,
                             int *countout) {
  global_State *g = G(L);
  int ow = otherwhite(g);
  int i;
  int white = luaC_white(g);  
  for (i = 0; *p != NULL && i < countin; i++) {
    GCObject *curr = *p;
    int marked = curr->marked;
    if (isdeadm(ow, marked)) {  
      *p = curr->next;  
      freeobj(L, curr);  
    }
    else {  
      curr->marked = cast_byte((marked & ~maskgcbits) | white);
      p = &curr->next;  
    }
  }
  if (countout)
    *countout = i;  
  return (*p == NULL) ? NULL : p;
}
static GCObject **sweeptolive (lua_State *L, GCObject **p) {
  GCObject **old = p;
  do {
    p = sweeplist(L, p, 1, NULL);
  } while (p == old);
  return p;
}
static void checkSizes (lua_State *L, global_State *g) {
  if (!g->gcemergency) {
    if (g->strt.nuse < g->strt.size / 4) {  
      l_mem olddebt = g->GCdebt;
      luaS_resize(L, g->strt.size / 2);
      g->GCestimate += g->GCdebt - olddebt;  
    }
  }
}
static GCObject *udata2finalize (global_State *g) {
  GCObject *o = g->tobefnz;  
  lua_assert(tofinalize(o));
  g->tobefnz = o->next;  
  o->next = g->allgc;  
  g->allgc = o;
  resetbit(o->marked, FINALIZEDBIT);  
  if (issweepphase(g))
    makewhite(g, o);  
  else if (getage(o) == G_OLD1)
    g->firstold1 = o;  
  return o;
}
static void dothecall (lua_State *L, void *ud) {
  UNUSED(ud);
  luaD_callnoyield(L, L->top - 2, 0);
}
static void GCTM (lua_State *L) {
  global_State *g = G(L);
  const TValue *tm;
  TValue v;
  lua_assert(!g->gcemergency);
  setgcovalue(L, &v, udata2finalize(g));
  tm = luaT_gettmbyobj(L, &v, TM_GC);
  if (!notm(tm)) {  
    int status;
    lu_byte oldah = L->allowhook;
    int oldgcstp  = g->gcstp;
    g->gcstp |= GCSTPGC;  
    L->allowhook = 0;  
    setobj2s(L, L->top++, tm);  
    setobj2s(L, L->top++, &v);  
    L->ci->callstatus |= CIST_FIN;  
    status = luaD_pcall(L, dothecall, NULL, savestack(L, L->top - 2), 0);
    L->ci->callstatus &= ~CIST_FIN;  
    L->allowhook = oldah;  
    g->gcstp = oldgcstp;  
    if (l_unlikely(status != LUA_OK)) {  
      luaE_warnerror(L, "__gc");
      L->top--;  
    }
  }
}
static int runafewfinalizers (lua_State *L, int n) {
  global_State *g = G(L);
  int i;
  for (i = 0; i < n && g->tobefnz; i++)
    GCTM(L);  
  return i;
}
static void callallpendingfinalizers (lua_State *L) {
  global_State *g = G(L);
  while (g->tobefnz)
    GCTM(L);
}
static GCObject **findlast (GCObject **p) {
  while (*p != NULL)
    p = &(*p)->next;
  return p;
}
static void separatetobefnz (global_State *g, int all) {
  GCObject *curr;
  GCObject **p = &g->finobj;
  GCObject **lastnext = findlast(&g->tobefnz);
  while ((curr = *p) != g->finobjold1) {  
    lua_assert(tofinalize(curr));
    if (!(iswhite(curr) || all))  
      p = &curr->next;  
    else {
      if (curr == g->finobjsur)  
        g->finobjsur = curr->next;  
      *p = curr->next;  
      curr->next = *lastnext;  
      *lastnext = curr;
      lastnext = &curr->next;
    }
  }
}
static void checkpointer (GCObject **p, GCObject *o) {
  if (o == *p)
    *p = o->next;
}
static void correctpointers (global_State *g, GCObject *o) {
  checkpointer(&g->survival, o);
  checkpointer(&g->old1, o);
  checkpointer(&g->reallyold, o);
  checkpointer(&g->firstold1, o);
}
void luaC_checkfinalizer (lua_State *L, GCObject *o, Table *mt) {
  global_State *g = G(L);
  if (tofinalize(o) ||                 
      gfasttm(g, mt, TM_GC) == NULL ||    
      (g->gcstp & GCSTPCLS))                   
    return;  
  else {  
    GCObject **p;
    if (issweepphase(g)) {
      makewhite(g, o);  
      if (g->sweepgc == &o->next)  
        g->sweepgc = sweeptolive(L, g->sweepgc);  
    }
    else
      correctpointers(g, o);
    
    for (p = &g->allgc; *p != o; p = &(*p)->next) {  }
    *p = o->next;  
    o->next = g->finobj;  
    g->finobj = o;
    l_setbit(o->marked, FINALIZEDBIT);  
  }
}
static void setpause (global_State *g);
static void sweep2old (lua_State *L, GCObject **p) {
  GCObject *curr;
  global_State *g = G(L);
  while ((curr = *p) != NULL) {
    if (iswhite(curr)) {  
      lua_assert(isdead(g, curr));
      *p = curr->next;  
      freeobj(L, curr);  
    }
    else {  
      setage(curr, G_OLD);
      if (curr->tt == LUA_VTHREAD) {  
        lua_State *th = gco2th(curr);
        linkgclist(th, g->grayagain);  
      }
      else if (curr->tt == LUA_VUPVAL && upisopen(gco2upv(curr)))
        set2gray(curr);  
      else  
        nw2black(curr);
      p = &curr->next;  
    }
  }
}
static GCObject **sweepgen (lua_State *L, global_State *g, GCObject **p,
                            GCObject *limit, GCObject **pfirstold1) {
  static const lu_byte nextage[] = {
    G_SURVIVAL,  
    G_OLD1,      
    G_OLD1,      
    G_OLD,       
    G_OLD,       
    G_TOUCHED1,  
    G_TOUCHED2   
  };
  int white = luaC_white(g);
  GCObject *curr;
  while ((curr = *p) != limit) {
    if (iswhite(curr)) {  
      lua_assert(!isold(curr) && isdead(g, curr));
      *p = curr->next;  
      freeobj(L, curr);  
    }
    else {  
      if (getage(curr) == G_NEW) {  
        int marked = curr->marked & ~maskgcbits;  
        curr->marked = cast_byte(marked | G_SURVIVAL | white);
      }
      else {  
        setage(curr, nextage[getage(curr)]);
        if (getage(curr) == G_OLD1 && *pfirstold1 == NULL)
          *pfirstold1 = curr;  
      }
      p = &curr->next;  
    }
  }
  return p;
}
static void whitelist (global_State *g, GCObject *p) {
  int white = luaC_white(g);
  for (; p != NULL; p = p->next)
    p->marked = cast_byte((p->marked & ~maskgcbits) | white);
}
static GCObject **correctgraylist (GCObject **p) {
  GCObject *curr;
  while ((curr = *p) != NULL) {
    GCObject **next = getgclist(curr);
    if (iswhite(curr))
      goto remove;  
    else if (getage(curr) == G_TOUCHED1) {  
      lua_assert(isgray(curr));
      nw2black(curr);  
      changeage(curr, G_TOUCHED1, G_TOUCHED2);
      goto remain;  
    }
    else if (curr->tt == LUA_VTHREAD) {
      lua_assert(isgray(curr));
      goto remain;  
    }
    else {  
      lua_assert(isold(curr));  
      if (getage(curr) == G_TOUCHED2)  
        changeage(curr, G_TOUCHED2, G_OLD);  
      nw2black(curr);  
      goto remove;
    }
    remove: *p = *next; continue;
    remain: p = next; continue;
  }
  return p;
}
static void correctgraylists (global_State *g) {
  GCObject **list = correctgraylist(&g->grayagain);
  *list = g->weak; g->weak = NULL;
  list = correctgraylist(list);
  *list = g->allweak; g->allweak = NULL;
  list = correctgraylist(list);
  *list = g->ephemeron; g->ephemeron = NULL;
  correctgraylist(list);
}
static void markold (global_State *g, GCObject *from, GCObject *to) {
  GCObject *p;
  for (p = from; p != to; p = p->next) {
    if (getage(p) == G_OLD1) {
      lua_assert(!iswhite(p));
      changeage(p, G_OLD1, G_OLD);  
      if (isblack(p))
        reallymarkobject(g, p);
    }
  }
}
static void finishgencycle (lua_State *L, global_State *g) {
  correctgraylists(g);
  checkSizes(L, g);
  g->gcstate = GCSpropagate;  
  if (!g->gcemergency)
    callallpendingfinalizers(L);
}
static void youngcollection (lua_State *L, global_State *g) {
  GCObject **psurvival;  
  GCObject *dummy;  
  lua_assert(g->gcstate == GCSpropagate);
  if (g->firstold1) {  
    markold(g, g->firstold1, g->reallyold);  
    g->firstold1 = NULL;  
  }
  markold(g, g->finobj, g->finobjrold);
  markold(g, g->tobefnz, NULL);
  atomic(L);
  
  g->gcstate = GCSswpallgc;
  psurvival = sweepgen(L, g, &g->allgc, g->survival, &g->firstold1);
  
  sweepgen(L, g, psurvival, g->old1, &g->firstold1);
  g->reallyold = g->old1;
  g->old1 = *psurvival;  
  g->survival = g->allgc;  
  
  dummy = NULL;  
  psurvival = sweepgen(L, g, &g->finobj, g->finobjsur, &dummy);
  
  sweepgen(L, g, psurvival, g->finobjold1, &dummy);
  g->finobjrold = g->finobjold1;
  g->finobjold1 = *psurvival;  
  g->finobjsur = g->finobj;  
  sweepgen(L, g, &g->tobefnz, NULL, &dummy);
  finishgencycle(L, g);
}
static void atomic2gen (lua_State *L, global_State *g) {
  cleargraylists(g);
  
  g->gcstate = GCSswpallgc;
  sweep2old(L, &g->allgc);
  
  g->reallyold = g->old1 = g->survival = g->allgc;
  g->firstold1 = NULL;  
  
  sweep2old(L, &g->finobj);
  g->finobjrold = g->finobjold1 = g->finobjsur = g->finobj;
  sweep2old(L, &g->tobefnz);
  g->gckind = KGC_GEN;
  g->lastatomic = 0;
  g->GCestimate = gettotalbytes(g);  
  finishgencycle(L, g);
}
static lu_mem entergen (lua_State *L, global_State *g) {
  lu_mem numobjs;
  luaC_runtilstate(L, bitmask(GCSpause));  
  luaC_runtilstate(L, bitmask(GCSpropagate));  
  numobjs = atomic(L);  
  atomic2gen(L, g);
  return numobjs;
}
static void enterinc (global_State *g) {
  whitelist(g, g->allgc);
  g->reallyold = g->old1 = g->survival = NULL;
  whitelist(g, g->finobj);
  whitelist(g, g->tobefnz);
  g->finobjrold = g->finobjold1 = g->finobjsur = NULL;
  g->gcstate = GCSpause;
  g->gckind = KGC_INC;
  g->lastatomic = 0;
}
void luaC_changemode (lua_State *L, int newmode) {
  global_State *g = G(L);
  if (newmode != g->gckind) {
    if (newmode == KGC_GEN)  
      entergen(L, g);
    else
      enterinc(g);  
  }
  g->lastatomic = 0;
}
static lu_mem fullgen (lua_State *L, global_State *g) {
  enterinc(g);
  return entergen(L, g);
}
static void setminordebt (global_State *g) {
  luaE_setdebt(g, -(cast(l_mem, (gettotalbytes(g) / 100)) * g->genminormul));
}
static void stepgenfull (lua_State *L, global_State *g) {
  lu_mem newatomic;  
  lu_mem lastatomic = g->lastatomic;  
  if (g->gckind == KGC_GEN)  
    enterinc(g);  
  luaC_runtilstate(L, bitmask(GCSpropagate));  
  newatomic = atomic(L);  
  if (newatomic < lastatomic + (lastatomic >> 3)) {  
    atomic2gen(L, g);  
    setminordebt(g);
  }
  else {  
    g->GCestimate = gettotalbytes(g);  ;
    entersweep(L);
    luaC_runtilstate(L, bitmask(GCSpause));  
    setpause(g);
    g->lastatomic = newatomic;
  }
}
static void genstep (lua_State *L, global_State *g) {
  if (g->lastatomic != 0)  
    stepgenfull(L, g);  
  else {
    lu_mem majorbase = g->GCestimate;  
    lu_mem majorinc = (majorbase / 100) * getgcparam(g->genmajormul);
    if (g->GCdebt > 0 && gettotalbytes(g) > majorbase + majorinc) {
      lu_mem numobjs = fullgen(L, g);  
      if (gettotalbytes(g) < majorbase + (majorinc / 2)) {
        
        setminordebt(g);
      }
      else {  
        g->lastatomic = numobjs;  
        setpause(g);  
      }
    }
    else {  
      youngcollection(L, g);
      setminordebt(g);
      g->GCestimate = majorbase;  
    }
  }
  lua_assert(isdecGCmodegen(g));
}
static void setpause (global_State *g) {
  l_mem threshold, debt;
  int pause = getgcparam(g->gcpause);
  l_mem estimate = g->GCestimate / PAUSEADJ;  
  lua_assert(estimate > 0);
  threshold = (pause < MAX_LMEM / estimate)  
            ? estimate * pause  
            : MAX_LMEM;  
  debt = gettotalbytes(g) - threshold;
  if (debt > 0) debt = 0;
  luaE_setdebt(g, debt);
}
static void entersweep (lua_State *L) {
  global_State *g = G(L);
  g->gcstate = GCSswpallgc;
  lua_assert(g->sweepgc == NULL);
  g->sweepgc = sweeptolive(L, &g->allgc);
}
static void deletelist (lua_State *L, GCObject *p, GCObject *limit) {
  while (p != limit) {
    GCObject *next = p->next;
    freeobj(L, p);
    p = next;
  }
}
void luaC_freeallobjects (lua_State *L) {
  global_State *g = G(L);
  g->gcstp = GCSTPCLS;  
  luaC_changemode(L, KGC_INC);
  separatetobefnz(g, 1);  
  lua_assert(g->finobj == NULL);
  callallpendingfinalizers(L);
  deletelist(L, g->allgc, obj2gco(g->mainthread));
  lua_assert(g->finobj == NULL);  
  deletelist(L, g->fixedgc, NULL);  
  lua_assert(g->strt.nuse == 0);
}
static lu_mem atomic (lua_State *L) {
  global_State *g = G(L);
  lu_mem work = 0;
  GCObject *origweak, *origall;
  GCObject *grayagain = g->grayagain;  
  g->grayagain = NULL;
  lua_assert(g->ephemeron == NULL && g->weak == NULL);
  lua_assert(!iswhite(g->mainthread));
  g->gcstate = GCSatomic;
  markobject(g, L);  
  
  markvalue(g, &g->l_registry);
  markmt(g);  
  work += propagateall(g);  
  
  work += remarkupvals(g);
  work += propagateall(g);  
  g->gray = grayagain;
  work += propagateall(g);  
  convergeephemerons(g);
  
  
  clearbyvalues(g, g->weak, NULL);
  clearbyvalues(g, g->allweak, NULL);
  origweak = g->weak; origall = g->allweak;
  separatetobefnz(g, 0);  
  work += markbeingfnz(g);  
  work += propagateall(g);  
  convergeephemerons(g);
  
  
  clearbykeys(g, g->ephemeron);  
  clearbykeys(g, g->allweak);  
  
  clearbyvalues(g, g->weak, origweak);
  clearbyvalues(g, g->allweak, origall);
  luaS_clearcache(g);
  g->currentwhite = cast_byte(otherwhite(g));  
  lua_assert(g->gray == NULL);
  return work;  
}
static int sweepstep (lua_State *L, global_State *g,
                      int nextstate, GCObject **nextlist) {
  if (g->sweepgc) {
    l_mem olddebt = g->GCdebt;
    int count;
    g->sweepgc = sweeplist(L, g->sweepgc, GCSWEEPMAX, &count);
    g->GCestimate += g->GCdebt - olddebt;  
    return count;
  }
  else {  
    g->gcstate = nextstate;
    g->sweepgc = nextlist;
    return 0;  
  }
}
static lu_mem singlestep (lua_State *L) {
  global_State *g = G(L);
  lu_mem work;
  lua_assert(!g->gcstopem);  
  g->gcstopem = 1;  
  switch (g->gcstate) {
    case GCSpause: {
      restartcollection(g);
      g->gcstate = GCSpropagate;
      work = 1;
      break;
    }
    case GCSpropagate: {
      if (g->gray == NULL) {  
        g->gcstate = GCSenteratomic;  
        work = 0;
      }
      else
        work = propagatemark(g);  
      break;
    }
    case GCSenteratomic: {
      work = atomic(L);  
      entersweep(L);
      g->GCestimate = gettotalbytes(g);  ;
      break;
    }
    case GCSswpallgc: {  
      work = sweepstep(L, g, GCSswpfinobj, &g->finobj);
      break;
    }
    case GCSswpfinobj: {  
      work = sweepstep(L, g, GCSswptobefnz, &g->tobefnz);
      break;
    }
    case GCSswptobefnz: {  
      work = sweepstep(L, g, GCSswpend, NULL);
      break;
    }
    case GCSswpend: {  
      checkSizes(L, g);
      g->gcstate = GCScallfin;
      work = 0;
      break;
    }
    case GCScallfin: {  
      if (g->tobefnz && !g->gcemergency) {
        g->gcstopem = 0;  
        work = runafewfinalizers(L, GCFINMAX) * GCFINALIZECOST;
      }
      else {  
        g->gcstate = GCSpause;  
        work = 0;
      }
      break;
    }
    default: luai_unreachable(); return 0;
  }
  g->gcstopem = 0;
  return work;
}
void luaC_runtilstate (lua_State *L, int statesmask) {
  global_State *g = G(L);
  while (!testbit(statesmask, g->gcstate))
    singlestep(L);
}
static void incstep (lua_State *L, global_State *g) {
  int stepmul = (getgcparam(g->gcstepmul) | 1);  
  l_mem debt = (g->GCdebt / WORK2MEM) * stepmul;
  l_mem stepsize = (g->gcstepsize <= log2maxs(l_mem))
                 ? ((cast(l_mem, 1) << g->gcstepsize) / WORK2MEM) * stepmul
                 : MAX_LMEM;  
  do {  
    lu_mem work = singlestep(L);  
    debt -= work;
  } while (debt > -stepsize && g->gcstate != GCSpause);
  if (g->gcstate == GCSpause)
    setpause(g);  
  else {
    debt = (debt / stepmul) * WORK2MEM;  
    luaE_setdebt(g, debt);
  }
}
void luaC_step (lua_State *L) {
  global_State *g = G(L);
  lua_assert(!g->gcemergency);
  if (gcrunning(g)) {  
    if(isdecGCmodegen(g))
      genstep(L, g);
    else
      incstep(L, g);
  }
}
static void fullinc (lua_State *L, global_State *g) {
  if (keepinvariant(g))  
    entersweep(L); 
  
  luaC_runtilstate(L, bitmask(GCSpause));
  luaC_runtilstate(L, bitmask(GCScallfin));  
  
  lua_assert(g->GCestimate == gettotalbytes(g));
  luaC_runtilstate(L, bitmask(GCSpause));  
  setpause(g);
}
void luaC_fullgc (lua_State *L, int isemergency) {
  global_State *g = G(L);
  lua_assert(!g->gcemergency);
  g->gcemergency = isemergency;  
  if (g->gckind == KGC_INC)
    fullinc(L, g);
  else
    fullgen(L, g);
  g->gcemergency = 0;
}
// root include llex.c
#include <locale.h>
//included "string.h" 
#ifndef lctype_h
#if defined(LUA_USE_CTYPE) && LUA_USE_CTYPE == 1
#undef LUA_USE_CTYPE
#endif
#if !defined(LUA_USE_CTYPE)
#if 'A' == 65 && '0' == 48
#define LUA_USE_CTYPE	0
#else
#define LUA_USE_CTYPE	1
#endif
#endif
#if !LUA_USE_CTYPE	//included "limits.h" 
#define ALPHABIT	0
#define DIGITBIT	1
#define PRINTBIT	2
#define SPACEBIT	3
#define XDIGITBIT	4
#define MASK(B)		(1 << (B))
#define testprop(c,p)	(luai_ctype_[(c)+1] & (p))
#define lislalpha(c)	testprop(c, MASK(ALPHABIT))
#define lislalnum(c)	testprop(c, (MASK(ALPHABIT) | MASK(DIGITBIT)))
#define lisdigit(c)	testprop(c, MASK(DIGITBIT))
#define lisspace(c)	testprop(c, MASK(SPACEBIT))
#define lisprint(c)	testprop(c, MASK(PRINTBIT))
#define lisxdigit(c)	testprop(c, MASK(XDIGITBIT))
#define ltolower(c)    check_exp(('A' <= (c) && (c) <= 'Z') || (c) == ((c) | ('A' ^ 'a')),              (c) | ('A' ^ 'a'))//included "limits.h" 
#if defined (LUA_UCID)		
#define NONA		0x01
#else
#define NONA		0x00	
#endif
static const lu_byte luai_ctype_[UCHAR_MAX + 2] = {
  0x00,  
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,	
  0x00,  0x08,  0x08,  0x08,  0x08,  0x08,  0x00,  0x00,
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,	
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
  0x0c,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,	
  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,
  0x16,  0x16,  0x16,  0x16,  0x16,  0x16,  0x16,  0x16,	
  0x16,  0x16,  0x04,  0x04,  0x04,  0x04,  0x04,  0x04,
  0x04,  0x15,  0x15,  0x15,  0x15,  0x15,  0x15,  0x05,	
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,	
  0x05,  0x05,  0x05,  0x04,  0x04,  0x04,  0x04,  0x05,
  0x04,  0x15,  0x15,  0x15,  0x15,  0x15,  0x15,  0x05,	
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,
  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,  0x05,	
  0x05,  0x05,  0x05,  0x04,  0x04,  0x04,  0x04,  0x00,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  0x00,  0x00,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,	
  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,  NONA,
  NONA,  NONA,  NONA,  NONA,  NONA,  0x00,  0x00,  0x00,	
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00
};
#else			
#include <ctype.h>
#define lislalpha(c)	(isalpha(c) || (c) == '_')
#define lislalnum(c)	(isalnum(c) || (c) == '_')
#define lisdigit(c)	(isdigit(c))
#define lisspace(c)	(isspace(c))
#define lisprint(c)	(isprint(c))
#define lisxdigit(c)	(isxdigit(c))
#define ltolower(c)	(tolower(c))
#endif			
#endif
#define next(ls)	(ls->current = zgetc(ls->z))
#define currIsNewline(ls)	(ls->current == '\n' || ls->current == '\r')
static const char *const luaX_tokens [] = {
    "and", "break", "do", "else", "elseif",
    "end", "false", "for", "function", "goto", "if",
    "in", "local", "nil", "not", "or", "repeat",
    "return", "then", "true", "until", "while",
    "//", "..", "...", "==", ">=", "<=", "~=",
    "<<", ">>", "::", "<eof>",
    "<number>", "<integer>", "<name>", "<string>"
};
#define save_and_next(ls) (save(ls, ls->current), next(ls))
static l_noret lexerror (LexState *ls, const char *msg, int token);
static void save (LexState *ls, int c) {
  Mbuffer *b = ls->buff;
  if (luaZ_bufflen(b) + 1 > luaZ_sizebuffer(b)) {
    size_t newsize;
    if (luaZ_sizebuffer(b) >= MAX_SIZE/2)
      lexerror(ls, "lexical element too long", 0);
    newsize = luaZ_sizebuffer(b) * 2;
    luaZ_resizebuffer(ls->L, b, newsize);
  }
  b->buffer[luaZ_bufflen(b)++] = cast_char(c);
}
void luaX_init (lua_State *L) {
  int i;
  TString *e = luaS_newliteral(L, LUA_ENV);  
  luaC_fix(L, obj2gco(e));  
  for (i=0; i<NUM_RESERVED; i++) {
    TString *ts = luaS_new(L, luaX_tokens[i]);
		for (int j = 0; j <= LUA_TOTALTYPES; j++) {
			if (G(L)->typenames[j] == ts) goto skip; // skip then
		}
    luaC_fix(L, obj2gco(ts));  
		skip:
    ts->extra = cast_byte(i+1);  
  }
}
const char *luaX_token2str (LexState *ls, int token) {
  if (token < FIRST_RESERVED) {  
    if (lisprint(token))
      return luaO_pushfstring(ls->L, "'%c'", token);
    else  
      return luaO_pushfstring(ls->L, "'<\\%d>'", token);
  }
  else {
    const char *s = luaX_tokens[token - FIRST_RESERVED];
    if (token < TK_EOS)  
      return luaO_pushfstring(ls->L, "'%s'", s);
    else  
      return s;
  }
}
static const char *txtToken (LexState *ls, int token) {
  switch (token) {
    case TK_NAME: case TK_STRING:
    case TK_FLT: case TK_INT:
      save(ls, '\0');
      return luaO_pushfstring(ls->L, "'%s'", luaZ_buffer(ls->buff));
    default:
      return luaX_token2str(ls, token);
  }
}
static l_noret lexerror (LexState *ls, const char *msg, int token) {
  msg = luaG_addinfo(ls->L, msg, ls->source, ls->linenumber);
  if (token)
    luaO_pushfstring(ls->L, "%s near %s", msg, txtToken(ls, token));
  luaD_throw(ls->L, LUA_ERRSYNTAX);
}
l_noret luaX_syntaxerror (LexState *ls, const char *msg) {
  lexerror(ls, msg, ls->t.token);
}
TString *luaX_newstring (LexState *ls, const char *str, size_t l) {
  lua_State *L = ls->L;
  TString *ts = luaS_newlstr(L, str, l);  
  const TValue *o = luaH_getstr(ls->h, ts);
  if (!ttisnil(o))  
    ts = keystrval(nodefromval(o));  
  else {  
    TValue *stv = s2v(L->top++);  
    setsvalue(L, stv, ts);  
    luaH_finishset(L, ls->h, stv, o, stv);  
    
    luaC_checkGC(L);
    L->top--;  
  }
  return ts;
}
static void inclinenumber (LexState *ls) {
  int old = ls->current;
  lua_assert(currIsNewline(ls));
  next(ls);  
  if (currIsNewline(ls) && ls->current != old)
    next(ls);  
  if (++ls->linenumber >= MAX_INT)
    lexerror(ls, "chunk has too many lines", 0);
}
void luaX_setinput (lua_State *L, LexState *ls, ZIO *z, TString *source,
                    int firstchar) {
  ls->t.token = 0;
  ls->L = L;
  ls->current = firstchar;
  ls->lookahead.token = TK_EOS;  
  ls->z = z;
  ls->fs = NULL;
  ls->linenumber = 1;
  ls->lastline = 1;
  ls->source = source;
  ls->envn = luaS_newliteral(L, LUA_ENV);  
  luaZ_resizebuffer(ls->L, ls->buff, LUA_MINBUFFER);  
}
static int check_next1 (LexState *ls, int c) {
  if (ls->current == c) {
    next(ls);
    return 1;
  }
  else return 0;
}
static int check_next2 (LexState *ls, const char *set) {
  lua_assert(set[2] == '\0');
  if (ls->current == set[0] || ls->current == set[1]) {
    save_and_next(ls);
    return 1;
  }
  else return 0;
}
static int read_numeral (LexState *ls, SemInfo *seminfo) {
  TValue obj;
  const char *expo = "Ee";
  int first = ls->current;
  lua_assert(lisdigit(ls->current));
  save_and_next(ls);
  if (first == '0' && check_next2(ls, "xX"))  
    expo = "Pp";
  for (;;) {
    if (check_next2(ls, expo))  
      check_next2(ls, "-+");  
    else if (lisxdigit(ls->current) || ls->current == '.')  
      save_and_next(ls);
    else break;
  }
  if (lislalpha(ls->current))  
    save_and_next(ls);  
  save(ls, '\0');
  if (luaO_str2num(luaZ_buffer(ls->buff), &obj) == 0)  
    lexerror(ls, "malformed number", TK_FLT);
  if (ttisinteger(&obj)) {
    seminfo->i = ivalue(&obj);
    return TK_INT;
  }
  else {
    lua_assert(ttisfloat(&obj));
    seminfo->r = fltvalue(&obj);
    return TK_FLT;
  }
}
static size_t skip_sep (LexState *ls) {
  size_t count = 0;
  int s = ls->current;
  lua_assert(s == '[' || s == ']');
  save_and_next(ls);
  while (ls->current == '=') {
    save_and_next(ls);
    count++;
  }
  return (ls->current == s) ? count + 2
         : (count == 0) ? 1
         : 0;
}
static void read_long_string (LexState *ls, SemInfo *seminfo, size_t sep) {
  int line = ls->linenumber;  
  save_and_next(ls);  
  if (currIsNewline(ls))  
    inclinenumber(ls);  
  for (;;) {
    switch (ls->current) {
      case EOZ: {  
        const char *what = (seminfo ? "string" : "comment");
        const char *msg = luaO_pushfstring(ls->L,
                     "unfinished long %s (starting at line %d)", what, line);
        lexerror(ls, msg, TK_EOS);
        break;  
      }
      case ']': {
        if (skip_sep(ls) == sep) {
          save_and_next(ls);  
          goto endloop;
        }
        break;
      }
      case '\n': case '\r': {
        save(ls, '\n');
        inclinenumber(ls);
        if (!seminfo) luaZ_resetbuffer(ls->buff);  
        break;
      }
      default: {
        if (seminfo) save_and_next(ls);
        else next(ls);
      }
    }
  } endloop:
  if (seminfo)
    seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + sep,
                                     luaZ_bufflen(ls->buff) - 2 * sep);
}
static void esccheck (LexState *ls, int c, const char *msg) {
  if (!c) {
    if (ls->current != EOZ)
      save_and_next(ls);  
    lexerror(ls, msg, TK_STRING);
  }
}
static int gethexa (LexState *ls) {
  save_and_next(ls);
  esccheck (ls, lisxdigit(ls->current), "hexadecimal digit expected");
  return luaO_hexavalue(ls->current);
}
static int readhexaesc (LexState *ls) {
  int r = gethexa(ls);
  r = (r << 4) + gethexa(ls);
  luaZ_buffremove(ls->buff, 2);  
  return r;
}
static unsigned long readutf8esc (LexState *ls) {
  unsigned long r;
  int i = 4;  
  save_and_next(ls);  
  esccheck(ls, ls->current == '{', "missing '{'");
  r = gethexa(ls);  
  while (cast_void(save_and_next(ls)), lisxdigit(ls->current)) {
    i++;
    esccheck(ls, r <= (0x7FFFFFFFu >> 4), "UTF-8 value too large");
    r = (r << 4) + luaO_hexavalue(ls->current);
  }
  esccheck(ls, ls->current == '}', "missing '}'");
  next(ls);  
  luaZ_buffremove(ls->buff, i);  
  return r;
}
static void utf8esc (LexState *ls) {
  char buff[UTF8BUFFSZ];
  int n = luaO_utf8esc(buff, readutf8esc(ls));
  for (; n > 0; n--)  
    save(ls, buff[UTF8BUFFSZ - n]);
}
static int readdecesc (LexState *ls) {
  int i;
  int r = 0;  
  for (i = 0; i < 3 && lisdigit(ls->current); i++) {  
    r = 10*r + ls->current - '0';
    save_and_next(ls);
  }
  esccheck(ls, r <= UCHAR_MAX, "decimal escape too large");
  luaZ_buffremove(ls->buff, i);  
  return r;
}
static void read_string (LexState *ls, int del, SemInfo *seminfo) {
  save_and_next(ls);  
  while (ls->current != del) {
    switch (ls->current) {
      case EOZ:
        lexerror(ls, "unfinished string", TK_EOS);
        break;  
      case '\n':
      case '\r':
        lexerror(ls, "unfinished string", TK_STRING);
        break;  
      case '\\': {  
        int c;  
        save_and_next(ls);  
        switch (ls->current) {
          case 'a': c = '\a'; goto read_save;
          case 'b': c = '\b'; goto read_save;
          case 'f': c = '\f'; goto read_save;
          case 'n': c = '\n'; goto read_save;
          case 'r': c = '\r'; goto read_save;
          case 't': c = '\t'; goto read_save;
          case 'v': c = '\v'; goto read_save;
          case 'x': c = readhexaesc(ls); goto read_save;
          case 'u': utf8esc(ls);  goto no_save;
          case '\n': case '\r':
            inclinenumber(ls); c = '\n'; goto only_save;
          case '\\': case '\"': case '\'':
            c = ls->current; goto read_save;
          case EOZ: goto no_save;  
          case 'z': {  
            luaZ_buffremove(ls->buff, 1);  
            next(ls);  
            while (lisspace(ls->current)) {
              if (currIsNewline(ls)) inclinenumber(ls);
              else next(ls);
            }
            goto no_save;
          }
          default: {
            esccheck(ls, lisdigit(ls->current), "invalid escape sequence");
            c = readdecesc(ls);  
            goto only_save;
          }
        }
       read_save:
         next(ls);
         
       only_save:
         luaZ_buffremove(ls->buff, 1);  
         save(ls, c);
         
       no_save: break;
      }
      default:
        save_and_next(ls);
    }
  }
  save_and_next(ls);  
  seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + 1,
                                   luaZ_bufflen(ls->buff) - 2);
}
static int llex (LexState *ls, SemInfo *seminfo) {
  luaZ_resetbuffer(ls->buff);
  for (;;) {
    switch (ls->current) {
      case '\n': case '\r': {  
        inclinenumber(ls);
        break;
      }
      case ' ': case '\f': case '\t': case '\v': {  
        next(ls);
        break;
      }
      case '-': {  
        next(ls);
        if (ls->current != '-') return '-';
        
        next(ls);
        if (ls->current == '[') {  
          size_t sep = skip_sep(ls);
          luaZ_resetbuffer(ls->buff);  
          if (sep >= 2) {
            read_long_string(ls, NULL, sep);  
            luaZ_resetbuffer(ls->buff);  
            break;
          }
        }
        
        while (!currIsNewline(ls) && ls->current != EOZ)
          next(ls);  
        break;
      }
      case '[': {  
        size_t sep = skip_sep(ls);
        if (sep >= 2) {
          read_long_string(ls, seminfo, sep);
          return TK_STRING;
        }
        else if (sep == 0)  
          lexerror(ls, "invalid long string delimiter", TK_STRING);
        return '[';
      }
      case '=': {
        next(ls);
        if (check_next1(ls, '=')) return TK_EQ;  
        else return '=';
      }
      case '<': {
        next(ls);
        if (check_next1(ls, '=')) return TK_LE;  
        else if (check_next1(ls, '<')) return TK_SHL;  
        else return '<';
      }
      case '>': {
        next(ls);
        if (check_next1(ls, '=')) return TK_GE;  
        else if (check_next1(ls, '>')) return TK_SHR;  
        else return '>';
      }
      case '/': {
        next(ls);
        if (check_next1(ls, '/')) return TK_IDIV;  
        else return '/';
      }
      case '~': {
        next(ls);
        if (check_next1(ls, '=')) return TK_NE;  
        else return '~';
      }
      case ':': {
        next(ls);
        if (check_next1(ls, ':')) return TK_DBCOLON;  
        else return ':';
      }
      case '"': case '\'': {  
        read_string(ls, ls->current, seminfo);
        return TK_STRING;
      }
      case '.': {  
        save_and_next(ls);
        if (check_next1(ls, '.')) {
          if (check_next1(ls, '.'))
            return TK_DOTS;   
          else return TK_CONCAT;   
        }
        else if (!lisdigit(ls->current)) return '.';
        else return read_numeral(ls, seminfo);
      }
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9': {
        return read_numeral(ls, seminfo);
      }
      case EOZ: {
        return TK_EOS;
      }
      default: {
        if (lislalpha(ls->current)) {  
          TString *ts;
          do {
            save_and_next(ls);
          } while (lislalnum(ls->current));
          ts = luaX_newstring(ls, luaZ_buffer(ls->buff),
                                  luaZ_bufflen(ls->buff));
          seminfo->ts = ts;
          if (isreserved(ts))  
            return ts->extra - 1 + FIRST_RESERVED;
          else {
            return TK_NAME;
          }
        }
        else {  
          int c = ls->current;
          next(ls);
          return c;
        }
      }
    }
  }
}
void luaX_next (LexState *ls) {
  ls->lastline = ls->linenumber;
  if (ls->lookahead.token != TK_EOS) {  
    ls->t = ls->lookahead;  
    ls->lookahead.token = TK_EOS;  
  }
  else
    ls->t.token = llex(ls, &ls->t.seminfo);  
}
int luaX_lookahead (LexState *ls) {
  lua_assert(ls->lookahead.token == TK_EOS);
  ls->lookahead.token = llex(ls, &ls->lookahead.seminfo);
  return ls->lookahead.token;
}
// root include lmem.c
//included "stddef.h" 
#if defined(EMERGENCYGCTESTS)
static void *firsttry (global_State *g, void *block, size_t os, size_t ns) {
  if (completestate(g) && ns > 0)  
    return NULL;  
  else  
    return (*g->frealloc)(g->ud, block, os, ns);
}
#else
#define firsttry(g,block,os,ns)    ((*g->frealloc)(g->ud, block, os, ns))
#endif
#define MINSIZEARRAY	4
void *luaM_growaux_ (lua_State *L, void *block, int nelems, int *psize,
                     int size_elems, int limit, const char *what) {
  void *newblock;
  int size = *psize;
  if (nelems + 1 <= size)  
    return block;  
  if (size >= limit / 2) {  
    if (l_unlikely(size >= limit))  
      luaG_runerror(L, "too many %s (limit is %d)", what, limit);
    size = limit;  
  }
  else {
    size *= 2;
    if (size < MINSIZEARRAY)
      size = MINSIZEARRAY;  
  }
  lua_assert(nelems + 1 <= size && size <= limit);
  
  newblock = luaM_saferealloc_(L, block, cast_sizet(*psize) * size_elems,
                                         cast_sizet(size) * size_elems);
  *psize = size;  
  return newblock;
}
void *luaM_shrinkvector_ (lua_State *L, void *block, int *size,
                          int final_n, int size_elem) {
  void *newblock;
  size_t oldsize = cast_sizet((*size) * size_elem);
  size_t newsize = cast_sizet(final_n * size_elem);
  lua_assert(newsize <= oldsize);
  newblock = luaM_saferealloc_(L, block, oldsize, newsize);
  *size = final_n;
  return newblock;
}
l_noret luaM_toobig (lua_State *L) {
  luaG_runerror(L, "memory allocation error: block too big");
}
void luaM_free_ (lua_State *L, void *block, size_t osize) {
  global_State *g = G(L);
  lua_assert((osize == 0) == (block == NULL));
  (*g->frealloc)(g->ud, block, osize, 0);
  g->GCdebt -= osize;
}
static void *tryagain (lua_State *L, void *block,
                       size_t osize, size_t nsize) {
  global_State *g = G(L);
  if (completestate(g) && !g->gcstopem) {
    luaC_fullgc(L, 1);  
    return (*g->frealloc)(g->ud, block, osize, nsize);  
  }
  else return NULL;  
}
void *luaM_realloc_ (lua_State *L, void *block, size_t osize, size_t nsize) {
  void *newblock;
  global_State *g = G(L);
  lua_assert((osize == 0) == (block == NULL));
  newblock = firsttry(g, block, osize, nsize);
  if (l_unlikely(newblock == NULL && nsize > 0)) {
    newblock = tryagain(L, block, osize, nsize);
    if (newblock == NULL)  
      return NULL;  
  }
  lua_assert((nsize == 0) == (newblock == NULL));
  g->GCdebt = (g->GCdebt + nsize) - osize;
  return newblock;
}
void *luaM_saferealloc_ (lua_State *L, void *block, size_t osize,
                                                    size_t nsize) {
  void *newblock = luaM_realloc_(L, block, osize, nsize);
  if (l_unlikely(newblock == NULL && nsize > 0))  
    luaM_error(L);
  return newblock;
}
void *luaM_malloc_ (lua_State *L, size_t size, int tag) {
  if (size == 0)
    return NULL;  
  else {
    global_State *g = G(L);
    void *newblock = firsttry(g, NULL, tag, size);
    if (l_unlikely(newblock == NULL)) {
      newblock = tryagain(L, NULL, tag, size);
      if (newblock == NULL)
        luaM_error(L);
    }
    g->GCdebt += size;
    return newblock;
  }
}
// root include lobject.c
//included "locale.h" 
//included "math.h" 
//included "stdarg.h" 
//included "stdio.h" 
//included "stdlib.h" 
//included "string.h" 
//included "lctype.h" 
int luaO_ceillog2 (unsigned int x) {
  static const lu_byte log_2[256] = {  
    0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
  };
  int l = 0;
  x--;
  while (x >= 256) { l += 8; x >>= 8; }
  return l + log_2[x];
}
static lua_Integer intarith (lua_State *L, int op, lua_Integer v1,
                                                   lua_Integer v2) {
  switch (op) {
    case LUA_OPADD: return intop(+, v1, v2);
    case LUA_OPSUB:return intop(-, v1, v2);
    case LUA_OPMUL:return intop(*, v1, v2);
    case LUA_OPMOD: return luaV_mod(L, v1, v2);
    case LUA_OPIDIV: return luaV_idiv(L, v1, v2);
    case LUA_OPBAND: return intop(&, v1, v2);
    case LUA_OPBOR: return intop(|, v1, v2);
    case LUA_OPBXOR: return intop(^, v1, v2);
    case LUA_OPSHL: return luaV_shiftl(v1, v2);
    case LUA_OPSHR: return luaV_shiftl(v1, -v2);
    case LUA_OPUNM: return intop(-, 0, v1);
    case LUA_OPBNOT: return intop(^, ~l_castS2U(0), v1);
    default: luai_unreachable(); return 0;
  }
}
static lua_Number numarith (lua_State *L, int op, lua_Number v1,
                                                  lua_Number v2) {
  switch (op) {
    case LUA_OPADD: return luai_numadd(L, v1, v2);
    case LUA_OPSUB: return luai_numsub(L, v1, v2);
    case LUA_OPMUL: return luai_nummul(L, v1, v2);
    case LUA_OPDIV: return luai_numdiv(L, v1, v2);
    case LUA_OPPOW: return luai_numpow(L, v1, v2);
    case LUA_OPIDIV: return luai_numidiv(L, v1, v2);
    case LUA_OPUNM: return luai_numunm(L, v1);
    case LUA_OPMOD: return luaV_modf(L, v1, v2);
    default: luai_unreachable(); return 0;
  }
}
int luaO_rawarith (lua_State *L, int op, const TValue *p1, const TValue *p2,
                   TValue *res) {
  switch (op) {
    case LUA_OPBAND: case LUA_OPBOR: case LUA_OPBXOR:
    case LUA_OPSHL: case LUA_OPSHR:
    case LUA_OPBNOT: {  
      lua_Integer i1; lua_Integer i2;
      if (tointegerns(p1, &i1) && tointegerns(p2, &i2)) {
        setivalue(res, intarith(L, op, i1, i2));
        return 1;
      }
      else return 0;  
    }
    case LUA_OPDIV: case LUA_OPPOW: {  
      lua_Number n1; lua_Number n2;
      if (tonumberns(p1, n1) && tonumberns(p2, n2)) {
        setfltvalue(res, numarith(L, op, n1, n2));
        return 1;
      }
      else return 0;  
    }
    default: {  
      lua_Number n1; lua_Number n2;
      if (ttisinteger(p1) && ttisinteger(p2)) {
        setivalue(res, intarith(L, op, ivalue(p1), ivalue(p2)));
        return 1;
      }
      else if (tonumberns(p1, n1) && tonumberns(p2, n2)) {
        setfltvalue(res, numarith(L, op, n1, n2));
        return 1;
      }
      else return 0;  
    }
  }
}
void luaO_arith (lua_State *L, int op, const TValue *p1, const TValue *p2,
                 StkId res) {
  if (!luaO_rawarith(L, op, p1, p2, s2v(res))) {
    
    luaT_trybinTM(L, p1, p2, res, cast(TMS, (op - LUA_OPADD) + TM_ADD));
  }
}
int luaO_hexavalue (int c) {
  if (lisdigit(c)) return c - '0';
  else return (ltolower(c) - 'a') + 10;
}
static int isneg (const char **s) {
  if (**s == '-') { (*s)++; return 1; }
  else if (**s == '+') (*s)++;
  return 0;
}
#if !defined(lua_strx2number)
#define MAXSIGDIG	30
static lua_Number lua_strx2number (const char *s, char **endptr) {
  int dot = lua_getlocaledecpoint();
  lua_Number r = l_mathop(0.0);  
  int sigdig = 0;  
  int nosigdig = 0;  
  int e = 0;  
  int neg;  
  int hasdot = 0;  
  *endptr = cast_charp(s);  
  while (lisspace(cast_uchar(*s))) s++;  
  neg = isneg(&s);  
  if (!(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')))  
    return l_mathop(0.0);  
  for (s += 2; ; s++) {  
    if (*s == dot) {
      if (hasdot) break;  
      else hasdot = 1;
    }
    else if (lisxdigit(cast_uchar(*s))) {
      if (sigdig == 0 && *s == '0')  
        nosigdig++;
      else if (++sigdig <= MAXSIGDIG)  
          r = (r * l_mathop(16.0)) + luaO_hexavalue(*s);
      else e++; 
      if (hasdot) e--;  
    }
    else break;  
  }
  if (nosigdig + sigdig == 0)  
    return l_mathop(0.0);  
  *endptr = cast_charp(s);  
  e *= 4;  
  if (*s == 'p' || *s == 'P') {  
    int exp1 = 0;  
    int neg1;  
    s++;  
    neg1 = isneg(&s);  
    if (!lisdigit(cast_uchar(*s)))
      return l_mathop(0.0);  
    while (lisdigit(cast_uchar(*s)))  
      exp1 = exp1 * 10 + *(s++) - '0';
    if (neg1) exp1 = -exp1;
    e += exp1;
    *endptr = cast_charp(s);  
  }
  if (neg) r = -r;
  return l_mathop(ldexp)(r, e);
}
#endif
#if !defined (L_MAXLENNUM)
#define L_MAXLENNUM	200
#endif
static const char *l_str2dloc (const char *s, lua_Number *result, int mode) {
  char *endptr;
  *result = (mode == 'x') ? lua_strx2number(s, &endptr)  
                          : lua_str2number(s, &endptr);
  if (endptr == s) return NULL;  
  while (lisspace(cast_uchar(*endptr))) endptr++;  
  return (*endptr == '\0') ? endptr : NULL;  
}
static const char *l_str2d (const char *s, lua_Number *result) {
  const char *endptr;
  const char *pmode = strpbrk(s, ".xXnN");  
  int mode = pmode ? ltolower(cast_uchar(*pmode)) : 0;
  if (mode == 'n')  
    return NULL;
  endptr = l_str2dloc(s, result, mode);  
  if (endptr == NULL) {  
    char buff[L_MAXLENNUM + 1];
    const char *pdot = strchr(s, '.');
    if (pdot == NULL || strlen(s) > L_MAXLENNUM)
      return NULL;  
    strcpy(buff, s);  
    buff[pdot - s] = lua_getlocaledecpoint();  
    endptr = l_str2dloc(buff, result, mode);  
    if (endptr != NULL)
      endptr = s + (endptr - buff);  
  }
  return endptr;
}
#define MAXBY10		cast(lua_Unsigned, LUA_MAXINTEGER / 10)
#define MAXLASTD	cast_int(LUA_MAXINTEGER % 10)
static const char *l_str2int (const char *s, lua_Integer *result) {
  lua_Unsigned a = 0;
  int empty = 1;
  int neg;
  while (lisspace(cast_uchar(*s))) s++;  
  neg = isneg(&s);
  if (s[0] == '0' &&
      (s[1] == 'x' || s[1] == 'X')) {  
    s += 2;  
    for (; lisxdigit(cast_uchar(*s)); s++) {
      a = a * 16 + luaO_hexavalue(*s);
      empty = 0;
    }
  }
  else {  
    for (; lisdigit(cast_uchar(*s)); s++) {
      int d = *s - '0';
      if (a >= MAXBY10 && (a > MAXBY10 || d > MAXLASTD + neg))  
        return NULL;  
      a = a * 10 + d;
      empty = 0;
    }
  }
  while (lisspace(cast_uchar(*s))) s++;  
  if (empty || *s != '\0') return NULL;  
  else {
    *result = l_castU2S((neg) ? 0u - a : a);
    return s;
  }
}
size_t luaO_str2num (const char *s, TValue *o) {
  lua_Integer i; lua_Number n;
  const char *e;
  if ((e = l_str2int(s, &i)) != NULL) {  
    setivalue(o, i);
  }
  else if ((e = l_str2d(s, &n)) != NULL) {  
    setfltvalue(o, n);
  }
  else
    return 0;  
  return (e - s) + 1;  
}
int luaO_utf8esc (char *buff, unsigned long x) {
  int n = 1;  
  lua_assert(x <= 0x7FFFFFFFu);
  if (x < 0x80)  
    buff[UTF8BUFFSZ - 1] = cast_char(x);
  else {  
    unsigned int mfb = 0x3f;  
    do {  
      buff[UTF8BUFFSZ - (n++)] = cast_char(0x80 | (x & 0x3f));
      x >>= 6;  
      mfb >>= 1;  
    } while (x > mfb);  
    buff[UTF8BUFFSZ - n] = cast_char((~mfb << 1) | x);  
  }
  return n;
}
#define MAXNUMBER2STR	44
static int tostringbuff (TValue *obj, char *buff) {
  int len;
  lua_assert(ttisnumber(obj));
  if (ttisinteger(obj))
    len = lua_integer2str(buff, MAXNUMBER2STR, ivalue(obj));
  else {
    len = lua_number2str(buff, MAXNUMBER2STR, fltvalue(obj));
    if (buff[strspn(buff, "-0123456789")] == '\0') {  
      buff[len++] = lua_getlocaledecpoint();
      buff[len++] = '0';  
    }
  }
  return len;
}
void luaO_tostring (lua_State *L, TValue *obj) {
  char buff[MAXNUMBER2STR];
  int len = tostringbuff(obj, buff);
  setsvalue(L, obj, luaS_newlstr(L, buff, len));
}
#define BUFVFS		200
typedef struct BuffFS {
  lua_State *L;
  int pushed;  
  int blen;  
  char space[BUFVFS];  
} BuffFS;
static void pushstr (BuffFS *buff, const char *str, size_t l) {
  lua_State *L = buff->L;
  setsvalue2s(L, L->top, luaS_newlstr(L, str, l));
  L->top++;  
  buff->pushed++;
  luaV_concat(L, buff->pushed);  
  buff->pushed = 1;
}
static void clearbuff (BuffFS *buff) {
  pushstr(buff, buff->space, buff->blen);  
  buff->blen = 0;  
}
static char *getbuff (BuffFS *buff, int sz) {
  lua_assert(buff->blen <= BUFVFS); lua_assert(sz <= BUFVFS);
  if (sz > BUFVFS - buff->blen)  
    clearbuff(buff);
  return buff->space + buff->blen;
}
#define addsize(b,sz)	((b)->blen += (sz))
static void addstr2buff (BuffFS *buff, const char *str, size_t slen) {
  if (slen <= BUFVFS) {  
    char *bf = getbuff(buff, cast_int(slen));
    memcpy(bf, str, slen);  
    addsize(buff, cast_int(slen));
  }
  else {  
    clearbuff(buff);  
    pushstr(buff, str, slen);  
  }
}
static void addnum2buff (BuffFS *buff, TValue *num) {
  char *numbuff = getbuff(buff, MAXNUMBER2STR);
  int len = tostringbuff(num, numbuff);  
  addsize(buff, len);
}
const char *luaO_pushvfstring (lua_State *L, const char *fmt, va_list argp) {
  BuffFS buff;  
  const char *e;  
  buff.pushed = buff.blen = 0;
  buff.L = L;
  while ((e = strchr(fmt, '%')) != NULL) {
    addstr2buff(&buff, fmt, e - fmt);  
    switch (*(e + 1)) {  
      case 's': {  
        const char *s = va_arg(argp, char *);
        if (s == NULL) s = "(null)";
        addstr2buff(&buff, s, strlen(s));
        break;
      }
      case 'c': {  
        char c = cast_uchar(va_arg(argp, int));
        addstr2buff(&buff, &c, sizeof(char));
        break;
      }
      case 'd': {  
        TValue num;
        setivalue(&num, va_arg(argp, int));
        addnum2buff(&buff, &num);
        break;
      }
      case 'I': {  
        TValue num;
        setivalue(&num, cast(lua_Integer, va_arg(argp, l_uacInt)));
        addnum2buff(&buff, &num);
        break;
      }
      case 'f': {  
        TValue num;
        setfltvalue(&num, cast_num(va_arg(argp, l_uacNumber)));
        addnum2buff(&buff, &num);
        break;
      }
      case 'p': {  
        const int sz = 3 * sizeof(void*) + 8; 
        char *bf = getbuff(&buff, sz);
        void *p = va_arg(argp, void *);
        int len = lua_pointer2str(bf, sz, p);
        addsize(&buff, len);
        break;
      }
      case 'U': {  
        char bf[UTF8BUFFSZ];
        int len = luaO_utf8esc(bf, va_arg(argp, long));
        addstr2buff(&buff, bf + UTF8BUFFSZ - len, len);
        break;
      }
      case '%': {
        addstr2buff(&buff, "%", 1);
        break;
      }
      default: {
        luaG_runerror(L, "invalid option '%%%c' to 'lua_pushfstring'",
                         *(e + 1));
      }
    }
    fmt = e + 2;  
  }
  addstr2buff(&buff, fmt, strlen(fmt));  
  clearbuff(&buff);  
  lua_assert(buff.pushed == 1);
  return svalue(s2v(L->top - 1));
}
const char *luaO_pushfstring (lua_State *L, const char *fmt, ...) {
  const char *msg;
  va_list argp;
  va_start(argp, fmt);
  msg = luaO_pushvfstring(L, fmt, argp);
  va_end(argp);
  return msg;
}
#define RETS	"..."
#define PRE	"[string \""
#define POS	"\"]"
#define addstr(a,b,l)	( memcpy(a,b,(l) * sizeof(char)), a += (l) )
void luaO_chunkid (char *out, const char *source, size_t srclen) {
  size_t bufflen = LUA_IDSIZE;  
  if (*source == '=') {  
    if (srclen <= bufflen)  
      memcpy(out, source + 1, srclen * sizeof(char));
    else {  
      addstr(out, source + 1, bufflen - 1);
      *out = '\0';
    }
  }
  else if (*source == '@') {  
    if (srclen <= bufflen)  
      memcpy(out, source + 1, srclen * sizeof(char));
    else {  
      addstr(out, RETS, LL(RETS));
      bufflen -= LL(RETS);
      memcpy(out, source + 1 + srclen - bufflen, bufflen * sizeof(char));
    }
  }
  else {  
    const char *nl = strchr(source, '\n');  
    addstr(out, PRE, LL(PRE));  
    bufflen -= LL(PRE RETS POS) + 1;  
    if (srclen < bufflen && nl == NULL) {  
      addstr(out, source, srclen);  
    }
    else {
      if (nl != NULL) srclen = nl - source;  
      if (srclen > bufflen) srclen = bufflen;
      addstr(out, source, srclen);
      addstr(out, RETS, LL(RETS));
    }
    memcpy(out, POS, (LL(POS) + 1) * sizeof(char));
  }
}
// root include lopcodes.c
// root include lparser.c
//included "limits.h" 
//included "string.h" 
#define MAXVARS		200
#define hasmultret(k)		((k) == VCALL || (k) == VVARARG)
#define eqstr(a,b)	((a) == (b))
typedef struct BlockCnt {
  struct BlockCnt *previous;  
  int firstlabel;  
  int firstgoto;  
  lu_byte nactvar;  
  lu_byte upval;  
  lu_byte isloop;  
  lu_byte insidetbc;  
} BlockCnt;
static void statement (LexState *ls);
static void expr (LexState *ls, expdesc *v);
static l_noret error_expected (LexState *ls, int token) {
  luaX_syntaxerror(ls,
      luaO_pushfstring(ls->L, "%s expected", luaX_token2str(ls, token)));
}
static l_noret errorlimit (FuncState *fs, int limit, const char *what) {
  lua_State *L = fs->ls->L;
  const char *msg;
  int line = fs->f->linedefined;
  const char *where = (line == 0)
                      ? "main function"
                      : luaO_pushfstring(L, "function at line %d", line);
  msg = luaO_pushfstring(L, "too many %s (limit is %d) in %s",
                             what, limit, where);
  luaX_syntaxerror(fs->ls, msg);
}
static void checklimit (FuncState *fs, int v, int l, const char *what) {
  if (v > l) errorlimit(fs, l, what);
}
static int testnext (LexState *ls, int c) {
  if (ls->t.token == c) {
    luaX_next(ls);
    return 1;
  }
  else return 0;
}
static void check (LexState *ls, int c) {
  if (ls->t.token != c)
    error_expected(ls, c);
}
static void checknext (LexState *ls, int c) {
  check(ls, c);
  luaX_next(ls);
}
#define check_condition(ls,c,msg)	{ if (!(c)) luaX_syntaxerror(ls, msg); }
static void check_match (LexState *ls, int what, int who, int where) {
  if (l_unlikely(!testnext(ls, what))) {
    if (where == ls->linenumber)  
      error_expected(ls, what);  
    else {
      luaX_syntaxerror(ls, luaO_pushfstring(ls->L,
             "%s expected (to close %s at line %d)",
              luaX_token2str(ls, what), luaX_token2str(ls, who), where));
    }
  }
}
static TString *str_checkname (LexState *ls) {
  TString *ts;
  check(ls, TK_NAME);
  ts = ls->t.seminfo.ts;
  luaX_next(ls);
  return ts;
}
static void init_exp (expdesc *e, expkind k, int i) {
  e->f = e->t = NO_JUMP;
  e->k = k;
  e->u.info = i;
}
static void codestring (expdesc *e, TString *s) {
  e->f = e->t = NO_JUMP;
  e->k = VKSTR;
  e->u.strval = s;
}
static void codename (LexState *ls, expdesc *e) {
  codestring(e, str_checkname(ls));
}
static int registerlocalvar (LexState *ls, FuncState *fs, TString *varname) {
  Proto *f = fs->f;
  int oldsize = f->sizelocvars;
  luaM_growvector(ls->L, f->locvars, fs->ndebugvars, f->sizelocvars,
                  LocVar, SHRT_MAX, "local variables");
  while (oldsize < f->sizelocvars)
    f->locvars[oldsize++].varname = NULL;
  f->locvars[fs->ndebugvars].varname = varname;
  f->locvars[fs->ndebugvars].startpc = fs->pc;
  luaC_objbarrier(ls->L, f, varname);
  return fs->ndebugvars++;
}
static int new_localvar (LexState *ls, TString *name) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Dyndata *dyd = ls->dyd;
  Vardesc *var;
  checklimit(fs, dyd->actvar.n + 1 - fs->firstlocal,
                 MAXVARS, "local variables");
  luaM_growvector(L, dyd->actvar.arr, dyd->actvar.n + 1,
                  dyd->actvar.size, Vardesc, USHRT_MAX, "local variables");
  var = &dyd->actvar.arr[dyd->actvar.n++];
  var->vd.kind = VDKREG;  
  var->vd.name = name;
  return dyd->actvar.n - 1 - fs->firstlocal;
}
#define new_localvarliteral(ls,v)     new_localvar(ls,        luaX_newstring(ls, "" v, (sizeof(v)/sizeof(char)) - 1));
static Vardesc *getlocalvardesc (FuncState *fs, int vidx) {
  return &fs->ls->dyd->actvar.arr[fs->firstlocal + vidx];
}
static int reglevel (FuncState *fs, int nvar) {
  while (nvar-- > 0) {
    Vardesc *vd = getlocalvardesc(fs, nvar);  
    if (vd->vd.kind != RDKCTC)  
      return vd->vd.ridx + 1;
  }
  return 0;  
}
int luaY_nvarstack (FuncState *fs) {
  return reglevel(fs, fs->nactvar);
}
static LocVar *localdebuginfo (FuncState *fs, int vidx) {
  Vardesc *vd = getlocalvardesc(fs,  vidx);
  if (vd->vd.kind == RDKCTC)
    return NULL;  
  else {
    int idx = vd->vd.pidx;
    lua_assert(idx < fs->ndebugvars);
    return &fs->f->locvars[idx];
  }
}
static void init_var (FuncState *fs, expdesc *e, int vidx) {
  e->f = e->t = NO_JUMP;
  e->k = VLOCAL;
  e->u.var.vidx = vidx;
  e->u.var.ridx = getlocalvardesc(fs, vidx)->vd.ridx;
}
static void check_readonly (LexState *ls, expdesc *e) {
  FuncState *fs = ls->fs;
  TString *varname = NULL;  
  switch (e->k) {
    case VCONST: {
      varname = ls->dyd->actvar.arr[e->u.info].vd.name;
      break;
    }
    case VLOCAL: {
      Vardesc *vardesc = getlocalvardesc(fs, e->u.var.vidx);
      if (vardesc->vd.kind != VDKREG)  
        varname = vardesc->vd.name;
      break;
    }
    case VUPVAL: {
      Upvaldesc *up = &fs->f->upvalues[e->u.info];
      if (up->kind != VDKREG)
        varname = up->name;
      break;
    }
    default:
      return;  
  }
  if (varname) {
    const char *msg = luaO_pushfstring(ls->L,
       "attempt to assign to const variable '%s'", getstr(varname));
    luaK_semerror(ls, msg);  
  }
}
static void adjustlocalvars (LexState *ls, int nvars) {
  FuncState *fs = ls->fs;
  int reglevel = luaY_nvarstack(fs);
  int i;
  for (i = 0; i < nvars; i++) {
    int vidx = fs->nactvar++;
    Vardesc *var = getlocalvardesc(fs, vidx);
    var->vd.ridx = reglevel++;
    var->vd.pidx = registerlocalvar(ls, fs, var->vd.name);
  }
}
static void removevars (FuncState *fs, int tolevel) {
  fs->ls->dyd->actvar.n -= (fs->nactvar - tolevel);
  while (fs->nactvar > tolevel) {
    LocVar *var = localdebuginfo(fs, --fs->nactvar);
    if (var)  
      var->endpc = fs->pc;
  }
}
static int searchupvalue (FuncState *fs, TString *name) {
  int i;
  Upvaldesc *up = fs->f->upvalues;
  for (i = 0; i < fs->nups; i++) {
    if (eqstr(up[i].name, name)) return i;
  }
  return -1;  
}
static Upvaldesc *allocupvalue (FuncState *fs) {
  Proto *f = fs->f;
  int oldsize = f->sizeupvalues;
  checklimit(fs, fs->nups + 1, MAXUPVAL, "upvalues");
  luaM_growvector(fs->ls->L, f->upvalues, fs->nups, f->sizeupvalues,
                  Upvaldesc, MAXUPVAL, "upvalues");
  while (oldsize < f->sizeupvalues)
    f->upvalues[oldsize++].name = NULL;
  return &f->upvalues[fs->nups++];
}
static int newupvalue (FuncState *fs, TString *name, expdesc *v) {
  Upvaldesc *up = allocupvalue(fs);
  FuncState *prev = fs->prev;
  if (v->k == VLOCAL) {
    up->instack = 1;
    up->idx = v->u.var.ridx;
    up->kind = getlocalvardesc(prev, v->u.var.vidx)->vd.kind;
    lua_assert(eqstr(name, getlocalvardesc(prev, v->u.var.vidx)->vd.name));
  }
  else {
    up->instack = 0;
    up->idx = cast_byte(v->u.info);
    up->kind = prev->f->upvalues[v->u.info].kind;
    lua_assert(eqstr(name, prev->f->upvalues[v->u.info].name));
  }
  up->name = name;
  luaC_objbarrier(fs->ls->L, fs->f, name);
  return fs->nups - 1;
}
static int searchvar (FuncState *fs, TString *n, expdesc *var) {
  int i;
  for (i = cast_int(fs->nactvar) - 1; i >= 0; i--) {
    Vardesc *vd = getlocalvardesc(fs, i);
    if (eqstr(n, vd->vd.name)) {  
      if (vd->vd.kind == RDKCTC)  
        init_exp(var, VCONST, fs->firstlocal + i);
      else  
        init_var(fs, var, i);
      return var->k;
    }
  }
  return -1;  
}
static void markupval (FuncState *fs, int level) {
  BlockCnt *bl = fs->bl;
  while (bl->nactvar > level)
    bl = bl->previous;
  bl->upval = 1;
  fs->needclose = 1;
}
static void marktobeclosed (FuncState *fs) {
  BlockCnt *bl = fs->bl;
  bl->upval = 1;
  bl->insidetbc = 1;
  fs->needclose = 1;
}
static void singlevaraux (FuncState *fs, TString *n, expdesc *var, int base) {
  if (fs == NULL)  
    init_exp(var, VVOID, 0);  
  else {
    int v = searchvar(fs, n, var);  
    if (v >= 0) {  
      if (v == VLOCAL && !base)
        markupval(fs, var->u.var.vidx);  
    }
    else {  
      int idx = searchupvalue(fs, n);  
      if (idx < 0) {  
        singlevaraux(fs->prev, n, var, 0);  
        if (var->k == VLOCAL || var->k == VUPVAL)  
          idx  = newupvalue(fs, n, var);  
        else  
          return;  
      }
      init_exp(var, VUPVAL, idx);  
    }
  }
}
static void singlevar (LexState *ls, expdesc *var) {
  TString *varname = str_checkname(ls);
  FuncState *fs = ls->fs;
  singlevaraux(fs, varname, var, 1);
  if (var->k == VVOID) {  
    expdesc key;
    singlevaraux(fs, ls->envn, var, 1);  
    lua_assert(var->k != VVOID);  
    codestring(&key, varname);  
    luaK_indexed(fs, var, &key);  
  }
}
static void adjust_assign (LexState *ls, int nvars, int nexps, expdesc *e) {
  FuncState *fs = ls->fs;
  int needed = nvars - nexps;  
  if (hasmultret(e->k)) {  
    int extra = needed + 1;  
    if (extra < 0)
      extra = 0;
    luaK_setreturns(fs, e, extra);  
  }
  else {
    if (e->k != VVOID)  
      luaK_exp2nextreg(fs, e);  
    if (needed > 0)  
      luaK_nil(fs, fs->freereg, needed);  
  }
  if (needed > 0)
    luaK_reserveregs(fs, needed);  
  else  
    fs->freereg += needed;  
}
#define enterlevel(ls)	luaE_incCstack(ls->L)
#define leavelevel(ls) ((ls)->L->nCcalls--)
static l_noret jumpscopeerror (LexState *ls, Labeldesc *gt) {
  const char *varname = getstr(getlocalvardesc(ls->fs, gt->nactvar)->vd.name);
  const char *msg = "<goto %s> at line %d jumps into the scope of local '%s'";
  msg = luaO_pushfstring(ls->L, msg, getstr(gt->name), gt->line, varname);
  luaK_semerror(ls, msg);  
}
static void solvegoto (LexState *ls, int g, Labeldesc *label) {
  int i;
  Labellist *gl = &ls->dyd->gt;  
  Labeldesc *gt = &gl->arr[g];  
  lua_assert(eqstr(gt->name, label->name));
  if (l_unlikely(gt->nactvar < label->nactvar))  
    jumpscopeerror(ls, gt);
  luaK_patchlist(ls->fs, gt->pc, label->pc);
  for (i = g; i < gl->n - 1; i++)  
    gl->arr[i] = gl->arr[i + 1];
  gl->n--;
}
static Labeldesc *findlabel (LexState *ls, TString *name) {
  int i;
  Dyndata *dyd = ls->dyd;
  
  for (i = ls->fs->firstlabel; i < dyd->label.n; i++) {
    Labeldesc *lb = &dyd->label.arr[i];
    if (eqstr(lb->name, name))  
      return lb;
  }
  return NULL;  
}
static int newlabelentry (LexState *ls, Labellist *l, TString *name,
                          int line, int pc) {
  int n = l->n;
  luaM_growvector(ls->L, l->arr, n, l->size,
                  Labeldesc, SHRT_MAX, "labels/gotos");
  l->arr[n].name = name;
  l->arr[n].line = line;
  l->arr[n].nactvar = ls->fs->nactvar;
  l->arr[n].close = 0;
  l->arr[n].pc = pc;
  l->n = n + 1;
  return n;
}
static int newgotoentry (LexState *ls, TString *name, int line, int pc) {
  return newlabelentry(ls, &ls->dyd->gt, name, line, pc);
}
static int solvegotos (LexState *ls, Labeldesc *lb) {
  Labellist *gl = &ls->dyd->gt;
  int i = ls->fs->bl->firstgoto;
  int needsclose = 0;
  while (i < gl->n) {
    if (eqstr(gl->arr[i].name, lb->name)) {
      needsclose |= gl->arr[i].close;
      solvegoto(ls, i, lb);  
    }
    else
      i++;
  }
  return needsclose;
}
static int createlabel (LexState *ls, TString *name, int line,
                        int last) {
  FuncState *fs = ls->fs;
  Labellist *ll = &ls->dyd->label;
  int l = newlabelentry(ls, ll, name, line, luaK_getlabel(fs));
  if (last) {  
    
    ll->arr[l].nactvar = fs->bl->nactvar;
  }
  if (solvegotos(ls, &ll->arr[l])) {  
    luaK_codeABC(fs, OP_CLOSE, luaY_nvarstack(fs), 0, 0);
    return 1;
  }
  return 0;
}
static void movegotosout (FuncState *fs, BlockCnt *bl) {
  int i;
  Labellist *gl = &fs->ls->dyd->gt;
  
  for (i = bl->firstgoto; i < gl->n; i++) {  
    Labeldesc *gt = &gl->arr[i];
    
    if (reglevel(fs, gt->nactvar) > reglevel(fs, bl->nactvar))
      gt->close |= bl->upval;  
    gt->nactvar = bl->nactvar;  
  }
}
static void enterblock (FuncState *fs, BlockCnt *bl, lu_byte isloop) {
  bl->isloop = isloop;
  bl->nactvar = fs->nactvar;
  bl->firstlabel = fs->ls->dyd->label.n;
  bl->firstgoto = fs->ls->dyd->gt.n;
  bl->upval = 0;
  bl->insidetbc = (fs->bl != NULL && fs->bl->insidetbc);
  bl->previous = fs->bl;
  fs->bl = bl;
  lua_assert(fs->freereg == luaY_nvarstack(fs));
}
static l_noret undefgoto (LexState *ls, Labeldesc *gt) {
  const char *msg;
  if (eqstr(gt->name, luaS_newliteral(ls->L, "break"))) {
    msg = "break outside loop at line %d";
    msg = luaO_pushfstring(ls->L, msg, gt->line);
  }
  else {
    msg = "no visible label '%s' for <goto> at line %d";
    msg = luaO_pushfstring(ls->L, msg, getstr(gt->name), gt->line);
  }
  luaK_semerror(ls, msg);
}
static void leaveblock (FuncState *fs) {
  BlockCnt *bl = fs->bl;
  LexState *ls = fs->ls;
  int hasclose = 0;
  int stklevel = reglevel(fs, bl->nactvar);  
  if (bl->isloop)  
    hasclose = createlabel(ls, luaS_newliteral(ls->L, "break"), 0, 0);
  if (!hasclose && bl->previous && bl->upval)
    luaK_codeABC(fs, OP_CLOSE, stklevel, 0, 0);
  fs->bl = bl->previous;
  removevars(fs, bl->nactvar);
  lua_assert(bl->nactvar == fs->nactvar);
  fs->freereg = stklevel;  
  ls->dyd->label.n = bl->firstlabel;  
  if (bl->previous)  
    movegotosout(fs, bl);  
  else {
    if (bl->firstgoto < ls->dyd->gt.n)  
      undefgoto(ls, &ls->dyd->gt.arr[bl->firstgoto]);  
  }
}
static Proto *addprototype (LexState *ls) {
  Proto *clp;
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;  
  if (fs->np >= f->sizep) {
    int oldsize = f->sizep;
    luaM_growvector(L, f->p, fs->np, f->sizep, Proto *, MAXARG_Bx, "functions");
    while (oldsize < f->sizep)
      f->p[oldsize++] = NULL;
  }
  f->p[fs->np++] = clp = luaF_newproto(L);
  luaC_objbarrier(L, f, clp);
  return clp;
}
static void codeclosure (LexState *ls, expdesc *v) {
  FuncState *fs = ls->fs->prev;
  init_exp(v, VRELOC, luaK_codeABx(fs, OP_CLOSURE, 0, fs->np - 1));
  luaK_exp2nextreg(fs, v);  
}
static void open_func (LexState *ls, FuncState *fs, BlockCnt *bl) {
  Proto *f = fs->f;
  fs->prev = ls->fs;  
  fs->ls = ls;
  ls->fs = fs;
  fs->pc = 0;
  fs->previousline = f->linedefined;
  fs->iwthabs = 0;
  fs->lasttarget = 0;
  fs->freereg = 0;
  fs->nk = 0;
  fs->nabslineinfo = 0;
  fs->np = 0;
  fs->nups = 0;
  fs->ndebugvars = 0;
  fs->nactvar = 0;
  fs->needclose = 0;
  fs->firstlocal = ls->dyd->actvar.n;
  fs->firstlabel = ls->dyd->label.n;
  fs->bl = NULL;
  f->source = ls->source;
  luaC_objbarrier(ls->L, f, f->source);
  f->maxstacksize = 2;  
  enterblock(fs, bl, 0);
}
static void close_func (LexState *ls) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  luaK_ret(fs, luaY_nvarstack(fs), 0);  
  leaveblock(fs);
  lua_assert(fs->bl == NULL);
  luaK_finish(fs);
  luaM_shrinkvector(L, f->code, f->sizecode, fs->pc, Instruction);
  luaM_shrinkvector(L, f->lineinfo, f->sizelineinfo, fs->pc, ls_byte);
  luaM_shrinkvector(L, f->abslineinfo, f->sizeabslineinfo,
                       fs->nabslineinfo, AbsLineInfo);
  luaM_shrinkvector(L, f->k, f->sizek, fs->nk, TValue);
  luaM_shrinkvector(L, f->p, f->sizep, fs->np, Proto *);
  luaM_shrinkvector(L, f->locvars, f->sizelocvars, fs->ndebugvars, LocVar);
  luaM_shrinkvector(L, f->upvalues, f->sizeupvalues, fs->nups, Upvaldesc);
  ls->fs = fs->prev;
  luaC_checkGC(L);
}
static int block_follow (LexState *ls, int withuntil) {
  switch (ls->t.token) {
    case TK_ELSE: case TK_ELSEIF:
    case TK_END: case TK_EOS:
      return 1;
    case TK_UNTIL: return withuntil;
    default: return 0;
  }
}
static void statlist (LexState *ls) {
  
  while (!block_follow(ls, 1)) {
    if (ls->t.token == TK_RETURN) {
      statement(ls);
      return;  
    }
    statement(ls);
  }
}
static void fieldsel (LexState *ls, expdesc *v) {
  
  FuncState *fs = ls->fs;
  expdesc key;
  luaK_exp2anyregup(fs, v);
  luaX_next(ls);  
  codename(ls, &key);
  luaK_indexed(fs, v, &key);
}
static void yindex (LexState *ls, expdesc *v) {
  
  luaX_next(ls);  
  expr(ls, v);
  luaK_exp2val(ls->fs, v);
  checknext(ls, ']');
}
typedef struct ConsControl {
  expdesc v;  
  expdesc *t;  
  int nh;  
  int na;  
  int tostore;  
} ConsControl;
static void recfield (LexState *ls, ConsControl *cc) {
  
  FuncState *fs = ls->fs;
  int reg = ls->fs->freereg;
  expdesc tab, key, val;
  if (ls->t.token == TK_NAME) {
    checklimit(fs, cc->nh, MAX_INT, "items in a constructor");
    codename(ls, &key);
  }
  else  
    yindex(ls, &key);
  cc->nh++;
  checknext(ls, '=');
  tab = *cc->t;
  luaK_indexed(fs, &tab, &key);
  expr(ls, &val);
  luaK_storevar(fs, &tab, &val);
  fs->freereg = reg;  
}
static void closelistfield (FuncState *fs, ConsControl *cc) {
  if (cc->v.k == VVOID) return;  
  luaK_exp2nextreg(fs, &cc->v);
  cc->v.k = VVOID;
  if (cc->tostore == LFIELDS_PER_FLUSH) {
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore);  
    cc->na += cc->tostore;
    cc->tostore = 0;  
  }
}
static void lastlistfield (FuncState *fs, ConsControl *cc) {
  if (cc->tostore == 0) return;
  if (hasmultret(cc->v.k)) {
    luaK_setmultret(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, LUA_MULTRET);
    cc->na--;  
  }
  else {
    if (cc->v.k != VVOID)
      luaK_exp2nextreg(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore);
  }
  cc->na += cc->tostore;
}
static void listfield (LexState *ls, ConsControl *cc) {
  
  expr(ls, &cc->v);
  cc->tostore++;
}
static void field (LexState *ls, ConsControl *cc) {
  
  switch(ls->t.token) {
    case TK_NAME: {  
      if (luaX_lookahead(ls) != '=')  
        listfield(ls, cc);
      else
        recfield(ls, cc);
      break;
    }
    case '[': {
      recfield(ls, cc);
      break;
    }
    default: {
      listfield(ls, cc);
      break;
    }
  }
}
static void constructor (LexState *ls, expdesc *t) {
  
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  int pc = luaK_codeABC(fs, OP_NEWTABLE, 0, 0, 0);
  ConsControl cc;
  luaK_code(fs, 0);  
  cc.na = cc.nh = cc.tostore = 0;
  cc.t = t;
  init_exp(t, VNONRELOC, fs->freereg);  
  luaK_reserveregs(fs, 1);
  init_exp(&cc.v, VVOID, 0);  
  checknext(ls, '{');
  do {
    lua_assert(cc.v.k == VVOID || cc.tostore > 0);
    if (ls->t.token == '}') break;
    closelistfield(fs, &cc);
    field(ls, &cc);
  } while (testnext(ls, ',') || testnext(ls, ';'));
  check_match(ls, '}', '{', line);
  lastlistfield(fs, &cc);
  luaK_settablesize(fs, pc, t->u.info, cc.na, cc.nh);
}
static void setvararg (FuncState *fs, int nparams) {
  fs->f->is_vararg = 1;
  luaK_codeABC(fs, OP_VARARGPREP, nparams, 0, 0);
}
static void parlist (LexState *ls) {
  
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int nparams = 0;
  int isvararg = 0;
  if (ls->t.token != ')') {  
    do {
      switch (ls->t.token) {
        case TK_NAME: {
          new_localvar(ls, str_checkname(ls));
          nparams++;
          break;
        }
        case TK_DOTS: {
          luaX_next(ls);
          isvararg = 1;
          break;
        }
        default: luaX_syntaxerror(ls, "<name> or '...' expected");
      }
    } while (!isvararg && testnext(ls, ','));
  }
  adjustlocalvars(ls, nparams);
  f->numparams = cast_byte(fs->nactvar);
  if (isvararg)
    setvararg(fs, f->numparams);  
  luaK_reserveregs(fs, fs->nactvar);  
}
static void body (LexState *ls, expdesc *e, int ismethod, int line) {
  
  FuncState new_fs;
  BlockCnt bl;
  new_fs.f = addprototype(ls);
  new_fs.f->linedefined = line;
  open_func(ls, &new_fs, &bl);
  checknext(ls, '(');
  if (ismethod) {
    new_localvarliteral(ls, "self");  
    adjustlocalvars(ls, 1);
  }
  parlist(ls);
  checknext(ls, ')');
  statlist(ls);
  new_fs.f->lastlinedefined = ls->linenumber;
  check_match(ls, TK_END, TK_FUNCTION, line);
  codeclosure(ls, e);
  close_func(ls);
}
static int explist (LexState *ls, expdesc *v) {
  
  int n = 1;  
  expr(ls, v);
  while (testnext(ls, ',')) {
    luaK_exp2nextreg(ls->fs, v);
    expr(ls, v);
    n++;
  }
  return n;
}
static void funcargs (LexState *ls, expdesc *f, int line) {
  FuncState *fs = ls->fs;
  expdesc args;
  int base, nparams;
  switch (ls->t.token) {
    case '(': {  
      luaX_next(ls);
      if (ls->t.token == ')')  
        args.k = VVOID;
      else {
        explist(ls, &args);
        if (hasmultret(args.k))
          luaK_setmultret(fs, &args);
      }
      check_match(ls, ')', '(', line);
      break;
    }
    case '{': {  
      constructor(ls, &args);
      break;
    }
    case TK_STRING: {  
      codestring(&args, ls->t.seminfo.ts);
      luaX_next(ls);  
      break;
    }
    default: {
      luaX_syntaxerror(ls, "function arguments expected");
    }
  }
  lua_assert(f->k == VNONRELOC);
  base = f->u.info;  
  if (hasmultret(args.k))
    nparams = LUA_MULTRET;  
  else {
    if (args.k != VVOID)
      luaK_exp2nextreg(fs, &args);  
    nparams = fs->freereg - (base+1);
  }
  init_exp(f, VCALL, luaK_codeABC(fs, OP_CALL, base, nparams+1, 2));
  luaK_fixline(fs, line);
  fs->freereg = base+1;  
}
static void primaryexp (LexState *ls, expdesc *v) {
  
  switch (ls->t.token) {
    case '(': {
      int line = ls->linenumber;
      luaX_next(ls);
      expr(ls, v);
      check_match(ls, ')', '(', line);
      luaK_dischargevars(ls->fs, v);
      return;
    }
    case TK_NAME: {
      singlevar(ls, v);
      return;
    }
    default: {
      luaX_syntaxerror(ls, "unexpected symbol");
    }
  }
}
static void suffixedexp (LexState *ls, expdesc *v) {
  
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  primaryexp(ls, v);
  for (;;) {
    switch (ls->t.token) {
      case '.': {  
        fieldsel(ls, v);
        break;
      }
      case '[': {  
        expdesc key;
        luaK_exp2anyregup(fs, v);
        yindex(ls, &key);
        luaK_indexed(fs, v, &key);
        break;
      }
      case ':': {  
        expdesc key;
        luaX_next(ls);
        codename(ls, &key);
        luaK_self(fs, v, &key);
        funcargs(ls, v, line);
        break;
      }
      case '(': case TK_STRING: case '{': {  
        luaK_exp2nextreg(fs, v);
        funcargs(ls, v, line);
        break;
      }
      default: return;
    }
  }
}
static void simpleexp (LexState *ls, expdesc *v) {
  
  switch (ls->t.token) {
    case TK_FLT: {
      init_exp(v, VKFLT, 0);
      v->u.nval = ls->t.seminfo.r;
      break;
    }
    case TK_INT: {
      init_exp(v, VKINT, 0);
      v->u.ival = ls->t.seminfo.i;
      break;
    }
    case TK_STRING: {
      codestring(v, ls->t.seminfo.ts);
      break;
    }
    case TK_NIL: {
      init_exp(v, VNIL, 0);
      break;
    }
    case TK_TRUE: {
      init_exp(v, VTRUE, 0);
      break;
    }
    case TK_FALSE: {
      init_exp(v, VFALSE, 0);
      break;
    }
    case TK_DOTS: {  
      FuncState *fs = ls->fs;
      check_condition(ls, fs->f->is_vararg,
                      "cannot use '...' outside a vararg function");
      init_exp(v, VVARARG, luaK_codeABC(fs, OP_VARARG, 0, 0, 1));
      break;
    }
    case '{': {  
      constructor(ls, v);
      return;
    }
    case TK_FUNCTION: {
      luaX_next(ls);
      body(ls, v, 0, ls->linenumber);
      return;
    }
    default: {
      suffixedexp(ls, v);
      return;
    }
  }
  luaX_next(ls);
}
static UnOpr getunopr (int op) {
  switch (op) {
    case TK_NOT: return OPR_NOT;
    case '-': return OPR_MINUS;
    case '~': return OPR_BNOT;
    case '#': return OPR_LEN;
    default: return OPR_NOUNOPR;
  }
}
static BinOpr getbinopr (int op) {
  switch (op) {
    case '+': return OPR_ADD;
    case '-': return OPR_SUB;
    case '*': return OPR_MUL;
    case '%': return OPR_MOD;
    case '^': return OPR_POW;
    case '/': return OPR_DIV;
    case TK_IDIV: return OPR_IDIV;
    case '&': return OPR_BAND;
    case '|': return OPR_BOR;
    case '~': return OPR_BXOR;
    case TK_SHL: return OPR_SHL;
    case TK_SHR: return OPR_SHR;
    case TK_CONCAT: return OPR_CONCAT;
    case TK_NE: return OPR_NE;
    case TK_EQ: return OPR_EQ;
    case '<': return OPR_LT;
    case TK_LE: return OPR_LE;
    case '>': return OPR_GT;
    case TK_GE: return OPR_GE;
    case TK_AND: return OPR_AND;
    case TK_OR: return OPR_OR;
    default: return OPR_NOBINOPR;
  }
}
static const struct {
  lu_byte left;  
  lu_byte right; 
} priority[] = {  
   {10, 10}, {10, 10},           
   {11, 11}, {11, 11},           
   {14, 13},                  
   {11, 11}, {11, 11},           
   {6, 6}, {4, 4}, {5, 5},   
   {7, 7}, {7, 7},           
   {9, 8},                   
   {3, 3}, {3, 3}, {3, 3},   
   {3, 3}, {3, 3}, {3, 3},   
   {2, 2}, {1, 1}            
};
#define UNARY_PRIORITY	12  
static BinOpr subexpr (LexState *ls, expdesc *v, int limit) {
  BinOpr op;
  UnOpr uop;
  enterlevel(ls);
  uop = getunopr(ls->t.token);
  if (uop != OPR_NOUNOPR) {  
    int line = ls->linenumber;
    luaX_next(ls);  
    subexpr(ls, v, UNARY_PRIORITY);
    luaK_prefix(ls->fs, uop, v, line);
  }
  else simpleexp(ls, v);
  
  op = getbinopr(ls->t.token);
  while (op != OPR_NOBINOPR && priority[op].left > limit) {
    expdesc v2;
    BinOpr nextop;
    int line = ls->linenumber;
    luaX_next(ls);  
    luaK_infix(ls->fs, op, v);
    
    nextop = subexpr(ls, &v2, priority[op].right);
    luaK_posfix(ls->fs, op, v, &v2, line);
    op = nextop;
  }
  leavelevel(ls);
  return op;  
}
static void expr (LexState *ls, expdesc *v) {
  subexpr(ls, v, 0);
}
static void block (LexState *ls) {
  
  FuncState *fs = ls->fs;
  BlockCnt bl;
  enterblock(fs, &bl, 0);
  statlist(ls);
  leaveblock(fs);
}
struct LHS_assign {
  struct LHS_assign *prev;
  expdesc v;  
};
static void check_conflict (LexState *ls, struct LHS_assign *lh, expdesc *v) {
  FuncState *fs = ls->fs;
  int extra = fs->freereg;  
  int conflict = 0;
  for (; lh; lh = lh->prev) {  
    if (vkisindexed(lh->v.k)) {  
      if (lh->v.k == VINDEXUP) {  
        if (v->k == VUPVAL && lh->v.u.ind.t == v->u.info) {
          conflict = 1;  
          lh->v.k = VINDEXSTR;
          lh->v.u.ind.t = extra;  
        }
      }
      else {  
        if (v->k == VLOCAL && lh->v.u.ind.t == v->u.var.ridx) {
          conflict = 1;  
          lh->v.u.ind.t = extra;  
        }
        
        if (lh->v.k == VINDEXED && v->k == VLOCAL &&
            lh->v.u.ind.idx == v->u.var.ridx) {
          conflict = 1;
          lh->v.u.ind.idx = extra;  
        }
      }
    }
  }
  if (conflict) {
    
    if (v->k == VLOCAL)
      luaK_codeABC(fs, OP_MOVE, extra, v->u.var.ridx, 0);
    else
      luaK_codeABC(fs, OP_GETUPVAL, extra, v->u.info, 0);
    luaK_reserveregs(fs, 1);
  }
}
static void restassign (LexState *ls, struct LHS_assign *lh, int nvars) {
  expdesc e;
  check_condition(ls, vkisvar(lh->v.k), "syntax error");
  check_readonly(ls, &lh->v);
  if (testnext(ls, ',')) {  
    struct LHS_assign nv;
    nv.prev = lh;
    suffixedexp(ls, &nv.v);
    if (!vkisindexed(nv.v.k))
      check_conflict(ls, lh, &nv.v);
    enterlevel(ls);  
    restassign(ls, &nv, nvars+1);
    leavelevel(ls);
  }
  else {  
    int nexps;
    checknext(ls, '=');
    nexps = explist(ls, &e);
    if (nexps != nvars)
      adjust_assign(ls, nvars, nexps, &e);
    else {
      luaK_setoneret(ls->fs, &e);  
      luaK_storevar(ls->fs, &lh->v, &e);
      return;  
    }
  }
  init_exp(&e, VNONRELOC, ls->fs->freereg-1);  
  luaK_storevar(ls->fs, &lh->v, &e);
}
static int cond (LexState *ls) {
  
  expdesc v;
  expr(ls, &v);  
  if (v.k == VNIL) v.k = VFALSE;  
  luaK_goiftrue(ls->fs, &v);
  return v.f;
}
static void gotostat (LexState *ls) {
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  TString *name = str_checkname(ls);  
  Labeldesc *lb = findlabel(ls, name);
  if (lb == NULL)  
    
    newgotoentry(ls, name, line, luaK_jump(fs));
  else {  
    
    int lblevel = reglevel(fs, lb->nactvar);  
    if (luaY_nvarstack(fs) > lblevel)  
      luaK_codeABC(fs, OP_CLOSE, lblevel, 0, 0);
    
    luaK_patchlist(fs, luaK_jump(fs), lb->pc);
  }
}
static void breakstat (LexState *ls) {
  int line = ls->linenumber;
  luaX_next(ls);  
  newgotoentry(ls, luaS_newliteral(ls->L, "break"), line, luaK_jump(ls->fs));
}
static void checkrepeated (LexState *ls, TString *name) {
  Labeldesc *lb = findlabel(ls, name);
  if (l_unlikely(lb != NULL)) {  
    const char *msg = "label '%s' already defined on line %d";
    msg = luaO_pushfstring(ls->L, msg, getstr(name), lb->line);
    luaK_semerror(ls, msg);  
  }
}
static void labelstat (LexState *ls, TString *name, int line) {
  
  checknext(ls, TK_DBCOLON);  
  while (ls->t.token == ';' || ls->t.token == TK_DBCOLON)
    statement(ls);  
  checkrepeated(ls, name);  
  createlabel(ls, name, line, block_follow(ls, 0));
}
static void whilestat (LexState *ls, int line) {
  
  FuncState *fs = ls->fs;
  int whileinit;
  int condexit;
  BlockCnt bl;
  luaX_next(ls);  
  whileinit = luaK_getlabel(fs);
  condexit = cond(ls);
  enterblock(fs, &bl, 1);
  checknext(ls, TK_DO);
  block(ls);
  luaK_jumpto(fs, whileinit);
  check_match(ls, TK_END, TK_WHILE, line);
  leaveblock(fs);
  luaK_patchtohere(fs, condexit);  
}
static void repeatstat (LexState *ls, int line) {
  
  int condexit;
  FuncState *fs = ls->fs;
  int repeat_init = luaK_getlabel(fs);
  BlockCnt bl1, bl2;
  enterblock(fs, &bl1, 1);  
  enterblock(fs, &bl2, 0);  
  luaX_next(ls);  
  statlist(ls);
  check_match(ls, TK_UNTIL, TK_REPEAT, line);
  condexit = cond(ls);  
  leaveblock(fs);  
  if (bl2.upval) {  
    int exit = luaK_jump(fs);  
    luaK_patchtohere(fs, condexit);  
    luaK_codeABC(fs, OP_CLOSE, reglevel(fs, bl2.nactvar), 0, 0);
    condexit = luaK_jump(fs);  
    luaK_patchtohere(fs, exit);  
  }
  luaK_patchlist(fs, condexit, repeat_init);  
  leaveblock(fs);  
}
static void exp1 (LexState *ls) {
  expdesc e;
  expr(ls, &e);
  luaK_exp2nextreg(ls->fs, &e);
  lua_assert(e.k == VNONRELOC);
}
static void fixforjump (FuncState *fs, int pc, int dest, int back) {
  Instruction *jmp = &fs->f->code[pc];
  int offset = dest - (pc + 1);
  if (back)
    offset = -offset;
  if (l_unlikely(offset > MAXARG_Bx))
    luaX_syntaxerror(fs->ls, "control structure too long");
  SETARG_Bx(*jmp, offset);
}
static void forbody (LexState *ls, int base, int line, int nvars, int isgen) {
  
  static const OpCode forprep[2] = {OP_FORPREP, OP_TFORPREP};
  static const OpCode forloop[2] = {OP_FORLOOP, OP_TFORLOOP};
  BlockCnt bl;
  FuncState *fs = ls->fs;
  int prep, endfor;
  checknext(ls, TK_DO);
  prep = luaK_codeABx(fs, forprep[isgen], base, 0);
  enterblock(fs, &bl, 0);  
  adjustlocalvars(ls, nvars);
  luaK_reserveregs(fs, nvars);
  block(ls);
  leaveblock(fs);  
  fixforjump(fs, prep, luaK_getlabel(fs), 0);
  if (isgen) {  
    luaK_codeABC(fs, OP_TFORCALL, base, 0, nvars);
    luaK_fixline(fs, line);
  }
  endfor = luaK_codeABx(fs, forloop[isgen], base, 0);
  fixforjump(fs, endfor, prep + 1, 1);
  luaK_fixline(fs, line);
}
static void fornum (LexState *ls, TString *varname, int line) {
  
  FuncState *fs = ls->fs;
  int base = fs->freereg;
  new_localvarliteral(ls, "(for state)");
  new_localvarliteral(ls, "(for state)");
  new_localvarliteral(ls, "(for state)");
  new_localvar(ls, varname);
  checknext(ls, '=');
  exp1(ls);  
  checknext(ls, ',');
  exp1(ls);  
  if (testnext(ls, ','))
    exp1(ls);  
  else {  
    luaK_int(fs, fs->freereg, 1);
    luaK_reserveregs(fs, 1);
  }
  adjustlocalvars(ls, 3);  
  forbody(ls, base, line, 1, 0);
}
static void forlist (LexState *ls, TString *indexname) {
  
  FuncState *fs = ls->fs;
  expdesc e;
  int nvars = 5;  
  int line;
  int base = fs->freereg;
  
  new_localvarliteral(ls, "(for state)");
  new_localvarliteral(ls, "(for state)");
  new_localvarliteral(ls, "(for state)");
  new_localvarliteral(ls, "(for state)");
  
  new_localvar(ls, indexname);
  while (testnext(ls, ',')) {
    new_localvar(ls, str_checkname(ls));
    nvars++;
  }
  checknext(ls, TK_IN);
  line = ls->linenumber;
  adjust_assign(ls, 4, explist(ls, &e), &e);
  adjustlocalvars(ls, 4);  
  marktobeclosed(fs);  
  luaK_checkstack(fs, 3);  
  forbody(ls, base, line, nvars - 4, 1);
}
static void forstat (LexState *ls, int line) {
  
  FuncState *fs = ls->fs;
  TString *varname;
  BlockCnt bl;
  enterblock(fs, &bl, 1);  
  luaX_next(ls);  
  varname = str_checkname(ls);  
  switch (ls->t.token) {
    case '=': fornum(ls, varname, line); break;
    case ',': case TK_IN: forlist(ls, varname); break;
    default: luaX_syntaxerror(ls, "'=' or 'in' expected");
  }
  check_match(ls, TK_END, TK_FOR, line);
  leaveblock(fs);  
}
static void test_then_block (LexState *ls, int *escapelist) {
  
  BlockCnt bl;
  FuncState *fs = ls->fs;
  expdesc v;
  int jf;  
  luaX_next(ls);  
  expr(ls, &v);  
  checknext(ls, TK_THEN);
  if (ls->t.token == TK_BREAK) {  
    int line = ls->linenumber;
    luaK_goiffalse(ls->fs, &v);  
    luaX_next(ls);  
    enterblock(fs, &bl, 0);  
    newgotoentry(ls, luaS_newliteral(ls->L, "break"), line, v.t);
    while (testnext(ls, ';')) {}  
    if (block_follow(ls, 0)) {  
      leaveblock(fs);
      return;  
    }
    else  
      jf = luaK_jump(fs);
  }
  else {  
    luaK_goiftrue(ls->fs, &v);  
    enterblock(fs, &bl, 0);
    jf = v.f;
  }
  statlist(ls);  
  leaveblock(fs);
  if (ls->t.token == TK_ELSE ||
      ls->t.token == TK_ELSEIF)  
    luaK_concat(fs, escapelist, luaK_jump(fs));  
  luaK_patchtohere(fs, jf);
}
static void ifstat (LexState *ls, int line) {
  
  FuncState *fs = ls->fs;
  int escapelist = NO_JUMP;  
  test_then_block(ls, &escapelist);  
  while (ls->t.token == TK_ELSEIF)
    test_then_block(ls, &escapelist);  
  if (testnext(ls, TK_ELSE))
    block(ls);  
  check_match(ls, TK_END, TK_IF, line);
  luaK_patchtohere(fs, escapelist);  
}
static void localfunc (LexState *ls) {
  expdesc b;
  FuncState *fs = ls->fs;
  int fvar = fs->nactvar;  
  new_localvar(ls, str_checkname(ls));  
  adjustlocalvars(ls, 1);  
  body(ls, &b, 0, ls->linenumber);  
  
  localdebuginfo(fs, fvar)->startpc = fs->pc;
}
static int getlocalattribute (LexState *ls) {
  
  if (testnext(ls, '<')) {
    const char *attr = getstr(str_checkname(ls));
    checknext(ls, '>');
    if (strcmp(attr, "const") == 0)
      return RDKCONST;  
    else if (strcmp(attr, "close") == 0)
      return RDKTOCLOSE;  
    else
      luaK_semerror(ls,
        luaO_pushfstring(ls->L, "unknown attribute '%s'", attr));
  }
  return VDKREG;  
}
static void checktoclose (FuncState *fs, int level) {
  if (level != -1) {  
    marktobeclosed(fs);
    luaK_codeABC(fs, OP_TBC, reglevel(fs, level), 0, 0);
  }
}
static void localstat (LexState *ls) {
  
  FuncState *fs = ls->fs;
  int toclose = -1;  
  Vardesc *var;  
  int vidx, kind;  
  int nvars = 0;
  int nexps;
  expdesc e;
  do {
    vidx = new_localvar(ls, str_checkname(ls));
    kind = getlocalattribute(ls);
    getlocalvardesc(fs, vidx)->vd.kind = kind;
    if (kind == RDKTOCLOSE) {  
      if (toclose != -1)  
        luaK_semerror(ls, "multiple to-be-closed variables in local list");
      toclose = fs->nactvar + nvars;
    }
    nvars++;
  } while (testnext(ls, ','));
  if (testnext(ls, '='))
    nexps = explist(ls, &e);
  else {
    e.k = VVOID;
    nexps = 0;
  }
  var = getlocalvardesc(fs, vidx);  
  if (nvars == nexps &&  
      var->vd.kind == RDKCONST &&  
      luaK_exp2const(fs, &e, &var->k)) {  
    var->vd.kind = RDKCTC;  
    adjustlocalvars(ls, nvars - 1);  
    fs->nactvar++;  
  }
  else {
    adjust_assign(ls, nvars, nexps, &e);
    adjustlocalvars(ls, nvars);
  }
  checktoclose(fs, toclose);
}
static int funcname (LexState *ls, expdesc *v) {
  
  int ismethod = 0;
  singlevar(ls, v);
  while (ls->t.token == '.')
    fieldsel(ls, v);
  if (ls->t.token == ':') {
    ismethod = 1;
    fieldsel(ls, v);
  }
  return ismethod;
}
static void funcstat (LexState *ls, int line) {
  
  int ismethod;
  expdesc v, b;
  luaX_next(ls);  
  ismethod = funcname(ls, &v);
  body(ls, &b, ismethod, line);
  check_readonly(ls, &v);
  luaK_storevar(ls->fs, &v, &b);
  luaK_fixline(ls->fs, line);  
}
static void exprstat (LexState *ls) {
  
  FuncState *fs = ls->fs;
  struct LHS_assign v;
  suffixedexp(ls, &v.v);
  if (ls->t.token == '=' || ls->t.token == ',') { 
    v.prev = NULL;
    restassign(ls, &v, 1);
  }
  else {  
    Instruction *inst;
    check_condition(ls, v.v.k == VCALL, "syntax error");
    inst = &getinstruction(fs, &v.v);
    SETARG_C(*inst, 1);  
  }
}
static void retstat (LexState *ls) {
  
  FuncState *fs = ls->fs;
  expdesc e;
  int nret;  
  int first = luaY_nvarstack(fs);  
  if (block_follow(ls, 1) || ls->t.token == ';')
    nret = 0;  
  else {
    nret = explist(ls, &e);  
    if (hasmultret(e.k)) {
      luaK_setmultret(fs, &e);
      if (e.k == VCALL && nret == 1 && !fs->bl->insidetbc) {  
        SET_OPCODE(getinstruction(fs,&e), OP_TAILCALL);
        lua_assert(GETARG_A(getinstruction(fs,&e)) == luaY_nvarstack(fs));
      }
      nret = LUA_MULTRET;  
    }
    else {
      if (nret == 1)  
        first = luaK_exp2anyreg(fs, &e);  
      else {  
        luaK_exp2nextreg(fs, &e);
        lua_assert(nret == fs->freereg - first);
      }
    }
  }
  luaK_ret(fs, first, nret);
  testnext(ls, ';');  
}
static void statement (LexState *ls) {
  int line = ls->linenumber;  
  enterlevel(ls);
  switch (ls->t.token) {
    case ';': {  
      luaX_next(ls);  
      break;
    }
    case TK_IF: {  
      ifstat(ls, line);
      break;
    }
    case TK_WHILE: {  
      whilestat(ls, line);
      break;
    }
    case TK_DO: {  
      luaX_next(ls);  
      block(ls);
      check_match(ls, TK_END, TK_DO, line);
      break;
    }
    case TK_FOR: {  
      forstat(ls, line);
      break;
    }
    case TK_REPEAT: {  
      repeatstat(ls, line);
      break;
    }
    case TK_FUNCTION: {  
      funcstat(ls, line);
      break;
    }
    case TK_LOCAL: {  
      luaX_next(ls);  
      if (testnext(ls, TK_FUNCTION))  
        localfunc(ls);
      else
        localstat(ls);
      break;
    }
    case TK_DBCOLON: {  
      luaX_next(ls);  
      labelstat(ls, str_checkname(ls), line);
      break;
    }
    case TK_RETURN: {  
      luaX_next(ls);  
      retstat(ls);
      break;
    }
    case TK_BREAK: {  
      breakstat(ls);
      break;
    }
    case TK_GOTO: {  
      luaX_next(ls);  
      gotostat(ls);
      break;
    }
    default: {  
      exprstat(ls);
      break;
    }
  }
  lua_assert(ls->fs->f->maxstacksize >= ls->fs->freereg &&
             ls->fs->freereg >= luaY_nvarstack(ls->fs));
  ls->fs->freereg = luaY_nvarstack(ls->fs);  
  leavelevel(ls);
}
static void mainfunc (LexState *ls, FuncState *fs) {
  BlockCnt bl;
  Upvaldesc *env;
  open_func(ls, fs, &bl);
  setvararg(fs, 0);  
  env = allocupvalue(fs);  
  env->instack = 1;
  env->idx = 0;
  env->kind = VDKREG;
  env->name = ls->envn;
  luaC_objbarrier(ls->L, fs->f, env->name);
  luaX_next(ls);  
  statlist(ls);  
  check(ls, TK_EOS);
  close_func(ls);
}
LClosure *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff,
                       Dyndata *dyd, const char *name, int firstchar) {
  LexState lexstate;
  FuncState funcstate;
  LClosure *cl = luaF_newLclosure(L, 1);  
  setclLvalue2s(L, L->top, cl);  
  luaD_inctop(L);
  lexstate.h = luaH_new(L);  
  sethvalue2s(L, L->top, lexstate.h);  
  luaD_inctop(L);
  funcstate.f = cl->p = luaF_newproto(L);
  luaC_objbarrier(L, cl, cl->p);
  funcstate.f->source = luaS_new(L, name);  
  luaC_objbarrier(L, funcstate.f, funcstate.f->source);
  lexstate.buff = buff;
  lexstate.dyd = dyd;
  dyd->actvar.n = dyd->gt.n = dyd->label.n = 0;
  luaX_setinput(L, &lexstate, z, funcstate.f->source, firstchar);
  mainfunc(&lexstate, &funcstate);
  lua_assert(!funcstate.prev && funcstate.nups == 1 && !lexstate.fs);
  
  lua_assert(dyd->actvar.n == 0 && dyd->gt.n == 0 && dyd->label.n == 0);
  L->top--;  
  return cl;  
}
// root include lstate.c
//included "stddef.h" 
//included "string.h" 
typedef struct LX {
  lu_byte extra_[LUA_EXTRASPACE];
  lua_State l;
} LX;
typedef struct LG {
  LX l;
  global_State g;
} LG;
#define fromstate(L)	(cast(LX *, cast(lu_byte *, (L)) - offsetof(LX, l)))
#if !defined(luai_makeseed)
#include <time.h>
#define addbuff(b,p,e)   { size_t t = cast_sizet(e);     memcpy(b + p, &t, sizeof(t)); p += sizeof(t); }
static unsigned int luai_makeseed (lua_State *L) {
  char buff[3 * sizeof(size_t)];
  unsigned int h = cast_uint(time(NULL));
  int p = 0;
  addbuff(buff, p, L);  
  addbuff(buff, p, &h);  
  addbuff(buff, p, &lua_newstate);  
  lua_assert(p == sizeof(buff));
  return luaS_hash(buff, p, h);
}
#endif
void luaE_setdebt (global_State *g, l_mem debt) {
  l_mem tb = gettotalbytes(g);
  lua_assert(tb > 0);
  if (debt < tb - MAX_LMEM)
    debt = tb - MAX_LMEM;  
  g->totalbytes = tb - debt;
  g->GCdebt = debt;
}
LUA_API int lua_setcstacklimit (lua_State *L, unsigned int limit) {
  UNUSED(L); UNUSED(limit);
  return LUAI_MAXCCALLS;  
}
CallInfo *luaE_extendCI (lua_State *L) {
  CallInfo *ci;
  lua_assert(L->ci->next == NULL);
  ci = luaM_new(L, CallInfo);
  lua_assert(L->ci->next == NULL);
  L->ci->next = ci;
  ci->previous = L->ci;
  ci->next = NULL;
  ci->u.l.trap = 0;
  L->nci++;
  return ci;
}
void luaE_freeCI (lua_State *L) {
  CallInfo *ci = L->ci;
  CallInfo *next = ci->next;
  ci->next = NULL;
  while ((ci = next) != NULL) {
    next = ci->next;
    luaM_free(L, ci);
    L->nci--;
  }
}
void luaE_shrinkCI (lua_State *L) {
  CallInfo *ci = L->ci->next;  
  CallInfo *next;
  if (ci == NULL)
    return;  
  while ((next = ci->next) != NULL) {  
    CallInfo *next2 = next->next;  
    ci->next = next2;  
    L->nci--;
    luaM_free(L, next);  
    if (next2 == NULL)
      break;  
    else {
      next2->previous = ci;
      ci = next2;  
    }
  }
}
void luaE_checkcstack (lua_State *L) {
  if (getCcalls(L) == LUAI_MAXCCALLS)
    luaG_runerror(L, "C stack overflow");
  else if (getCcalls(L) >= (LUAI_MAXCCALLS / 10 * 11))
    luaD_throw(L, LUA_ERRERR);  
}
LUAI_FUNC void luaE_incCstack (lua_State *L) {
  L->nCcalls++;
  if (l_unlikely(getCcalls(L) >= LUAI_MAXCCALLS))
    luaE_checkcstack(L);
}
static void stack_init (lua_State *L1, lua_State *L) {
  int i; CallInfo *ci;
  
  L1->stack = luaM_newvector(L, BASIC_STACK_SIZE + EXTRA_STACK, StackValue);
  L1->tbclist = L1->stack;
  for (i = 0; i < BASIC_STACK_SIZE + EXTRA_STACK; i++)
    setnilvalue(s2v(L1->stack + i));  
  L1->top = L1->stack;
  L1->stack_last = L1->stack + BASIC_STACK_SIZE;
  
  ci = &L1->base_ci;
  ci->next = ci->previous = NULL;
  ci->callstatus = CIST_C;
  ci->func = L1->top;
  ci->u.c.k = NULL;
  ci->nresults = 0;
  setnilvalue(s2v(L1->top));  
  L1->top++;
  ci->top = L1->top + LUA_MINSTACK;
  L1->ci = ci;
}
static void freestack (lua_State *L) {
  if (L->stack == NULL)
    return;  
  L->ci = &L->base_ci;  
  luaE_freeCI(L);
  lua_assert(L->nci == 0);
  luaM_freearray(L, L->stack, stacksize(L) + EXTRA_STACK);  
}
static void init_registry (lua_State *L, global_State *g) {
  
  Table *registry = luaH_new(L);
  sethvalue(L, &g->l_registry, registry);
  luaH_resize(L, registry, LUA_RIDX_LAST, 0);
  
  setthvalue(L, &registry->array[LUA_RIDX_MAINTHREAD - 1], L);
  
  sethvalue(L, &registry->array[LUA_RIDX_GLOBALS - 1], luaH_new(L));
}
static void f_luaopen (lua_State *L, void *ud) {
  global_State *g = G(L);
  UNUSED(ud);
  stack_init(L, L);  
  init_registry(L, g);
  luaS_init(L);
  luaT_init(L);
  luaX_init(L);
  g->gcstp = 0;  
  setnilvalue(&g->nilvalue);  
  luai_userstateopen(L);
}
static void preinit_thread (lua_State *L, global_State *g) {
  G(L) = g;
  L->stack = NULL;
  L->ci = NULL;
  L->nci = 0;
  L->twups = L;  
  L->nCcalls = 0;
  L->errorJmp = NULL;
  L->hook = NULL;
  L->hookmask = 0;
  L->basehookcount = 0;
  L->allowhook = 1;
  resethookcount(L);
  L->openupval = NULL;
  L->status = LUA_OK;
  L->errfunc = 0;
  L->oldpc = 0;
}
static void close_state (lua_State *L) {
  global_State *g = G(L);
  if (!completestate(g))  
    luaC_freeallobjects(L);  
  else {  
    L->ci = &L->base_ci;  
    luaD_closeprotected(L, 1, LUA_OK);  
    luaC_freeallobjects(L);  
    luai_userstateclose(L);
  }
  luaM_freearray(L, G(L)->strt.hash, G(L)->strt.size);
  freestack(L);
  lua_assert(gettotalbytes(g) == sizeof(LG));
  (*g->frealloc)(g->ud, fromstate(L), sizeof(LG), 0);  
}
LUA_API lua_State *lua_newthread (lua_State *L) {
  global_State *g;
  lua_State *L1;
  lua_lock(L);
  g = G(L);
  luaC_checkGC(L);
  
  L1 = &cast(LX *, luaM_newobject(L, LUA_TTHREAD, sizeof(LX)))->l;
  L1->marked = luaC_white(g);
  L1->tt = LUA_VTHREAD;
  
  L1->next = g->allgc;
  g->allgc = obj2gco(L1);
  
  setthvalue2s(L, L->top, L1);
  api_incr_top(L);
  preinit_thread(L1, g);
  L1->hookmask = L->hookmask;
  L1->basehookcount = L->basehookcount;
  L1->hook = L->hook;
  resethookcount(L1);
  
  memcpy(lua_getextraspace(L1), lua_getextraspace(g->mainthread),
         LUA_EXTRASPACE);
  luai_userstatethread(L, L1);
  stack_init(L1, L);  
  lua_unlock(L);
  return L1;
}
void luaE_freethread (lua_State *L, lua_State *L1) {
  LX *l = fromstate(L1);
  luaF_closeupval(L1, L1->stack);  
  lua_assert(L1->openupval == NULL);
  luai_userstatefree(L, L1);
  freestack(L1);
  luaM_free(L, l);
}
int luaE_resetthread (lua_State *L, int status) {
  CallInfo *ci = L->ci = &L->base_ci;  
  setnilvalue(s2v(L->stack));  
  ci->func = L->stack;
  ci->callstatus = CIST_C;
  if (status == LUA_YIELD)
    status = LUA_OK;
  L->status = LUA_OK;  
  status = luaD_closeprotected(L, 1, status);
  if (status != LUA_OK)  
    luaD_seterrorobj(L, status, L->stack + 1);
  else
    L->top = L->stack + 1;
  ci->top = L->top + LUA_MINSTACK;
  luaD_reallocstack(L, cast_int(ci->top - L->stack), 0);
  return status;
}
LUA_API int lua_resetthread (lua_State *L) {
  int status;
  lua_lock(L);
  status = luaE_resetthread(L, L->status);
  lua_unlock(L);
  return status;
}
LUA_API lua_State *lua_newstate (lua_Alloc f, void *ud) {
  int i;
  lua_State *L;
  global_State *g;
  LG *l = cast(LG *, (*f)(ud, NULL, LUA_TTHREAD, sizeof(LG)));
  if (l == NULL) return NULL;
  L = &l->l.l;
  g = &l->g;
  L->tt = LUA_VTHREAD;
  g->currentwhite = bitmask(WHITE0BIT);
  L->marked = luaC_white(g);
  preinit_thread(L, g);
  g->allgc = obj2gco(L);  
  L->next = NULL;
  incnny(L);  
  g->frealloc = f;
  g->ud = ud;
  g->warnf = NULL;
  g->ud_warn = NULL;
  g->mainthread = L;
  g->seed = luai_makeseed(L);
  g->gcstp = GCSTPGC;  
  g->strt.size = g->strt.nuse = 0;
  g->strt.hash = NULL;
  setnilvalue(&g->l_registry);
  g->panic = NULL;
  g->gcstate = GCSpause;
  g->gckind = KGC_INC;
  g->gcstopem = 0;
  g->gcemergency = 0;
  g->finobj = g->tobefnz = g->fixedgc = NULL;
  g->firstold1 = g->survival = g->old1 = g->reallyold = NULL;
  g->finobjsur = g->finobjold1 = g->finobjrold = NULL;
  g->sweepgc = NULL;
  g->gray = g->grayagain = NULL;
  g->weak = g->ephemeron = g->allweak = NULL;
  g->twups = NULL;
  g->totalbytes = sizeof(LG);
  g->GCdebt = 0;
  g->lastatomic = 0;
  setivalue(&g->nilvalue, 0);  
  setgcparam(g->gcpause, LUAI_GCPAUSE);
  setgcparam(g->gcstepmul, LUAI_GCMUL);
  g->gcstepsize = LUAI_GCSTEPSIZE;
  setgcparam(g->genmajormul, LUAI_GENMAJORMUL);
  g->genminormul = LUAI_GENMINORMUL;
  for (i=0; i < LUA_NUMTAGS; i++) g->mt[i] = NULL;
  if (luaD_rawrunprotected(L, f_luaopen, NULL) != LUA_OK) {
    
    close_state(L);
    L = NULL;
  }
  return L;
}
LUA_API void lua_close (lua_State *L) {
  lua_lock(L);
  L = G(L)->mainthread;  
  close_state(L);
}
void luaE_warning (lua_State *L, const char *msg, int tocont) {
  lua_WarnFunction wf = G(L)->warnf;
  if (wf != NULL)
    wf(G(L)->ud_warn, msg, tocont);
}
void luaE_warnerror (lua_State *L, const char *where) {
  TValue *errobj = s2v(L->top - 1);  
  const char *msg = (ttisstring(errobj))
                  ? svalue(errobj)
                  : "error object is not a string";
  
  luaE_warning(L, "error in ", 1);
  luaE_warning(L, where, 1);
  luaE_warning(L, " (", 1);
  luaE_warning(L, msg, 1);
  luaE_warning(L, ")", 0);
}
// root include lstring.c
//included "string.h" 
#define MAXSTRTB	cast_int(luaM_limitN(MAX_INT, TString*))
int luaS_eqlngstr (TString *a, TString *b) {
  size_t len = a->u.lnglen;
  lua_assert(a->tt == LUA_VLNGSTR && b->tt == LUA_VLNGSTR);
  return (a == b) ||  
    ((len == b->u.lnglen) &&  
     (memcmp(getstr(a), getstr(b), len) == 0));  
}
unsigned int luaS_hash (const char *str, size_t l, unsigned int seed) {
  unsigned int h = seed ^ cast_uint(l);
  for (; l > 0; l--)
    h ^= ((h<<5) + (h>>2) + cast_byte(str[l - 1]));
  return h;
}
unsigned int luaS_hashlongstr (TString *ts) {
  lua_assert(ts->tt == LUA_VLNGSTR);
  if (ts->extra == 0) {  
    size_t len = ts->u.lnglen;
    ts->hash = luaS_hash(getstr(ts), len, ts->hash);
    ts->extra = 1;  
  }
  return ts->hash;
}
static void tablerehash (TString **vect, int osize, int nsize) {
  int i;
  for (i = osize; i < nsize; i++)  
    vect[i] = NULL;
  for (i = 0; i < osize; i++) {  
    TString *p = vect[i];
    vect[i] = NULL;
    while (p) {  
      TString *hnext = p->u.hnext;  
      unsigned int h = lmod(p->hash, nsize);  
      p->u.hnext = vect[h];  
      vect[h] = p;
      p = hnext;
    }
  }
}
void luaS_resize (lua_State *L, int nsize) {
  stringtable *tb = &G(L)->strt;
  int osize = tb->size;
  TString **newvect;
  if (nsize < osize)  
    tablerehash(tb->hash, osize, nsize);  
  newvect = luaM_reallocvector(L, tb->hash, osize, nsize, TString*);
  if (l_unlikely(newvect == NULL)) {  
    if (nsize < osize)  
      tablerehash(tb->hash, nsize, osize);  
    
  }
  else {  
    tb->hash = newvect;
    tb->size = nsize;
    if (nsize > osize)
      tablerehash(newvect, osize, nsize);  
  }
}
void luaS_clearcache (global_State *g) {
  int i, j;
  for (i = 0; i < STRCACHE_N; i++)
    for (j = 0; j < STRCACHE_M; j++) {
      if (iswhite(g->strcache[i][j]))  
        g->strcache[i][j] = g->memerrmsg;  
    }
}
void luaS_init (lua_State *L) {
  global_State *g = G(L);
  int i, j;
  stringtable *tb = &G(L)->strt;
  tb->hash = luaM_newvector(L, MINSTRTABSIZE, TString*);
  tablerehash(tb->hash, 0, MINSTRTABSIZE);  
  tb->size = MINSTRTABSIZE;
  
  g->memerrmsg = luaS_newliteral(L, MEMERRMSG);
  luaC_fix(L, obj2gco(g->memerrmsg));  
  for (i = 0; i < STRCACHE_N; i++)  
    for (j = 0; j < STRCACHE_M; j++)
      g->strcache[i][j] = g->memerrmsg;
}
static TString *createstrobj (lua_State *L, size_t l, int tag, unsigned int h) {
  TString *ts;
  GCObject *o;
  size_t totalsize;  
  totalsize = sizelstring(l);
  o = luaC_newobj(L, tag, totalsize);
  ts = gco2ts(o);
  ts->hash = h;
  ts->extra = 0;
  getstr(ts)[l] = '\0';  
  return ts;
}
TString *luaS_createlngstrobj (lua_State *L, size_t l) {
  TString *ts = createstrobj(L, l, LUA_VLNGSTR, G(L)->seed);
  ts->u.lnglen = l;
  return ts;
}
void luaS_remove (lua_State *L, TString *ts) {
  stringtable *tb = &G(L)->strt;
  TString **p = &tb->hash[lmod(ts->hash, tb->size)];
  while (*p != ts)  
    p = &(*p)->u.hnext;
  *p = (*p)->u.hnext;  
  tb->nuse--;
}
static void growstrtab (lua_State *L, stringtable *tb) {
  if (l_unlikely(tb->nuse == MAX_INT)) {  
    luaC_fullgc(L, 1);  
    if (tb->nuse == MAX_INT)  
      luaM_error(L);  
  }
  if (tb->size <= MAXSTRTB / 2)  
    luaS_resize(L, tb->size * 2);
}
static TString *internshrstr (lua_State *L, const char *str, size_t l) {
  TString *ts;
  global_State *g = G(L);
  stringtable *tb = &g->strt;
  unsigned int h = luaS_hash(str, l, g->seed);
  TString **list = &tb->hash[lmod(h, tb->size)];
  lua_assert(str != NULL);  
  for (ts = *list; ts != NULL; ts = ts->u.hnext) {
    if (l == ts->shrlen && (memcmp(str, getstr(ts), l * sizeof(char)) == 0)) {
      
      if (isdead(g, ts))  
        changewhite(ts);  
      return ts;
    }
  }
  
  if (tb->nuse >= tb->size) {  
    growstrtab(L, tb);
    list = &tb->hash[lmod(h, tb->size)];  
  }
  ts = createstrobj(L, l, LUA_VSHRSTR, h);
  memcpy(getstr(ts), str, l * sizeof(char));
  ts->shrlen = cast_byte(l);
  ts->u.hnext = *list;
  *list = ts;
  tb->nuse++;
  return ts;
}
TString *luaS_newlstr (lua_State *L, const char *str, size_t l) {
  if (l <= LUAI_MAXSHORTLEN)  
    return internshrstr(L, str, l);
  else {
    TString *ts;
    if (l_unlikely(l >= (MAX_SIZE - sizeof(TString))/sizeof(char)))
      luaM_toobig(L);
    ts = luaS_createlngstrobj(L, l);
    memcpy(getstr(ts), str, l * sizeof(char));
    return ts;
  }
}
TString *luaS_new (lua_State *L, const char *str) {
  unsigned int i = (point2uint(str)) % STRCACHE_N;  
  int j;
  TString **p = G(L)->strcache[i];
  for (j = 0; j < STRCACHE_M; j++) {
    if (str[0] == getstr(p[j])[0] && strcmp(str, getstr(p[j])) == 0)  
      return p[j];  
  }
  
  for (j = STRCACHE_M - 1; j > 0; j--)
    p[j] = p[j - 1];  
  
  p[0] = luaS_newlstr(L, str, strlen(str));
  return p[0];
}
Udata *luaS_newudata (lua_State *L, size_t s, int nuvalue) {
  Udata *u;
  int i;
  GCObject *o;
  if (l_unlikely(s > MAX_SIZE - udatamemoffset(nuvalue)))
    luaM_toobig(L);
  o = luaC_newobj(L, LUA_VUSERDATA, sizeudata(nuvalue, s));
  u = gco2u(o);
  u->len = s;
  u->nuvalue = nuvalue;
  u->metatable = NULL;
  for (i = 0; i < nuvalue; i++)
    setnilvalue(&u->uv[i].uv);
  return u;
}
// root include ltable.c
//included "math.h" 
//included "limits.h" 
#define MAXABITS	cast_int(sizeof(int) * CHAR_BIT - 1)
#define MAXASIZE	luaM_limitN(1u << MAXABITS, TValue)
#define MAXHBITS	(MAXABITS - 1)
#define MAXHSIZE	luaM_limitN(1u << MAXHBITS, Node)
#if 1
#define hashpow2(t,n)		(gnode(t, fibonacci_hash((n), t->lsizenode)))
#else
#define hashpow2(t,n)		(gnode(t, lmod((n), sizenode(t))))
#endif
#if 0
#define hashmod(t,n)	(gnode(t, ((n) % ((sizenode(t)-1)|1))))
#else
#define hashmod(t, n) (gnode(t, fibonacci_hash((n), t->lsizenode)))
#endif
#define hashstr(t,str)		hashpow2(t, (str)->hash)
#define hashboolean(t,p)	hashpow2(t, p)
#define dummynode		(&dummynode_)
static const Node dummynode_ = {
  {{NULL}, LUA_VEMPTY,  
   LUA_VNIL, 0, {NULL}}  
};
static const TValue absentkey = {ABSTKEYCONSTANT};
static Node* hashpointer (const Table *t, void* p) {
	lua_Unsigned ui = point2uint((size_t)p/sizeof(Udata0));
	return hashmod(t, ui);
}
static Node *hashint (const Table *t, lua_Integer i) {
  lua_Unsigned ui = l_castS2U(i);
  if (ui <= (unsigned int)INT_MAX)
    return hashmod(t, cast_int(ui));
  else
    return hashmod(t, ui);
}
#if !defined(l_hashfloat)
static int l_hashfloat (lua_Number n) {
  int i;
  lua_Integer ni;
  n = l_mathop(frexp)(n, &i) * -cast_num(INT_MIN);
  if (!lua_numbertointeger(n, &ni)) {  
    lua_assert(luai_numisnan(n) || l_mathop(fabs)(n) == cast_num(HUGE_VAL));
    return 0;
  }
  else {  
    unsigned int u = cast_uint(i) + cast_uint(ni);
    return cast_int(u <= cast_uint(INT_MAX) ? u : ~u);
  }
}
#endif
static Node *mainpositionTV (const Table *t, const TValue *key) {
  switch (ttypetag(key)) {
    case LUA_VNUMINT: {
      lua_Integer i = ivalue(key);
      return hashint(t, i);
    }
    case LUA_VNUMFLT: {
      lua_Number n = fltvalue(key);
      return hashmod(t, l_hashfloat(n));
    }
    case LUA_VSHRSTR: {
      TString *ts = tsvalue(key);
      return hashstr(t, ts);
    }
    case LUA_VLNGSTR: {
      TString *ts = tsvalue(key);
      return hashpow2(t, luaS_hashlongstr(ts));
    }
    case LUA_VFALSE:
      return hashboolean(t, 0);
    case LUA_VTRUE:
      return hashboolean(t, 1);
    case LUA_VLIGHTUSERDATA: {
      void *p = pvalue(key);
      return hashpointer(t, p);
    }
    case LUA_VLCF: {
      lua_CFunction f = fvalue(key);
      return hashpointer(t, f);
    }
    default: {
      GCObject *o = gcvalue(key);
      return hashpointer(t, o);
    }
  }
}
l_sinline Node *mainpositionfromnode (const Table *t, Node *nd) {
  TValue key;
  getnodekey(cast(lua_State *, NULL), &key, nd);
  return mainpositionTV(t, &key);
}
static int equalkey (const TValue *k1, const Node *n2, int deadok) {
  if ((rawtt(k1) != keytt(n2)) &&  
       !(deadok && keyisdead(n2) && iscollectable(k1)))
   return 0;  
  switch (keytt(n2)) {
    case LUA_VNIL: case LUA_VFALSE: case LUA_VTRUE:
      return 1;
    case LUA_VNUMINT:
      return (ivalue(k1) == keyival(n2));
    case LUA_VNUMFLT:
      return luai_numeq(fltvalue(k1), fltvalueraw(keyval(n2)));
    case LUA_VLIGHTUSERDATA:
      return pvalue(k1) == pvalueraw(keyval(n2));
    case LUA_VLCF:
      return fvalue(k1) == fvalueraw(keyval(n2));
    case ctb(LUA_VLNGSTR):
      return luaS_eqlngstr(tsvalue(k1), keystrval(n2));
    default:
      return gcvalue(k1) == gcvalueraw(keyval(n2));
  }
}
#define limitequalsasize(t)	(isrealasize(t) || ispow2((t)->alimit))
LUAI_FUNC unsigned int luaH_realasize (const Table *t) {
  if (limitequalsasize(t))
    return t->alimit;  
  else {
    unsigned int size = t->alimit;
    
    size |= (size >> 1);
    size |= (size >> 2);
    size |= (size >> 4);
    size |= (size >> 8);
    size |= (size >> 16);
#if (UINT_MAX >> 30) > 3
    size |= (size >> 32);  
#endif
    size++;
    lua_assert(ispow2(size) && size/2 < t->alimit && t->alimit < size);
    return size;
  }
}
static int ispow2realasize (const Table *t) {
  return (!isrealasize(t) || ispow2(t->alimit));
}
static unsigned int setlimittosize (Table *t) {
  t->alimit = luaH_realasize(t);
  setrealasize(t);
  return t->alimit;
}
#define limitasasize(t)	check_exp(isrealasize(t), t->alimit)
static const TValue *getgeneric (Table *t, const TValue *key, int deadok) {
  Node *n = mainpositionTV(t, key);
  for (;;) {  
    if (equalkey(key, n, deadok))
      return gval(n);  
    else {
      int nx = gnext(n);
      if (nx == 0)
        return &absentkey;  
      n += nx;
    }
  }
}
static unsigned int arrayindex (lua_Integer k) {
  if (l_castS2U(k) - 1u < MAXASIZE)  
    return cast_uint(k);  
  else
    return 0;
}
static unsigned int findindex (lua_State *L, Table *t, TValue *key,
                               unsigned int asize) {
  unsigned int i;
  if (ttisnil(key)) return 0;  
  i = ttisinteger(key) ? arrayindex(ivalue(key)) : 0;
  if (i - 1u < asize)  
    return i;  
  else {
    const TValue *n = getgeneric(t, key, 1);
    if (l_unlikely(isabstkey(n)))
      luaG_runerror(L, "invalid key to 'next'");  
    i = cast_int(nodefromval(n) - gnode(t, 0));  
    
    return (i + 1) + asize;
  }
}
int luaH_next (lua_State *L, Table *t, StkId key) {
  unsigned int asize = luaH_realasize(t);
  unsigned int i = findindex(L, t, s2v(key), asize);  
  for (; i < asize; i++) {  
    if (!isempty(&t->array[i])) {  
      setivalue(s2v(key), i + 1);
      setobj2s(L, key + 1, &t->array[i]);
      return 1;
    }
  }
  for (i -= asize; cast_int(i) < sizenode(t); i++) {  
    if (!isempty(gval(gnode(t, i)))) {  
      Node *n = gnode(t, i);
      getnodekey(L, s2v(key), n);
      setobj2s(L, key + 1, gval(n));
      return 1;
    }
  }
  return 0;  
}
static void freehash (lua_State *L, Table *t) {
  if (!isdummy(t))
    luaM_freearray(L, t->node, cast_sizet(sizenode(t)));
}
static unsigned int computesizes (unsigned int nums[], unsigned int *pna) {
  int i;
  unsigned int twotoi;  
  unsigned int a = 0;  
  unsigned int na = 0;  
  unsigned int optimal = 0;  
  
  for (i = 0, twotoi = 1;
       twotoi > 0 && *pna > twotoi / 2;
       i++, twotoi *= 2) {
    a += nums[i];
    if (a > twotoi/2) {  
      optimal = twotoi;  
      na = a;  
    }
  }
  lua_assert((optimal == 0 || optimal / 2 < na) && na <= optimal);
  *pna = na;
  return optimal;
}
static int countint (lua_Integer key, unsigned int *nums) {
  unsigned int k = arrayindex(key);
  if (k != 0) {  
    nums[luaO_ceillog2(k)]++;  
    return 1;
  }
  else
    return 0;
}
static unsigned int numusearray (const Table *t, unsigned int *nums) {
  int lg;
  unsigned int ttlg;  
  unsigned int ause = 0;  
  unsigned int i = 1;  
  unsigned int asize = limitasasize(t);  
  
  for (lg = 0, ttlg = 1; lg <= MAXABITS; lg++, ttlg *= 2) {
    unsigned int lc = 0;  
    unsigned int lim = ttlg;
    if (lim > asize) {
      lim = asize;  
      if (i > lim)
        break;  
    }
    
    for (; i <= lim; i++) {
      if (!isempty(&t->array[i-1]))
        lc++;
    }
    nums[lg] += lc;
    ause += lc;
  }
  return ause;
}
static int numusehash (const Table *t, unsigned int *nums, unsigned int *pna) {
  int totaluse = 0;  
  int ause = 0;  
  int i = sizenode(t);
  while (i--) {
    Node *n = &t->node[i];
    if (!isempty(gval(n))) {
      if (keyisinteger(n))
        ause += countint(keyival(n), nums);
      totaluse++;
    }
  }
  *pna += ause;
  return totaluse;
}
static void setnodevector (lua_State *L, Table *t, unsigned int size) {
  if (size == 0) {  
    t->node = cast(Node *, dummynode);  
    t->lsizenode = 0;
    t->lastfree = NULL;  
  }
  else {
    int i;
    int lsize = luaO_ceillog2(size);
    if (lsize > MAXHBITS || (1u << lsize) > MAXHSIZE)
      luaG_runerror(L, "table overflow");
    size = twoto(lsize);
    t->node = luaM_newvector(L, size, Node);
    for (i = 0; i < (int)size; i++) {
      Node *n = gnode(t, i);
      gnext(n) = 0;
      setnilkey(n);
      setempty(gval(n));
    }
    t->lsizenode = cast_byte(lsize);
    t->lastfree = gnode(t, size);  
  }
}
static void reinsert (lua_State *L, Table *ot, Table *t) {
  int j;
  int size = sizenode(ot);
  for (j = 0; j < size; j++) {
    Node *old = gnode(ot, j);
    if (!isempty(gval(old))) {
      
      TValue k;
      getnodekey(L, &k, old);
      luaH_set(L, t, &k, gval(old));
    }
  }
}
static void exchangehashpart (Table *t1, Table *t2) {
  lu_byte lsizenode = t1->lsizenode;
  Node *node = t1->node;
  Node *lastfree = t1->lastfree;
  t1->lsizenode = t2->lsizenode;
  t1->node = t2->node;
  t1->lastfree = t2->lastfree;
  t2->lsizenode = lsizenode;
  t2->node = node;
  t2->lastfree = lastfree;
}
void luaH_resize (lua_State *L, Table *t, unsigned int newasize,
                                          unsigned int nhsize) {
  unsigned int i;
  Table newt;  
  unsigned int oldasize = setlimittosize(t);
  TValue *newarray;
  
  setnodevector(L, &newt, nhsize);
  if (newasize < oldasize) {  
    t->alimit = newasize;  
    exchangehashpart(t, &newt);  
    
    for (i = newasize; i < oldasize; i++) {
      if (!isempty(&t->array[i]))
        luaH_setint(L, t, i + 1, &t->array[i]);
    }
    t->alimit = oldasize;  
    exchangehashpart(t, &newt);  
  }
  
  newarray = luaM_reallocvector(L, t->array, oldasize, newasize, TValue);
  if (l_unlikely(newarray == NULL && newasize > 0)) {  
    freehash(L, &newt);  
    luaM_error(L);  
  }
  
  exchangehashpart(t, &newt);  
  t->array = newarray;  
  t->alimit = newasize;
  for (i = oldasize; i < newasize; i++)  
     setempty(&t->array[i]);
  
  reinsert(L, &newt, t);  
  freehash(L, &newt);  
}
void luaH_resizearray (lua_State *L, Table *t, unsigned int nasize) {
  int nsize = allocsizenode(t);
  luaH_resize(L, t, nasize, nsize);
}
static void rehash (lua_State *L, Table *t, const TValue *ek) {
  unsigned int asize;  
  unsigned int na;  
  unsigned int nums[MAXABITS + 1];
  int i;
  int totaluse;
  for (i = 0; i <= MAXABITS; i++) nums[i] = 0;  
  setlimittosize(t);
  na = numusearray(t, nums);  
  totaluse = na;  
  totaluse += numusehash(t, nums, &na);  
  
  if (ttisinteger(ek))
    na += countint(ivalue(ek), nums);
  totaluse++;
  
  asize = computesizes(nums, &na);
  
  luaH_resize(L, t, asize, totaluse - na);
}
Table *luaH_new (lua_State *L) {
  GCObject *o = luaC_newobj(L, LUA_VTABLE, sizeof(Table));
  Table *t = gco2t(o);
  t->metatable = NULL;
  t->flags = cast_byte(maskflags);  
  t->array = NULL;
  t->alimit = 0;
  setnodevector(L, t, 0);
  return t;
}
void luaH_free (lua_State *L, Table *t) {
  freehash(L, t);
  luaM_freearray(L, t->array, luaH_realasize(t));
  luaM_free(L, t);
}
static Node *getfreepos (Table *t) {
  if (!isdummy(t)) {
    while (t->lastfree > t->node) {
      t->lastfree--;
      if (keyisnil(t->lastfree))
        return t->lastfree;
    }
  }
  return NULL;  
}
void luaH_newkey (lua_State *L, Table *t, const TValue *key, TValue *value) {
  Node *mp;
  TValue aux;
  if (l_unlikely(ttisnil(key)))
    luaG_runerror(L, "table index is nil");
  else if (ttisfloat(key)) {
    lua_Number f = fltvalue(key);
    lua_Integer k;
    if (luaV_flttointeger(f, &k, F2Ieq)) {  
      setivalue(&aux, k);
      key = &aux;  
    }
    else if (l_unlikely(luai_numisnan(f)))
      luaG_runerror(L, "table index is NaN");
  }
  if (ttisnil(value))
    return;  
  mp = mainpositionTV(t, key);
  if (!isempty(gval(mp)) || isdummy(t)) {  
    Node *othern;
    Node *f = getfreepos(t);  
    if (f == NULL) {  
      rehash(L, t, key);  
      
      luaH_set(L, t, key, value);  
      return;
    }
    lua_assert(!isdummy(t));
    othern = mainpositionfromnode(t, mp);
    if (othern != mp) {  
      
      while (othern + gnext(othern) != mp)  
        othern += gnext(othern);
      gnext(othern) = cast_int(f - othern);  
      *f = *mp;  
      if (gnext(mp) != 0) {
        gnext(f) += cast_int(mp - f);  
        gnext(mp) = 0;  
      }
      setempty(gval(mp));
    }
    else {  
      
      if (gnext(mp) != 0)
        gnext(f) = cast_int((mp + gnext(mp)) - f);  
      else lua_assert(gnext(f) == 0);
      gnext(mp) = cast_int(f - mp);
      mp = f;
    }
  }
  setnodekey(L, mp, key);
  luaC_barrierback(L, obj2gco(t), key);
  lua_assert(isempty(gval(mp)));
  setobj2t(L, gval(mp), value);
}
const TValue *luaH_getint (Table *t, lua_Integer key) {
  if (l_castS2U(key) - 1u < t->alimit)  
    return &t->array[key - 1];
  else if (!limitequalsasize(t) &&  
           (l_castS2U(key) == t->alimit + 1 ||
            l_castS2U(key) - 1u < luaH_realasize(t))) {
    t->alimit = cast_uint(key);  
    return &t->array[key - 1];
  }
  else {
    Node *n = hashint(t, key);
    for (;;) {  
      if (keyisinteger(n) && keyival(n) == key)
        return gval(n);  
      else {
        int nx = gnext(n);
        if (nx == 0) break;
        n += nx;
      }
    }
    return &absentkey;
  }
}
const TValue *luaH_getshortstr (Table *t, TString *key) {
  Node *n = hashstr(t, key);
  lua_assert(key->tt == LUA_VSHRSTR);
  for (;;) {  
    if (keyisshrstr(n) && eqshrstr(keystrval(n), key))
      return gval(n);  
    else {
      int nx = gnext(n);
      if (nx == 0)
        return &absentkey;  
      n += nx;
    }
  }
}
const TValue *luaH_getstr (Table *t, TString *key) {
  if (key->tt == LUA_VSHRSTR)
    return luaH_getshortstr(t, key);
  else {  
    TValue ko;
    setsvalue(cast(lua_State *, NULL), &ko, key);
    return getgeneric(t, &ko, 0);
  }
}
const TValue *luaH_get (Table *t, const TValue *key) {
  switch (ttypetag(key)) {
    case LUA_VSHRSTR: return luaH_getshortstr(t, tsvalue(key));
    case LUA_VNUMINT: return luaH_getint(t, ivalue(key));
    case LUA_VNIL: return &absentkey;
    case LUA_VNUMFLT: {
      lua_Integer k;
      if (luaV_flttointeger(fltvalue(key), &k, F2Ieq)) 
        return luaH_getint(t, k);  
      return getgeneric(t, key, 0);
    }
    default:
      return getgeneric(t, key, 0);
  }
}
void luaH_finishset (lua_State *L, Table *t, const TValue *key,
                                   const TValue *slot, TValue *value) {
  if (isabstkey(slot))
    luaH_newkey(L, t, key, value);
  else
    setobj2t(L, cast(TValue *, slot), value);
}
void luaH_set (lua_State *L, Table *t, const TValue *key, TValue *value) {
  const TValue *slot = luaH_get(t, key);
  luaH_finishset(L, t, key, slot, value);
}
void luaH_setint (lua_State *L, Table *t, lua_Integer key, TValue *value) {
  const TValue *p = luaH_getint(t, key);
  if (isabstkey(p)) {
    TValue k;
    setivalue(&k, key);
    luaH_newkey(L, t, &k, value);
  }
  else
    setobj2t(L, cast(TValue *, p), value);
}
static lua_Unsigned hash_search (Table *t, lua_Unsigned j) {
  lua_Unsigned i;
  if (j == 0) j++;  
  do {
    i = j;  
    if (j <= l_castS2U(LUA_MAXINTEGER) / 2)
      j *= 2;
    else {
      j = LUA_MAXINTEGER;
      if (isempty(luaH_getint(t, j)))  
        break;  
      else  
        return j;  
    }
  } while (!isempty(luaH_getint(t, j)));  
  
  while (j - i > 1u) {  
    lua_Unsigned m = (i + j) / 2;
    if (isempty(luaH_getint(t, m))) j = m;
    else i = m;
  }
  return i;
}
static unsigned int binsearch (const TValue *array, unsigned int i,
                                                    unsigned int j) {
  while (j - i > 1u) {  
    unsigned int m = (i + j) / 2;
    if (isempty(&array[m - 1])) j = m;
    else i = m;
  }
  return i;
}
lua_Unsigned luaH_getn (Table *t) {
  unsigned int limit = t->alimit;
  if (limit > 0 && isempty(&t->array[limit - 1])) {  
    
    if (limit >= 2 && !isempty(&t->array[limit - 2])) {
      
      if (ispow2realasize(t) && !ispow2(limit - 1)) {
        t->alimit = limit - 1;
        setnorealasize(t);  
      }
      return limit - 1;
    }
    else {  
      unsigned int boundary = binsearch(t->array, 0, limit);
      
      if (ispow2realasize(t) && boundary > luaH_realasize(t) / 2) {
        t->alimit = boundary;  
        setnorealasize(t);
      }
      return boundary;
    }
  }
  
  if (!limitequalsasize(t)) {  
    
    if (isempty(&t->array[limit]))  
      return limit;  
    
    limit = luaH_realasize(t);
    if (isempty(&t->array[limit - 1])) {  
      
      unsigned int boundary = binsearch(t->array, t->alimit, limit);
      t->alimit = boundary;
      return boundary;
    }
    
  }
  
  lua_assert(limit == luaH_realasize(t) &&
             (limit == 0 || !isempty(&t->array[limit - 1])));
  if (isdummy(t) || isempty(luaH_getint(t, cast(lua_Integer, limit + 1))))
    return limit;  
  else  
    return hash_search(t, limit);
}
#if defined(LUA_DEBUG)
Node *luaH_mainposition (const Table *t, const TValue *key) {
  return mainpositionTV(t, key);
}
int luaH_isdummy (const Table *t) { return isdummy(t); }
#endif
// root include ltm.c
//included "string.h" 
void luaT_init (lua_State *L) {
  static const char *const luaT_eventname[] = {  
    "__index", "__newindex",
    "__gc", "__mode", "__len", "__eq",
    "__add", "__sub", "__mul", "__mod", "__pow",
    "__div", "__idiv",
    "__band", "__bor", "__bxor", "__shl", "__shr",
    "__unm", "__bnot", "__lt", "__le",
    "__concat", "__call", "__close"
  };
	// init tag methods names at first
  int i;
  for (i=0; i<TM_N; i++) {
    G(L)->tmname[i] = luaS_new(L, luaT_eventname[i]);
    luaC_fix(L, obj2gco(G(L)->tmname[i]));  
  }
	// now let's init typenames
	for (i=0; i<LUA_TOTALTYPES; i++) {
    G(L)->typenames[i] = luaS_new(L, luaT_typenames_[i]);
    luaC_fix(L, obj2gco(G(L)->typenames[i]));  
  }
	G(L)->typenames[LUA_TOTALTYPES] = luaS_new(L, "__name");
	luaC_fix(L, obj2gco(G(L)->typenames[LUA_TOTALTYPES])); 
}
#define TYPESTR(L) (G(L)->typenames[LUA_TOTALTYPES])
const TValue *luaT_gettm (Table *events, TMS event, TString *ename) {
  const TValue *tm = luaH_getshortstr(events, ename);
  lua_assert(event <= TM_EQ);
  if (notm(tm)) {  
    events->flags |= cast_byte(1u<<event);  
    return NULL;
  }
  else return tm;
}
const TValue *luaT_gettmbyobj (lua_State *L, const TValue *o, TMS event) {
  Table *mt;
  switch (ttype(o)) {
    case LUA_TTABLE:
      mt = hvalue(o)->metatable;
      break;
    case LUA_TUSERDATA:
      mt = uvalue(o)->metatable;
      break;
    default:
      mt = G(L)->mt[ttype(o)];
  }
  return (mt ? luaH_getshortstr(mt, G(L)->tmname[event]) : &G(L)->nilvalue);
}
TString* luaT_objtypestr (lua_State *L, const TValue *o) {
	Table *mt;
  if ((ttistable(o) && (mt = hvalue(o)->metatable) != NULL) ||
      (ttisfulluserdata(o) && (mt = uvalue(o)->metatable) != NULL)) {
    const TValue *name = luaH_getshortstr(mt, TYPESTR(L));
    if (ttisstring(name))  
      return tsvalue(name);  
  }
  return G(L)->typenames[ttype(o) + 1];  
}
const char *luaT_objtypename (lua_State *L, const TValue *o) {
	return getstr(luaT_objtypestr(L, o));
}
void luaT_callTM (lua_State *L, const TValue *f, const TValue *p1,
                  const TValue *p2, const TValue *p3) {
  StkId func = L->top;
  setobj2s(L, func, f);  
  setobj2s(L, func + 1, p1);  
  setobj2s(L, func + 2, p2);  
  setobj2s(L, func + 3, p3);  
  L->top = func + 4;
  
  if (isLuacode(L->ci))
    luaD_call(L, func, 0);
  else
    luaD_callnoyield(L, func, 0);
}
void luaT_callTMres (lua_State *L, const TValue *f, const TValue *p1,
                     const TValue *p2, StkId res) {
  ptrdiff_t result = savestack(L, res);
  StkId func = L->top;
  setobj2s(L, func, f);  
  setobj2s(L, func + 1, p1);  
  setobj2s(L, func + 2, p2);  
  L->top += 3;
  
  if (isLuacode(L->ci))
    luaD_call(L, func, 1);
  else
    luaD_callnoyield(L, func, 1);
  res = restorestack(L, result);
  setobjs2s(L, res, --L->top);  
}
static int callbinTM (lua_State *L, const TValue *p1, const TValue *p2,
                      StkId res, TMS event) {
  const TValue *tm = luaT_gettmbyobj(L, p1, event);  
  if (notm(tm))
    tm = luaT_gettmbyobj(L, p2, event);  
  if (notm(tm)) return 0;
  luaT_callTMres(L, tm, p1, p2, res);
  return 1;
}
void luaT_trybinTM (lua_State *L, const TValue *p1, const TValue *p2,
                    StkId res, TMS event) {
  if (l_unlikely(!callbinTM(L, p1, p2, res, event))) {
    switch (event) {
      case TM_BAND: case TM_BOR: case TM_BXOR:
      case TM_SHL: case TM_SHR: case TM_BNOT: {
        if (ttisnumber(p1) && ttisnumber(p2))
          luaG_tointerror(L, p1, p2);
        else
          luaG_opinterror(L, p1, p2, "perform bitwise operation on");
      }
      
      default:
        luaG_opinterror(L, p1, p2, "perform arithmetic on");
    }
  }
}
void luaT_tryconcatTM (lua_State *L) {
  StkId top = L->top;
  if (l_unlikely(!callbinTM(L, s2v(top - 2), s2v(top - 1), top - 2,
                               TM_CONCAT)))
    luaG_concaterror(L, s2v(top - 2), s2v(top - 1));
}
void luaT_trybinassocTM (lua_State *L, const TValue *p1, const TValue *p2,
                                       int flip, StkId res, TMS event) {
  if (flip)
    luaT_trybinTM(L, p2, p1, res, event);
  else
    luaT_trybinTM(L, p1, p2, res, event);
}
void luaT_trybiniTM (lua_State *L, const TValue *p1, lua_Integer i2,
                                   int flip, StkId res, TMS event) {
  TValue aux;
  setivalue(&aux, i2);
  luaT_trybinassocTM(L, p1, &aux, flip, res, event);
}
int luaT_callorderTM (lua_State *L, const TValue *p1, const TValue *p2,
                      TMS event) {
  if (callbinTM(L, p1, p2, L->top, event))  
    return !l_isfalse(s2v(L->top));
#if defined(LUA_COMPAT_LT_LE)
  else if (event == TM_LE) {
      
      L->ci->callstatus |= CIST_LEQ;  
      if (callbinTM(L, p2, p1, L->top, TM_LT)) {
        L->ci->callstatus ^= CIST_LEQ;  
        return l_isfalse(s2v(L->top));
      }
      
  }
#endif
  luaG_ordererror(L, p1, p2);  
  return 0;  
}
int luaT_callorderiTM (lua_State *L, const TValue *p1, int v2,
                       int flip, int isfloat, TMS event) {
  TValue aux; const TValue *p2;
  if (isfloat) {
    setfltvalue(&aux, cast_num(v2));
  }
  else
    setivalue(&aux, v2);
  if (flip) {  
    p2 = p1; p1 = &aux;  
  }
  else
    p2 = &aux;
  return luaT_callorderTM(L, p1, p2, event);
}
void luaT_adjustvarargs (lua_State *L, int nfixparams, CallInfo *ci,
                         const Proto *p) {
  int i;
  int actual = cast_int(L->top - ci->func) - 1;  
  int nextra = actual - nfixparams;  
  ci->u.l.nextraargs = nextra;
  luaD_checkstack(L, p->maxstacksize + 1);
  
  setobjs2s(L, L->top++, ci->func);
  
  for (i = 1; i <= nfixparams; i++) {
    setobjs2s(L, L->top++, ci->func + i);
    setnilvalue(s2v(ci->func + i));  
  }
  ci->func += actual + 1;
  ci->top += actual + 1;
  lua_assert(L->top <= ci->top && ci->top <= L->stack_last);
}
void luaT_getvarargs (lua_State *L, CallInfo *ci, StkId where, int wanted) {
  int i;
  int nextra = ci->u.l.nextraargs;
  if (wanted < 0) {
    wanted = nextra;  
    checkstackGCp(L, nextra, where);  
    L->top = where + nextra;  
  }
  for (i = 0; i < wanted && i < nextra; i++)
    setobjs2s(L, where + i, ci->func - nextra + i);
  for (; i < wanted; i++)   
    setnilvalue(s2v(where + i));
}
// root include lundump.c
//included "limits.h" 
//included "string.h" 
#if !defined(luai_verifycode)
#define luai_verifycode(L,f)  
#endif
typedef struct {
  lua_State *L;
  ZIO *Z;
  const char *name;
} LoadState;
static l_noret error (LoadState *S, const char *why) {
  luaO_pushfstring(S->L, "%s: bad binary format (%s)", S->name, why);
  luaD_throw(S->L, LUA_ERRSYNTAX);
}
#define loadVector(S,b,n)	loadBlock(S,b,(n)*sizeof((b)[0]))
static void loadBlock (LoadState *S, void *b, size_t size) {
  if (luaZ_read(S->Z, b, size) != 0)
    error(S, "truncated chunk");
}
#define loadVar(S,x)		loadVector(S,&x,1)
static lu_byte loadByte (LoadState *S) {
  int b = zgetc(S->Z);
  if (b == EOZ)
    error(S, "truncated chunk");
  return cast_byte(b);
}
static size_t loadUnsigned (LoadState *S, size_t limit) {
  size_t x = 0;
  int b;
  limit >>= 7;
  do {
    b = loadByte(S);
    if (x >= limit)
      error(S, "integer overflow");
    x = (x << 7) | (b & 0x7f);
  } while ((b & 0x80) == 0);
  return x;
}
static size_t loadSize (LoadState *S) {
  return loadUnsigned(S, ~(size_t)0);
}
static int loadInt (LoadState *S) {
  return cast_int(loadUnsigned(S, INT_MAX));
}
static lua_Number loadNumber (LoadState *S) {
  lua_Number x;
  loadVar(S, x);
  return x;
}
static lua_Integer loadInteger (LoadState *S) {
  lua_Integer x;
  loadVar(S, x);
  return x;
}
static TString *loadStringN (LoadState *S, Proto *p) {
  lua_State *L = S->L;
  TString *ts;
  size_t size = loadSize(S);
  if (size == 0)  
    return NULL;
  else if (--size <= LUAI_MAXSHORTLEN) {  
    char buff[LUAI_MAXSHORTLEN];
    loadVector(S, buff, size);  
    ts = luaS_newlstr(L, buff, size);  
  }
  else {  
    ts = luaS_createlngstrobj(L, size);  
    setsvalue2s(L, L->top, ts);  
    luaD_inctop(L);
    loadVector(S, getstr(ts), size);  
    L->top--;  
  }
  luaC_objbarrier(L, p, ts);
  return ts;
}
static TString *loadString (LoadState *S, Proto *p) {
  TString *st = loadStringN(S, p);
  if (st == NULL)
    error(S, "bad format for constant string");
  return st;
}
static void loadCode (LoadState *S, Proto *f) {
  int n = loadInt(S);
  f->code = luaM_newvectorchecked(S->L, n, Instruction);
  f->sizecode = n;
  loadVector(S, f->code, n);
}
static void loadFunction(LoadState *S, Proto *f, TString *psource);
static void loadConstants (LoadState *S, Proto *f) {
  int i;
  int n = loadInt(S);
  f->k = luaM_newvectorchecked(S->L, n, TValue);
  f->sizek = n;
  for (i = 0; i < n; i++)
    setnilvalue(&f->k[i]);
  for (i = 0; i < n; i++) {
    TValue *o = &f->k[i];
    int t = loadByte(S);
    switch (t) {
      case LUA_VNIL:
        setnilvalue(o);
        break;
      case LUA_VFALSE:
        setbfvalue(o);
        break;
      case LUA_VTRUE:
        setbtvalue(o);
        break;
      case LUA_VNUMFLT:
        setfltvalue(o, loadNumber(S));
        break;
      case LUA_VNUMINT:
        setivalue(o, loadInteger(S));
        break;
      case LUA_VSHRSTR:
      case LUA_VLNGSTR:
        setsvalue2n(S->L, o, loadString(S, f));
        break;
      default: luai_unreachable();
    }
  }
}
static void loadProtos (LoadState *S, Proto *f) {
  int i;
  int n = loadInt(S);
  f->p = luaM_newvectorchecked(S->L, n, Proto *);
  f->sizep = n;
  for (i = 0; i < n; i++)
    f->p[i] = NULL;
  for (i = 0; i < n; i++) {
    f->p[i] = luaF_newproto(S->L);
    luaC_objbarrier(S->L, f, f->p[i]);
    loadFunction(S, f->p[i], f->source);
  }
}
static void loadUpvalues (LoadState *S, Proto *f) {
  int i, n;
  n = loadInt(S);
  f->upvalues = luaM_newvectorchecked(S->L, n, Upvaldesc);
  f->sizeupvalues = n;
  for (i = 0; i < n; i++)  
    f->upvalues[i].name = NULL;
  for (i = 0; i < n; i++) {  
    f->upvalues[i].instack = loadByte(S);
    f->upvalues[i].idx = loadByte(S);
    f->upvalues[i].kind = loadByte(S);
  }
}
static void loadDebug (LoadState *S, Proto *f) {
  int i, n;
  n = loadInt(S);
  f->lineinfo = luaM_newvectorchecked(S->L, n, ls_byte);
  f->sizelineinfo = n;
  loadVector(S, f->lineinfo, n);
  n = loadInt(S);
  f->abslineinfo = luaM_newvectorchecked(S->L, n, AbsLineInfo);
  f->sizeabslineinfo = n;
  for (i = 0; i < n; i++) {
    f->abslineinfo[i].pc = loadInt(S);
    f->abslineinfo[i].line = loadInt(S);
  }
  n = loadInt(S);
  f->locvars = luaM_newvectorchecked(S->L, n, LocVar);
  f->sizelocvars = n;
  for (i = 0; i < n; i++)
    f->locvars[i].varname = NULL;
  for (i = 0; i < n; i++) {
    f->locvars[i].varname = loadStringN(S, f);
    f->locvars[i].startpc = loadInt(S);
    f->locvars[i].endpc = loadInt(S);
  }
  n = loadInt(S);
  for (i = 0; i < n; i++)
    f->upvalues[i].name = loadStringN(S, f);
}
static void loadFunction (LoadState *S, Proto *f, TString *psource) {
  f->source = loadStringN(S, f);
  if (f->source == NULL)  
    f->source = psource;  
  f->linedefined = loadInt(S);
  f->lastlinedefined = loadInt(S);
  f->numparams = loadByte(S);
  f->is_vararg = loadByte(S);
  f->maxstacksize = loadByte(S);
  loadCode(S, f);
  loadConstants(S, f);
  loadUpvalues(S, f);
  loadProtos(S, f);
  loadDebug(S, f);
}
static void checkliteral (LoadState *S, const char *s, const char *msg) {
  char buff[sizeof(LUA_SIGNATURE) + sizeof(LUAC_DATA)]; 
  size_t len = strlen(s);
  loadVector(S, buff, len);
  if (memcmp(s, buff, len) != 0)
    error(S, msg);
}
static void fchecksize (LoadState *S, size_t size, const char *tname) {
  if (loadByte(S) != size)
    error(S, luaO_pushfstring(S->L, "%s size mismatch", tname));
}
#define checksize(S,t)	fchecksize(S,sizeof(t),#t)
static void checkHeader (LoadState *S) {
  
  checkliteral(S, &LUA_SIGNATURE[1], "not a binary chunk");
  if (loadByte(S) != LUAC_VERSION)
    error(S, "version mismatch");
  if (loadByte(S) != LUAC_FORMAT)
    error(S, "format mismatch");
  checkliteral(S, LUAC_DATA, "corrupted chunk");
  checksize(S, Instruction);
  checksize(S, lua_Integer);
  checksize(S, lua_Number);
  if (loadInteger(S) != LUAC_INT)
    error(S, "integer format mismatch");
  if (loadNumber(S) != LUAC_NUM)
    error(S, "float format mismatch");
}
LClosure *luaU_undump(lua_State *L, ZIO *Z, const char *name) {
  LoadState S;
  LClosure *cl;
  if (*name == '@' || *name == '=')
    S.name = name + 1;
  else if (*name == LUA_SIGNATURE[0])
    S.name = "binary string";
  else
    S.name = name;
  S.L = L;
  S.Z = Z;
  checkHeader(&S);
  cl = luaF_newLclosure(L, loadByte(&S));
  setclLvalue2s(L, L->top, cl);
  luaD_inctop(L);
  cl->p = luaF_newproto(L);
  luaC_objbarrier(L, cl, cl->p);
  loadFunction(&S, cl->p, NULL);
  lua_assert(cl->nupvalues == cl->p->sizeupvalues);
  luai_verifycode(L, cl->p);
  return cl;
}
// root include lvm.c
//included "float.h" 
//included "limits.h" 
//included "math.h" 
//included "stdio.h" 
//included "stdlib.h" 
//included "string.h" 
#if !defined(LUA_USE_JUMPTABLE)
#if defined(__GNUC__)
#define LUA_USE_JUMPTABLE	0
#else
#define LUA_USE_JUMPTABLE	0
#endif
#endif
#define MAXTAGLOOP	2000
#define NBM		(l_floatatt(MANT_DIG))
#if ((((LUA_MAXINTEGER >> (NBM / 4)) >> (NBM / 4)) >> (NBM / 4)) 	>> (NBM - (3 * (NBM / 4))))  >  0
#define MAXINTFITSF	((lua_Unsigned)1 << NBM)
#define l_intfitsf(i)	((MAXINTFITSF + l_castS2U(i)) <= (2 * MAXINTFITSF))
#else  
#define l_intfitsf(i)	1
#endif
static int l_strton (const TValue *obj, TValue *result) {
  lua_assert(obj != result);
  if (!cvt2num(obj))  
    return 0;
  else
    return (luaO_str2num(svalue(obj), result) == vslen(obj) + 1);
}
int luaV_tonumber_ (const TValue *obj, lua_Number *n) {
  TValue v = {0};
  if (ttisinteger(obj)) {
    *n = cast_num(ivalue(obj));
    return 1;
  }
  else if (l_strton(obj, &v)) {  
    *n = nvalue(&v);  
    return 1;
  }
  else
    return 0;  
}
int luaV_flttointeger (lua_Number n, lua_Integer *p, F2Imod mode) {
  lua_Number f = l_floor(n);
  if (n != f) {  
    if (mode == F2Ieq) return 0;  
    else if (mode == F2Iceil)  
      f += 1;  
  }
  return lua_numbertointeger(f, p);
}
int luaV_tointegerns (const TValue *obj, lua_Integer *p, F2Imod mode) {
  if (ttisfloat(obj))
    return luaV_flttointeger(fltvalue(obj), p, mode);
  else if (ttisinteger(obj)) {
    *p = ivalue(obj);
    return 1;
  }
  else
    return 0;
}
int luaV_tointeger (const TValue *obj, lua_Integer *p, F2Imod mode) {
  TValue v;
  if (l_strton(obj, &v))  
    obj = &v;  
  return luaV_tointegerns(obj, p, mode);
}
static int forlimit (lua_State *L, lua_Integer init, const TValue *lim,
                                   lua_Integer *p, lua_Integer step) {
  if (!luaV_tointeger(lim, p, (step < 0 ? F2Iceil : F2Ifloor))) {
    
    lua_Number flim;  
    if (!tonumber(lim, &flim)) 
      luaG_forerror(L, lim, "limit");
    
    if (luai_numlt(0, flim)) {  
      if (step < 0) return 1;  
      *p = LUA_MAXINTEGER;  
    }
    else {  
      if (step > 0) return 1;  
      *p = LUA_MININTEGER;  
    }
  }
  return (step > 0 ? init > *p : init < *p);  
}
static int forprep (lua_State *L, StkId ra) {
  TValue *pinit = s2v(ra);
  TValue *plimit = s2v(ra + 1);
  TValue *pstep = s2v(ra + 2);
  if (ttisinteger(pinit) && ttisinteger(pstep)) { 
    lua_Integer init = ivalue(pinit);
    lua_Integer step = ivalue(pstep);
    lua_Integer limit;
    if (step == 0)
      luaG_runerror(L, "'for' step is zero");
    setivalue(s2v(ra + 3), init);  
    if (forlimit(L, init, plimit, &limit, step))
      return 1;  
    else {  
      lua_Unsigned count;
      if (step > 0) {  
        count = l_castS2U(limit) - l_castS2U(init);
        if (step != 1)  
          count /= l_castS2U(step);
      }
      else {  
        count = l_castS2U(init) - l_castS2U(limit);
        
        count /= l_castS2U(-(step + 1)) + 1u;
      }
      
      setivalue(plimit, l_castU2S(count));
    }
  }
  else {  
    lua_Number init; lua_Number limit; lua_Number step;
    if (l_unlikely(!tonumber(plimit, &limit)))
      luaG_forerror(L, plimit, "limit");
    if (l_unlikely(!tonumber(pstep, &step)))
      luaG_forerror(L, pstep, "step");
    if (l_unlikely(!tonumber(pinit, &init)))
      luaG_forerror(L, pinit, "initial value");
    if (step == 0)
      luaG_runerror(L, "'for' step is zero");
    if (luai_numlt(0, step) ? luai_numlt(limit, init)
                            : luai_numlt(init, limit))
      return 1;  
    else {
      
      setfltvalue(plimit, limit);
      setfltvalue(pstep, step);
      setfltvalue(s2v(ra), init);  
      setfltvalue(s2v(ra + 3), init);  
    }
  }
  return 0;
}
static int floatforloop (StkId ra) {
  lua_Number step = fltvalue(s2v(ra + 2));
  lua_Number limit = fltvalue(s2v(ra + 1));
  lua_Number idx = fltvalue(s2v(ra));  
  idx = luai_numadd(L, idx, step);  
  if (luai_numlt(0, step) ? luai_numle(idx, limit)
                          : luai_numle(limit, idx)) {
    chgfltvalue(s2v(ra), idx);  
    setfltvalue(s2v(ra + 3), idx);  
    return 1;  
  }
  else
    return 0;  
}
void luaV_finishget (lua_State *L, const TValue *t, TValue *key, StkId val,
                      const TValue *slot) {
  int loop;  
  const TValue *tm;  
  for (loop = 0; loop < MAXTAGLOOP; loop++) {
    if (slot == NULL) {  
      lua_assert(!ttistable(t));
      tm = luaT_gettmbyobj(L, t, TM_INDEX);
      if (l_unlikely(notm(tm)))
        luaG_typeerror(L, t, "index");  
      
    }
    else {  
      lua_assert(isempty(slot));
      tm = fasttm(L, hvalue(t)->metatable, TM_INDEX);  
      if (tm == NULL) {  
        setnilvalue(s2v(val));  
        return;
      }
      
    }
    if (ttisfunction(tm)) {  
      luaT_callTMres(L, tm, t, key, val);  
      return;
    }
    t = tm;  
    if (luaV_fastget(L, t, key, slot, luaH_get)) {  
      setobj2s(L, val, slot);  
      return;
    }
    
  }
  luaG_runerror(L, "'__index' chain too long; possible loop");
}
void luaV_finishset (lua_State *L, const TValue *t, TValue *key,
                     TValue *val, const TValue *slot) {
  int loop;  
  for (loop = 0; loop < MAXTAGLOOP; loop++) {
    const TValue *tm;  
    if (slot != NULL) {  
      Table *h = hvalue(t);  
      lua_assert(isempty(slot));  
      tm = fasttm(L, h->metatable, TM_NEWINDEX);  
      if (tm == NULL) {  
        luaH_finishset(L, h, key, slot, val);  
        invalidateTMcache(h);
        luaC_barrierback(L, obj2gco(h), val);
        return;
      }
      
    }
    else {  
      tm = luaT_gettmbyobj(L, t, TM_NEWINDEX);
      if (l_unlikely(notm(tm)))
        luaG_typeerror(L, t, "index");
    }
    
    if (ttisfunction(tm)) {
      luaT_callTM(L, tm, t, key, val);
      return;
    }
    t = tm;  
    if (luaV_fastget(L, t, key, slot, luaH_get)) {
      luaV_finishfastset(L, t, slot, val);
      return;  
    }
    
  }
  luaG_runerror(L, "'__newindex' chain too long; possible loop");
}
static int l_strcmp (const TString *ls, const TString *rs) {
  const char *l = getstr(ls);
  size_t ll = tsslen(ls);
  const char *r = getstr(rs);
  size_t lr = tsslen(rs);
  for (;;) {  
    int temp = strcoll(l, r);
    if (temp != 0)  
      return temp;  
    else {  
      size_t len = strlen(l);  
      if (len == lr)  
        return (len == ll) ? 0 : 1;  
      else if (len == ll)  
        return -1;  
      
      len++;
      l += len; ll -= len; r += len; lr -= len;
    }
  }
}
l_sinline int LTintfloat (lua_Integer i, lua_Number f) {
  if (l_intfitsf(i))
    return luai_numlt(cast_num(i), f);  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Iceil))  
      return i < fi;   
    else  
      return f > 0;  
  }
}
l_sinline int LEintfloat (lua_Integer i, lua_Number f) {
  if (l_intfitsf(i))
    return luai_numle(cast_num(i), f);  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Ifloor))  
      return i <= fi;   
    else  
      return f > 0;  
  }
}
l_sinline int LTfloatint (lua_Number f, lua_Integer i) {
  if (l_intfitsf(i))
    return luai_numlt(f, cast_num(i));  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Ifloor))  
      return fi < i;   
    else  
      return f < 0;  
  }
}
l_sinline int LEfloatint (lua_Number f, lua_Integer i) {
  if (l_intfitsf(i))
    return luai_numle(f, cast_num(i));  
  else {  
    lua_Integer fi;
    if (luaV_flttointeger(f, &fi, F2Iceil))  
      return fi <= i;   
    else  
      return f < 0;  
  }
}
l_sinline int LTnum (const TValue *l, const TValue *r) {
  lua_assert(ttisnumber(l) && ttisnumber(r));
  if (ttisinteger(l)) {
    lua_Integer li = ivalue(l);
    if (ttisinteger(r))
      return li < ivalue(r);  
    else  
      return LTintfloat(li, fltvalue(r));  
  }
  else {
    lua_Number lf = fltvalue(l);  
    if (ttisfloat(r))
      return luai_numlt(lf, fltvalue(r));  
    else  
      return LTfloatint(lf, ivalue(r));
  }
}
l_sinline int LEnum (const TValue *l, const TValue *r) {
  lua_assert(ttisnumber(l) && ttisnumber(r));
  if (ttisinteger(l)) {
    lua_Integer li = ivalue(l);
    if (ttisinteger(r))
      return li <= ivalue(r);  
    else  
      return LEintfloat(li, fltvalue(r));  
  }
  else {
    lua_Number lf = fltvalue(l);  
    if (ttisfloat(r))
      return luai_numle(lf, fltvalue(r));  
    else  
      return LEfloatint(lf, ivalue(r));
  }
}
static int lessthanothers (lua_State *L, const TValue *l, const TValue *r) {
  lua_assert(!ttisnumber(l) || !ttisnumber(r));
  if (ttisstring(l) && ttisstring(r))  
    return l_strcmp(tsvalue(l), tsvalue(r)) < 0;
  else
    return luaT_callorderTM(L, l, r, TM_LT);
}
int luaV_lessthan (lua_State *L, const TValue *l, const TValue *r) {
  if (ttisnumber(l) && ttisnumber(r))  
    return LTnum(l, r);
  else return lessthanothers(L, l, r);
}
static int lessequalothers (lua_State *L, const TValue *l, const TValue *r) {
  lua_assert(!ttisnumber(l) || !ttisnumber(r));
  if (ttisstring(l) && ttisstring(r))  
    return l_strcmp(tsvalue(l), tsvalue(r)) <= 0;
  else
    return luaT_callorderTM(L, l, r, TM_LE);
}
int luaV_lessequal (lua_State *L, const TValue *l, const TValue *r) {
  if (ttisnumber(l) && ttisnumber(r))  
    return LEnum(l, r);
  else return lessequalothers(L, l, r);
}
int luaV_equalobj (lua_State *L, const TValue *t1, const TValue *t2) {
  const TValue *tm;
  if (ttypetag(t1) != ttypetag(t2)) {  
    if (ttype(t1) != ttype(t2) || ttype(t1) != LUA_TNUMBER)
      return 0;  
    else {  
      
      lua_Integer i1, i2;
      return (luaV_tointegerns(t1, &i1, F2Ieq) &&
              luaV_tointegerns(t2, &i2, F2Ieq) &&
              i1 == i2);
    }
  }
  
  switch (ttypetag(t1)) {
    case LUA_VNIL: case LUA_VFALSE: case LUA_VTRUE: return 1;
    case LUA_VNUMINT: return (ivalue(t1) == ivalue(t2));
    case LUA_VNUMFLT: return luai_numeq(fltvalue(t1), fltvalue(t2));
    case LUA_VLIGHTUSERDATA: return pvalue(t1) == pvalue(t2);
    case LUA_VLCF: return fvalue(t1) == fvalue(t2);
    case LUA_VSHRSTR: return eqshrstr(tsvalue(t1), tsvalue(t2));
    case LUA_VLNGSTR: return luaS_eqlngstr(tsvalue(t1), tsvalue(t2));
    case LUA_VUSERDATA: {
      if (uvalue(t1) == uvalue(t2)) return 1;
      else if (L == NULL) return 0;
      tm = fasttm(L, uvalue(t1)->metatable, TM_EQ);
      if (tm == NULL)
        tm = fasttm(L, uvalue(t2)->metatable, TM_EQ);
      break;  
    }
    case LUA_VTABLE: {
      if (hvalue(t1) == hvalue(t2)) return 1;
      else if (L == NULL) return 0;
      tm = fasttm(L, hvalue(t1)->metatable, TM_EQ);
      if (tm == NULL)
        tm = fasttm(L, hvalue(t2)->metatable, TM_EQ);
      break;  
    }
    default:
      return gcvalue(t1) == gcvalue(t2);
  }
  if (tm == NULL)  
    return 0;  
  else {
    luaT_callTMres(L, tm, t1, t2, L->top);  
    return !l_isfalse(s2v(L->top));
  }
}
#define tostring(L,o)  	(ttisstring(o) || (cvt2str(o) && (luaO_tostring(L, o), 1)))
#define isemptystr(o)	(ttisshrstring(o) && tsvalue(o)->shrlen == 0)
static void copy2buff (StkId top, int n, char *buff) {
  size_t tl = 0;  
  do {
    size_t l = vslen(s2v(top - n));  
    memcpy(buff + tl, svalue(s2v(top - n)), l * sizeof(char));
    tl += l;
  } while (--n > 0);
}
void luaV_concat (lua_State *L, int total) {
  if (total == 1)
    return;  
  do {
    StkId top = L->top;
    int n = 2;  
    if (!(ttisstring(s2v(top - 2)) || cvt2str(s2v(top - 2))) ||
        !tostring(L, s2v(top - 1)))
      luaT_tryconcatTM(L);
    else if (isemptystr(s2v(top - 1)))  
      cast_void(tostring(L, s2v(top - 2)));  
    else if (isemptystr(s2v(top - 2))) {  
      setobjs2s(L, top - 2, top - 1);  
    }
    else {
      
      size_t tl = vslen(s2v(top - 1));
      TString *ts;
      
      for (n = 1; n < total && tostring(L, s2v(top - n - 1)); n++) {
        size_t l = vslen(s2v(top - n - 1));
        if (l_unlikely(l >= (MAX_SIZE/sizeof(char)) - tl))
          luaG_runerror(L, "string length overflow");
        tl += l;
      }
      if (tl <= LUAI_MAXSHORTLEN) {  
        char buff[LUAI_MAXSHORTLEN];
        copy2buff(top, n, buff);  
        ts = luaS_newlstr(L, buff, tl);
      }
      else {  
        ts = luaS_createlngstrobj(L, tl);
        copy2buff(top, n, getstr(ts));
      }
      setsvalue2s(L, top - n, ts);  
    }
    total -= n-1;  
    L->top -= n-1;  
  } while (total > 1);  
}
void luaV_objlen (lua_State *L, StkId ra, const TValue *rb) {
  const TValue *tm;
  switch (ttypetag(rb)) {
    case LUA_VTABLE: {
      Table *h = hvalue(rb);
      tm = fasttm(L, h->metatable, TM_LEN);
      if (tm) break;  
      setivalue(s2v(ra), luaH_getn(h));  
      return;
    }
    case LUA_VSHRSTR: {
      setivalue(s2v(ra), tsvalue(rb)->shrlen);
      return;
    }
    case LUA_VLNGSTR: {
      setivalue(s2v(ra), tsvalue(rb)->u.lnglen);
      return;
    }
    default: {  
      tm = luaT_gettmbyobj(L, rb, TM_LEN);
      if (l_unlikely(notm(tm)))  
        luaG_typeerror(L, rb, "get length of");
      break;
    }
  }
  luaT_callTMres(L, tm, rb, rb, ra);
}
lua_Integer luaV_idiv (lua_State *L, lua_Integer m, lua_Integer n) {
  if (l_unlikely(l_castS2U(n) + 1u <= 1u)) {  
    if (n == 0)
      luaG_runerror(L, "attempt to divide by zero");
    return intop(-, 0, m);   
  }
  else {
    lua_Integer q = m / n;  
    if ((m ^ n) < 0 && m % n != 0)  
      q -= 1;  
    return q;
  }
}
lua_Integer luaV_mod (lua_State *L, lua_Integer m, lua_Integer n) {
  if (l_unlikely(l_castS2U(n) + 1u <= 1u)) {  
    if (n == 0)
      luaG_runerror(L, "attempt to perform 'n%%0'");
    return 0;   
  }
  else {
    lua_Integer r = m % n;
    if (r != 0 && (r ^ n) < 0)  
      r += n;  
    return r;
  }
}
lua_Number luaV_modf (lua_State *L, lua_Number m, lua_Number n) {
  lua_Number r;
  luai_nummod(L, m, n, r);
  return r;
}
#define NBITS	cast_int(sizeof(lua_Integer) * CHAR_BIT)
#define luaV_shiftr(x,y)	luaV_shiftl(x,intop(-, 0, y))
lua_Integer luaV_shiftl (lua_Integer x, lua_Integer y) {
  if (y < 0) {  
    if (y <= -NBITS) return 0;
    else return intop(>>, x, -y);
  }
  else {  
    if (y >= NBITS) return 0;
    else return intop(<<, x, y);
  }
}
static void pushclosure (lua_State *L, Proto *p, UpVal **encup, StkId base,
                         StkId ra) {
  int nup = p->sizeupvalues;
  Upvaldesc *uv = p->upvalues;
  int i;
  LClosure *ncl = luaF_newLclosure(L, nup);
  ncl->p = p;
  setclLvalue2s(L, ra, ncl);  
  for (i = 0; i < nup; i++) {  
    if (uv[i].instack)  
      ncl->upvals[i] = luaF_findupval(L, base + uv[i].idx);
    else  
      ncl->upvals[i] = encup[uv[i].idx];
    luaC_objbarrier(L, ncl, ncl->upvals[i]);
  }
}
void luaV_finishOp (lua_State *L) {
  CallInfo *ci = L->ci;
  StkId base = ci->func + 1;
  Instruction inst = *(ci->u.l.savedpc - 1);  
  OpCode op = GET_OPCODE(inst);
  switch (op) {  
    case OP_MMBIN: case OP_MMBINI: case OP_MMBINK: {
      setobjs2s(L, base + GETARG_A(*(ci->u.l.savedpc - 2)), --L->top);
      break;
    }
    case OP_UNM: case OP_BNOT: case OP_LEN:
    case OP_GETTABUP: case OP_GETTABLE: case OP_GETI:
    case OP_GETFIELD: case OP_SELF: {
      setobjs2s(L, base + GETARG_A(inst), --L->top);
      break;
    }
    case OP_LT: case OP_LE:
    case OP_LTI: case OP_LEI:
    case OP_GTI: case OP_GEI:
    case OP_EQ: {  
      int res = !l_isfalse(s2v(L->top - 1));
      L->top--;
#if defined(LUA_COMPAT_LT_LE)
      if (ci->callstatus & CIST_LEQ) {  
        ci->callstatus ^= CIST_LEQ;  
        res = !res;  
      }
#endif
      lua_assert(GET_OPCODE(*ci->u.l.savedpc) == OP_JMP);
      if (res != GETARG_k(inst))  
        ci->u.l.savedpc++;  
      break;
    }
    case OP_CONCAT: {
      StkId top = L->top - 1;  
      int a = GETARG_A(inst);      
      int total = cast_int(top - 1 - (base + a));  
      setobjs2s(L, top - 2, top);  
      L->top = top - 1;  
      luaV_concat(L, total);  
      break;
    }
    case OP_CLOSE: {  
      ci->u.l.savedpc--;  
      break;
    }
    case OP_RETURN: {  
      StkId ra = base + GETARG_A(inst);
      
      L->top = ra + ci->u2.nres;
      
      ci->u.l.savedpc--;
      break;
    }
    default: {
      
      lua_assert(op == OP_TFORCALL || op == OP_CALL ||
           op == OP_TAILCALL || op == OP_SETTABUP || op == OP_SETTABLE ||
           op == OP_SETI || op == OP_SETFIELD);
      break;
    }
  }
}
#define l_addi(L,a,b)	intop(+, a, b)
#define l_subi(L,a,b)	intop(-, a, b)
#define l_muli(L,a,b)	intop(*, a, b)
#define l_band(a,b)	intop(&, a, b)
#define l_bor(a,b)	intop(|, a, b)
#define l_bxor(a,b)	intop(^, a, b)
#define l_lti(a,b)	(a < b)
#define l_lei(a,b)	(a <= b)
#define l_gti(a,b)	(a > b)
#define l_gei(a,b)	(a >= b)
#define op_arithI(L,iop,fop) {    TValue *v1 = vRB(i);    int imm = GETARG_sC(i);    if (ttisinteger(v1)) {      lua_Integer iv1 = ivalue(v1);      pc++; setivalue(s2v(ra), iop(L, iv1, imm));    }    else if (ttisfloat(v1)) {      lua_Number nb = fltvalue(v1);      lua_Number fimm = cast_num(imm);      pc++; setfltvalue(s2v(ra), fop(L, nb, fimm));   }}
#define op_arithf_aux(L,v1,v2,fop) {    lua_Number n1; lua_Number n2;    if (tonumberns(v1, n1) && tonumberns(v2, n2)) {      pc++; setfltvalue(s2v(ra), fop(L, n1, n2));    }}
#define op_arithf(L,fop) {    TValue *v1 = vRB(i);    TValue *v2 = vRC(i);    op_arithf_aux(L, v1, v2, fop); }
#define op_arithfK(L,fop) {    TValue *v1 = vRB(i);    TValue *v2 = KC(i); lua_assert(ttisnumber(v2));    op_arithf_aux(L, v1, v2, fop); }
#define op_arith_aux(L,v1,v2,iop,fop) {    if (ttisinteger(v1) && ttisinteger(v2)) {      lua_Integer i1 = ivalue(v1); lua_Integer i2 = ivalue(v2);      pc++; setivalue(s2v(ra), iop(L, i1, i2));    }    else op_arithf_aux(L, v1, v2, fop); }
#define op_arith(L,iop,fop) {    TValue *v1 = vRB(i);    TValue *v2 = vRC(i);    op_arith_aux(L, v1, v2, iop, fop); }
#define op_arithK(L,iop,fop) {    TValue *v1 = vRB(i);    TValue *v2 = KC(i); lua_assert(ttisnumber(v2));    op_arith_aux(L, v1, v2, iop, fop); }
#define op_bitwiseK(L,op) {    TValue *v1 = vRB(i);    TValue *v2 = KC(i);    lua_Integer i1;    lua_Integer i2 = ivalue(v2);    if (tointegerns(v1, &i1)) {      pc++; setivalue(s2v(ra), op(i1, i2));    }}
#define op_bitwise(L,op) {    TValue *v1 = vRB(i);    TValue *v2 = vRC(i);    lua_Integer i1; lua_Integer i2;    if (tointegerns(v1, &i1) && tointegerns(v2, &i2)) {      pc++; setivalue(s2v(ra), op(i1, i2));    }}
#define op_order(L,opi,opn,other) {          int cond;          TValue *rb = vRB(i);          if (ttisinteger(s2v(ra)) && ttisinteger(rb)) {            lua_Integer ia = ivalue(s2v(ra));            lua_Integer ib = ivalue(rb);            cond = opi(ia, ib);          }          else if (ttisnumber(s2v(ra)) && ttisnumber(rb))            cond = opn(s2v(ra), rb);          else            Protect(cond = other(L, s2v(ra), rb));          docondjump(); }
#define op_orderI(L,opi,opf,inv,tm) {          int cond;          int im = GETARG_sB(i);          if (ttisinteger(s2v(ra)))            cond = opi(ivalue(s2v(ra)), im);          else if (ttisfloat(s2v(ra))) {            lua_Number fa = fltvalue(s2v(ra));            lua_Number fim = cast_num(im);            cond = opf(fa, fim);          }          else {            int isf = GETARG_C(i);            Protect(cond = luaT_callorderiTM(L, s2v(ra), im, inv, isf, tm));          }          docondjump(); }
#define RA(i)	(base+GETARG_A(i))
#define RB(i)	(base+GETARG_B(i))
#define vRB(i)	s2v(RB(i))
#define KB(i)	(k+GETARG_B(i))
#define RC(i)	(base+GETARG_C(i))
#define vRC(i)	s2v(RC(i))
#define KC(i)	(k+GETARG_C(i))
#define RKC(i)	((TESTARG_k(i)) ? k + GETARG_C(i) : s2v(base + GETARG_C(i)))
#define updatetrap(ci)  (trap = ci->u.l.trap)
#define updatebase(ci)	(base = ci->func + 1)
#define updatestack(ci)  	{ if (l_unlikely(trap)) { updatebase(ci); ra = RA(i); } }
#define dojump(ci,i,e)	{ pc += GETARG_sJ(i) + e; updatetrap(ci); }
#define donextjump(ci)	{ Instruction ni = *pc; dojump(ci, ni, 1); }
#define docondjump()	if (cond != GETARG_k(i)) pc++; else donextjump(ci);
#define savepc(L)	(ci->u.l.savedpc = pc)
#define savestate(L,ci)		(savepc(L), L->top = ci->top)
#define Protect(exp)  (savestate(L,ci), (exp), updatetrap(ci))
#define ProtectNT(exp)  (savepc(L), (exp), updatetrap(ci))
#define halfProtect(exp)  (savestate(L,ci), (exp))
#define checkGC(L,c)  	{ luaC_condGC(L, (savepc(L), L->top = (c)),                          updatetrap(ci));            luai_threadyield(L); }
#define vmfetch()	{   if (l_unlikely(trap)) {       trap = luaG_traceexec(L, pc);       updatebase(ci);     }   i = *(pc++);   ra = RA(i);  }
#define vmdispatch(o)	switch(o)
#define vmcase(l)	case l:
#define vmbreak		break
void luaV_execute (lua_State *L, CallInfo *ci) {
  LClosure *cl;
  TValue *k;
  StkId base;
  const Instruction *pc;
  int trap;
#if LUA_USE_JUMPTABLE
#undef vmdispatch
#undef vmcase
#undef vmbreak
#define vmdispatch(x)     goto *disptab[x];
#define vmcase(l)     L_##l:
#define vmbreak		vmfetch(); vmdispatch(GET_OPCODE(i));
static const void *const disptab[NUM_OPCODES] = {
#if 0
** you can update the following list with this command:
**
**  sed -n '/^OP_/\!d; s/OP_/\&\&L_OP_/ ; s/,.*/,/ ; s/\/.*// ; p'  lopcodes.h
**
#endif
&&L_OP_MOVE,
&&L_OP_LOADI,
&&L_OP_LOADF,
&&L_OP_LOADK,
&&L_OP_LOADKX,
&&L_OP_LOADFALSE,
&&L_OP_LFALSESKIP,
&&L_OP_LOADTRUE,
&&L_OP_LOADNIL,
&&L_OP_GETUPVAL,
&&L_OP_SETUPVAL,
&&L_OP_GETTABUP,
&&L_OP_GETTABLE,
&&L_OP_GETI,
&&L_OP_GETFIELD,
&&L_OP_SETTABUP,
&&L_OP_SETTABLE,
&&L_OP_SETI,
&&L_OP_SETFIELD,
&&L_OP_NEWTABLE,
&&L_OP_SELF,
&&L_OP_ADDI,
&&L_OP_ADDK,
&&L_OP_SUBK,
&&L_OP_MULK,
&&L_OP_MODK,
&&L_OP_POWK,
&&L_OP_DIVK,
&&L_OP_IDIVK,
&&L_OP_BANDK,
&&L_OP_BORK,
&&L_OP_BXORK,
&&L_OP_SHRI,
&&L_OP_SHLI,
&&L_OP_ADD,
&&L_OP_SUB,
&&L_OP_MUL,
&&L_OP_MOD,
&&L_OP_POW,
&&L_OP_DIV,
&&L_OP_IDIV,
&&L_OP_BAND,
&&L_OP_BOR,
&&L_OP_BXOR,
&&L_OP_SHL,
&&L_OP_SHR,
&&L_OP_MMBIN,
&&L_OP_MMBINI,
&&L_OP_MMBINK,
&&L_OP_UNM,
&&L_OP_BNOT,
&&L_OP_NOT,
&&L_OP_LEN,
&&L_OP_CONCAT,
&&L_OP_CLOSE,
&&L_OP_TBC,
&&L_OP_JMP,
&&L_OP_EQ,
&&L_OP_LT,
&&L_OP_LE,
&&L_OP_EQK,
&&L_OP_EQI,
&&L_OP_LTI,
&&L_OP_LEI,
&&L_OP_GTI,
&&L_OP_GEI,
&&L_OP_TEST,
&&L_OP_TESTSET,
&&L_OP_CALL,
&&L_OP_TAILCALL,
&&L_OP_RETURN,
&&L_OP_RETURN0,
&&L_OP_RETURN1,
&&L_OP_FORLOOP,
&&L_OP_FORPREP,
&&L_OP_TFORPREP,
&&L_OP_TFORCALL,
&&L_OP_TFORLOOP,
&&L_OP_SETLIST,
&&L_OP_CLOSURE,
&&L_OP_VARARG,
&&L_OP_VARARGPREP,
&&L_OP_EXTRAARG
};
#endif
 startfunc:
  trap = L->hookmask;
 returning:  
  cl = clLvalue(s2v(ci->func));
  k = cl->p->k;
  pc = ci->u.l.savedpc;
  if (l_unlikely(trap)) {
    if (pc == cl->p->code) {  
      if (cl->p->is_vararg)
        trap = 0;  
      else  
        luaD_hookcall(L, ci);
    }
    ci->u.l.trap = 1;  
  }
  base = ci->func + 1;
  
  for (;;) {
    Instruction i;  
    StkId ra;  
		if (l_unlikely(trap)) {   
    	trap = luaG_traceexec(L, pc);   
   		updatebase(ci);   
 		} 
 		i = *(pc++); 
  	ra = RA(i);  
    #if 0
      
      printf("line: %d\n", luaG_getfuncline(cl->p, pcRel(pc, cl->p)));
    #endif
    lua_assert(base == ci->func + 1);
    lua_assert(base <= L->top && L->top < L->stack_last);
    
    lua_assert(isIT(i) || (cast_void(L->top = base), 1));
    vmdispatch (GET_OPCODE(i)) {
      vmcase(OP_MOVE) {
        setobjs2s(L, ra, RB(i));
        vmbreak;
      }
      vmcase(OP_LOADI) {
        lua_Integer b = GETARG_sBx(i);
        setivalue(s2v(ra), b);
        vmbreak;
      }
      vmcase(OP_LOADF) {
        int b = GETARG_sBx(i);
        setfltvalue(s2v(ra), cast_num(b));
        vmbreak;
      }
      vmcase(OP_LOADK) {
        TValue *rb = k + GETARG_Bx(i);
        setobj2s(L, ra, rb);
        vmbreak;
      }
      vmcase(OP_LOADKX) {
        TValue *rb;
        rb = k + GETARG_Ax(*pc); pc++;
        setobj2s(L, ra, rb);
        vmbreak;
      }
      vmcase(OP_LOADFALSE) {
        setbfvalue(s2v(ra));
        vmbreak;
      }
      vmcase(OP_LFALSESKIP) {
        setbfvalue(s2v(ra));
        pc++;  
        vmbreak;
      }
      vmcase(OP_LOADTRUE) {
        setbtvalue(s2v(ra));
        vmbreak;
      }
      vmcase(OP_LOADNIL) {
        int b = GETARG_B(i);
        do {
          setnilvalue(s2v(ra++));
        } while (b--);
        vmbreak;
      }
      vmcase(OP_GETUPVAL) {
        int b = GETARG_B(i);
        setobj2s(L, ra, cl->upvals[b]->v);
        vmbreak;
      }
      vmcase(OP_SETUPVAL) {
        UpVal *uv = cl->upvals[GETARG_B(i)];
        setobj(L, uv->v, s2v(ra));
        luaC_barrier(L, uv, s2v(ra));
        vmbreak;
      }
      vmcase(OP_GETTABUP) {
        const TValue *slot;
        TValue *upval = cl->upvals[GETARG_B(i)]->v;
        TValue *rc = KC(i);
        TString *key = tsvalue(rc);  
        if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
          setobj2s(L, ra, slot);
        }
        else
          Protect(luaV_finishget(L, upval, rc, ra, slot));
        vmbreak;
      }
      vmcase(OP_GETTABLE) {
        const TValue *slot;
        TValue *rb = vRB(i);
        TValue *rc = vRC(i);
        lua_Unsigned n;
        if (ttisinteger(rc)  
            ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
            : luaV_fastget(L, rb, rc, slot, luaH_get)) {
          setobj2s(L, ra, slot);
        }
        else
          Protect(luaV_finishget(L, rb, rc, ra, slot));
        vmbreak;
      }
      vmcase(OP_GETI) {
        const TValue *slot;
        TValue *rb = vRB(i);
        int c = GETARG_C(i);
        if (luaV_fastgeti(L, rb, c, slot)) {
          setobj2s(L, ra, slot);
        }
        else {
          TValue key;
          setivalue(&key, c);
          Protect(luaV_finishget(L, rb, &key, ra, slot));
        }
        vmbreak;
      }
      vmcase(OP_GETFIELD) {
        const TValue *slot;
        TValue *rb = vRB(i);
        TValue *rc = KC(i);
        TString *key = tsvalue(rc);  
        if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
          setobj2s(L, ra, slot);
        }
        else
          Protect(luaV_finishget(L, rb, rc, ra, slot));
        vmbreak;
      }
      vmcase(OP_SETTABUP) {
        const TValue *slot;
        TValue *upval = cl->upvals[GETARG_A(i)]->v;
        TValue *rb = KB(i);
        TValue *rc = RKC(i);
        TString *key = tsvalue(rb);  
        if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
          luaV_finishfastset(L, upval, slot, rc);
        }
        else
          Protect(luaV_finishset(L, upval, rb, rc, slot));
        vmbreak;
      }
      vmcase(OP_SETTABLE) {
        const TValue *slot;
        TValue *rb = vRB(i);  
        TValue *rc = RKC(i);  
        lua_Unsigned n;
        if (ttisinteger(rb)  
            ? (cast_void(n = ivalue(rb)), luaV_fastgeti(L, s2v(ra), n, slot))
            : luaV_fastget(L, s2v(ra), rb, slot, luaH_get)) {
          luaV_finishfastset(L, s2v(ra), slot, rc);
        }
        else
          Protect(luaV_finishset(L, s2v(ra), rb, rc, slot));
        vmbreak;
      }
      vmcase(OP_SETI) {
        const TValue *slot;
        int c = GETARG_B(i);
        TValue *rc = RKC(i);
        if (luaV_fastgeti(L, s2v(ra), c, slot)) {
          luaV_finishfastset(L, s2v(ra), slot, rc);
        }
        else {
          TValue key;
          setivalue(&key, c);
          Protect(luaV_finishset(L, s2v(ra), &key, rc, slot));
        }
        vmbreak;
      }
      vmcase(OP_SETFIELD) {
        const TValue *slot;
        TValue *rb = KB(i);
        TValue *rc = RKC(i);
        TString *key = tsvalue(rb);  
        if (luaV_fastget(L, s2v(ra), key, slot, luaH_getshortstr)) {
          luaV_finishfastset(L, s2v(ra), slot, rc);
        }
        else
          Protect(luaV_finishset(L, s2v(ra), rb, rc, slot));
        vmbreak;
      }
      vmcase(OP_NEWTABLE) {
        int b = GETARG_B(i);  
        int c = GETARG_C(i);  
        Table *t;
        if (b > 0)
          b = 1 << (b - 1);  
        lua_assert((!TESTARG_k(i)) == (GETARG_Ax(*pc) == 0));
        if (TESTARG_k(i))  
          c += GETARG_Ax(*pc) * (MAXARG_C + 1);  
        pc++;  
        L->top = ra + 1;  
        t = luaH_new(L);  
        sethvalue2s(L, ra, t);
        if (b != 0 || c != 0)
          luaH_resize(L, t, c, b);  
        checkGC(L, ra + 1);
        vmbreak;
      }
      vmcase(OP_SELF) {
        const TValue *slot;
        TValue *rb = vRB(i);
        TValue *rc = RKC(i);
        TString *key = tsvalue(rc);  
        setobj2s(L, ra + 1, rb);
        if (luaV_fastget(L, rb, key, slot, luaH_getstr)) {
          setobj2s(L, ra, slot);
        }
        else
          Protect(luaV_finishget(L, rb, rc, ra, slot));
        vmbreak;
      }
      vmcase(OP_ADDI) {
        op_arithI(L, l_addi, luai_numadd);
        vmbreak;
      }
      vmcase(OP_ADDK) {
        op_arithK(L, l_addi, luai_numadd);
        vmbreak;
      }
      vmcase(OP_SUBK) {
        op_arithK(L, l_subi, luai_numsub);
        vmbreak;
      }
      vmcase(OP_MULK) {
        op_arithK(L, l_muli, luai_nummul);
        vmbreak;
      }
      vmcase(OP_MODK) {
        op_arithK(L, luaV_mod, luaV_modf);
        vmbreak;
      }
      vmcase(OP_POWK) {
        op_arithfK(L, luai_numpow);
        vmbreak;
      }
      vmcase(OP_DIVK) {
        op_arithfK(L, luai_numdiv);
        vmbreak;
      }
      vmcase(OP_IDIVK) {
        op_arithK(L, luaV_idiv, luai_numidiv);
        vmbreak;
      }
      vmcase(OP_BANDK) {
        op_bitwiseK(L, l_band);
        vmbreak;
      }
      vmcase(OP_BORK) {
        op_bitwiseK(L, l_bor);
        vmbreak;
      }
      vmcase(OP_BXORK) {
        op_bitwiseK(L, l_bxor);
        vmbreak;
      }
      vmcase(OP_SHRI) {
        TValue *rb = vRB(i);
        int ic = GETARG_sC(i);
        lua_Integer ib;
        if (tointegerns(rb, &ib)) {
          pc++; setivalue(s2v(ra), luaV_shiftl(ib, -ic));
        }
        vmbreak;
      }
      vmcase(OP_SHLI) {
        TValue *rb = vRB(i);
        int ic = GETARG_sC(i);
        lua_Integer ib;
        if (tointegerns(rb, &ib)) {
          pc++; setivalue(s2v(ra), luaV_shiftl(ic, ib));
        }
        vmbreak;
      }
      vmcase(OP_ADD) {
        op_arith(L, l_addi, luai_numadd);
        vmbreak;
      }
      vmcase(OP_SUB) {
        op_arith(L, l_subi, luai_numsub);
        vmbreak;
      }
      vmcase(OP_MUL) {
        op_arith(L, l_muli, luai_nummul);
        vmbreak;
      }
      vmcase(OP_MOD) {
        op_arith(L, luaV_mod, luaV_modf);
        vmbreak;
      }
      vmcase(OP_POW) {
        op_arithf(L, luai_numpow);
        vmbreak;
      }
      vmcase(OP_DIV) {  
        op_arithf(L, luai_numdiv);
        vmbreak;
      }
      vmcase(OP_IDIV) {  
        op_arith(L, luaV_idiv, luai_numidiv);
        vmbreak;
      }
      vmcase(OP_BAND) {
        op_bitwise(L, l_band);
        vmbreak;
      }
      vmcase(OP_BOR) {
        op_bitwise(L, l_bor);
        vmbreak;
      }
      vmcase(OP_BXOR) {
        op_bitwise(L, l_bxor);
        vmbreak;
      }
      vmcase(OP_SHR) {
        op_bitwise(L, luaV_shiftr);
        vmbreak;
      }
      vmcase(OP_SHL) {
        op_bitwise(L, luaV_shiftl);
        vmbreak;
      }
      vmcase(OP_MMBIN) {
        Instruction pi = *(pc - 2);  
        TValue *rb = vRB(i);
        TMS tm = (TMS)GETARG_C(i);
        StkId result = RA(pi);
        lua_assert(OP_ADD <= GET_OPCODE(pi) && GET_OPCODE(pi) <= OP_SHR);
        Protect(luaT_trybinTM(L, s2v(ra), rb, result, tm));
        vmbreak;
      }
      vmcase(OP_MMBINI) {
        Instruction pi = *(pc - 2);  
        int imm = GETARG_sB(i);
        TMS tm = (TMS)GETARG_C(i);
        int flip = GETARG_k(i);
        StkId result = RA(pi);
        Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
        vmbreak;
      }
      vmcase(OP_MMBINK) {
        Instruction pi = *(pc - 2);  
        TValue *imm = KB(i);
        TMS tm = (TMS)GETARG_C(i);
        int flip = GETARG_k(i);
        StkId result = RA(pi);
        Protect(luaT_trybinassocTM(L, s2v(ra), imm, flip, result, tm));
        vmbreak;
      }
      vmcase(OP_UNM) {
        TValue *rb = vRB(i);
        lua_Number nb;
        if (ttisinteger(rb)) {
          lua_Integer ib = ivalue(rb);
          setivalue(s2v(ra), intop(-, 0, ib));
        }
        else if (tonumberns(rb, nb)) {
          setfltvalue(s2v(ra), luai_numunm(L, nb));
        }
        else
          Protect(luaT_trybinTM(L, rb, rb, ra, TM_UNM));
        vmbreak;
      }
      vmcase(OP_BNOT) {
        TValue *rb = vRB(i);
        lua_Integer ib;
        if (tointegerns(rb, &ib)) {
          setivalue(s2v(ra), intop(^, ~l_castS2U(0), ib));
        }
        else
          Protect(luaT_trybinTM(L, rb, rb, ra, TM_BNOT));
        vmbreak;
      }
      vmcase(OP_NOT) {
        TValue *rb = vRB(i);
        if (l_isfalse(rb))
          setbtvalue(s2v(ra));
        else
          setbfvalue(s2v(ra));
        vmbreak;
      }
      vmcase(OP_LEN) {
        Protect(luaV_objlen(L, ra, vRB(i)));
        vmbreak;
      }
      vmcase(OP_CONCAT) {
        int n = GETARG_B(i);  
        L->top = ra + n;  
        ProtectNT(luaV_concat(L, n));
        checkGC(L, L->top); 
        vmbreak;
      }
      vmcase(OP_CLOSE) {
        Protect(luaF_close(L, ra, LUA_OK, 1));
        vmbreak;
      }
      vmcase(OP_TBC) {
        
        halfProtect(luaF_newtbcupval(L, ra));
        vmbreak;
      }
      vmcase(OP_JMP) {
        dojump(ci, i, 0);
        vmbreak;
      }
      vmcase(OP_EQ) {
        int cond;
        TValue *rb = vRB(i);
        Protect(cond = luaV_equalobj(L, s2v(ra), rb));
        docondjump();
        vmbreak;
      }
      vmcase(OP_LT) {
        op_order(L, l_lti, LTnum, lessthanothers);
        vmbreak;
      }
      vmcase(OP_LE) {
        op_order(L, l_lei, LEnum, lessequalothers);
        vmbreak;
      }
      vmcase(OP_EQK) {
        TValue *rb = KB(i);
        
        int cond = luaV_rawequalobj(s2v(ra), rb);
        docondjump();
        vmbreak;
      }
      vmcase(OP_EQI) {
        int cond;
        int im = GETARG_sB(i);
        if (ttisinteger(s2v(ra)))
          cond = (ivalue(s2v(ra)) == im);
        else if (ttisfloat(s2v(ra)))
          cond = luai_numeq(fltvalue(s2v(ra)), cast_num(im));
        else
          cond = 0;  
        docondjump();
        vmbreak;
      }
      vmcase(OP_LTI) {
        op_orderI(L, l_lti, luai_numlt, 0, TM_LT);
        vmbreak;
      }
      vmcase(OP_LEI) {
        op_orderI(L, l_lei, luai_numle, 0, TM_LE);
        vmbreak;
      }
      vmcase(OP_GTI) {
        op_orderI(L, l_gti, luai_numgt, 1, TM_LT);
        vmbreak;
      }
      vmcase(OP_GEI) {
        op_orderI(L, l_gei, luai_numge, 1, TM_LE);
        vmbreak;
      }
      vmcase(OP_TEST) {
        int cond = !l_isfalse(s2v(ra));
        docondjump();
        vmbreak;
      }
      vmcase(OP_TESTSET) {
        TValue *rb = vRB(i);
        if (l_isfalse(rb) == GETARG_k(i))
          pc++;
        else {
          setobj2s(L, ra, rb);
          donextjump(ci);
        }
        vmbreak;
      }
      vmcase(OP_CALL) {
        CallInfo *newci;
        int b = GETARG_B(i);
        int nresults = GETARG_C(i) - 1;
        if (b != 0)  
          L->top = ra + b;  
        
        savepc(L);  
        if ((newci = luaD_precall(L, ra, nresults)) == NULL)
          updatetrap(ci);  
        else {  
          ci = newci;
          goto startfunc;
        }
        vmbreak;
      }
      vmcase(OP_TAILCALL) {
        int b = GETARG_B(i);  
        int n;  
        int nparams1 = GETARG_C(i);
        
        int delta = (nparams1) ? ci->u.l.nextraargs + nparams1 : 0;
        if (b != 0)
          L->top = ra + b;
        else  
          b = cast_int(L->top - ra);
        savepc(ci);  
        if (TESTARG_k(i)) {
          luaF_closeupval(L, base);  
          lua_assert(L->tbclist < base);  
          lua_assert(base == ci->func + 1);
        }
        if ((n = luaD_pretailcall(L, ci, ra, b, delta)) < 0)  
          goto startfunc;  
        else {  
          ci->func -= delta;  
          luaD_poscall(L, ci, n);  
          updatetrap(ci);  
          goto ret;  
        }
      }
      vmcase(OP_RETURN) {
        int n = GETARG_B(i) - 1;  
        int nparams1 = GETARG_C(i);
        if (n < 0)  
          n = cast_int(L->top - ra);  
        savepc(ci);
        if (TESTARG_k(i)) {  
          ci->u2.nres = n;  
          if (L->top < ci->top)
            L->top = ci->top;
          luaF_close(L, base, CLOSEKTOP, 1);
          updatetrap(ci);
          updatestack(ci);
        }
        if (nparams1)  
          ci->func -= ci->u.l.nextraargs + nparams1;
        L->top = ra + n;  
        luaD_poscall(L, ci, n);
        updatetrap(ci);  
        goto ret;
      }
      vmcase(OP_RETURN0) {
        if (l_unlikely(L->hookmask)) {
          L->top = ra;
          savepc(ci);
          luaD_poscall(L, ci, 0);  
          trap = 1;
        }
        else {  
          int nres;
          L->ci = ci->previous;  
          L->top = base - 1;
          for (nres = ci->nresults; l_unlikely(nres > 0); nres--)
            setnilvalue(s2v(L->top++));  
        }
        goto ret;
      }
      vmcase(OP_RETURN1) {
        if (l_unlikely(L->hookmask)) {
          L->top = ra + 1;
          savepc(ci);
          luaD_poscall(L, ci, 1);  
          trap = 1;
        }
        else {  
          int nres = ci->nresults;
          L->ci = ci->previous;  
          if (nres == 0)
            L->top = base - 1;  
          else {
            setobjs2s(L, base - 1, ra);  
            L->top = base;
            for (; l_unlikely(nres > 1); nres--)
              setnilvalue(s2v(L->top++));  
          }
        }
       ret:  
        if (ci->callstatus & CIST_FRESH)
          return;  
        else {
          ci = ci->previous;
          goto returning;  
        }
      }
      vmcase(OP_FORLOOP) {
        if (ttisinteger(s2v(ra + 2))) {  
          lua_Unsigned count = l_castS2U(ivalue(s2v(ra + 1)));
          if (count > 0) {  
            lua_Integer step = ivalue(s2v(ra + 2));
            lua_Integer idx = ivalue(s2v(ra));  
            chgivalue(s2v(ra + 1), count - 1);  
            idx = intop(+, idx, step);  
            chgivalue(s2v(ra), idx);  
            setivalue(s2v(ra + 3), idx);  
            pc -= GETARG_Bx(i);  
          }
        }
        else if (floatforloop(ra))  
          pc -= GETARG_Bx(i);  
        updatetrap(ci);  
        vmbreak;
      }
      vmcase(OP_FORPREP) {
        savestate(L, ci);  
        if (forprep(L, ra))
          pc += GETARG_Bx(i) + 1;  
        vmbreak;
      }
      vmcase(OP_TFORPREP) {
        
        halfProtect(luaF_newtbcupval(L, ra + 3));
        pc += GETARG_Bx(i);
        i = *(pc++);  
        lua_assert(GET_OPCODE(i) == OP_TFORCALL && ra == RA(i));
        goto l_tforcall;
      }
      vmcase(OP_TFORCALL) {
       l_tforcall:
        
        
        memcpy(ra + 4, ra, 3 * sizeof(*ra));
        L->top = ra + 4 + 3;
        ProtectNT(luaD_call(L, ra + 4, GETARG_C(i)));  
        updatestack(ci);  
        i = *(pc++);  
        lua_assert(GET_OPCODE(i) == OP_TFORLOOP && ra == RA(i));
        goto l_tforloop;
      }
      vmcase(OP_TFORLOOP) {
        l_tforloop:
        if (!ttisnil(s2v(ra + 4))) {  
          setobjs2s(L, ra + 2, ra + 4);  
          pc -= GETARG_Bx(i);  
        }
        vmbreak;
      }
      vmcase(OP_SETLIST) {
        int n = GETARG_B(i);
        unsigned int last = GETARG_C(i);
        Table *h = hvalue(s2v(ra));
        if (n == 0)
          n = cast_int(L->top - ra) - 1;  
        else
          L->top = ci->top;  
        last += n;
        if (TESTARG_k(i)) {
          last += GETARG_Ax(*pc) * (MAXARG_C + 1);
          pc++;
        }
        if (last > luaH_realasize(h))  
          luaH_resizearray(L, h, last);  
        for (; n > 0; n--) {
          TValue *val = s2v(ra + n);
          setobj2t(L, &h->array[last - 1], val);
          last--;
          luaC_barrierback(L, obj2gco(h), val);
        }
        vmbreak;
      }
      vmcase(OP_CLOSURE) {
        Proto *p = cl->p->p[GETARG_Bx(i)];
        halfProtect(pushclosure(L, p, cl->upvals, base, ra));
        checkGC(L, ra + 1);
        vmbreak;
      }
      vmcase(OP_VARARG) {
        int n = GETARG_C(i) - 1;  
        Protect(luaT_getvarargs(L, ci, ra, n));
        vmbreak;
      }
      vmcase(OP_VARARGPREP) {
        ProtectNT(luaT_adjustvarargs(L, GETARG_A(i), ci, cl->p));
        if (l_unlikely(trap)) {  
          luaD_hookcall(L, ci);
          L->oldpc = 1;  
        }
        updatebase(ci);  
        vmbreak;
      }
      vmcase(OP_EXTRAARG) {
        luai_unreachable(0);
        vmbreak;
      }
			#ifndef LUA_USE_JUMPTABLE
			default : luai_unreachable();
			#endif
    }
  }
}
// root include lzio.c
//included "string.h" 
int luaZ_fill (ZIO *z) {
  size_t size;
  lua_State *L = z->L;
  const char *buff;
  lua_unlock(L);
  buff = z->reader(L, z->data, &size);
  lua_lock(L);
  if (buff == NULL || size == 0)
    return EOZ;
  z->n = size - 1;  
  z->p = buff;
  return cast_uchar(*(z->p++));
}
void luaZ_init (lua_State *L, ZIO *z, lua_Reader reader, void *data) {
  z->L = L;
  z->reader = reader;
  z->data = data;
  z->n = 0;
  z->p = NULL;
}
size_t luaZ_read (ZIO *z, void *b, size_t n) {
  while (n) {
    size_t m;
    if (z->n == 0) {  
      if (luaZ_fill(z) == EOZ)  
        return n;  
      else {
        z->n++;  
        z->p--;
      }
    }
    m = (n <= z->n) ? n : z->n;  
    memcpy(b, z->p, m);
    z->n -= m;
    z->p += m;
    b = (char *)b + m;
    n -= m;
  }
  return 0;
}
