# Compiler flags
CC = g++
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c++11

# Sanitizer flags
SANITIZE_ADDRESS = -fsanitize=address
SANITIZE_LEAK = -fsanitize=leak
SANITIZE_UNDEFINED = -fsanitize=undefined

# Object files
OBJS = Tree.o

# Targets
all: $(OBJS)

Tree.o: Tree.cpp Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c Tree.cpp -o Tree.o

clean:
	rm -f $(OBJS)

