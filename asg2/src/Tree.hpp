#ifndef TREE_H
#define TREE_H

#include <cassert>
#include <iostream>

// The documentation is written in Doxygen format
// (Though the format may be wrong, inspiration from
// https://en.wikipedia.org/wiki/Doxygen?useskin=vector)
// To see documentation in html format, install doxygen
// run `$ doxygen -g dconfig` to create config file
// run `doxygen dconfig` to create html files

namespace DM852 {

/// @brief A binary search tree
///
/// @tparam Key The key type
/// @tparam Value The value type
/// @tparam Comp The comparison function
///
/// This tree is not a red-black tree, i.e., it is *not* self-balancing.
template <typename Key, typename Value, typename Comp = std::less<Key>>
class Tree {
public:
  using value_type =
      std::pair<const Key, Value>; ///< The value type, a pair consisting of the
                                   ///< key and the value of a node

private:
  /// @brief A node in the tree
  class Node {
  public:
    friend class iterator; ///< The iterator class can access private members of
                           ///< Node
    friend class const_iterator; ///< The const_iterator class can access
                                 ///< private members of Node

    Node *parent; ///< The parent node
    Node *left;   ///< The left child node (smaller key), or nullptr if none
    Node *right;  ///< The right child node (larger key), or nullptr if none
    value_type *values; ///< The key-value pair of the node
    Comp comp;          ///< The comparison function

    /// @brief Constructor for a node
    ///
    /// This constructor simply sets all the appropriate values through a member
    /// initalization list.
    ///
    /// The values are initializes to be the key-value pair, using the
    /// value_type type.
    ///
    /// @param key The key of the node
    /// @param value The value of the node
    /// @param cmp The comparison function
    Node(const Key &key, const Value &value, Comp cmp)
        : parent(nullptr), left(nullptr), right(nullptr),
          values(new value_type(key, value)), comp(cmp) {}

    /// @brief Copy constructor for a node
    ///
    /// This constructor copies one node to another.
    ///
    /// @param other The node to copy
    Node(const Node &other) : values(other.values), comp(other.comp) {}

    /// @brief Move constructor for a node
    ///
    /// This constructor moves one node to another, using std::move.
    ///
    /// @param key The key of the node
    /// @param value The value of the node
    /// @param cmp The comparison function
    Node(Key &&key, Value &&value, Comp cmp)
        : parent(nullptr), left(nullptr), right(nullptr),
          values(new value_type(std::move(key), std::move(value))), comp(cmp) {}

    /// @brief Destructor for a node
    ///
    /// This destructor deletes the values pointer, which is a pointer to a
    /// value_type of the key-value pair.
    ~Node() { delete values; }

    /// @brief Equality operator for a node.
    ///
    /// This operator uses the comparison function to compare the keys of two
    /// nodes, and if they are equal, it compares the elements.
    ///
    /// @param other The node to compare to
    bool operator==(Node &other) {
      return (!(comp(values->first, other.values->first) &&
                !comp(other.values->first, values->first)) &&
              values->second == other.values->second);
    }

    /// @brief Const equality operator for a node.
    ///
    /// Const version of the equality operator.
    /// @see operator==(Node &other)
    ///
    /// @param other The node to compare to
    bool operator==(const Node &other) const {
      return (!(comp(values->first, other.values->first) &&
                !comp(other.values->first, values->first)) &&
              values->second == other.values->second);
    }

    /// @brief Inequality operator for a node.
    ///
    /// This operator uses the equality operator to compare two nodes.
    /// @see operator==(Node &other)
    ///
    /// @param other The node to compare to
    bool operator!=(Node &other) { return !(operator==(other)); }

    /// @brief Inequality operator for a node.
    ///
    /// This operator uses the equality operator to compare two nodes.
    /// @see operator==(Node &other)
    ///
    /// @param other The node to compare to
    bool operator!=(const Node &other) const { return !(operator==(other)); }

