.PHONY: all

all : lua.c luatest

lua.c : ./source/*.c
	cd ./source && lua make.lua

luatest : lua.c test.c
	gcc -I. $^ -o $@ -Wall -Wextra -lm -O2 -g0 -flto -s
