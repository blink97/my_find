CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -g
CPPFLAGS = -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE
OBJ = myfind.o mystrlib.o without_option.o option_d.o expression_name.o ast.o
EXEC = myfind
VPATH = ./src

.PHONY: doc

all: $(OBJ)
	$(CC) -o $(EXEC) $^ $(CFLAGS) $(CPPFLAGS)

test: all
	./tests/testsuite.sh

doc:
	doxygen doc/Doxyfile
	firefox html/index.html

clean:
	rm -f $(OBJ) $(EXEC) output.txt expected.txt diff.txt
	rm -rf html
