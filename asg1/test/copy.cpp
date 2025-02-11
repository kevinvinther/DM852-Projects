// This file tests if copyable is correctl implemented
#include "test.hpp"

void testCopyable() {
  DM852::Tree *copyTree = new DM852::Tree();
  copyTree->insert(1, "This is the first value");
  DM852::Tree *copyTree2 = new DM852::Tree(*copyTree);
  TEST(*copyTree == *copyTree2, "Two copied trees are not equal.")

  DM852::Tree copyTree3 = *copyTree;
  TEST(copyTree3 == *copyTree,
       "Two copies trees are not equal with = operator");

  copyTree3.insert(2, "This is the second value");
  TEST(copyTree3 != *copyTree, "Two copied trees are equal after change");

  delete copyTree;
  delete copyTree2;
}
