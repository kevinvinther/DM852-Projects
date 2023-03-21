// This file checks if the empty() function has correct functionality

#include "test.hpp"

void testEmpty() {
  DM852::Tree *tree = new DM852::Tree();
  TEST(tree->empty() == true,
       "The tree should be empty upon first initialization");
  tree->insert(5, "hello");
  TEST(tree->empty() == false, "Tree tree should not be empty after an insert");
  tree->clear();
  TEST(tree->empty() == true, "The tree should be empty after clear()");
  tree->insert(5, "hello");
  tree->insert(6, "hello");
  tree->insert(7, "hello");
  tree->insert(8, "hello");
  delete tree;
  TEST(tree->empty() == true, "The tree should be empty after delete. This is "
                              "likely a problem with the destructor.");
}
