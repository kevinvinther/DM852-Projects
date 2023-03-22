/*
 * This test file defines a macro #define TEST, which is simply a short way to
 * write a cassert statement.
 */
#include "../src/Tree.hpp"
#include <cassert>

// The reason for using this define, rather than just using assert() is the &&
// message part, which pops up if an assert fails.
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