    /// @brief Goes right once, and then left as much as possible
    ///
    /// This function finds the next node in the tree.
    ///
    /// Running time is: O(n) where n is the height of the tree.
    /// In the worst case scenario, the function could traverse as many nodes as
    /// there are levens in the tree, which is the height of the tree.
    ///
    /// @return The next node in the tree. If none return nullptr.
    Node *next() {
      if (right != nullptr) {
        Node *next = right;
        // In a binary search tree the right-side is larger, and we want to
        // find the smallest number that is still larger. Therefore we first
        // go to the right, and keep going left until we get the smallest
        // larger number.
        while (next->left != nullptr) {
          next = next->left;
        }
        return next;
      } else { // The right node does not exist
        Node *next = this;
        while (next->parent) {
          if (next != next->parent->right) {
            return next->parent;
          }
          next = next->parent;
        }
        return nullptr;
      }
    }

    /// @brief Const version of next()
    ///
    /// @see next()
    ///
    /// @return The next node in the tree.
    const Node *next() const { return const_cast<Node *>(this)->next(); }

    /// @brief Goes left once, and then right as much as possible
    ///
    /// This function finds the previous node in the tree.
    /// The running time is: O(n) where n is the height of the tree.
    /// In the worst case scenario, the function could traverse as many nodes as
    /// there are levens in the tree, which is the height of the tree.
    ///
    /// @return The previous node in the tree.
    Node *prev() {
      if (left != nullptr) {
        Node *next = left;
        // In a binary search tree the right-side is larger, and we want to
        // find the smallest number that is still larger. Therefore we first
        // go to the right, and keep going left until we get the smallest
        // larger number.
        while (next->right != nullptr) {
          next = next->right;
        }
        return next;
      } else { // The right node does not exist
        Node *next = this;
        while (next->parent) {
          if (next != next->parent->left) {
            return next->parent;
          }
          next = next->parent;
        }
        return nullptr;
      }
    }

    /// @brief Const version of prev()
    ///
    /// @see prev()
    ///
    /// @return The previous node in the tree.
    const Node *prev() const { return const_cast<Node *>(this)->prev(); }
  };

  Node *root;       ///< The root of the tree
  int node_count;   ///< The number of nodes in the tree
  Comp comp;        ///< The comparison function
  Node *first_node; ///< The first node in the tree (smallest)
  Node *last_node;  ///< The last node in the tree (largest)

  /// @brief Copies nodes of tree from root down
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// We copy each node in the tree, guaranteed, and we can't copy more
  /// nodes than there are in the tree. We only do this once, securing
  /// the O(n) running time.
  ///
  /// @param root The root of the tree to copy
  Node *TreeCopy(Node *root) {
    // If the root exists
    if (root) {
      // Create a new node identical to root
      Node *newNode = new Node(root->values->first, root->values->second, comp);
      // Copy the left node
      newNode->left = TreeCopy(root->left);
      if (newNode->left) {
        newNode->left->parent = newNode;
      }
      // Copy the right node
      newNode->right = TreeCopy(root->right);
      if (newNode->right) {
        newNode->right->parent = newNode;
      }
      return newNode;
    }
    // If the root doesn't exist, copy nothing.
    return nullptr;
  }

  /// @brief Finds the left-most node in the tree
  ///
  /// Running time is O(n) where n is the height of the tree.
  /// Explanation:
  /// The function goes left as much as possible, and then returns the node.
  /// In the worst case scenario, the function could traverse as many nodes as
  /// there are levens in the tree, which is the height of the tree.
  /// This would happen if the tree only has left-side nodes.
  ///
  /// @param node The node to start from
  ///
  /// @return The left-most node in the tree
  Node *leftMost(Node *node) {
    if (node) {
      if (node->left) {
        return leftMost(node->left);
      }
      return node;
    }
    return nullptr;
  }

  /// @brief Finds the right-most node in the tree
  ///
  /// Running time is O(n) where n is the height of the tree.
  /// Explanation:
  /// The function goes right as much as possible, and then returns the node.
  /// In the worst case scenario, the function could traverse as many nodes as
  /// there are levens in the tree, which is the height of the tree.
  /// This would happen if the tree only has right-side nodes.
  ///
  /// @param node The node to start from
  ///
  /// @return The right-most node in the tree
  Node *rightMost(Node *node) {
    if (node) {
      if (node->right) {
        return rightMost(node->right);
      }
      return node;
    }
    return nullptr;
  }

public:
  /// @brief Default constructor for tree
  ///
  /// Initializes all the values to their default values.
  Tree()
      : root(nullptr), node_count(0), comp(Comp()), first_node(nullptr),
        last_node(nullptr) {}

