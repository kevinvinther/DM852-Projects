# Binary Tree

## Requirements

* The implementation should be in the namespace DM852.
* The tree must be an associative data structure that maps `int`s to `std::string`s.
    * associative = mapping a set of keys to a set of values
* It must have a nested class `Node`, which must have the following public member variables: 
    * `const int key;`: Is the immutable key stored in this node. 
    * `std::string value;` is the value for the key stored in this node.
    * `Node *parent;` must point to the parent node of this node in the tree, or be `nullptr` if it is the root.
    * `Node *left`, `Node *right` Must point to the left/right child Node of this node in the tree, or b nullptr if no such child exists.
* The nested class `Node` must additionally have the following member functions for in-order iteration through the tree:
    * `Node *next(); const Node *next() const;` Two overloads that returns the node with the smallest key larger than the key in this node. Returns `nullptr` if no such node exists.
    * `Node *prev(); const node *prev() const;` same but largest 
* It must be a *Regular* type, i.e. be *DefaultConstructible*, *Copyable*, and *EqualityComparable*.
* It must additionally have the following member functions. Some of them have pre-conditions which you must document as a comment at the function declaration in the header file. Similarly, if a member function oes not run in constant time (ignoring memory), you must document the computational complexity. If you have considered an alternative implementation to achieve a different computational complexity, write a short note about it as well. 
    - `int size() const;` Returns the number of elements stored
    - `bool empty() const;` returns `true` iff the container is empty. I.e. `empty() == (size() == 0)`
    - `std::pair<Node*, bool> insert(int key, const std::string &value);` Inserts a new element in the tree, or overwrites the value for key if it already exists. Return a pointer to the newly inserted updated node, and a boolean being true if a new node was inserted and false if an existing node was updated.
    - `Node *find(int key); const Node *find(int key) const;` Two overloads that look up the given key and returns a pointer, or nullptr.
    - `void clear()` Erase all elements
    - `std::string &front(); const std::string &front() const;` Two overloads for accessing the first element
    - `std::string &back(); const std::string &back() const;` Same for last element
    - `Node *begin(); const Node *begin() const;` Two overloads for accessing the node storing the first elementt. Returns nullptr if empty()
    - `Node *end(); const Node *end() const;` two overloads that always returns nullptr.
