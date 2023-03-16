/*
 * DO NOT USE THIS FILE!
 * It's main purpose - testing.
 * 
 * Same license as license in lua.h file!
 */

#include <lua.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

static size_t memory_used = 0;

static void *customAlloc (void*, void *ptr, size_t osize, size_t nsize) {
	if (!nsize) {
		memory_used -= osize;
		free(ptr); return NULL;
	} else {  /* cannot fail when shrinking a block */
		void *newptr = realloc(ptr, nsize);

		if (!ptr && newptr) memory_used += nsize;
		if (ptr  && newptr) {
			memory_used -= osize;
			memory_used += nsize;
		}

		if (newptr == NULL && ptr != NULL && nsize <= osize)
			return ptr;  /* keep the original block */
		else  /* no fail or not shrinking */
			return newptr;  /* use the new block */
  }
}

static int panic (lua_State *L) {
  fprintf(stderr, "PANIC: unprotected error (%s)\n", lua_tostring(L, -1));
  return 0;  /* return to Lua to abort */
}

static int Bmemory(lua_State* L) {
	lua_pushnumber(L, memory_used);
	return 1;
}

// there is no print function
static int myprint(lua_State* L) {
	int n = lua_gettop(L); // count
	for (int i = 1; i <= n; i++) {
		fprintf(stdout, "%s ", idxtostr(L, i));
	}
	fputc('\n', stdout);
	return 0;
}


int main() {
	lua_State *L = lua_newstate(customAlloc, NULL);
	lua_atpanic(L, panic);

	luaopen_base();
	lua_pushcfunction(L, Bmemory);
	lua_setglobal(L, "memusage");
	lua_pushcfunction(L, myprint);
	lua_setglobal(L, "print");

	lua_close(L);
}
