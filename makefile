.PHONY: all clean profile tests

DEBUG = 0

ifeq ($(DEBUG), 1)
CCFLAGS = -Wall -Wextra -Wunused-macros -Og -DLUA_DEBUG -DLUA_USE_APICHECK -DLUAI_ASSERT
LDFLAGS = -g -fsanitize=undefined # for debugging
else ifeq ($(DEBUG), 2)
CCFLAGS = -Wall -Wextra -Wunused -O1 #-Wc++-compat
LDFLAGS = -g0 -pg -fprofile-arcs # for profiling
else
CCFLAGS = -Wall -Wextra -Wunused -O3
LDFLAGS = -flto -g0 -s
endif

all : luatest
NOPACK = 0

ifeq ($(NOPACK), 0)

lua.c : ./source/* 
	cd ./source && lua make.lua

luatest : lua.c test.c lualib.c
	$(CC) -I. $^ -o $@ $(CCFLAGS) -lm $(LDFLAGS)

else

CCFLAGS += -DLUAI_FUNC='' -I./source -DNOPACK

./build/%.o: ./source/%.c
	mkdir -p $(dir $@)
	$(CC) -I. -c $< -o $@ $(CCFLAGS) $(LDFLAGS)

CFILES = $(shell find ./source/ -name '*.c')
OFILES = $(CFILES:./source/%.c=./build/%.o)

luatest : $(OFILES) test.c
	$(CC) -I. $^ -o $@ $(CCFLAGS) -lm $(LDFLAGS)

endif

callgrind.out: all
	cd tests && valgrind --tool=callgrind --collect-systime=nsec --dump-instr=yes --callgrind-out-file=../callgrind.out -v ../luatest ./all.lua

profile: callgrind.out
	cd tests && callgrind_annotate ../callgrind.out ../*.c ../*.h > ../profiler.out
	gvim ./profiler.out & disown

tests: all
	cd tests && ../luatest ./all.lua

clean :
	rm -f ./luatest
