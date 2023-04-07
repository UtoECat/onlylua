/*
 * DO NOT USE THIS FILE DIRECTLY!
 * It's main purpose - testing.
 *
 * But you can take some parts of code for u :З
 * 
 * Same license as license in lua.h file!
 */

#ifdef NOPACK
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#else
#include <lua.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>

static int osclock (lua_State *L) {
  lua_pushnumber(L, ((lua_Number)clock())/(lua_Number)CLOCKS_PER_SEC);
  return 1;
}

static int getreg (lua_State *L) {
	lua_pushvalue(L, LUA_REGISTRYINDEX);
  return 1;
}

static int stacktrace(lua_State* L) {
	if (lua_isnone(L, 1)) lua_pushstring(L, "fuck");
	luaL_traceback(L, L, lua_tostring(L, -1), 1);
	return 1;
}

/****************
 * We need to load files somehow...
 */

typedef struct LoadF {
  int n;  /* number of pre-read characters */
  FILE *f;  /* file being read */
  char buff[BUFSIZ];  /* area for reading file */
} LoadF;

static const char *getF (lua_State *L, void *ud, size_t *size) {
  LoadF *lf = (LoadF *)ud;
  (void)L;  /* not used */
  if (lf->n > 0) {  /* are there pre-read characters to be read? */
    *size = lf->n;  /* return them (chars already in buffer) */
    lf->n = 0;  /* no more pre-read characters */
  }
  else {
    if (feof(lf->f)) return NULL;
    *size = fread(lf->buff, 1, sizeof(lf->buff), lf->f);  /* read block */
  }
  return lf->buff;
}

static int errfile (lua_State *L, const char *what, int fnameindex) {
  const char *serr = strerror(errno);
  const char *filename = lua_tostring(L, fnameindex) + 1;
  lua_pushfstring(L, "cannot %s %s: %s", what, filename, serr);
  lua_remove(L, fnameindex);
  return LUA_ERRERR;
}

static int skipBOM (LoadF *lf) {
  const char *p = "\xEF\xBB\xBF";  /* UTF-8 BOM mark */
  int c;
  lf->n = 0;
  do {
    c = getc(lf->f);
    if (c == EOF || c != *(const unsigned char *)p++) return c;
    lf->buff[lf->n++] = c;  /* to be read by the parser */
  } while (*p != '\0');
  lf->n = 0;  /* prefix matched; discard it */
  return getc(lf->f);  /* return next character */
}


/*
** reads the first character of file 'f' and skips an optional BOM mark
** in its beginning plus its first line if it starts with '#'. Returns
** true if it skipped the first line.  In any case, '*cp' has the
** first "valid" character of the file (after the optional BOM and
** a first-line comment).
*/
static int skipcomment (LoadF *lf, int *cp) {
  int c = *cp = skipBOM(lf);
  if (c == '#') {  /* first line is a comment (Unix exec. file)? */
    do {  /* skip first line */
      c = getc(lf->f);
    } while (c != EOF && c != '\n');
    *cp = getc(lf->f);  /* skip end-of-line, if present */
    return 1;  /* there was a comment */
  }
  else return 0;  /* no comment */
}

LUALIB_API int loadfilex (lua_State *L, const char *filename, const char *mode) {
  LoadF lf;
  int status, readstatus;
  int c;
  int fnameindex = lua_gettop(L) + 1;  /* index of filename on the stack */
  if (filename == NULL) {
    lua_pushliteral(L, "=stdin");
    lf.f = stdin;
  }
  else {
    lua_pushfstring(L, "@%s", filename);
    lf.f = fopen(filename, "r");
    if (lf.f == NULL) return errfile(L, "open", fnameindex);
  }
  if (skipcomment(&lf, &c))  /* read initial portion */
    lf.buff[lf.n++] = '\n';  /* add line to correct line numbers */
  if (c == LUA_SIGNATURE[0] && filename) {  /* binary file? */
    lf.f = freopen(filename, "rb", lf.f);  /* reopen in binary mode */
    if (lf.f == NULL) return errfile(L, "reopen", fnameindex);
    skipcomment(&lf, &c);  /* re-read initial portion */
  }
  if (c != EOF)
    lf.buff[lf.n++] = c;  /* 'c' is the first character of the stream */
  status = lua_load(L, getF, &lf, lua_tostring(L, -1), mode);
  readstatus = ferror(lf.f);
  if (filename) fclose(lf.f);  /* close file (even in case of errors) */
  if (readstatus) {
    lua_settop(L, fnameindex);  /* ignore results from 'lua_load' */
    return errfile(L, "read", fnameindex);
  }
  lua_remove(L, fnameindex);
  return status;
}

