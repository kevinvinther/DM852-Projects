#include "../src/Tree.hpp"
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

#define assert_eq(a, b) assert(a == b)
#define assert_ne(a, b) assert(a != b)

using namespace DM852;

class Test {
public:
  /// Test the Tree class

  // Default constructor
  void test_tree_default_constructor() {
    Tree<int, char> tree;
    assert_eq(tree.size(), 0);
    assert_eq(tree.empty(), true);
    assert_eq(tree.begin(), nullptr);
    assert_eq(tree.end(), nullptr);
    std::cout << "PASS: test_tree_default_constructor\n";
  }

  // Constructor with comparison function
  template <typename Key, typename Value, typename Comp = std::less<Key>>
  void test_tree_constructor_with_comparison_function() {
    Comp comp;
    Tree<Key, Value, Comp> tree(comp);
    assert_eq(tree.size(), 0);
    assert_eq(tree.empty(), true);
    assert_eq(tree.begin(), tree.end());
    std::cout << "PASS: test_tree_constructor_with_comparison_function\n";
  }

  // Copy constructor
  void test_tree_copy_constructor_is_correct() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'c');

    Tree<int, char> tree_copy(tree);

    assert_eq(tree.size(), tree_copy.size());   // The sizes are equal
    assert_eq(tree.empty(), tree_copy.empty()); // The trees are empty or not
    assert_eq(tree.begin(), tree_copy.begin()); // The iterators are equal
    assert_eq(tree.end(), tree_copy.end());     // The iterators are equal

    std::cout << "PASS: test_tree_copy_constructor_is_correct\n";
  }

  void test_tree_copy_constructor_does_not_change() {
    Tree<int, char> tree;
    Tree<int, char> tree_copy(tree);

    assert_eq(tree, tree_copy);
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'c');
    assert_ne(tree, tree_copy);

    std::cout << "PASS: test_tree_copy_constructor_does_not_change\n";
  }

  // Move Constructor
  void test_tree_move_constructor_is_correct() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'c');

    Tree<int, char> tree_copy(std::move(tree));

    assert_eq(tree_copy.size(), 3);
    assert_eq(tree_copy.empty(), false);

    assert(tree.size() == 0);

    std::cout << "PASS: test_tree_move_constructor_is_correct\n";
  }

  // Copy assignment operator
  void test_tree_copy_assignment_operator_is_correct() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'c');

    Tree<int, char> tree_copy = tree;

    assert_eq(tree.size(), tree_copy.size());   // The sizes are equal
    assert_eq(tree.empty(), tree_copy.empty()); // The trees are empty or not
    assert_eq(tree.begin(), tree_copy.begin()); // The iterators are equal
    assert_eq(tree.end(), tree_copy.end());     // The iterators are equal

    std::cout << "PASS: test_tree_copy_assignment_operator_is_correct\n";
  }

  void test_tree_copy_assignment_operator_does_not_affect_new_tree() {
    Tree<int, char> tree;

    tree.insert(1, 'a');

    Tree<int, char> tree_copy = tree;

    tree_copy.insert(2, 'b');

    assert_ne(tree, tree_copy);

    std::cout
        << "PASS: "
           "test_tree_copy_assignment_operator_does_not_affect_new_tree\n";
  }

  // Move assignment operator
  void test_tree_move_assignment_operator_is_correct() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'd');
    Tree<int, char> newTree = std::move(tree);

    assert_eq(newTree.size(), 3);
    assert_eq(newTree.empty(), false);
    assert_eq(tree.size(), 0);

    std::cout << "PASS: test_tree_move_assignment_operator_is_correct\n";
  }

  // Destructor
  void test_tree_destructor_is_correct() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'd');
    tree.~Tree();

    // We assume that, in case of no error, the destructor is correct

    std::cout << "PASS: test_tree_destructor_is_correct\n"; // So this is not
                                                            // entirely correct
  }

  // Equality
  void test_tree_equality_operator_is_correct() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'd');
    Tree<int, char> tree_copy = tree;

    assert_eq(tree, tree_copy);

    std::cout << "PASS: test_tree_equality_operator_is_correct\n";
  }

  // Inequality
  void test_tree_inequality_operator_is_correct() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    Tree<int, char> new_tree;

    assert_ne(tree, new_tree);

    std::cout << "PASS: test_tree_inequality_operator_is_correct\n";
  }

  // Clear
  void test_tree_clear() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    tree.clear();

    assert_eq(tree.size(), 0);

    std::cout << "PASS: test_tree_clear\n";
  }

  // Size
  void test_tree_size() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    assert_eq(tree.size(), 2);

    assert_ne(tree.size(), 0);

    std::cout << "PASS: test_tree_size\n";
  }

  // Empty
  void test_tree_empty() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    assert_eq(tree.empty(), false);

    std::cout << "PASS: test_tree_empty\n";
  }

  // FindNode
  void test_tree_find_node() {
    Tree<int, char> tree;
    tree.insert(5, 'a');
    tree.insert(6, 'c');
    tree.insert(7, 'd');

    assert_eq(tree.findNode(5)->values->second, 'a');
    assert_ne(tree.findNode(6)->values->second, 'b');

    std::cout << "PASS: test_tree_find_node\n";
  }

  // Insert
  void test_tree_insert() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(3, 'd');

    assert_eq(tree.size(), 3);

    tree.insert(3, 'b');

    assert_eq(tree.size(), 3);

    assert_eq(tree.findNode(3)->values->second, 'b');

    std::cout << "PASS: test_tree_insert\n";
  }

  void test_tree_find() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    assert_eq(tree.find(1)->second, 'a');

    std::cout << "PASS: test_tree_find\n";
  }

  void test_tree_begin() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    assert_eq(tree.begin()->second, 'a');

    std::cout << "PASS: test_tree_begin\n";
  }

  void test_tree_end() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    // End returns an out of bound iterator
    assert_eq((--tree.end())->second, 'b');

    std::cout << "PASS: test_tree_end\n";
  }

  void test_tree_front() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    assert_eq(tree.front().second, 'a');

    std::cout << "PASS: test_tree_front\n";
  }

  void test_tree_back() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    assert_eq(tree.back().second, 'b');

    std::cout << "PASS: test_tree_back\n";
  }

  /// Iterator

  void test_iterator_iterator_default_constructor() {
    Tree<int, char>::iterator iterator;

    assert_eq(iterator, nullptr);

    std::cout << "PASS: test_tree_iterator_default_constructor\n";
  }

  void test_iterator_iterator_with_node_pointer_constructor() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(1));

    assert_eq(iterator->second, 'a');

    std::cout << "PASS: test_tree_iterator_with_node_pointer_constructor\n";
  }

  void test_iterator_iterator_copy_constructor() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(1));

    Tree<int, char>::iterator iterator_copy(iterator);

    assert_eq(iterator_copy->second, 'a');

    std::cout << "PASS: test_tree_iterator_copy_constructor\n";
  }

  void test_iterator_dereference_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(1));
    assert_eq((*iterator).second, 'a');

    std::cout << "PASS: test_tree_iterator_dereference_operator\n";
  }

  void test_iterator_arrow_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(1));
    assert_eq(iterator->second, 'a');

    std::cout << "PASS: test_tree_iterator_arrow_operator\n";
  }

  void test_iterator_pre_increment_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(1));
    ++iterator;
    assert_eq(iterator->second, 'b');

    std::cout << "PASS: test_tree_iterator_pre_increment_operator\n";
  }

  void test_iterator_post_increment_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(1));
    iterator++;
    assert_eq(iterator->second, 'b');

    std::cout << "PASS: test_tree_iterator_post_increment_operator\n";
  }

  void test_iterator_pre_decrement_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(2));
    --iterator;
    assert_eq(iterator->second, 'a');

    std::cout << "PASS: test_tree_iterator_pre_decrement_operator\n";
  }

  void test_iterator_post_decrement_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(2));
    iterator--;
    assert_eq(iterator->second, 'a');

    std::cout << "PASS: test_tree_iterator_post_decrement_operator\n";
  }

  void test_iterator_equal_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(2));

    assert_eq(iterator, tree.findNode(2));

    std::cout << "PASS: test_tree_iterator_equal_operator\n";
  }

  void test_iterator_not_equal_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::iterator iterator(tree.findNode(2));

    assert_ne(iterator, tree.findNode(1));

    std::cout << "PASS: test_tree_iterator_not_equal_operator\n";
  }

  // Const_iterator

  void test_const_iterator_const_iterator_default_constructor() {
    Tree<int, char>::const_iterator const_iterator;

    assert_eq(const_iterator, nullptr);

    std::cout << "PASS: test_tree_const_iterator_default_constructor\n";
  }

  void test_const_iterator_const_iterator_with_node_pointer_constructor() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(1));

    assert_eq(const_iterator->second, 'a');

    std::cout
        << "PASS: test_tree_const_iterator_with_node_pointer_constructor\n";
  }

  void test_const_iterator_const_iterator_copy_constructor() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(1));

    Tree<int, char>::const_iterator const_iterator_copy(const_iterator);

    assert_eq(const_iterator_copy->second, 'a');

    std::cout << "PASS: test_tree_const_iterator_copy_constructor\n";
  }

  void test_const_iterator_dereference_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(1));
    assert_eq((*const_iterator).second, 'a');

    std::cout << "PASS: test_tree_const_iterator_dereference_operator\n";
  }

  void test_const_iterator_arrow_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(1));
    assert_eq(const_iterator->second, 'a');

    std::cout << "PASS: test_tree_const_iterator_arrow_operator\n";
  }

  void test_const_iterator_pre_increment_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(1));
    ++const_iterator;
    assert_eq(const_iterator->second, 'b');

    std::cout << "PASS: test_tree_const_iterator_pre_increment_operator\n";
  }

  void test_const_iterator_post_increment_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(1));
    const_iterator++;
    assert_eq(const_iterator->second, 'b');

    std::cout << "PASS: test_tree_const_iterator_post_increment_operator\n";
  }

  void test_const_iterator_pre_decrement_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(2));
    --const_iterator;
    assert_eq(const_iterator->second, 'a');

    std::cout << "PASS: test_tree_const_iterator_pre_decrement_operator\n";
  }

  void test_const_iterator_post_decrement_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(2));
    const_iterator--;
    assert_eq(const_iterator->second, 'a');

    std::cout << "PASS: test_tree_const_iterator_post_decrement_operator\n";
  }

  void test_const_iterator_equal_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(2));

    assert_eq(const_iterator, tree.findNode(2));

    std::cout << "PASS: test_tree_const_iterator_equal_operator\n";
  }

  void test_const_iterator_not_equal_operator() {
    Tree<int, char> tree;
    tree.insert(1, 'a');
    tree.insert(2, 'b');

    Tree<int, char>::const_iterator const_iterator(tree.findNode(2));

    assert_ne(const_iterator, tree.findNode(1));

    std::cout << "PASS: test_tree_const_iterator_not_equal_operator\n";
  }
};

