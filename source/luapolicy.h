/*
** $Id: lua.h $
** LuaPolicy - security flags for enviroment and functions.
** Lua.org, PUC-Rio, Brazil (http://www.lua.org)
*/

#ifndef luapolicy_h
#define luapolicy_h

#include "luaconf.h"
#include "lua.h"

/* Theese policy flags, when setted, allows to do named stuff.
 * When unsetted, disallows.
 *
 * They setted globally for the whole lua state and all threads.
 * Trying to implement their dynamic changing is a bad idea.
 *
 * Here is the thing : all unsecure stuff MUST be done in C.
 * You can enable some things in lua ONLY in case CAPI is not a
 * best solution. But with this, you must understand all consiquences */

/* description : allow lua code to access and modify registry
 * security impact : HIGH
 * reasons : C API uses registry for reference counting and metatables.
 * 				 : Poorly C libraries can leak or use after free if their
 * 				 : referenced values gets collected */
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

/* allow lua code to load bytecode
 * security impact : VERY HIGH
 * reasons : bytecode is VERY UNSECURE! 
 *         : Malicious bytecode can crash interpreter */
#define LUAPOLICY_BYTECODE  LUAPOLICY_BYTECODE

/* description : allow lua code to start/stop Garbage collector
 * security impact : LOW 
 * reasons : user can setup GC to run collection cycle forever */
#define LUAPOLICY_CONTROLGC LUAPOLICY_CONTROLGC

/* description : allow lua code to run GCSTEP and full collection
 * security impact : VERY LOW 
 * reasons : user can spam collectgarbage(), but nothing very bad here.
 * 				 : going out of memory in case of table spamming is much
 * 				 : worse than this :) */
#define LUAPOLICY_CANRUNGC  LUAPOLICY_CANRUNGC

/* description : this flag does nothing. IO library developers 
 * 						 : should check for this flag in their libs. 
 * 						 : Without this flag, it should be disallowed to
 * 						 : open/read/write files outside of some directory,
 * 						 : or to do files at all (except stdin/stdout/stderr).
 * 						 : Also should apply to directory/links/pipe managment.
 * security impact : INSANE INSANE INSANE INSANE
 * reasons : If your app/game/service crashes, it's bad, it's disturbing
 * 				 : but can be fixed (just restart it). BUT with filesystems,
 * 				 : what can contain user's personal data, passwords, documents,
 * 				 : NFT's, payment(bitcoin and etc.), corporative data, etc and etc.
 * 				 : losing SOMETHING OF THIS can cause user/office workers/developer to
 * 				 : VERY FUCKIN BIG TROUBLES. NEVER. GIVE. ACCESS. FOR. UNKNOWN. CODE.
 * 				 : TO. THE. WHOLE. FILESYSTEM. 
 * p.s: of cource, modern systems and corporative setups has some security
 * things like file acess modes, groups of users and etc. but OFTEN they are NOT
 * USED, or USED POORLY, even when it's REALLY important to do in modern times.
 */

#define LUAPOLICY_FILESYSTEM LUAPOLICY_FILESYSTEM

/* description : allows debug.getlocal/debud.setlocal, function retrival and other stuff...
 * 						 : by default, theese functions are disabled anyway, but just in case.
 * security impact : HIGH
 * reasons : with libraries, that assumes unreachability of upvalues, bad things can happen.
 */
#define LUAPOLICY_EXTRADEBUG LUAPOLICY_EXTRADEBUG

/* description : does nothing. BUT
 * 						 : IF YOU DON'T LISTEN ABOUT WARNINGS to NOT GIVE 
 * 						 : policy control to LUA, then at least check FOR THIS 
 * 						 : FLAG in your wrapper.
 * security impact : HIGHEST
 * reasons : with this all policy system becomes a fuckin joke, and ALL
 * 				 : THE WORTHEST THINGS COULD HAPPEN!*/
#define LUAPOLICY_POLICYCTL LUAPOLICY_POLICYCTL

/* default option */
#define LUAPOLICY_DEFAULT LUAPOLICY_CANRUNGC | LUAPOLICY_CONTROLGC 

/* Returns OR'ed global policy flags
 * Check for flag : RETURNED_FLAGS & NEEDED_FLAG
 */
LUA_API int lua_getpolicy(lua_State* L); 

#define lua_getsubpolicy(L, FLAG) (lua_getpolicy(L) & FLAG)

/* Changes global policy flags
 * needed flags should be logically OR'ed (|)
 * unneeded just not be added.
 *
 * DO NOT GIVE THIS FUNCTION TO LUA!
 * Does not changes current frame flags */
LUA_API void lua_setpolicy(lua_State* L, int flags);

/* Aux api
 * Returns CSTRING name of ONE policy flag!
 */
LUALIB_API const char* luaL_policyname(lua_State* L, int flag);

/* Aux api
 * Checks for policy flag setted, raise an error if not setted.
 */
LUALIB_API void luaL_checkpolicy(lua_State *L, int flag);

#endif