  /// @brief Constructor for tree with a comparison function
  ///
  /// Initializes all the values to their default values, and sets the
  /// comparison function to the one given.
  ///
  /// @param comp The comparison function to use
  Tree(Comp comp)
      : root(nullptr), node_count(0), comp(comp), first_node(nullptr),
        last_node(nullptr) {}

  /// @brief Copy constructor for tree
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// O(n) * 3 = O(n)
  ///
  /// @see TreeCopy()
  /// @see leftMost()
  /// @see rightMost()
  ///
  /// @param other The tree to copy
  ///
  /// @return A copy of the tree
  Tree(const Tree &other)
      : root(TreeCopy(other.root)), node_count(other.node_count), comp(Comp()),
        first_node(leftMost(root)), last_node(rightMost(root)){};

  /// @brief Move constructor for tree
  ///
  /// @param other The tree to move
  ///
  /// @return A copy of the tree
  Tree(Tree &&other)
      : root(other.root), node_count(other.node_count), comp(other.comp),
        first_node(root.first_node), last_node(other.last_node) {
    other.root = nullptr;
    other.first_node = nullptr;
    other.last_node = nullptr;
    other.node_count = 0;
  };

  /// @brief Copy assignment operator for tree
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// O(n) * 3 = O(n)
  ///
  /// @see TreeCopy()
  /// @see leftMost()
  /// @see rightMost()
  ///
  /// @param other The tree to copy
  ///
  /// @return A copy of the tree
  Tree &operator=(const Tree &other) {
    if (this != &other) { // Check for self-assignment
      clear();
      comp = other.comp;
      node_count = other.node_count;
      root = TreeCopy(other.root);
      first_node = leftMost(root);
      last_node = rightMost(root);
    }
    return *this;
  };

  /// @brief Move assignment operator for tree
  ///
  /// @param other The tree to move
  ///
  /// @return A copy of the tree
  Tree &operator=(Tree &&other) {
    if (this != &other) { // Check for self-assignment
      clear();
      comp = other.comp;
      node_count = other.node_count;
      root = other.root;
      first_node = other.first_node;
      last_node = other.last_node;

      other.root = nullptr;
      other.first_node = nullptr;
      other.last_node = nullptr;
      other.node_count = 0;
    }
    return *this;
  };

  /// @brief Destructor for tree
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// @see clearTraversal()
  ~Tree() { clear(); }

  /// @brief Equality operator for tree
  ///
  /// This operator uses iterators to traverse the trees and compare the
  /// values.
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// @see iteratorTraversal()
  ///
  /// @param other The tree to compare to
  ///
  /// @return True if the trees are equal, false otherwise
  bool operator==(const Tree &other) { return iteratorTraversal(other); }

  /// @brief Const equality operator for tree
  ///
  /// @see operator==(const Tree &other)
  ///
  /// @param other The tree to compare to
  ///
  /// @return True if the trees are equal, false otherwise
  bool operator==(const Tree &other) const { return iteratorTraversal(other); }

  /// @brief Inequality operator for tree
  ///
  /// This operator uses iterators to traverse the trees and compare the
  /// values.
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// @see iteratorTraversal()
  ///
  /// @param other The tree to compare to
  ///
  /// @return True if the trees are not equal, false otherwise
  bool operator!=(const Tree &other) { return !iteratorTraversal(other); }

  /// @brief Const inequality operator for tree
  ///
  /// @see operator!=(const Tree &other)
  ///
  /// @param other The tree to compare to
  bool operator!=(const Tree &other) const { return !iteratorTraversal(other); }

