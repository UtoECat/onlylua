# OnlyLua
Only lua - minified 5.3 lua, only luaCore and minified baselib.
Features :
- no `io`, `os`, and `debug` unsafe libraries. Actually no standart libraries here :D
- no dynamic C libraries loading
- no C auxlib *(you can probably export vanilla auxlib sources if you really need it)*
- **DOXYGEN** documentation ~~OH BOI :D~~ is in progress .\_.
- Inspired by *minilua*, all lua is packed in 2 files : `lua.h` - standart lua api, and `lua.c` - full lua core and baselib packed in one file for amalgam compilation

# why?

This is a good base for your own standard lua library, or for embendding lua in trully safe manner. (how i planned to use this)   

# changes in baselib 
- no get/setmetatable on anything, that is not a table
- remove auxlib dependency, remove some strange, useless (at my opinion) extensions
- remove stdio-depeneded `loadfile`, `print` and etc.

# CAPI extensions
## lua\_anytostring
```c 
const char* (lua_anytostring) (lua_State *L, int idx)
```
Makes cast anything to string :
- `LUA_TNIL` gives **"nil"** string
- `LUA_TBOOLEAN` gives **"true"** and **"false"** strings
- `LUA_TNUMBER` will be converted to the string
- `LUA_TSTRING` will be just returned
- any other type will be converted to **"typename (address)"**. (*__name* methametod for userdata and tables used too)
This function do not pushes anything to stack - it replaces old value on the stack with string (like `lua_tostring` does).
## lua\_loadbufferx
```c
int lua_loadbufferx(lua_State* L, const char* str, size_t len, const char* chunkname, const char* mode);
```
Simple wrapper around `lua_load` function to load lua code from fixed string buffer, not a callback with userdata.

# how to include in your project
- Remember to follow lua licensing terms : [LICENSE](LICENSE).
- **(Optional)** *Install lua (if you want to have possibility to rebuild `lua.c` and `lua.h`)*
- **(Optional)** *Change some luaconf.h flags for your purposes, and in* `source` *directory run* `lua make.lua` *, to build your customized* `lua.c` *and* `lua.h` *:)*
- Copy `lua.h` and `lua.c` to the your C/C++ project
- Enjoy :D

# TODO and known issues, warnings, or additional info.
- **WARNING** : **AVOID USAGE OF -fanalyzer compiler flag ON DEVICES WITH LESS THAN 8 GIGS OF RAM!** 
- *TODO* Need to make doxygen documentation for CAPI and custom baselib :)
- *TODO* Need to fix some compiler warnings in luavm (.\_.)
- *TODO* Need to change commentary removing a bit... To keep doxygen commentaries in future...
- *TODO* Look for and remove unused preprocessor macros
- *TODO* test custom extensions a bit more...
- **IMPORTANT TODO** mark all private functions as static...

# how to build a test example ? (unix only)
It's not very recommended to do that... But... Here is :
- Make sure *lua* and *Gnu Make* is installed and acessable in your system.
- `cd ` into root of this repository, and run `make`
- You will see some errors from `make.lua` about not founded `stdint.h`, `string.h` - that's ok :)
- At the end you will see `testlua` in the root of the repository.
- You can run example with lua file in argument, to test... something :)

*It's should be possible to build this on windows, but some changes required in makefile for this : change compiler for suitable for you and name of the target as luatest.exe :)*
