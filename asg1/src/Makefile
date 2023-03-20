CPPFLAGS=
CXXFLAGS=-g -O2 $(SANFLAGS)
LDFLAGS=$(SANFLAGS)
LIBS=

SANFLAGS=-fsanitize=undefined -fsanitize=address -fsanitize=leak

.PHONY: all
all: Tree

Tree: Tree.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

Tree.o: Tree.cpp Tree.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o Tree