  /// @brief Equality operator for tree
  ///
  /// This operator uses iterators to traverse the trees and compare the
  /// values.
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// Two iterators are used to traverse the trees, and the comparison
  /// function is used to compare the values, which are all nodes.
  ///
  /// @param other The tree to compare to
  ///
  /// @return True if the trees are equal, false otherwise
  bool iteratorTraversal(const Tree &other) const {
    if (node_count != other.node_count) {
      return false;
    }
    for (const_iterator it = begin(), otherIt = other.begin();
         it != end() && otherIt != other.end(); ++it, ++otherIt) {
      if (!(!comp(it->first, otherIt->first) &&
            !comp(otherIt->first, it->first)) ||
          it->second != otherIt->second) {
        return false;
      }
    }
    return true;
  }

  /// @brief Clears tree to empty state
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// @see clearTraversal()
  void clear() {
    clearTraversal(root);
    root = nullptr;
    first_node = nullptr;
    last_node = nullptr;
    node_count = 0;
  }

  /// @brief Deletes all nodes in tree recursively
  ///
  /// Running time is O(n) where n is the number of nodes in the tree.
  /// Explanation:
  /// Each node is recursively deleted, thus each node is visited once.
  ///
  /// @param node The node to start from
  void clearTraversal(Node *node) {
    if (node) {
      clearTraversal(node->left);
      clearTraversal(node->right);
      delete node;
    }
  }

  /*!< Iterator class to go through tree */
  class iterator : public std::iterator<std::bidirectional_iterator_tag, Tree> {
  private:
    Node *p;       ///< The node the iterator points to
    int OOBMargin; ///< The margin of how much the iterator is out of bounds

  public:
    using value_type =
        std::pair<const Key, Value>; ///< The value type, a pair consisting of
                                     ///< the key and the value of a node
    using reference = value_type &;  ///< The reference type, a reference to a
                                     ///< value_type
    friend class Tree;               ///< Tree can access private members

    /// @brief Default constructor for iterator
    ///
    /// @return An iterator pointing to nullptr
    iterator() : p(nullptr), OOBMargin(1) {}

    /// @brief Constructor for iterator
    ///
    /// @param p The node to point to
    ///
    /// @return An iterator pointing to p
    iterator(Node *p) : p(p), OOBMargin(0) {}

    /// @brief Dereference operator for iterator
    ///
    /// @throw std::out_of_range if the iterator is out of bounds
    ///
    /// @see operator->()
    ///
    /// @return A reference to the value_type
    reference &operator*() const {
      if (!p || OOBMargin != 0)
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      return *p->values;
    }

    /// @brief Arrow operator for iterator
    ///
    /// @throw std::out_of_range if the iterator is out of bounds
    ///
    /// @return A pointer to the value_type
    value_type *operator->() const {
      if (!p || OOBMargin != 0)
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      return p->values;
    }

    /// @brief Increment operator for iterator
    ///
    /// This function uses the OOBMargin variable, to see if the iterator gets
    /// "out of bounds" or "past the end" after being incremented. If so, OOB is
    /// incremented. If not, the iterator is incremented if the next node is not
    /// null, otherwise OOBMargin is incremented.
    ///
    /// @return The iterator after being incremented
    iterator &operator++() {
      if (!p)
        return *this;
      if (OOBMargin != 0) {
        OOBMargin++;
      } else if (p->next()) {
        p = p->next();
      } else {
        OOBMargin++;
      }
      return *this;
    }

    /// @brief Post-increment operator for iterator (++i)
    ///
    /// @see operator++()
    ///
    /// @return The iterator before being incremented
    iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

    /// @brief Decrement operator for iterator
    ///
    /// This function uses the OOBMargin variable, to see if the iterator gets
    /// "out of bounds" or "past the end" after being decremented. If so, OOB is
    /// decremented. If not, the iterator is decremented if the previous node is
    /// not null, otherwise OOBMargin is decremented.
    ///
    /// @return The iterator after being decremented
    iterator &operator--() {
      if (!p)
        return *this;
      if (OOBMargin != 0) {
        OOBMargin--;
        return *this;
      }
      if (p->prev()) {
        p = p->prev();

      } else {
        OOBMargin--;
      }
      return *this;
    }

    /// @brief Post-decrement operator for iterator (--i)
    ///
    /// @see operator--()
    ///
    /// @return The iterator before being decremented
    iterator operator--(int) {
      iterator temp = *this;
      --(*this);
      return temp;
    }

