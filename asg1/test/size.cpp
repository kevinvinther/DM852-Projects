// File to test the size of the Tree
#include "test.hpp"

void testSize() {
  DM852::Tree *sizeTree = new DM852::Tree();
  TEST(sizeTree->size() == 0,
       "The size of the Tree should be zero on initialization");

  for (int i = 0; i < 10; i++) {
    sizeTree->insert(i, std::to_string(i));
    TEST((sizeTree->size() == (i + 1)),
         "The size of the tree is wrong after insert.");
  }

  delete sizeTree;
}
