#include "test.hpp"
#include <iostream>

#define passed(function)                                                       \
  std::cout << "TESTS: " << function << "pased!\n-------------------------"    \
            << std::endl;

int main() {
  testClear();
  passed("testClear()");
  testCopyable();
  passed("testCopyable()");
  testFind();
  passed("testFind()");
  testEquality();
  passed("testEquality()");
  testInsert();
  passed("testInsert()");
  testSize();
  passed("testSize()");
  std::cout << "TESTS: Passed all tests!" << std::endl;
  return 0;
}
