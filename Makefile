all:
		gcc -Wall -Wextra -std=c11 -O2 -lxcb src/main.c -o bin/title
