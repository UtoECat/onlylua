local w = 640
local s = w * 480

if jit then jit.off() end

-- init

local function packcolor(r, g, b)
	return r + g * 256 + b * 256 * 256
end

local clock = os and os.clock or clock
local m = {}
local t = clock()

for j = 1, 60 do
	for i = 1, s+1 do
		m[i] = j 
	end
end

print(clock() - t)
t = clock()

