#ifndef TREE_H
#define TREE_H

#include <cassert>
#include <iostream>

// The documentation is written in Doxygen format
// (Though the format may be wrong, inspiration from
// https://en.wikipedia.org/wiki/Doxygen?useskin=vector)

/*
** @file
** @author Kevin Vinther
** @version 1.0
** @section DESCRIPTION
** A generic binary search tree implementation.
*/

namespace DM852 {
template <typename Key, typename Value, typename Comp = std::less<Key>>
class Tree {
public:
  using value_type = std::pair<const Key, Value>;

private:
  /*
  ** Node class for the binary search tree.
  ** Contains a key, value, and pointers to the parent, left and right nodes.
  */
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

    /*
     * Constructor that initializes the node with the given key and value, and
     * sets next and prev to nullptr.
     *
     * @param key The key of the node.
     * @param value The value of the node.
     * @param comp The comparator used to compare keys.
     */
    Node(const Key key, const Value value, Comp cmp)
        : comp(cmp), values(new value_type(key, value)), left(nullptr),
          right(nullptr), parent(nullptr) {}

    /*
    ** Destructor for the node.
    */
    ~Node() {
      delete values;
      if (left) {
        delete left;
      }
      if (right) {
        delete right;
      }
      right = nullptr;
      left = nullptr;
      parent = nullptr;
    }

    /*
    ** Equality operator for the node.
    **
    ** @param &other The node to compare to.
    **
    ** @return True if the nodes are equal, false otherwise.
    */
    bool operator==(Node &other) {
      // When (!comp(key, other.key) && !comp(other.key, key) is true, the keys
      // are equal.
      // values->first is the key.
      return (!(comp(values->first, other.values->first) &&
                !comp(other.values->first, values->first)) &&
              values->second == other.values->second);
    }

    /*
    ** Const equality operator for the node.
    **
    ** @param &other The node to compare to.
    **
    ** @return True if the nodes are equal, false otherwise.
    */
    bool operator==(const Node &other) const {
      // When (!comp(key, other.key) && !comp(other.key, key) is true, the keys
      // are equal.
      // values->first is the key.
      return (!(comp(values->first, other.values->first) &&
                !comp(other.values->first, values->first)) &&
              values->second == other.values->second);
    }

    /*
    ** Inequality operator for the node.
    **
    ** @param &other The node to compare to.
    **
    ** @return True if the nodes are not equal, false otherwise.
    */
    bool operator!=(Node &other) { return !(*this == other); }

    /*
    ** Inequality operator for the node.
    **
    ** @param &other The node to compare to.
    **
    ** @return True if the nodes are not equal, false otherwise.
    */
    bool operator!=(const Node &other) const { return !(*this == other); }

    /*
    ** Goes right once, and then left as much as possible
    **
    ** @return The next node in the tree. If none return nullptr.
    */
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
  Node *first_node;
  Node *last_node;

  Comp comp;

