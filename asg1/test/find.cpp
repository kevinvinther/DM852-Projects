// This file test the Tree::find() function
#include "test.hpp"

void testFind() {
  DM852::Tree *findTree = new DM852::Tree();
  TEST(findTree->find(1) == nullptr,
       "find() did not return nullptr on no node found");

  auto [node, _] = findTree->insert(1, "This is the first value");
  TEST(findTree->find(1) == node, "find() did not return the correct node");
}
