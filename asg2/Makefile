# Compiler flags
CC = g++
CFLAGS = -Wall -Wextra -Werror -pedantic -O2 -std=c++20

# Sanitizer flags
SANITIZE_ADDRESS = -fsanitize=address
SANITIZE_LEAK = -fsanitize=leak
SANITIZE_UNDEFINED = -fsanitize=undefined

# Object files
OBJS = test/test.o

# Target executable
TARGET = runTests

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) $(OBJS) -o $(TARGET)

test/test.o: test/test.cpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/test.cpp -o test/test.o

clean:
	rm -f $(OBJS) $(TARGET)