    /// @brief Equality operator for iterator
    ///
    /// This function uses the OOBMargin variable, to see if the iterators are
    /// equal. If both iterators are not out of bounds, the equality is
    /// determined by comparing the values of the nodes they point to.
    ///
    /// Otherwise, if one of the iterators is out of bounds, the equality is
    /// determined by checking if both iterators are out of bounds.
    ///
    /// @param a The first iterator to compare
    /// @param b The second iterator to compare
    ///
    /// @return True if the iterators are equal, false otherwise
    friend bool operator==(iterator a, iterator b) {
      if (a.p && b.p && a.OOBMargin == 0 && b.OOBMargin == 0) {
        return *a.p == *b.p;
      }
      bool aOOB = a.OOBMargin != 0 || !a.p;
      bool bOOB = b.OOBMargin != 0 || !b.p;

      return aOOB && bOOB;
    }

    /// @brief Inequality operator for iterator
    ///
    /// @see operator==(iterator a, iterator b)
    ///
    /// @return True if not equal, false otherwise
    friend bool operator!=(iterator a, iterator b) { return !(a == b); }
  };

  /*!< Const iterator to go through trees */
  class const_iterator
      : public std::iterator<std::bidirectional_iterator_tag, Tree> {
  private:
    const Node *p; ///< The node the iterator points to
    int OOBMargin; ///< The margin of how much out of bounds the iterator is

  public:
    using value_type =
        std::pair<const Key, Value>; ///< The value type, a pair consisting of
                                     ///< the key and the value of a node
    using reference = const value_type &; ///< The reference type, a reference
                                          ///< to a value_type
    friend class Tree;                    ///< Tree can access private members

    /// @brief Constructor for const iterator
    ///
    /// @return A const iterator pointing to nullptr
    const_iterator() : p(nullptr), OOBMargin(1) {}

    /// @brief Constructor for const iterator
    ///
    /// @param p The node to point to
    ///
    /// @return A const iterator pointing to p
    const_iterator(Node *p) : p(p), OOBMargin(0) {}

    /// @brief Constructor for const iterator
    ///
    /// Takes a non-const iterator and makes it const
    ///
    /// @param other The iterator to copy
    ///
    /// @return A const iterator pointing to the same node as other
    const_iterator(const iterator &other)
        : p(other.p), OOBMargin(other.OOBMargin) {}

    /// @brief Dereference operator for const iterator
    ///
    /// @throw std::out_of_range if the iterator is out of bounds
    ///
    /// @see operator->()
    ///
    /// @return A reference to the value_type
    reference &operator*() const {
      if (!p || OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      }
      return *p->values;
    }

    /// @brief Arrow operator for const iterator
    ///
    /// @throw std::out_of_range if the iterator is out of bounds
    ///
    /// @return A pointer to the value_type
    value_type *operator->() const {
      if (!p || OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iteartor");
      }
      return p->values;
    }

    /// @brief Increment operator for const iterator
    ///
    /// This function uses the OOBMargin variable, to see if the iterator gets
    /// "out of bounds" or "past the end" after being incremented. If so, OOB is
    /// incremented. If not, the iterator is incremented if the next node is not
    /// null, otherwise OOBMargin is incremented.
    ///
    /// @return The iterator after being incremented
    const_iterator &operator++() {
      if (!p)
        return *this;
      if (OOBMargin != 0) {
        OOBMargin++;
      } else if (p->next()) {
        p = p->next();
      } else {
        OOBMargin++;
      }
      return *this;
    }

    /// @brief Post-increment operator for iterator (++i)
    ///
    /// @see operator++()
    ///
    /// @return The iterator before being incremented
    const_iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

    /// @brief Decrement operator for iterator
    ///
    /// This function uses the OOBMargin variable, to see if the iterator gets
    /// "out of bounds" or "past the end" after being decremented. If so, OOB is
    /// decremented. If not, the iterator is decremented if the previous node is
    /// not null, otherwise OOBMargin is decremented.
    ///
    /// @return The iterator after being decremented
    const_iterator &operator--() {
      if (!p)
        return *this;
      if (OOBMargin != 0) {
        OOBMargin--;
      } else if (p->prev()) {
        p = p->prev();
      } else {
        OOBMargin--;
      }
      return *this;
    }

    /// @brief Post-decrement operator for iterator (--i)
    ///
    /// @see operator--()
    ///
    /// @return The iterator before being decremented
    const_iterator operator--(int) {
      iterator temp = *this;
      --(*this);
      return temp;
    }

    /// @brief Equality operator for iterator
    ///
    /// This function uses the OOBMargin variable, to see if the iterators are
    /// equal. If both iterators are not out of bounds, the equality is
    /// determined by comparing the values of the nodes they point to.
    ///
    /// Otherwise, if one of the iterators is out of bounds, the equality is
    /// determined by checking if both iterators are out of bounds.
    ///
    /// @param a The first iterator to compare
    /// @param b The second iterator to compare
    ///
    /// @return True if the iterators are equal, false otherwise
    friend bool operator==(const const_iterator a, const const_iterator b) {

      if (a.p && b.p && a.OOBMargin == 0 && b.OOBMargin == 0) {
        return *a.p == *b.p;
      }
      bool aOOB = a.OOBMargin != 0 || !a.p;
      bool bOOB = b.OOBMargin != 0 || !b.p;

      return aOOB && bOOB;
    }

    /// @brief Inequality operator for iterator
    ///
    /// @see operator==(iterator a, iterator b)
    ///
    /// @return True if not equal, false otherwise
    friend bool operator!=(const const_iterator a, const const_iterator b) {
      return !(a == b);
    }
  };

