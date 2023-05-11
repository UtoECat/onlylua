
if jit then
	jit.off()
end

local test = "tablea"
--for k,v in pairs(_G) do
--	print(k, v)
--end

if not newtable then
	newtable = function() return {} end
end

local t = newtable(10000, 5000000)

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

for k,v in pairs(t) do
	t[k] = nil
end

print("is done with", clock()-time, "seconds!")
print("done!")
