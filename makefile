.PHONY: all clean profile tests

DEBUG  = 0
HOST   = $(OS)
HOSTCC = $(CC)
LUA    = lua5.4

TARGET ?= $(OS)

ifeq ($(DEBUG), 1)
# deep debug
CCFLAGS = -Wall -Wextra -Wunused-macros -Og -DLUA_DEBUG -DLUA_USE_APICHECK -DLUAI_ASSERT
LDFLAGS = -g -fsanitize=undefined # for debugging
else ifeq ($(DEBUG), 2)
# profiling route
CCFLAGS = -Wall -Wextra -Wunused -O1 #-Wc++-compat
LDFLAGS = -g0 -pg -fprofile-arcs # for profiling
else
# release
CCFLAGS = -Wall -Wextra -Wunused -O3
LDFLAGS = -flto -g0 -s
endif

# target platform checks
ifeq ($(TARGET),Windows_NT)
	# windows
  EXEC = luatest.exe
  ifneq ($(OS), $(TARGET))
	# unix -> windows case => use mingw compiler
  HOSTCC = x86_64-w64-mingw32-cc
  endif
else
 # unix way
 EXEC = luatest
 ifneq ($(OS), $(TARGET))
 $(error Winows to Unix compilation is not implemented)
 endif
endif

all : $(EXEC)
NOPACK = 0

ifeq ($(NOPACK), 0)

lua.c : ./source/* 
	cd ./source && $(LUA) make.lua

$(EXEC) : lua.c test.c lualib.c
	$(HOSTCC) -I. $^ -o $@ $(CCFLAGS) -lm $(LDFLAGS)

else

CCFLAGS += -DLUAI_FUNC='' -I./source -DNOPACK

./build/%.o: ./source/%.c
	mkdir -p $(dir $@)
	$(HOSTCC) -I. -c $< -o $@ $(CCFLAGS) $(LDFLAGS)

CFILES = $(shell find ./source/ -name '*.c')
OFILES = $(CFILES:./source/%.c=./build/%.o)

$(EXEC) : $(OFILES) test.c
	$(HOSTCC) -I. $^ -o $@ $(CCFLAGS) -lm $(LDFLAGS)

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
