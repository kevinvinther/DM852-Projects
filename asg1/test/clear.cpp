// This file test the Tree::clear() function
#include "test.hpp"

void testClear() {
  DM852::Tree *clearTree = new DM852::Tree();
  clearTree->insert(1, "This is the first value");
  clearTree->insert(2, "This is the 2 value");
  clearTree->insert(3, "This is the 3 value");
  clearTree->clear();
  TEST(clearTree->size() == 0,
       "The tree did not clear after using the clear() function");

  delete clearTree;
}
