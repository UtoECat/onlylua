-- $Id: testes/big.lua $
-- See Copyright Notice in file all.lua

if _soft then
  return 'a'
end


local debug = require"debug" 
print "testing pcall and xpcall"

local f = function(a, t, b)
	assert(a == 10 and b == nil) 
	if not t then error("hehe") end
	return 'A', nil, 'B'
end

local a, b, c, e = f(10, true)
assert(a == 'A' and b == nil and c == 'B')

e, a, b, c = pcall(f, 10, true)
print(e, a, b, c)
assert(e and a == 'A' and b == nil and c == 'B')
e, a, b, c = pcall(pcall, nil, 10, true)
print(e, a, b, c)
assert(not e)
e, a, b, c = pcall(f, 10, true, 1)
assert(not e)
e, a, b, c = pcall(f, 9, true)
assert(not e)
e, a, b, c = pcall(f, 10)
assert(not e)

local te = function(m) return m end

e, a, b, c = xpcall(f, te, 10, true)
print(e, a, b, c)
assert(e and a == 'A' and b == nil and c == 'B')
e, a, b, c = pcall(xpcall, nil, te, 10, true)
print(e, a, b, c)
assert(not e)
e, a, b, c = xpcall(f, te, 10, true, 1)
assert(not e)
e, a, b, c = xpcall(f, te, 9, true)
assert(not e)
e, a, b, c = xpcall(f, te, 10)
assert(not e)

print "testing large tables"
local lim = 2^18 + 1000
local prog = { "local y = {0" }
for i = 1, lim do prog[#prog + 1] = i  end
prog[#prog + 1] = "}\n"
prog[#prog + 1] = "X = y\n"
prog[#prog + 1] = ("assert(X[%d] == %d)"):format(lim - 1, lim - 2)
prog[#prog + 1] = "return 0"
prog = table.concat(prog, ";")

local env = {string = string, assert = assert}
local f = assert(load(prog, nil, nil, env))

f()
assert(env.X[lim] == lim - 1 and env.X[lim + 1] == lim)
for k in pairs(env) do env[k] = undef end

-- yields during accesses larger than K (in RK)
setmetatable(env, {
  __index = function (t, n) coroutine.yield('g'); return _G[n] end,
  __newindex = function (t, n, v) coroutine.yield('s'); _G[n] = v end,
})

X = nil
co = coroutine.wrap(f)
assert(co() == 's')
assert(co() == 'g')
assert(co() == 'g')
assert(co() == 0)

assert(X[lim] == lim - 1 and X[lim + 1] == lim)

getmetatable(env).__index = function () end
rawset(env, 'print', print)
getmetatable(env).__newindex = function () end
f = load('X = 10; _ENV.Y = 20; print("good"); return B', "test", nil, env)
assert(select(2, debug.getupvalue(f, 1)) == env)

-- errors in metamethods 
getmetatable(env).__newindex = function () error("hi") end
local e, m = xpcall(f, debug.traceback)
print(e, m)
assert(not e and m:find("newindex"))

f, X = nil

coroutine.yield'b'

if 2^32 == 0 then   -- (small integers) {   

print "testing string length overflow"

local repstrings = 192          -- number of strings to be concatenated
local ssize = math.ceil(2.0^32 / repstrings) + 1   -- size of each string

assert(repstrings * ssize > 2.0^32)  -- it should be larger than maximum size

local longs = string.rep("\0", ssize)   -- create one long string

-- create function to concatenate 'repstrings' copies of its argument
local rep = assert(load(
  "local a = ...; return " .. string.rep("a", repstrings, "..")))

local a, b = pcall(rep, longs)   -- call that function

-- it should fail without creating string (result would be too large)
assert(not a and string.find(b, "overflow"))

end   -- }

print'OK'

return 'a'
