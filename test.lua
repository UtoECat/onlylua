print("Heloo world!")

print("------Test error and pcall---------")
function a(safe) 
	if not safe then error("error :(") end
	return 'OK! :)'
end

print("not safe call trough pcall", pcall(a))
print("safe call trough pcall", pcall(a, true))

local function b(level)
	if level <= 0 then error("error throwed") end
	local _, v = pcall(b, level-1)
	return v.." lvl "..tostring(level)..";"
end

print("-----------Test pairs--------------")

local t = {
	"a", "b", "c",
	sus    = "amogus",
	notsus = "sus",
	picious= "sus",
	endall = "a, b, c, d, e, anchor, 1, 2, sus, sus, sus, self"
}

for k, v in pairs(t) do
	print(k, '=', v)
end

print(pcall(pairs, nil))
print(pcall(pairs, {}))

print("---------Test loadstring-----------")
a = loadstring([[
	return function() 
		print('I am loaded string inside a loaded function! :З')
	end, 'I am loaded string! :D' .. ' and concated!'
]]);

print(a, select(2, a()))
a()()

a = loadstring([[
	
]])
print("---------Test metatables-----------")

local orig = {10, 'hehe', 'boi'}
t = {}
print(t)

local mt = {
	__index = function(t, k)
		print('access to', k)
		return rawget(orig, k)
	end,
	__len  = function(t) return 666 end,
	__name = 'watcher'
}

setmetatable(t, mt)
print(t, t[1], t[2], t[3])

print("------------ Test assertions ------")
local function b(f, n)
	assert(f == n, "impossible ._.")
	return 'always not equal?'
end

print(pcall(b, 1, 2))
print(pcall(b, 4, 4))

print("--------- invalid test --------")

local t = {1, 2, 3}

print(pcall(next, 1, 1))
print(pcall(next, t, 4))
print(pcall(inext, "k", 1))
print(pcall(inext, t, 4))


print("--------- test strings --------")
print(tostring("veeeeeeery loooooong striiiiiing :D"),
tostring("veeeeeeery veeeeeeeeeeeeeeery loooooong striiiiiing :D"),
tostring([[iiiiiiiiiiiiiinsssssssssssaaaannneeellllyyyyyyyyyyyyy veeeeeeery veeeeeeeeeeeeeeery loooooong striiiiiing :D]]))