int main() {
  Test test;

  std::vector<std::thread> threads;

  threads.emplace_back([&]() { test.test_tree_default_constructor(); });
  threads.emplace_back([&]() {
    test.test_tree_constructor_with_comparison_function<int, char>();
  });
  threads.emplace_back([&]() { test.test_tree_copy_constructor_is_correct(); });
  threads.emplace_back(
      [&]() { test.test_tree_copy_constructor_does_not_change(); });
  threads.emplace_back([&]() { test.test_tree_move_constructor_is_correct(); });
  threads.emplace_back(
      [&]() { test.test_tree_copy_assignment_operator_is_correct(); });
  threads.emplace_back([&]() {
    test.test_tree_copy_assignment_operator_does_not_affect_new_tree();
  });
  threads.emplace_back(
      [&]() { test.test_tree_move_assignment_operator_is_correct(); });
  threads.emplace_back([&]() { test.test_tree_destructor_is_correct(); });
  threads.emplace_back(
      [&]() { test.test_tree_equality_operator_is_correct(); });
  threads.emplace_back(
      [&]() { test.test_tree_inequality_operator_is_correct(); });
  threads.emplace_back([&]() { test.test_tree_clear(); });
  threads.emplace_back([&]() { test.test_tree_size(); });
  threads.emplace_back([&]() { test.test_tree_empty(); });
  threads.emplace_back([&]() { test.test_tree_find_node(); });
  threads.emplace_back([&]() { test.test_tree_insert(); });
  threads.emplace_back([&]() { test.test_tree_find(); });
  threads.emplace_back([&]() { test.test_tree_begin(); });
  threads.emplace_back([&]() { test.test_tree_end(); });
  threads.emplace_back([&]() { test.test_tree_front(); });
  threads.emplace_back([&]() { test.test_tree_back(); });

  threads.emplace_back(
      [&]() { test.test_iterator_iterator_default_constructor(); });
  threads.emplace_back(
      [&]() { test.test_iterator_iterator_with_node_pointer_constructor(); });
  threads.emplace_back(
      [&]() { test.test_iterator_iterator_copy_constructor(); });
  threads.emplace_back([&]() { test.test_iterator_dereference_operator(); });
  threads.emplace_back([&]() { test.test_iterator_arrow_operator(); });
  threads.emplace_back([&]() { test.test_iterator_pre_increment_operator(); });
  threads.emplace_back([&]() { test.test_iterator_post_increment_operator(); });
  threads.emplace_back([&]() { test.test_iterator_pre_decrement_operator(); });
  threads.emplace_back([&]() { test.test_iterator_post_decrement_operator(); });
  threads.emplace_back([&]() { test.test_iterator_equal_operator(); });
  threads.emplace_back([&]() { test.test_iterator_not_equal_operator(); });

  threads.emplace_back(
      [&]() { test.test_const_iterator_const_iterator_default_constructor(); });
  threads.emplace_back([&]() {
    test.test_const_iterator_const_iterator_with_node_pointer_constructor();
  });
  threads.emplace_back(
      [&]() { test.test_const_iterator_const_iterator_copy_constructor(); });
  threads.emplace_back(
      [&]() { test.test_const_iterator_dereference_operator(); });
  threads.emplace_back([&]() { test.test_const_iterator_arrow_operator(); });
  threads.emplace_back(
      [&]() { test.test_const_iterator_pre_increment_operator(); });
  threads.emplace_back(
      [&]() { test.test_const_iterator_post_increment_operator(); });
  threads.emplace_back(
      [&]() { test.test_const_iterator_pre_decrement_operator(); });
  threads.emplace_back(
      [&]() { test.test_const_iterator_post_decrement_operator(); });
  threads.emplace_back([&]() { test.test_const_iterator_equal_operator(); });
  threads.emplace_back(
      [&]() { test.test_const_iterator_not_equal_operator(); });

  for (auto &t : threads) {
    t.join();
  }
  return 0;
}
