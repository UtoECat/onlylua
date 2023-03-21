local w = 640
local s = w * 480

-- init

local clock = os and os.clock or clock
local m = {}
local t = clock()

for j = 1, 30 do
	for i = 1, s+1 do
		m[i] = j
	end
end

print(clock() - t)
t = clock()
