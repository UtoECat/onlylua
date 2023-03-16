print("Heloo world!")

print("------Test error and pcall---------")
function a(safe) 
	if not safe then error("error :(") end
	return 'OK! :)'
end

print("not safe call trough pcall", pcall(a))
print("safe call trough pcall", pcall(a, true))

print("-----------Test print--------------")

for k, v in pairs(_G) do
	print(k, '=', v)
end

print("---------Test loadstring-----------")
a = loadstring([[
	return function() 
		print('I am loaded string inside a loaded function! :З')
	end, 'I am loaded string! :D' .. ' and concated!'
]]);

print(a, select(2, a()))
a()()
print("---------Test metatables-----------")

local orig = {10, 'hehe', 'boi'}
local t = {}
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
