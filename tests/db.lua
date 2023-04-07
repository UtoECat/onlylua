-- $Id: testes/db.lua $
-- See Copyright Notice in file all.lua

-- testing debug library

local debug = require "debug"

local function dostring(s) return assert(load(s))() end

print"testing debug library and debug information"

do
local a=1
end

assert(not debug.gethook())

local testline = 19         -- line where 'test' is defined
function test (s, l, p)     -- this must be line 19
  collectgarbage()   -- avoid gc during trace
  local function f (event, line)
    assert(event == 'line')
    local l = table.remove(l, 1)
    if p then print(l, line) end
    assert(l == line, "wrong trace!!")
  end
  debug.sethook(f,"l"); load(s)(); debug.sethook()
  assert(#l == 0)
end

do
  assert(not pcall(debug.getinfo, print, "X"))   -- invalid option
  assert(not pcall(debug.getinfo, 0, ">"))   -- invalid option
  assert(not debug.getinfo(1000))   -- out of range level
  assert(not debug.getinfo(-1))     -- out of range level
  local a = debug.getinfo(print)
  assert(a.what == "C" and a.short_src == "[C]")
  a = debug.getinfo(print, "L")
  assert(a.activelines == nil)
  local b = debug.getinfo(test, "SfL")
  assert(b.name == nil and b.what == "Lua" and b.linedefined == testline and
         b.lastlinedefined == b.linedefined + 10 and
         b.func == test and not string.find(b.short_src, "%["))
  assert(b.activelines[b.linedefined + 1] and
         b.activelines[b.lastlinedefined])
  assert(not b.activelines[b.linedefined] and
         not b.activelines[b.lastlinedefined + 1])
end


-- test file and string names truncation
a = "function f () end"
local function dostring (s, x) return load(s, x)() end
dostring(a)
assert(debug.getinfo(f).short_src == string.format('[string "%s"]', a))
dostring(a..string.format("; %s\n=1", string.rep('p', 400)))
assert(string.find(debug.getinfo(f).short_src, '^%[string [^\n]*%.%.%."%]$'))
dostring(a..string.format("; %s=1", string.rep('p', 400)))
assert(string.find(debug.getinfo(f).short_src, '^%[string [^\n]*%.%.%."%]$'))
dostring("\n"..a)
assert(debug.getinfo(f).short_src == '[string "..."]')
dostring(a, "")
assert(debug.getinfo(f).short_src == '[string ""]')
dostring(a, "@xuxu")
assert(debug.getinfo(f).short_src == "xuxu")
dostring(a, "@"..string.rep('p', 1000)..'t')
assert(string.find(debug.getinfo(f).short_src, "^%.%.%.p*t$"))
dostring(a, "=xuxu")
assert(debug.getinfo(f).short_src == "xuxu")
dostring(a, string.format("=%s", string.rep('x', 500)))
assert(string.find(debug.getinfo(f).short_src, "^x*$"))
dostring(a, "=")
assert(debug.getinfo(f).short_src == "")
a = nil; f = nil;


repeat
  local g = {x = function ()
    local a = debug.getinfo(2)
    assert(a.name == 'f' and a.namewhat == 'local')
    a = debug.getinfo(1)
    assert(a.name == 'x' and a.namewhat == 'field')
    return 'xixi'
  end}
  local f = function () return 1+1 and (not 1 or g.x()) end
  assert(f() == 'xixi')
  g = debug.getinfo(f)
  assert(g.what == "Lua" and g.func == f and g.namewhat == "" and not g.name)

  function f (x, name)   -- local!
    name = name or 'f'
    local a = debug.getinfo(1)
    assert(a.name == name and a.namewhat == 'local')
    return x
  end

  -- breaks in different conditions
  if 3>4 then break end; f()
  if 3<4 then a=1 else break end; f()
  while 1 do local x=10; break end; f()
  local b = 1
  if 3>4 then return math.sin(1) end; f()
  a = 3<4; f()
  a = 3<4 or 1; f()
  repeat local x=20; if 4>3 then f() else break end; f() until 1
  g = {}
  f(g).x = f(2) and f(10)+f(9)
  assert(g.x == f(19))
  function g(x) if not x then return 3 end return (x('a', 'x')) end
  assert(g(f) == 'a')
until 1

test([[if
math.sin(1)
then
  a=1
else
  a=2
end
]], {2,3,4,7})

test([[
local function foo()
end
foo()
A = 1
A = 2
A = 3
]], {2, 3, 2, 4, 5, 6})


test([[--
if nil then
  a=1
else
  a=2
end
]], {2,5,6})

test([[a=1
repeat
  a=a+1
until a==3
]], {1,3,4,3,4})

test([[ do
  return
end
]], {2})

test([[local a
a=1
while a<=3 do
  a=a+1
end
]], {1,2,3,4,3,4,3,4,3,5})

test([[while math.sin(1) do
  if math.sin(1)
  then break
  end
end
a=1]], {1,2,3,6})

test([[for i=1,3 do
  a=i
end
]], {1,2,1,2,1,2,1,3})

test([[for i,v in pairs{'a','b'} do
  a=tostring(i) .. v
end
]], {1,2,1,2,1,3})

test([[for i=1,4 do a=1 end]], {1,1,1,1})


do   -- testing line info/trace with large gaps in source

  local a = {1, 2, 3, 10, 124, 125, 126, 127, 128, 129, 130,
             255, 256, 257, 500, 1000}
  local s = [[
     local b = {10}
     a = b[1] X + Y b[1]
     b = 4
  ]]
  for _, i in ipairs(a) do
    local subs = {X = string.rep("\n", i)}
    for _, j in ipairs(a) do
      subs.Y = string.rep("\n", j)
      local s = string.gsub(s, "[XY]", subs)
      test(s, {1, 2 + i, 2 + i + j, 2 + i, 2 + i + j, 3 + i + j})
    end
  end
end


do   -- testing active lines
  local function checkactivelines (f, lines)
    local t = debug.getinfo(f, "SL")
    for _, l in pairs(lines) do
      l = l + t.linedefined
      assert(t.activelines[l])
      t.activelines[l] = undef
    end
    assert(next(t.activelines) == nil)   -- no extra lines
  end

  checkactivelines(function (...)   -- vararg function
    -- 1st line is empty
    -- 2nd line is empty
    -- 3th line is empty
    local a = 20
    -- 5th line is empty
    local b = 30
    -- 7th line is empty
  end, {4, 6, 8})

  checkactivelines(function (a)
    -- 1st line is empty
    -- 2nd line is empty
    local a = 20
    local b = 30
    -- 5th line is empty
  end, {3, 4, 6})

  checkactivelines(function (a, b, ...) end, {0})

  checkactivelines(function (a, b)
  end, {1})

  for _, n in pairs{0, 1, 2, 10, 50, 100, 1000, 10000} do
    checkactivelines(
      load(string.format("%s return 1", string.rep("\n", n))),
      {n + 1})
  end

end

print'+'

debug.sethook();

do   -- test hook presence in debug info
  assert(not debug.gethook())
  local count = 0
  local function f ()
    assert(debug.getinfo(1).namewhat == "hook")
    local sndline = string.match(debug.traceback(), "\n(.-)\n")
    assert(string.find(sndline, "hook"))
    count = count + 1
  end
  debug.sethook(f, "l")
  local a = 0
  _ENV.a = a
  a = 1
  debug.sethook()
  assert(count == 4)
end


-- hook table has weak keys
--assert(getmetatable(debug.getregistry()._HOOKKEY).__mode == 'k')

a = {}; L = nil
local glob = 1
local oldglob = glob
debug.sethook(function (e,l)
  collectgarbage()   -- force GC during a hook
  local f, m, c = debug.gethook()
  assert(m == 'crl' and c == 0)
  if e == "line" then
    if glob ~= oldglob then
      L = l-1   -- get the first line where "glob" has changed
      oldglob = glob
    end
  elseif e == "call" then
      local f = debug.getinfo(2, "f").func
      a[f] = 1
  else assert(e == "return")
  end
end, "crl")

-- tests for manipulating non-registered locals (C and Lua temporaries)
-- minimal tests for setuservalue/getuservalue

-- testing iteraction between multiple values x hooks
do
  local function f(...) return 3, ... end
  local count = 0
  local a = {}
  for i = 1, 100 do a[i] = i end
  debug.sethook(function () count = count + 1 end, "", 1)
  local t = {table.unpack(a)}
  assert(#t == 100)
  t = {table.unpack(a, 1, 3)}
  assert(#t == 3)
  t = {f(table.unpack(a, 1, 30))}
  assert(#t == 31)
end
-- testing count hooks
local a=0
debug.sethook(function (e) a=a+1 end, "", 1)
a=0; for i=1,1000 do end; assert(1000 < a and a < 1012)
debug.sethook(function (e) a=a+1 end, "", 4)
a=0; for i=1,1000 do end; assert(250 < a and a < 255)
local f,m,c = debug.gethook()
assert(m == "" and c == 4)
debug.sethook(function (e) a=a+1 end, "", 4000)
a=0; for i=1,1000 do end; assert(a == 0)

do
  debug.sethook(print, "", 2^24 - 1)   -- count upperbound
  local f,m,c = debug.gethook()
  assert(({debug.gethook()})[3] == 2^24 - 1)
end

debug.sethook()


-- tests for tail calls
local function f (x)
  if x then
    assert(debug.getinfo(1, "S").what == "Lua")
    assert(debug.getinfo(1, "t").istailcall == true)
    local tail = debug.getinfo(2)
    assert(tail.func == g1 and tail.istailcall == true)
    assert(debug.getinfo(3, "S").what == "main")
    print"+"
    end
end

function g(x) return f(x) end

function g1(x) g(x) end

local function h (x) local f=g1; return f(x) end

h(true)

local b = {}
debug.sethook(function (e) table.insert(b, e) end, "cr")
h(false)
debug.sethook()
local res = {"return",   -- first return (from sethook)
  "call", "tail call", "call", "tail call",
  "return", "return",
  "call",    -- last call (to sethook)
}
for i = 1, #res do assert(res[i] == table.remove(b, 1)) end

b = 0
debug.sethook(function (e)
                if e == "tail call" then
                  b = b + 1
                  assert(debug.getinfo(2, "t").istailcall == true)
                else
                  assert(debug.getinfo(2, "t").istailcall == false)
                end
              end, "c")
h(false)
debug.sethook()
assert(b == 2)   -- two tail calls

lim = _soft and 3000 or 30000
local function foo (x)
  if x==0 then
    assert(debug.getinfo(2).what == "main")
    local info = debug.getinfo(1)
    assert(info.istailcall == true and info.func == foo)
  else return foo(x-1)
  end
end

foo(lim)


print"+"


-- testing local function information
co = load[[
  local A = function ()
    return x
  end
  return
]]

-- testing traceback

assert(debug.traceback(print) == print)
assert(debug.traceback(print, 4) == print)
assert(string.find(debug.traceback("hi", 4), "^hi\n"))
assert(string.find(debug.traceback("hi"), "^hi\n"))
assert(not string.find(debug.traceback("hi"), "'debug.traceback'"))
print(string.find(debug.traceback("hi", 0), "'debug.traceback'"))
assert(string.find(debug.traceback(), "^stack traceback:\n"))

-- testing nparams, nups e isvararg
local t = debug.getinfo(print, "u")
assert(t.isvararg == true and t.nparams == 0 and t.nups == 0)

t = debug.getinfo(function (a,b,c) end, "u")
assert(t.isvararg == false and t.nparams == 3 and t.nups == 0)

t = debug.getinfo(function (a,b,...) return t[a] end, "u")
assert(t.isvararg == true and t.nparams == 2 and t.nups == 1)

t = debug.getinfo(1)   -- main
assert(t.isvararg == true and t.nparams == 0 and t.nups == 1 and
       debug.getupvalue(t.func, 1) == "_ENV")

t = debug.getinfo(math.sin)   -- C function
assert(t.isvararg == true and t.nparams == 0 and t.nups == 0)

t = debug.getinfo(string.gmatch("abc", "a"))   -- C closure
assert(t.isvararg == true and t.nparams == 0 and t.nups > 0)



-- testing debugging of coroutines

local function checktraceback (co, p, level)
  local tb = debug.traceback(co, nil, level)
  local i = 0
  for l in string.gmatch(tb, "[^\n]+\n?") do
    assert(i == 0 or string.find(l, p[i]))
    i = i+1
  end
  assert(p[i] == undef)
end


local function f (n)
  if n > 0 then f(n-1)
  else coroutine.yield() end
end

local co = coroutine.create(f)
coroutine.resume(co, 3)
checktraceback(co, {"yield", "db.lua", "db.lua", "db.lua", "db.lua"})
checktraceback(co, {"db.lua", "db.lua", "db.lua", "db.lua"}, 1)
checktraceback(co, {"db.lua", "db.lua", "db.lua"}, 2)
checktraceback(co, {"db.lua"}, 4)
checktraceback(co, {}, 40)


co = coroutine.create(function (x)
       local a = 1
       coroutine.yield(debug.getinfo(1, "l"))
       coroutine.yield(debug.getinfo(1, "l").currentline)
       return a
     end)

local tr = {}
local foo = function (e, l) if l then table.insert(tr, l) end end
debug.sethook(co, foo, "lcr")

local _, l = coroutine.resume(co, 10)
local x = debug.getinfo(co, 1, "lfLS")
assert(x.currentline == l.currentline and x.activelines[x.currentline])
assert(type(x.func) == "function")
for i=x.linedefined + 1, x.lastlinedefined do
  assert(x.activelines[i])
  x.activelines[i] = undef
end
assert(next(x.activelines) == nil)   -- no 'extra' elements
assert(not debug.getinfo(co, 2))

-- test acessing line numbers of a coroutine from a resume inside
-- a C function (this is a known bug in Lua 5.0)

local function g(x)
    coroutine.yield(x)
end

local function f (i)
  debug.sethook(function () end, "l")
  for j=1,1000 do
    g(i+j)
  end
end

local co = coroutine.wrap(f)
co(10)
pcall(co)
pcall(co)

assert(type(debug.getregistry()) == "table")


-- test tagmethod information
local a = {}
local function f (t)
  local info = debug.getinfo(1);
  assert(info.namewhat == "metamethod")
  a.op = info.name
  return info.name
end
setmetatable(a, {
  __index = f; __add = f; __div = f; __mod = f; __concat = f; __pow = f;
  __mul = f; __idiv = f; __unm = f; __len = f; __sub = f;
  __shl = f; __shr = f; __bor = f; __bxor = f;
  __eq = f; __le = f; __lt = f; __unm = f; __len = f; __band = f;
  __bnot = f;
})

local b = setmetatable({}, getmetatable(a))

assert(a[3] == "index" and a^3 == "pow" and a..a == "concat")
assert(a/3 == "div" and 3%a == "mod")
assert(a+3 == "add" and 3-a == "sub" and a*3 == "mul" and
       -a == "unm" and #a == "len" and a&3 == "band")
assert(a + 30000 == "add" and a - 3.0 == "sub" and a * 3.0 == "mul" and
       -a == "unm" and #a == "len" and a & 3 == "band")
assert(a|3 == "bor" and 3~a == "bxor" and a<<3 == "shl" and a>>1 == "shr")
assert (a==b and a.op == "eq")
assert (a>=b and a.op == "le")
assert ("x">=a and a.op == "le")
assert (a>b and a.op == "lt")
assert (a>10 and a.op == "lt")
assert(~a == "bnot")

do   -- testing for-iterator name
  local function f()
    assert(debug.getinfo(1).name == "for iterator")
  end

  for i in f do end
end


do   -- testing debug info for finalizers
  local name = nil

  -- create a piece of garbage with a finalizer
  setmetatable({}, {__gc = function ()
    local t = debug.getinfo(1)   -- get function information
    assert(t.namewhat == "metamethod")
    name = t.name
  end})

  -- repeat until previous finalizer runs (setting 'name')
  repeat local a = {} until name
  assert(name == "__gc")
end


do
  print("testing traceback sizes")

  local function countlines (s)
    return select(2, string.gsub(s, "\n", ""))
  end

  local function deep (lvl, n)
    if lvl == 0 then
      return (debug.traceback("message", n))
    else
      return (deep(lvl-1, n))
    end
  end

  local function checkdeep (total, start)
    local s = deep(total, start)
    local rest = string.match(s, "^message\nstack traceback:\n(.*)$")
    local cl = countlines(rest)
    -- at most 10 lines in first part, 11 in second, plus '...'
    assert(cl <= 10 + 11 + 1)
    local brk = string.find(rest, "%.%.%.")
    if brk then   -- does message have '...'?
      local rest1 = string.sub(rest, 1, brk)
      local rest2 = string.sub(rest, brk, #rest)
      assert(countlines(rest1) == 10 and countlines(rest2) == 11)
    else
      assert(cl == total - start + 2)
    end
  end

  for d = 1, 51, 10 do
    for l = 1, d do
      -- use coroutines to ensure complete control of the stack
      coroutine.wrap(checkdeep)(d, l)
    end
  end

end

do   -- bug in 5.4.0: line hooks in stripped code
  local function foo ()
    local a = 1
    local b = 2
    return b
  end

  local s = load(string.dump(foo, true))
  local line = true
  debug.sethook(function (e, l)
    assert(e == "line")
    line = l
  end, "l")
  assert(s() == 2); debug.sethook(nil)
  assert(line == nil)  -- hook called withoug debug info for 1st instruction
end

do   -- tests for 'source' in binary dumps
  local prog = [[
    return function (x)
      return function (y) 
        return x + y
      end
    end
  ]]
  local name = string.rep("x", 1000)
  local p = assert(load(prog, name))
  -- load 'p' as a binary chunk with debug information
  local c = string.dump(p)
  assert(#c > 1000 and #c < 2000)   -- no repetition of 'source' in dump
  local f = assert(load(c))
  local g = f()
  local h = g(3)
  assert(h(5) == 8)
  assert(debug.getinfo(f).source == name and   -- all functions have 'source'
         debug.getinfo(g).source == name and 
         debug.getinfo(h).source == name)
  -- again, without debug info
  local c = string.dump(p, true)
  assert(#c < 500)   -- no 'source' in dump
  local f = assert(load(c))
  local g = f()
  local h = g(30)
  assert(h(50) == 80)
  assert(debug.getinfo(f).source == '=?' and   -- no function has 'source'
         debug.getinfo(g).source == '=?' and 
         debug.getinfo(h).source == '=?')
end

print"OK"

