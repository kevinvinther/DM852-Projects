#include "test.hpp"
#include <iostream>

int main() {
  testClear();
  std::cout << "TESTS: testClear() passed!" << std::endl;
  std::cout << "-------------------------" << std::endl;
  testCopyable();
  std::cout << "TESTS: testCopyable() passed!" << std::endl;
  std::cout << "-------------------------" << std::endl;
  testFind();
  std::cout << "TESTS: testFind() passed!" << std::endl;
  std::cout << "-------------------------" << std::endl;
  testEquality();
  std::cout << "TESTS: testEquality() passed!" << std::endl;
  std::cout << "-------------------------" << std::endl;
  testInsert();
  std::cout << "TESTS: testInsert() passed!" << std::endl;
  std::cout << "-------------------------" << std::endl;
  testSize();
  std::cout << "TESTS: testSize() passed!" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << "TESTS: Passed all tests!" << std::endl;
  return 0;
}
