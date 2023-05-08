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
  using value_type = std::pair<const Key, Value>;

private:
  /*
  ** Node class for the binary search tree.
  ** Contains a key, value, and pointers to the parent, left and right nodes.
  */
  class Node {
  public:
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
    bool operator!=(Node &other) { return !*this == other; }

    /*
    ** Inequality operator for the node.
    **
    ** @param &other The node to compare to.
    **
    ** @return True if the nodes are not equal, false otherwise.
    */
    bool operator!=(const Node &other) const { return !*this == other; }

    /*
    ** Goes right once, and then left as much as possible
    **
    ** @return The next node in the tree. If none return nullptr.
    */
    Node *next() {
      if (Tree::Node::right != nullptr) {
        Node *next = Tree::Node::right;
        // In a binary search tree the right-side is larger, and we want to
        // find the smallest number that is still larger. Therefore we first
        // go to the right, and keep going left until we get the smallest
        // larger number.
        while (next->left != nullptr) {
          next = next->left;
        }
        return next;
      } else { // The right node does not exist
        Tree::Node *next = this;
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
      if (Tree::Node::left != nullptr) {
        Node *next = Tree::Node::left;
        // In a binary search tree the right-side is larger, and we want to
        // find the smallest number that is still larger. Therefore we first
        // go to the right, and keep going left until we get the smallest
        // larger number.
        while (next->right != nullptr) {
          next = next->right;
        }
        return next;
      } else { // The right node does not exist
        Tree::Node *next = this;
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

  Tree *TreeCopy(Node *root) {
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

  void clear(Tree tree) {
    clearTraversal(tree.root);
    tree.root = nullptr;
    tree.first_node = nullptr;
    tree.last_node = nullptr;
    tree.node_count = 0;
  }

  void clearTraversal(Node *node) {
    if (node) {
      clearTraversal(node->left);
      clearTraversal(node->right);
      delete node;
    }
  }

  class iterator {
  public:
    iterator() = default;
    typedef std::pair<const Key, Value> value_type;

  private:
    friend class Tree;
    iterator(Node *p) : p(p) {}

  public:
    Value &operator*() const {
      assert(p);
      return p->values->second;
    }

    iterator &operator++() {
      assert(p);
      p = p->next();
      return *this;
    }

    friend bool operator==(iterator a, iterator b) { return a.p == b.p; }

    friend bool operator!=(iterator a, iterator b) { return !(a == b); }

  private:
    Node *p = nullptr;
  };

  class const_iterator {
  private:
    const_iterator() = default;

  public:
    Value &operator*() const {
      assert(p);
      return p->values->second;
    }

    iterator &operator++() {
      assert(p);
      p = p->next;
      return *this;
    }

    friend bool operator==(iterator a, iterator b) { return a.p == b.p; }

    friend bool operator!=(iterator a, iterator b) { return !(a == b); }

  private:
    const Node *p = nullptr;
  };

  int size() const { return node_count; };        // must run constant time
  bool empty() const { return node_count == 0; }; // must run constant time

  std::pair<iterator, bool> insert(const Key &key, const Value &value);
  std::pair<iterator, bool> insert(Key &&key, Value &&value);

  iterator find(const Key &key);
  const_iterator find(const Key &key) const;

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

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
