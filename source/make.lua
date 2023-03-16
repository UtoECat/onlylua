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
	"lapi.c", "lcode.c", "lctype.c", "ldebug.c", "ldo.c", "ldump.c",
	"lfunc.c", "lgc.c", "linit.c", "llex.c", "lmem.c", "lobject.c",
	"lopcodes.c", "lparser.c", "lstate.c", "lstring.c", "ltable.c",
	"ltm.c", "lundump.c", "lvm.c", "lzio.c"
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

local buf = {}

local function donefile(file, append)
	local str = table.concat(buf, '\n')
	buf = {}
	included = {}
	collectgarbage()
	
	str = str:gsub("/%*.-%*/", "") -- remove /* */ commentaries
	str = str:gsub("\n\n+", "\n\n") -- remove extra newlines...
	str = append .. str

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
donefile('../lua.c', "#include \"lua.h\"\n/*\n".. COPYRIGHT .."\n*/")

-- do lua.h
buf[1] = incfile("lua.h")

-- pack and write :D
donefile('../lua.h', "/*\n".. COPYRIGHT .."\n*/")

print("Done! :) See files lua.c and lua.h in THE ROOT OF THIS REPOSITORY!")
