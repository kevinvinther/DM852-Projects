#include "../src/Tree.hpp"
#include <cassert>

#define TEST(condition, message) assert((condition) && message);

#ifndef TEST_H
#define TEST_H

void testClear();
void testCopyable();
void testFind();
void testEquality();
void testInsert();
void testSize();

#endif // TEST_H
