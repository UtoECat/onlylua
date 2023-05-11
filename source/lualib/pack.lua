local f = io.open('require.lua', 'r')
assert(f)
f = f:read('a')

f = f:gsub('[\n\t ]+', ' ')
f = f:gsub('"', '\'')
print(f)
collectgarbage()
