CPPFLAGS=
CXXFLAGS=-g -O2 $(SANFLAGS)
LDFLAGS=$(SANFLAGS)
LIBS=

SANFLAGS=-fsanitize=undefined -fsanitize=address -fsanitize=leak

.PHONY: all
all: Tree

Tree: Tree.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f *.o Tree

