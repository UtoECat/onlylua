function require(name)
	local B
	local A

	A = loaded[name]
	if A then
		return A
	end

	A = preload[name]
	if A then
		A = A()
		loaded[name] = A
		return A
	end

	A, B = loadfile(name..".lua")
	if not A then
		error("file "..name.." is not found!\n"..B)
	end
	A = A()
	loaded[name] = A
	return A
end
