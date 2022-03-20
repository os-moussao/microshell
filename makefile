all: mysh

mysh: exec.c main.c parse.c
	cc -Wall -Werror -Wextra $^ -o $@

cl:
	rm -f mysh

re: cl all