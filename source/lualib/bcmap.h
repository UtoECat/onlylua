// Wrapper this around lua source string in C file
// In case preprocessing is done, this will be inlined directly as precompiled bytecode :D

#define LUA_BCDEF(NAME, SRC) \
static const char BC_##NAME##_DATA[] = SRC;\
static const size_t BC_##NAME##_SIZE = sizeof(BC_##NAME##_DATA);

#define _TOKENIZE(A) #A
// cause source always has main chunk (except for precompiled ones)
// Loads and runs bytecode with one argument from the top of the stack
// (keeping it on the stack). Returns nothing
#ifdef NOPACK
#define LUA_BCLOAD(L, NAME) luaL_loadbufferx(L, BC_##NAME##_DATA, BC_##NAME##_SIZE, _TOKENIZE(NAME), "t") != LUA_OK ? lua_error(L) : 0; \
	lua_pushvalue(L, -2); lua_call(L, 1, 0);
#else
#define LUA_BCLOAD(L, NAME) luaL_loadbufferx(L, BC_##NAME##_DATA, BC_##NAME##_SIZE, _TOKENIZE(NAME), "b") != LUA_OK ? lua_error(L) : 0; \
	lua_pushvalue(L, -2); lua_call(L, 1, 0);
#endif
