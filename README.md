# OnlyLua
Only lua - minified 5.3 lua, only luaCore and minified baselib.
Features :
- no `io`, `os`, and `debug` unsafe libraries. Actually no base library here :D
- no dynamic C libraries loading
- no C auxlib *(you can export vanilla auxlib sources if you need it)*
- **DOXYGEN** documentation OH BOI :D
- Inspired by *minilua*, all lua is packed in 2 files : `lua.h` - standart lua api, and `lua.c` - full lua core and baselib compiled staticly (amalgam compilation).

# why?

This is a good base for your own standard lua library, or for embendding lua in trully safe manner. (how i planned to use this)   

# changes in baselib 
- no get/setmetatable on anything, that is not a table
- remove auxlib dependency, remove some strange, useless (at my opinion) extensions

# how to include in your project
- Remember to follow lua licensing terms : [LICENSE](LICENSE).
- Install lua (that's right :D), cd into source directory, and run `lua make.lua`
- Copy `lua.h` and `lua.c` in your C/C++ project
- Enjoy :D
- P.S possible build command : `gcc -fPIC -shared -I. lua.c -o lua.so -Wall -Wextra`
