-- some declarations first

local COPYRIGHT = [[
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
]]

local sources = {
	"lapi.c", "lauxlib.c", "lbaselib.c","lcode.c",
	"lcorolib.c", "lctype.c", "ldblib.c", "ldebug.c",
	"ldo.c", "ldump.c", "lfunc.c", "lgc.c", "linit.c",
	"llex.c", "lmathlib.c", "lmem.c", "lobject.c",
	"lopcodes.c", "lparser.c", "lstate.c",
	"lstring.c", "lstrlib.c", "ltable.c", "ltablib.c",
	"ltests.c", "ltm.c", "lundump.c", "lutf8lib.c", "lvm.c",
	"lzio.c"
}


-- some useful functions

local function readfile(n)
	local f, err = io.open(n, 'r')
	if err then
		print("Can't open file " ..n.. " : " .. err)
		return "#include <"..n..">\n", false
	end
	local str = f:read('a')
	f:close()
	return str, true
end

local included = {}

-- recursive :D
local function incfile(name)
	if not included[name] then
		print("Including", name, "!")
		local txt, stat = readfile(name)
		included[name] = true
		if stat then
			return ("\n"..txt):gsub('\n?%s*#%s*include%s+["<](.-)[">]', incfile)
		else
			return "\n"..txt -- include error :(
		end	
	end
	return '// include '..name..' \n' -- no double inclusion
end

-- unused macros to remove
local unused_macros_orig = {
	"LUA_CORE"
}
local unused_macros = {}
for _,v in pairs(unused_macros_orig) do
	unused_macros[v] = true
end

local function unused_check(m, v)
	if unused_macros[m] or included[m:gsub("_", ".")] then
		print("[MACRO] : unused macros '"..m.."' removed!")
		return "\n\n"
	end
	--print("[MACRO] : '"..m.."' = "..v)
end

local buf = {}

local function donefile(file, append)
	local str = table.concat(buf, '\n')
	buf = {}
	collectgarbage()
	
	str = str:gsub("/%*.-%*/", "") -- remove /* */ commentaries
	str = str:gsub("\\%s-\n", "") -- remove continue line
	str = str:gsub("\n?%s*#%s*define[ \t]+([_%w]*)[ \t]?(.-)\n", unused_check)
	str = str:gsub("\n\n+", "\n\n") -- remove extra newlines...
	str = append .. str
	included = {}

	local f = io.open(file, "w")
	f:write(str)
	f:flush()
	f:close()
end

-- do lua.c

included = {["lua.h"] = true}

for _, v in pairs(sources) do
	buf[#buf + 1] = incfile(v)
end

-- pack and write lua.c!
donefile('../lua.c', [[
#define LUA_CORE
#include "lua.h"
/*
".. COPYRIGHT .."
*/
]])

-- do lua.h
buf[1] = incfile("lua.h")
buf[#buf + 1] = incfile("lualib.h")

-- pack and write :D
donefile('../lua.h', "/*\n".. COPYRIGHT .."\n*/")

print("Done! :) See files lua.c and lua.h in THE ROOT OF THIS REPOSITORY!")
