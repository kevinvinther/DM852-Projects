// This file test the == and != operator
#include "test.hpp"

void testEquality() {
  DM852::Tree *tree1 = new DM852::Tree();
  DM852::Tree *tree2 = new DM852::Tree();

  TEST(*tree1 == *tree2, "Two empty trees are not equal");
  tree1->insert(1, "This is the first value");
  tree2->insert(1, "This is the first value");

  TEST(*tree1 == *tree2, "Two trees with the same values are not equal");

  tree1->insert(2, "This is the 2 value");

  TEST(*tree1 != *tree2, "Two trees with different values are equal");

  tree1->clear();
  tree2->clear();
  TEST(*tree1 == *tree2, "Two empty trees are not equal after clear()");
}
