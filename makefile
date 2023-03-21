.PHONY: all clean profile

DEBUG = 0

ifeq ($(DEBUG), 1)
CCFLAGS = -Wall -Wextra -Wunused-macros -O1 #-Wc++-compat
LDFLAGS = -g # for debugging
else ifeq ($(DEBUG), 2)
CCFLAGS = -Wall -Wextra -Wunused -O1 #-Wc++-compat
LDFLAGS = -g0 -pg -fprofile-arcs # for profiling
else
CCFLAGS = -Wall -Wextra -Wunused -O3
LDFLAGS = -flto -g0 -s
endif

all : lua.c luatest

lua.c : ./source/*.c ./source/make.lua
	cd ./source && lua make.lua

luatest : lua.c test.c
	gcc -I. $^ -o $@ $(CCFLAGS) -lm $(LDFLAGS)

profile:
	./luatest bench.lua
	gprof luatest > ./profiler.out
	gvim profiler.out

clean :
	rm -f ./luatest
