# OnlyLua
OnlyLua - minified and modified 5.4 lua, only luaCore and minified base libraries.
# Features :
1. no `io`, and `os`, libraries, no loadfile/dofile() function by default.
2. Lua debug is not loaded by default, some unsafe functions was removed
3. no dynamic runtime CAPI libraries loading *(only basic require)*
4. **DOXYGEN** documentation is in progress .\_.
5. Inspired by *minilua*, all lua is packed in 2/3 files : 
- `lua.h` - standart lua api + `lauxlib.h` and `lualib.h` in one file
- `lua.c` - full lua core packed in one file for amalgam compilation
- `lualib.c` - all lua libraries packed into one c file.
6. Some small perfomance extensions : 
- typenames are GCFIXED now
- `void lua_pushobjtype(lua_State* L, int idx)` returns it to the stack without expensive string table lookups.
7. Some CAPI/lua baselib useful but not very compatible changes
8. Policy system *see luapolicy.h*

## warning
this project is still in development. If you have some issues - please send them to the `issues` on the GitHub page with this repository : [link](https://github.com/UtoECat/onlylua)

# why?
This is a good base for your own standard lua library, or for embendding lua in trully safe or fast manner. (how i planned to use this)   

# changes in baselib
- `get/setmetatable` is allowed **ONLY** for the tables.
- `type()` now will return `__name` field in metatable (if exists)
- `rawtype()` works like original type
- removed strange (at my opinion) extensions in baselib
- some functions was simplified
- some functions was rewrited on lua itself (`require`)
- `table.new()` function - create table with specified hash and array length
- `table.clear()` function - cleanups table in fastest way possible

# CAPI extensions
see `lextensions.c` file for them.

# how to include in your project
- Remember to follow lua licensing terms : [LICENSE](LICENSE).
- **(Optional)** *Install lua (if you want to have possibility to rebuild `lua.c` and `lua.h`)*
- **(Optional)** *Change some luaconf.h flags for your purposes, and in* `source` *directory run* `lua make.lua` *, to build your customized* `lua.c` *and* `lua.h` *:)*
- Copy `lua.h` and `lua.c` to the your project
- Enjoy :D

*WARNING* this packing of lua sources breaks compability with C++! be careful, and build lua.c with c compiler, and nothing more!

# TODO and known issues, warnings, or additional info.
- **WARNING** : **AVOID USAGE OF -fanalyzer compiler flag ON DEVICES WITH LESS THAN 4-6 GIGS OF RAM!** 
- *TODO* Need to make doxygen documentation for CAPI and custom baselib :)
- *TODO* test custom extensions a bit more...
- **ISSUE** this packed lua version is not very `c++-compat`. Some fixes are required.
- **TODO** remove locale and stdio dependency!
- **TODO** Add police system checks in libs
- **TODO** fix copyrights, optimize some libraries code
- **TODO** Implement custom internal block allocator for GC objects

- **HELPFUL TIP** Compile your project and lua sources with -flto and -O2/-O3! It becomes really faster. 

# how to build a test example ? (unix only)
It's not very recommended to do that... But... Here is :
- Make sure *lua* and *Gnu Make* is installed and acessable in your system.
- `cd ` into the root directory of this repository, and run `make`
- You will see some errors from `make.lua` about not founded `stdint.h`, `string.h` and etc. - that's ok :)
- At the end you will see `luatest` in the root of the repository.
- You can run example with lua file in argument, to test... something :)

## building for windows on windows
- use same makefile
## building for windows from unix
- install `x86-64-w64-mingw32` compiler
- run `make TARGET=Windows_NT`

**WARNING! BUILDING FROM WINDOWS TO UNIX IS NOT IMPLEMENTED!**
