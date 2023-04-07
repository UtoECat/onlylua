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
a = load([[
	return function() 
		print('I am loaded string inside a loaded function! :З')
	end, 'I am loaded string! :D' .. ' and concated!'
]]);

print(a, select(2, a()))
a()()

a = load([[]])
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

print("--------- test strings --------")
print(tostring("veeeeeeery loooooong striiiiiing :D"),
tostring("veeeeeeery veeeeeeeeeeeeeeery loooooong striiiiiing :D"),
tostring([[iiiiiiiiiiiiiinsssssssssssaaaannneeellllyyyyyyyyyyyyy veeeeeeery veeeeeeeeeeeeeeery loooooong striiiiiing :D]]))

print("--------- test strings --------")

function deepprint(_value, _name)
	local stack = {} 
	local _deep  = " "
	_name = _name or "G"
	local tos
	tos = function(val, name, deep)
		if stack[val] then return stack[val] end
		local t = type(val)
		if t == "string" then
			return string.format("%q", tostring(val))
		elseif t == "number" then
			return tostring(val)
		elseif t == "table" then
			stack[val] = _name
			local buff = {"{\n"}
			for k,v in pairs(val) do
				local kname = tos(k, name, deep .. " ")
				buff[#buff+1] = deep.." "..kname
				buff[#buff+1] = " = "..tos(v, name .. "." .. tos(k), deep .. " ")
				buff[#buff+1] = next(val, k) and ",\n" or "\n"
			end
			buff[#buff+1] = deep.."}"
			return table.concat(buff)
		else
			return tostring(val)
		end
	end
	print(tos(_value, _name, _deep))
end

deepprint(getregistry(), "_REG")