  Node *TreeCopy(Node *root) {
    // If the root exists
    if (root) {
      // Create a new node identical to root
      Node *newNode = new Tree::Node(root->values->first, root->values->second);
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
    if (node->left) {
      return leftMost(node->left);
    }
    return node;
  }

  Node *rightMost(Node *node) {
    if (node->right) {
      return rightMost(node->right);
    }
    return node;
  }

public:
  Tree()
      : root(nullptr), comp(Comp()), node_count(0), first_node(nullptr),
        last_node(nullptr) {}

  Tree(Comp comp)
      : root(nullptr), comp(comp), node_count(0), first_node(nullptr),
        last_node(nullptr) {}

  Tree(const Tree &other)
      : comp(Comp()), node_count(other.node_count), root(TreeCopy(other.root)),
        first_node(leftMost(other.root)), last_node(rightMost(other.root)){};

  Tree(Tree &&other)
      : comp(Comp()), node_count(other.node_count), root(TreeCopy(other.root)),
        first_node(leftMost(other.root)), last_node(rightMost(other.root)){};

  Tree &operator=(const Tree &other) { return Tree(other); };
  Tree &operator=(Tree &&other) { return Tree(other); };
  ~Tree() { clear(); }

  bool &operator==(const Tree &other) {
    return iteratorTraversal(root, other.root);
  }

  bool &operator==(const Tree &other) const {
    return iteratorTraversal(root, other.root);
  }

  bool &operator!=(const Tree &other) {
    return !iteratorTraversal(root, other.root);
  }

  bool &operator!=(const Tree &other) const {
    return !iteratorTraversal(root, other.root);
  }

  bool iteratorTraversal(Node *root, Node *otherRoot) {
    if (root->node_count != otherRoot->node_count) {
      return false;
    }
    for (iterator it = root->begin(), otherIt = otherRoot->begin();
         it != root->end() && otherIt != otherRoot->end(); ++it, ++otherIt) {
      if (it->values->first != otherIt->values->first ||
          it->values->second != otherIt->values->second) {
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
    int OOBMargin;
    Node *p;

  public:
    using value_type = std::pair<const Key, Value>;
    using reference = value_type &;
    friend class Tree;
    iterator() : p(nullptr), OOBMargin(1) {}
    iterator(Node *p) : p(p), OOBMargin(0) {}

    reference &operator*() const {
      assert(p);
      if (OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      }
      return *p->values;
    }

    value_type *operator->() const {
      assert(p);
      if (OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iteartor");
      }
      return p->values;
    }

    iterator &operator++() {
      assert(p);
      if (p->next() == nullptr) {
        OOBMargin++;
      } else if (OOBMargin > 0) {
        OOBMargin++;
      } else {
        p = p->next();
      }
      return *this;
    }

    // to satisfy bidirectional iterator
    iterator &operator++(int) {
      assert(p);
      if (p->next() == nullptr) {
        OOBMargin++;
      } else if (OOBMargin > 0) {
        OOBMargin++;
      } else {
        p = p->next();
      }
      return *this;
    }

    iterator &operator--() {
      assert(p);
      if (p->prev() == nullptr) {
        OOBMargin--;
      } else if (OOBMargin > 0) {
        OOBMargin--;
      } else {
        p = p->prev();
      }
      return *this;
    }

    // to satisfy bidirectional iterator
    iterator &operator--(int) {
      assert(p);
      if (p->prev() == nullptr) {
        OOBMargin--;
      } else if (OOBMargin > 0) {
        OOBMargin--;
      } else {
        p = p->prev();
      }
    }

    friend bool operator==(iterator a, iterator b) { return a.p == b.p; }

    friend bool operator!=(iterator a, iterator b) { return !(a == b); }
  };

  class const_iterator
      : public std::iterator<std::bidirectional_iterator_tag, Tree> {
  private:
    // Out of bounds
    int OOBMargin;
    const Node *p;

  public:
    using value_type = std::pair<const Key, Value>;
    using reference = const value_type &;
    friend class Tree;
    const_iterator() : p(nullptr), OOBMargin(1) {}
    const_iterator(Node *p) : p(p), OOBMargin(0) {}
    const_iterator(const iterator &other)
        : p(other.p), OOBMargin(other.OOBMargin) {}

    reference &operator*() const {
      assert(p);
      if (OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iterator");
      }
      return *p->values;
    }

    value_type *operator->() const {
      assert(p);
      if (OOBMargin != 0) {
        throw std::out_of_range(
            "Element is not accesible: Out of bounds iteartor");
      }
      return p->values;
    }

    const_iterator &operator++() {
      assert(p);
      if (p->next() == nullptr) {
        OOBMargin++;
      } else if (OOBMargin > 0) {
        OOBMargin++;
      } else {
        p = p->next();
      }
      return *this;
    }

    // to satisfy bidirectional iterator
    const_iterator operator++(int) {
      assert(p);
      if (p->next() == nullptr) {
        OOBMargin++;
      } else if (OOBMargin > 0) {
        OOBMargin++;
      } else {
        p = p->next();
      }
      return *this;
    }

    const_iterator &operator--() {
      assert(p);
      if (p->prev() == nullptr) {
        OOBMargin--;
      } else if (OOBMargin > 0) {
        OOBMargin--;
      } else {
        p = p->prev();
      }
      return *this;
    }

    // to satisfy bidirectional iterator
    const_iterator operator--(int) {
      assert(p);
      if (p->prev() == nullptr) {
        OOBMargin--;
      } else if (OOBMargin > 0) {
        OOBMargin--;
      } else {
        p = p->prev();
      }
    }

    friend bool operator==(const const_iterator a, const const_iterator b) {
      return a.p == b.p;
    }

    friend bool operator!=(const const_iterator a, const const_iterator b) {
      return !(a == b);
    }
  };

  int size() const { return node_count; };        // must run constant time
  bool empty() const { return node_count == 0; }; // must run constant time

  std::pair<iterator, bool> insert(const Key &key, const Value &value) {
    Node *current = find(key);
    if (current != nullptr) {
      current->values->second = value;
      return {iterator(current), false};
    }

    if (root == nullptr) {
      root = new Node(key, value);
      node_count++;
      return {iterator(root), true};
    }

    Node *newNode = new Node(key, value);
    current = root;

    while (true) {
      if (comp(key, current->values->first)) {
        if (current->left) {
          current = current->left;
        } else {
          current->left = newNode;
          newNode->parent = current;
          return {iterator(newNode), true};
        }
      } else {
        if (current->right) {
          current = current->right;
        } else {
          current->right = newNode;
          newNode->parent = current;
          return {iterator(newNode), true};
        }
      }
    }
  };
  std::pair<iterator, bool> insert(Key &&key, Value &&value) {
    Node *current = find(key);
    if (current != nullptr) {
      current->values->second = value;
      return {iterator(current), false};
    }

    if (root == nullptr) {
      root = new Node(key, value);
      node_count++;
      return {iterator(root), true};
    }

    Node *newNode = new Node(key, value);
    current = root;

    while (true) {
      if (comp(key, current->values->first)) {
        if (current->left) {
          current = current->left;
        } else {
          current->left = newNode;
          newNode->parent = current;
          return {iterator(newNode), true};
        }
      } else {
        if (current->right) {
          current = current->right;
        } else {
          current->right = newNode;
          newNode->parent = current;
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
    return first_node->values->second;
  }
  const value_type &front() const {
    assert(first_node);
    return first_node->values->second;
  }
  value_type &back() {
    assert(last_node);
    return last_node->values->second;
  }
  const value_type &back() const {
    assert(last_node);
    return last_node->values->second;
  }
};
} // namespace DM852

#endif // TREE_H
