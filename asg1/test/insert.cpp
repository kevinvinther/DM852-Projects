// This file tests the Tree::insert() function
// This file also tests the following functions:
//      - find()
//      - front()
//      - back()
//      - size()
//   To make sure that the insertion is correct. These may also be tested in
//   other files.
#include "test.hpp"

void testInsert() {
  {
    DM852::Tree *insertTree = new DM852::Tree();
    auto [node, boolean] = insertTree->insert(1, "hej med dig");
    TEST(boolean, "Insert tree returned false when inserting a new node");
    TEST(node, "Insert tree returned nullptr when inserting a new node");
    TEST(node->value == "hej med dig",
         "Insert tree did not correctly set the value of the node");
    TEST(insertTree->find(1) == node,
         "Insert did not correctly set the key.\nNOTE: This may also be an "
         "error with find()")
  }
  {
    DM852::Tree *insertTree = new DM852::Tree();
    insertTree->insert(1, "This is the first value");
    insertTree->insert(2, "This is the 2 value");
    insertTree->insert(3, "This is the 3 value");
    insertTree->insert(4, "This is the 4 value");
    insertTree->insert(5, "This is the 5 value");
    insertTree->insert(6, "This is the 6 value");
    insertTree->insert(7, "This is the 7 value");
    insertTree->insert(8, "This is the 8 value");
    insertTree->insert(9, "This is the 9 value");
    insertTree->insert(10, "This is the last value");

    TEST(insertTree->front() == "This is the first value",
         "Front() did not find the correct first value");
    TEST(insertTree->back() == "This is the last value",
         "Back() did not find the correct last value");
    TEST(insertTree->size() == 10, "size() did not return the correct size.")
  }
  {
    DM852::Tree *insertTree = new DM852::Tree();
    insertTree->insert(1, "This is the first value");
    auto [node, boolean] = insertTree->insert(1, "new value");
    TEST(boolean == false, "Insert returned true when inserting an old key");
    TEST(insertTree->find(1)->value == "new value",
         "Insert assigned wrong value to node");
  }
}
