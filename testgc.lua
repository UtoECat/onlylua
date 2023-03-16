print("Heloo world!")

print("------Test function shit---------")

if not memusage then
	memusage = function() return collectgarbage("count")*1024 end
end

local t = {}
setmetatable(t, {__mode="kv"})

local need = 0
local nt = {}

while #t < 100 do
	local len = #t
	local rnd = memusage()
	t[len+1] = function(a)
		return a + len / rnd
	end
	need = need + 1
	if need == 10 then
		need = 0
		nt[len] = t[len + 1]
		print("key", len, 'saved at mem state', rnd)
	end
end

print("------Trigger gc---------")
nt = {}

local wgc = memusage()
print("GCmem =", wgc)
local cntneeded = 0

while wgc - 10000 < memusage() do
	wgc   = memusage()
	t[#t + 1] = {tostring({})}
	cntneeded = cntneeded + 1
end

print("GCmem =", memusage(), " Objects needed =", cntneeded)
