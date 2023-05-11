// Wrapper this around lua source string in C file
// In case preprocessing is done, this will be inlined directly as precompiled bytecode :D

#define LUA_BCDEF(NAME, SRC) \
static const char BC_##NAME##_DATA[] = SRC;\
static const size_t BC_##NAME##_SIZE = sizeof(BC_##NAME##_DATA);

#if defined(COMPILED_BYTECODE)
	#define INTERNAL_MODE '\033'
	#define _TOKENIZE(A) #A
	#define LUA_BCLOAD(L, NAME) luaL_loadbufferx(L, BC_##NAME##_DATA, BC_##NAME##_SIZE, _TOKENIZE(NAME), INTERNAL_MODE);
#else
// cause source always has main chunk (except for precompiled ones)
	#define LUA_BCLOAD(L, NAME) luaL_loadbufferx(L, BC_##NAME##_DATA, BC_##NAME##_SIZE, NULL, "i"); lua_call(L, 0, 0);
#endif