  /// @brief Returns the size of the tree
  ///
  /// Uses the variable node_count which is incremented when a new node is
  /// inserted.
  ///
  /// @return The size of the tree
  int size() const { return node_count; };

  /// @brief Whether or not the tree is empty
  ///
  /// @returns True if tree is empty, false otherwise
  bool empty() const { return node_count == 0; };

  /// @brief Finds node from key
  ///
  /// This functions goes through each node to find the one with the specified
  /// key. If none is found, it returns nullptr
  ///
  /// Running time is O(n) where n is the height of the tree.
  /// Explanation:
  /// In the worst case scenario, we need to go through each node.
  ///
  /// @return Node from key or nullptr if none is found
  Node *findNode(const Key &key) {
    Node *node = root;
    while (node != nullptr) {
      if (!comp(node->values->first, key) && !comp(key, node->values->first)) {
        return node;
      } else if (comp(node->values->first, key)) {
        node = node->right;
      } else {
        node = node->left;
      }
    }
    return nullptr;
  }

  /// @brief Inserts node into tree
  ///
  /// This function first checks if the key already exists in the tree. If it
  /// does, the value is updated. Otherwise, the function checks if the tree is
  /// empty. If it is, the root is set to the new node. Otherwise, the function
  /// goes through the tree until it finds the correct place to insert the new
  /// node.
  ///
  /// Running time is O(n) where n is the height of the tree.
  /// Explanation:
  /// In the worst case scenario, we need to go through each node.
  ///
  /// @param key The key to insert
  /// @param value The value to insert
  ///
  /// @return A pair with an iterator to the inserted node and a bool that is
  /// true if the node was inserted, false otherwise
  std::pair<iterator, bool> insert(const Key &key, const Value &value) {
    Node *current = findNode(key);

    if (current != nullptr) {
      current->values->second = value;
      return {iterator(current), false};
    }

    if (root == nullptr) {
      root = new Node(key, value, comp);
      node_count++;
      first_node = root;
      last_node = root;
      return {iterator(root), true};
    }

    Node *newNode = new Node(key, value, comp);
    current = root;

    while (true) {
      if (comp(key, current->values->first)) {
        if (current->left) {
          current = current->left;
        } else {
          current->left = newNode;
          newNode->parent = current;
          node_count++;
          if (comp(key, first_node->values->first))
            first_node = newNode;
          return {iterator(newNode), true};
        }
      } else {
        if (current->right) {
          current = current->right;
        } else {
          current->right = newNode;
          newNode->parent = current;
          node_count++;
          if (comp(last_node->values->first, key))
            last_node = newNode;
          return {iterator(newNode), true};
        }
      }
    }
  };
  /// @brief Inserts node into tree using move semantics
  ///
  /// @see insert(const Key &key, const Value &value)
  ///
  /// @param key The key to insert
  /// @param value The value to insert
  ///
  /// @return A pair with an iterator to the inserted node and a bool that is
  /// true if the node was inserted, false otherwise.
  std::pair<iterator, bool> insert(Key &&key, Value &&value) {
    Node *current = findNode(key);

    if (current != nullptr) {
      current->values->second = std::move(value);
      return {iterator(current), false};
    }

    if (root == nullptr) {
      root = new Node(std::move(key), std::move(value), comp);
      node_count++;
      first_node = root;
      last_node = root;
      return {iterator(root), true};
    }

    Node *newNode = new Node(std::move(key), std::move(value), comp);
    current = root;

    while (true) {
      if (comp(key, current->values->first)) {
        if (current->left) {
          current = current->left;
        } else {
          current->left = newNode;
          newNode->parent = current;
          node_count++;
          if (comp(key, first_node->values->first))
            first_node = newNode;
          return {iterator(newNode), true};
        }
      } else {
        if (current->right) {
          current = current->right;
        } else {
          current->right = newNode;
          newNode->parent = current;
          node_count++;
          if (comp(last_node->values->first, key))
            last_node = newNode;
          return {iterator(newNode), true};
        }
      }
    }
  };

