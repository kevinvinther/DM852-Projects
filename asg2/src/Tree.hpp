#ifndef TREE_H
#define TREE_H

#include <cassert>
#include <iostream>

// The documentation is written in Doxygen format
// (Though the format may be wrong, inspiration from
// https://en.wikipedia.org/wiki/Doxygen?useskin=vector)

namespace DM852 {
template <typename Key, typename Value, typename Comp = std::less<Key>>
class Tree {
public:
  using value_type = std::pair<const Key, Value>;

private:
  class Node {
  public:
    using value_type = std::pair<const Key, Value>;
    friend class iterator;
    friend class const_iterator;

    Node *parent;
    Node *left;
    Node *right;
    value_type *values;
    Comp comp;

    Node(const Key key, const Value value, Comp cmp)
        : parent(nullptr), left(nullptr), right(nullptr),
          values(new value_type(key, value)), comp(cmp) {}

    Node(const Node &other) : values(other.values), comp(other.comp) {}

    ~Node() { delete values; }

    bool operator==(Node &other) {
      return (!(comp(values->first, other.values->first) &&
                !comp(other.values->first, values->first)) &&
              values->second == other.values->second);
    }

    bool operator==(const Node &other) const {
      return (!(comp(values->first, other.values->first) &&
                !comp(other.values->first, values->first)) &&
              values->second == other.values->second);
    }

    bool operator!=(Node &other) { return !(operator==(other)); }

    bool operator!=(const Node &other) const { return !(operator==(other)); }

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

    /*
    ** Goes right once, and then left as much as possible
    **
    ** @return The next node in the tree. If none return nullptr.
    */
    const Node *next() const { return const_cast<Node *>(this)->next(); }

    /*
    ** Goes left once, and then right as much as possible
    **
    ** @return The previous node in the tree. If none return nullptr.
    */
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

    /*
    ** Goes left once, and then right as much as possible
    **
    ** @return The previous node in the tree. If none return nullptr.
    */
    const Node *prev() const { return const_cast<Node *>(this)->prev(); }
  };

  Node *root;
  int node_count;
  Comp comp;
  Node *first_node;
  Node *last_node;

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

  Node *leftMost(Node *node) {
    if (node) {
      if (node->left) {
        return leftMost(node->left);
      }
      return node;
    }
    return nullptr;
  }

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
  Tree()
      : root(nullptr), node_count(0), comp(Comp()), first_node(nullptr),
        last_node(nullptr) {}

  Tree(Comp comp)
      : root(nullptr), node_count(0), comp(comp), first_node(nullptr),
        last_node(nullptr) {}

  Tree(const Tree &other)
      : root(TreeCopy(other.root)), node_count(other.node_count), comp(Comp()),
        first_node(leftMost(root)), last_node(rightMost(root)){};

  Tree(Tree &&other)
      : root(TreeCopy(other.root)), node_count(other.node_count),
        comp(other.comp), first_node(leftMost(root)),
        last_node(rightMost(root)){};

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

  ~Tree() { clear(); }

  bool operator==(const Tree &other) { return iteratorTraversal(other); }

  bool operator==(const Tree &other) const { return iteratorTraversal(other); }

  bool operator!=(const Tree &other) { return !iteratorTraversal(other); }

  bool operator!=(const Tree &other) const { return !iteratorTraversal(other); }

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

  void clear() {
    clearTraversal(root);
    root = nullptr;
    first_node = nullptr;
    last_node = nullptr;
    node_count = 0;
  }

  void clearTraversal(Node *node) {
    if (node) {
      clearTraversal(node->left);
      clearTraversal(node->right);
      delete node;
    }
  }

  class iterator : public std::iterator<std::bidirectional_iterator_tag, Tree> {
  private:
    // Out of bounds
    Node *p;
    int OOBMargin;

  public:
    using value_type = std::pair<const Key, Value>;
    using reference = value_type &;
    friend class Tree;
    iterator() : p(nullptr), OOBMargin(1) {}
    iterator(Node *p) : p(p), OOBMargin(0) {}

    reference &operator*() const {
      if (!p || OOBMargin != 0)
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      return *p->values;
    }

    value_type *operator->() const {
      if (!p || OOBMargin != 0)
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      return p->values;
    }

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

    // to satisfy bidirectional iterator
    iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

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

    // to satisfy bidirectional iterator
    iterator operator--(int) {
      iterator temp = *this;
      --(*this);
      return temp;
    }

    friend bool operator==(iterator a, iterator b) {
      if (a.p && b.p && a.OOBMargin == 0 && b.OOBMargin == 0) {
        return *a.p == *b.p;
      }
      bool aOOB = a.OOBMargin != 0 || !a.p;
      bool bOOB = b.OOBMargin != 0 || !b.p;

      return aOOB && bOOB;
    }

    friend bool operator!=(iterator a, iterator b) { return !(a == b); }
  };

  class const_iterator
      : public std::iterator<std::bidirectional_iterator_tag, Tree> {
  private:
    // Out of bounds
    const Node *p;
    int OOBMargin;

  public:
    using value_type = std::pair<const Key, Value>;
    using reference = const value_type &;
    friend class Tree;
    const_iterator() : p(nullptr), OOBMargin(1) {}
    const_iterator(Node *p) : p(p), OOBMargin(0) {}
    const_iterator(const iterator &other)
        : p(other.p), OOBMargin(other.OOBMargin) {}

    reference &operator*() const {
      if (!p || OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      }
      return *p->values;
    }

    value_type *operator->() const {
      if (!p || OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iteartor");
      }
      return p->values;
    }

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

    // to satisfy bidirectional iterator
    const_iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

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

    // to satisfy bidirectional iterator
    const_iterator operator--(int) {
      iterator temp = *this;
      --(*this);
      return temp;
    }

    friend bool operator==(const const_iterator a, const const_iterator b) {

      if (a.p && b.p && a.OOBMargin == 0 && b.OOBMargin == 0) {
        return *a.p == *b.p;
      }
      bool aOOB = a.OOBMargin != 0 || !a.p;
      bool bOOB = b.OOBMargin != 0 || !b.p;

      return aOOB && bOOB;
    }

    friend bool operator!=(const const_iterator a, const const_iterator b) {
      return !(a == b);
    }
  };

  int size() const { return node_count; };        // must run constant time
  bool empty() const { return node_count == 0; }; // must run constant time

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
  std::pair<iterator, bool> insert(Key &&key, Value &&value) {
    Node *current = findNode(key);

    if (current != nullptr) {
      current->values->second = value;
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

  iterator begin() { return iterator(first_node); };
  const_iterator begin() const { return const_iterator(first_node); };

  iterator end() { return ++iterator(last_node); };
  const_iterator end() const { return ++const_iterator(last_node); };

  value_type &front() {
    assert(first_node);
    return *first_node->values;
  }
  const value_type &front() const {
    assert(first_node);
    return *first_node->values;
  }
  value_type &back() {
    assert(last_node);
    return *last_node->values;
  }
  const value_type &back() const {
    assert(last_node);
    return *last_node->values;
  }
};
} // namespace DM852

#endif // TREE_H
