# Compiler flags
CC = g++
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c++20

# Sanitizer flags
SANITIZE_ADDRESS = -fsanitize=address
SANITIZE_LEAK = -fsanitize=leak
SANITIZE_UNDEFINED = -fsanitize=undefined

# Object files
OBJS = src/Tree.o test/clear.o test/copy.o test/empty.o test/equal.o test/find.o test/insert.o test/size.o test/runAllTests.o

# Target executable
TARGET = runTests

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) $(OBJS) -o $(TARGET)

src/Tree.o: src/Tree.cpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c src/Tree.cpp -o src/Tree.o

test/clear.o: test/clear.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/clear.cpp -o test/clear.o

test/copy.o: test/copy.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/copy.cpp -o test/copy.o

test/empty.o: test/empty.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/empty.cpp -o test/empty.o

test/equal.o: test/equal.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/equal.cpp -o test/equal.o

test/find.o: test/find.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/find.cpp -o test/find.o

test/insert.o: test/insert.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/insert.cpp -o test/insert.o

test/size.o: test/size.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/size.cpp -o test/size.o

test/runAllTests.o: test/runAllTests.cpp test/test.hpp src/Tree.hpp
	$(CC) $(CFLAGS) $(SANITIZE_ADDRESS) $(SANITIZE_LEAK) $(SANITIZE_UNDEFINED) -c test/runAllTests.cpp -o test/runAllTests.o

clean:
	rm -f $(OBJS) $(TARGET)
