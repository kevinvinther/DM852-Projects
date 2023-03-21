#include "Tree.hpp"

namespace DM852 {

// -------------------------------------------------
// NODE CLASS
// -------------------------------------------------

// Default Constructor
Tree::Node::Node()
	: key(0), value(""), parent(nullptr), left(nullptr), right(nullptr) {}

Tree::Node::Node(int key, const std::string &value)
	: key(key), value(value), parent(nullptr), left(nullptr), right(nullptr) {}

// Destructor, deletes the node and all its children
Tree::Node::~Node() {
	if (Tree::Node::left != nullptr) {
		delete Tree::Node::left;
	}
	if (Tree::Node::right != nullptr) {
		delete Tree::Node::right;
	}
}

// Returns the node with the smallest key larger than the key in this node.
// Returns nullptr if no such node exists.
Tree::Node *Tree::Node::next() {
	if (Tree::Node::right != nullptr) {
		Node *next = Tree::Node::right;
		// In a binary search tree the right-side is larger, and we want to find
		// the smallest number that is still larger. Therefore we first go to
		// the right, and keep going left until we get the smallest larger
		// number.
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
		}
		return next;
	}
}

// Returns the node with the smallest key larger than the key in this node.
// Returns nullptr if no such node exists.
const Tree::Node *Tree::Node::next() const {
	return const_cast<Node *>(this)->next();
}

// Returns the node with the largest key smaller than the key in this node.
// Returns nullptr if no such node exists.
Tree::Node *Tree::Node::prev() {
	if (Tree::Node::left != nullptr) {
		Node *next = Tree::Node::left;
		// In a binary search tree the right-side is larger, and we want to find
		// the smallest number that is still larger. Therefore we first go to
		// the right, and keep going left until we get the smallest larger
		// number.
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
		}
		return next;
	}
}

// Returns the node with the largest key smaller than the key in this node.
// Returns nullptr if no such node exists.
const Tree::Node *Tree::Node::prev() const {
	return const_cast<Node *>(this)->prev();
}

// -------------------------------------------------
// TREE CLASS
// -------------------------------------------------

Tree::Tree() : root(nullptr) {}

Tree::Tree(const Tree &other) : root(Tree::TreeCopy(other.root)) {}

Tree::Node *Tree::TreeCopy(Node *root) {
	if (root) {
		Tree::Node *newNode = new Tree::Node(root->key, root->value);
		newNode->left = TreeCopy(root->left);
		if (newNode->left) {
			newNode->left->parent = newNode;
		}
		newNode->right = TreeCopy(root->right);
		if (newNode->right) {
			newNode->right->parent = newNode;
		}
		return newNode;
	}
	return nullptr;
}

Tree &Tree::operator=(const Tree &other) {
	Tree::clear(); // Clear the current tree, this will make sure that both
				   // trees have the same structure (as opposed to checking if
				   // they're equal).
	this->root = Tree::TreeCopy(other.root);
	return *this;
}

int Tree::size() const { return Tree::size_traversal(Tree::root); }
int Tree::size_traversal(Node *node) const {
	if (node == nullptr) {
		return 0;
	} else {
		return (Tree::size_traversal(node->left) + 1 +
				Tree::size_traversal(node->right));
	}
}
bool Tree::empty() const { return Tree::size() == 0; }
std::pair<Tree::Node *, bool> Tree::insert(int key, const std::string &value) {
	Tree::Node *current = Tree::find(key);
	// If the key already exists, we update it:
	if (current != nullptr) {
		current->value = value;
		return {current, false};
	}
	// If no node exists (i.e. there is no root), we create one:
	if (Tree::root == nullptr) {
		Tree::root = new Tree::Node(key, value);
		return {Tree::root, true};
	}

	// If the key does not exist, and root exists, we create a new node:
	Tree::Node *newNode = new Tree::Node(key, value);
	current = root;
	// clang-format off
	while (true) {
                if(key < current->key) {
                        if(!(current->left == nullptr)) {
                                current = current->left;
                        } else {
                                /* newNode = current->left; */
                                /* newNode->parent = current; */
                                /* newNode->parent->left = newNode; */
                                current->left = newNode;
                                newNode->parent = current;
                                return {newNode, true};
                        } 
                } else { // key > current->key
                        if(!(current->right == nullptr)) {
                                current = current->right;
                        } else {
                                /* newNode = current->right; */
                                /* newNode->parent = current; */
                                /* newNode->parent->right = newNode; */
                                current->right = newNode;
                                newNode->parent = current;
                                return {newNode, true};
                        }
                }
	}
}

Tree::Node *Tree::find(int key) {
	Tree::Node *current = Tree::root;
	while (current->key != key) {
		if (key < current->key) {
			current = current->left;
		} else {
			current = current->right;
		}
		if (current->left == nullptr && current->right == nullptr) {
			return nullptr;
		}
	}
	return current;
}

const Tree::Node *Tree::find(int key) const {
	// We simply cast the non-const version as const.
	return const_cast<Tree *>(this)->find(key);
}

void Tree::clear() {
	// This function calls a helper function, which recursively traverses each
	// node, and then deletes the node if it exists.
	Tree::clearTraversal(Tree::root);
	Tree::root = nullptr;
}

void Tree::clearTraversal(Tree::Node *node) {
	if (node) {
		clearTraversal(node->left);
		clearTraversal(node->right);
		delete node;
	}
}

std::string &Tree::front() { 
	Node *node = Tree::root;
	if (node == nullptr) {
		throw "Root doesn't exist";
	}
	while (node->left != nullptr) {
		node = node->left;
	}
	return node->value;
}
const std::string &Tree::front() const {
	return const_cast<Tree *>(this)->front();
}
std::string &Tree::back() {
	Node *node = Tree::root;
	if (node == nullptr) {
		throw "Root doesn't exist";
	}
	while (node->right != nullptr) {
		node = node->right;
	}
	return node->value;
}
const std::string &Tree::back() const {
	return const_cast<Tree *>(this)->back();
}

Tree::Node *Tree::begin() {
	Node *node = Tree::root;
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}
const Tree::Node *Tree::begin() const {
	Node *node = Tree::root;
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}
Tree::Node *Tree::end() { return nullptr; }
const Tree::Node *Tree::end() const { return nullptr; }

// O(n)
Tree::~Tree() { Tree::clear(); }

bool Tree::operator==(const Tree &other) {
        return compareTraversal(this->root, other.root);
}

bool Tree::operator!=(const Tree &other) {
        return !compareTraversal(this->root, other.root);
}

bool Tree::compareTraversal(Tree::Node *root, Tree::Node *otherRoot) {
        // If the roots neither of the roots exist, we say that they are equal
        if (!root && !otherRoot) {
                return true;
        }
        // If one of the roots exist, but not the other, we say that they are not equal
        if (!root || !otherRoot) {
                return false;
        }
        // If the keys and values are not equal, we say that they are not equal
        if (root->key != otherRoot->key || root->value != otherRoot->value) {
                return false;
        } 
        return compareTraversal(root->left, otherRoot->left) && compareTraversal(root->right, otherRoot->right);
}


} // namespace DM852
