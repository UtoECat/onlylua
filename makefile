luatest :
	gcc -I. lua.c test.c -o luatest -Wall -Wextra -fanalyzer -lm
