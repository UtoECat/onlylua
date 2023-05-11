
if jit then
	jit.off()
end

local test = "string"
--for k,v in pairs(_G) do
--	print(k, v)
--end

if not table.new then
	newtable = function() return {} end
else
	newtable = table.new
end

local t = newtable(10000, 500000)

local clock = clock

if os then
	clock = os.clock
end

local time = clock()

print("Table creation")

for i = 1, 10000 do
	t[i] = i
end

if test == "table" then
	for i = 1, 500000 do
		t[{i}] = i
	end
elseif test == "stringfmt" then
	local sfmt = string.format
	for i = 1, 500000 do
		t[sfmt('%ia', i)] = i
	end
elseif test == "string" then
	local sfmt = tostring
	for i = 1, 500000 do
		t[sfmt(i)] = i
	end
else
	for i = 1, 500000 do
		t[i*50] = i
	end
end

print("is done in", clock() - time, "sec")

local i = 0
time = clock()

i = 0
print("iteration trough pairs")
time = clock()

for k,v in pairs(t) do
	i = i + 1
end

print(i, "done with", clock()-time, "seconds!")
time = clock()

print("cleaning table")
i = 0

if table.clear then
	table.clear(t, false)
else
	for k,v in pairs(t) do
		t[k] = nil
	end
end

print("is done with", clock()-time, "seconds!")
print("check is table alive")
print('#=', #t, t[0], t[1], t[2], t.hehe, t.boi)
t.hehe = 1
t.sus = "among"
t["amogus"] = "sus"
t[0] = 300
t[1] = "omegamogus"
print('#=', #t, t[0], t[1], t[2], t.hehe, t.boi, t.sus)
print("done!")