  /// @brief Finds node from key and returns an iterator to it
  ///
  /// This is like findNode, but it returns an iterator instead of a node.
  /// If the node is not found, it returns an iterator to end().
  /// @see findNode(const Key &key)
  ///
  /// Running time is O(n) where n is the height of the tree.
  /// Explanation:
  /// In the worst case scenario, we need to go through each node.
  ///
  /// @param key The key to find
  ///
  /// @return Iterator to node from key or end() if none is found
  iterator find(const Key &key) {
    Node *node = root;
    while (node != nullptr) {
      if (!comp(node->values->first, key) && !comp(key, node->values->first)) {
        return iterator(node);
      } else if (comp(node->values->first, key)) {
        node = node->right;
      } else {
        node = node->left;
      }
    }
    return end();
  };

  /// @brief Finds node from key and returns a const_iterator to it.
  ///
  /// @see find(const Key &key)
  ///
  /// @param key The key to find
  ///
  /// @return Const_iterator to node from key or end() if none is found
  const_iterator find(const Key &key) const {
    Node *node = root;
    while (node != nullptr) {
      if (!comp(node->values->first, key) && !comp(key, node->values->first)) {
        return const_iterator(node);
      } else if (comp(node->values->first, key)) {
        node = node->right;
      } else {
        node = node->left;
      }
    }
    return end();
  };

  /// @brief An iterator to the first node in the tree
  ///
  /// @return An iterator to the first node
  iterator begin() { return iterator(first_node); };

  /// @brief A const_iterator to the first node in the tree
  ///
  /// @return A const_iterator to the first node
  const_iterator begin() const { return const_iterator(first_node); };

  /// @brief An iterator to the node after the last node in the tree
  ///
  /// @return An iterator to the node after the last node
  iterator end() { return ++iterator(last_node); };

  /// @brief A const_iterator to the node after the last node in the tree
  ///
  /// @return A const_iterator to the node after the last node
  const_iterator end() const { return ++const_iterator(last_node); };

  /// @brief Returns the key-value pair of the first node
  ///
  /// @return The key-value pair of the first node
  value_type &front() {
    assert(first_node);
    return *first_node->values;
  }

  /// @brief Returns the key-value pair of the first node
  ///
  /// @return The key-value pair of the first node
  const value_type &front() const {
    assert(first_node);
    return *first_node->values;
  }

  /// @brief Returns the key-value pair of the last node
  ///
  /// @return The key-value pair of the last node
  value_type &back() {
    assert(last_node);
    return *last_node->values;
  }

  /// @brief Returns the key-value pair of the last node
  ///
  /// @return The key-value pair of the last node
  const value_type &back() const {
    assert(last_node);
    return *last_node->values;
  }
};
} // namespace DM852

#endif // TREE_H