/*************
 * And lua wrapper. Theese functions are REMOVED from baselib... so add them here...
 */

static int load_aux (lua_State *L, int status, int envidx) {
  if (luai_likely(status == LUA_OK)) {
    if (envidx != 0) {  /* 'env' parameter? */
      lua_pushvalue(L, envidx);  /* environment for loaded function */
      if (!lua_setupvalue(L, -2, 1))  /* set it as 1st upvalue */
        lua_pop(L, 1);  /* remove 'env' if not used by previous call */
    }
    return 1;
  }
  else {  /* error (message is on top of the stack) */
    luaL_pushfail(L);
    lua_insert(L, -2);  /* put before error message */
    return 2;  /* return fail plus error message */
  }
}

static int luaB_loadfile (lua_State *L) {
  const char *fname = luaL_optstring(L, 1, NULL);
  const char *mode = luaL_optstring(L, 2, NULL);
  int env = (!lua_isnone(L, 3) ? 3 : 0);  /* 'env' index or 0 if no 'env' */
  int status = loadfilex(L, fname, mode);
  return load_aux(L, status, env);
}

static int dofilecont (lua_State *L, int d1, lua_KContext d2) {
  (void)d1;  (void)d2;  /* only to match 'lua_Kfunction' prototype */
  return lua_gettop(L) - 1;
}

static int luaB_dofile (lua_State *L) {
  const char *fname = luaL_optstring(L, 1, NULL);
  lua_settop(L, 1);
  if (luai_unlikely(loadfilex(L, fname, "bt") != LUA_OK)) {
    return lua_error(L);
	}
  lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
  return dofilecont(L, 0, 0);
}

/*
 * Don't ask...
 * There was some dragons... Now they are done :D
 */

int protected (lua_State* L) {
	luaL_openlibs(L);

	// required for testing. You should NOT use this in your release application!
	luaL_requiref(L, "debug", luaopen_debug, 1);

	lua_pushcfunction(L, osclock);
	lua_setglobal(L, "clock");
	lua_pushcfunction(L, getreg);
	lua_setglobal(L, "getregistry");
	lua_pushcfunction(L, luaB_loadfile);
	lua_setglobal(L, "loadfile");
	lua_pushcfunction(L, luaB_dofile);
	lua_setglobal(L, "dofile");
	return 0;
}

void myHook (lua_State* L, lua_Debug* ar) {
	if (ar->event == LUA_HOOKLINE || ar->event == LUA_HOOKCOUNT) {
		lua_getinfo(L, "Snl", ar);
		fprintf(stderr, "HOOK (LINE) > %s (%s) %s at %i\n", ar->namewhat, ar->name, ar->source, ar->currentline);
	} else if (ar->event == LUA_HOOKCALL || ar->event == LUA_HOOKTAILCALL) {
		lua_getinfo(L, "Snl", ar);
		fprintf(stderr, "HOOK (CALL) > %s (%s) %s at %i\n", ar->namewhat, ar->name, ar->source, ar->currentline);
	} else {
		lua_getinfo(L, "Snl", ar);
		fprintf(stderr, "HOOK (RETN) > %s (%s) %s at %i\n", ar->namewhat, ar->name, ar->source, ar->currentline);
	}
}

int main(int l, const char** argv) {
	if (l < 2) {
		fileerr:
		perror("file argument required!\n");
		return -1;
	}

	const char* filename = NULL;
	int hook = 0;

	for (char** p = argv + 1; *p; p++) {
		if (p[0][0] == '-' && p[0][1] == 'h') hook = 1;
		else if (p[0][0] == '-') {
			fprintf(stderr, "bad flag! only -h is supported!");				
			return -1;
		} else filename = p[0];
	}

	if (!filename) goto fileerr;

	lua_State *L = luaL_newstate();

	if (hook)
	lua_sethook(L, myHook, LUA_MASKCALL | LUA_MASKRET, 1);

	lua_pushcclosure(L, stacktrace, 0);
	lua_pushcclosure(L, protected, 0);
	if (lua_pcallk(L, 0, 1, -2, 0, NULL) != LUA_OK) {
		printf("Error! %s\n", lua_tostring(L, -1));
		return -66;
	}

	lua_pushcclosure(L, stacktrace, 0);
	if (loadfilex(L, filename, "bt") != LUA_OK) {
		fprintf(stderr, "Can't load lua source! (%s, %s, %i)\n", lua_tostring(L, -1), filename, lua_gettop(L));
		return -3;
	}

	if (lua_pcallk(L, 0, 0, -2, 0, NULL) != LUA_OK) {
		printf("Error! (%s)\n", lua_tostring(L, -1));
	};

	lua_close(L);
	return 0;
}
