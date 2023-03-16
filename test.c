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
		fprintf(stdout, "%s ", lua_anytostring(L, i));
	}
	fputc('\n', stdout);
	return 0;
}

struct fld {
	FILE* f;
	char buff[255];
};

static const char* filereader(lua_State*, void* ud, size_t* len) {
	struct fld* x = (struct fld*) ud;
	*len = fread(x->buff, 1, 254, x->f);
	return x->buff;
};

int main(int l, const char** argv) {
	if (l < 2) {
		perror("file argument required!");
		return -1;
	}
	lua_State *L = lua_newstate(customAlloc, NULL);
	lua_atpanic(L, panic);

	luaopen_base(L);
	lua_pushcfunction(L, Bmemory);
	lua_setglobal(L, "memusage");
	lua_pushcfunction(L, myprint);
	lua_setglobal(L, "print");

	FILE* f = fopen(argv[1], "r");
	if (!f) {
		perror("Can't open file!");
		return -2;
	}

	struct fld x = {f, {0}};
	if (lua_load(L, filereader, &x, argv[1], "t") != LUA_OK) {
		fprintf(stderr, "Can't load lua source! (%i)\n", lua_status(L));
		fprintf(stdout, "%s\n", lua_anytostring(L, -1));
		return -3;
	}
	lua_callk(L, 0, 0, 0, NULL);
	lua_close(L);
	return 0;
}
