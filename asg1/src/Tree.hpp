#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>

#ifndef TREE_H
#define TREE_H
namespace DM852 {
class Tree {
  public:
	class Node {
	  public:
		// They key held by the node. Unique to this node
		const int key;
		// The value of the node
		std::string value;
		// The parent of the node, updated during insert()
		Node *parent;
		// The left child of the node, updated during insert()
		Node *left;
		// The right child of the node, updated during insert()
		Node *right;
		/*
		 *  Returns the node with the smallest key larger than the key in this
		 *  node.
		 *  Arguments
		 *  ----------
		 *      - None
		 *
		 *  Returns
		 *  -------
		 *      - The node with the smallest key larger than the key in this
		 *      node
		 *      - Nullptr if no such node exists
		 *
		 *  Time Complexity
		 *  ---------------
		 *      - O(n)
		 *      - Each node is traversed at most once.
		 */
		Node *next();
		/*
		 * const version of next()
		 */
		const Node *next() const;
		/*
		 * Returns the node with the largest key smaller than the key in this
		 * node.
		 *
		 * Arguments
		 * ---------
		 *       - None
		 *
		 * Returns
		 * -------
		 *       - Node with the largest key smaller than the key in this node
		 *       - Nullptr if no such node exists
		 *
		 * Time Complexity
		 * ---------------
		 *       - O(n)
		 *       - Each node is traversed at most once
		 */
		Node *prev();
		/*
		 * const version of prev()
		 *
		 *
		 */
		const Node *prev() const;
		/*
		 * Default constructor.
		 *
		 * The default constructor is simply a member initialization list, which
		 * initializes all values to nullptr, except for key which is set to 0.
		 *
		 * Arguments
		 * ---------
		 *      - None
		 *
		 * Returns
		 * -------
		 *      - Nothing
		 *
		 */
		Node();
		/*
		 * Constructor with two arguments.
		 *
		 * This constructor is used to initialize a node, where it sets the key
		 * and value to the specified values given in the parameters, and the
		 * rest are set to nullptr.
		 *
		 * Arguments
		 * ---------
		 *      - int key
		 *              - The node key
		 *      - const std::string &value
		 *              - The value for the node, to be associated with the key
		 *
		 * Returns
		 * -------
		 *      - Nothing
		 */
		Node(int key, const std::string &value);
	};
	/*
	 * Get the size of the tree.
	 *
	 * This function calls a helper function, which recursively traverses the
	 * tree and adds one for each node visited.
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - The size of the tree
	 *
	 * Time Complexity
	 * ---------------
	 *  - O(n)
	 *  - Goes through each node exactly once.
	 */
	int size() const;
	/*
	 * Returns if the tree is empty.
	 *
	 * This function uses the size() function to check if it is zero. If it is
	 * zero, it returns true, meaning that the tree is empty.
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - True if the tree is empty
	 *      - False if the tree is not empty
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *
	 *      - Since the size() function is called, and it has a time complexity
	 *              of O(n), so does this function.
	 */
	bool empty() const;

	/*
	 * This function inserts a new node into the tree.
	 *
	 * This function inserts a new node into the tree, going through various
	 * checks throughout the functions runtime. The function appropriately sets
	 * the parent to the new node, as well as making the new node the
	 * appropriate child for the parent.
	 * If the key already exists in the tree, then instead of inserting the same
	 * key, it updates the value of the node with the associated key.
	 *
	 * Arguments
	 * ---------
	 *      - int key
	 *              - The unique key for the node, to be associated with the
	 *                      value
	 *      - const std::string &value
	 *              - The value of the node, associated with the key
	 *
	 * Returns
	 * -------
	 *      - {node*, false} if a node has been changed
	 *      - {node*, true} if a node has been inserted
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - Since the function traverses the tree, it has a time complexity of
	 *              O(n).
	 */
	std::pair<Node *, bool> insert(int key, const std::string &value);
	/*
	 * Finds the appropriate node for the given key.
	 *
	 * Through an interative traversal through the tree, the correct node
	 * is returned through the calculation according to the key.
	 *
	 * Arguments
	 * ---------
	 *      - int key
	 *              - The key of the node to be found
	 *
	 * Returns
	 * -------
	 *      - The node with the associated key if found
	 *      - Nullptr if no such node exists
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - This function traverses through the entire tree at at most once
	 *              per node.
	 *
	 * This function could have been O(log n) if I had implemented a balanced
	 * search tree.
	 */
	Node *find(int key);
	/*
	 * Const version of find()
	 */
	const Node *find(int key) const;
	/*
	 * Erase all elements
	 *
	 * This function calls a helper function, which recursively traverses the
	 * tree and deletes each node.
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - None
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - Since this function calls a traversing helper function which goes
	 *              through each node, the time is O(n).
	 */
	void clear();
	/*
	 * Get the value of the first element in the tree.
	 *
	 * This function goes to the leftmost node and returns it, as it must be
	 * the first element in the tree.
	 *
	 * Throw
	 * -----
	 *      - Throws an error if the tree is empty.
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - The value of the first element in the tree
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - In case the entire tree is on the lefthand side, it goes through
	 *              all nodes.
	 */
	std::string &front();
	/*
	 * Const version of front()
	 */
	const std::string &front() const;
	/*
	 * Get the value of the last element in the tree.
	 *
	 * This function goes to the rightmost node and returns it, as it must be
	 * the last element in the tree.
	 *
	 * Throw
	 * -----
	 *      - Throws an error if the tree is empty.
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - The value of the last element in the tree
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - In case the entire tree is on the righthand side, it goes through
	 *              all nodes.
	 */
	std::string &back();
	/*
	 * const version of back()
	 */
	const std::string &back() const;
	/*
	 * Get the first node in the tree.
	 *
	 * This function returns the leftmost node in the tree.
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - The first node in the tree.
	 *      - Nullptr if the tree is empty
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - Worst case scenario, the entire tree is on the left side, and we
	 *              traverse each node
	 */
	Node *begin();
	/*
	 * Const version of begin()
	 */
	const Node *begin() const;
	/*
	 * Return nullptr
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - nullptr
	 */
	Node *end();
	/*
	 * Const version of end()
	 */
	const Node *end() const;
	/*
	 * Initializes the tree to an empty tree.
	 *
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - None
	 */
	Tree();
	/*
	 * Deletes the entire tree
	 *
	 * Recursively traverses the tree and deletes each node. By calling clear();
	 *
	 * Arguments
	 * ---------
	 *      - None
	 *
	 * Returns
	 * -------
	 *      - None
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - Uses the clear() function which recursively goes through the tree.
	 */
	~Tree();

	/*
	 * Compare two Trees
	 *
	 * This function calls a helper function to decide whether two trees are
	 * equal or not.
	 *
	 * Arguments
	 * ---------
	 *      - const Tree &other
	 *              - The tree to be compared with
	 *
	 * Returns
	 * -------
	 *      - true if the two trees are equal
	 *      - false otherwise
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - The helper function recurisvely traverses through both of the
	 *              trees at the same time, going through each node.
	 */
	bool operator==(const Tree &other);
	/*
	 * Const version of ==
	 */
	bool operator==(const Tree &other) const;
	/*
	 * Compare two Trees
	 *
	 * This function calls a helper function to decide whether two trees are
	 * equal or not.
	 *
	 * Arguments
	 * ---------
	 *      - const Tree &other
	 *              - The tree to be compared with
	 *
	 * Returns
	 * -------
	 *      - true if the two trees are not equal
	 *      - false otherwise
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - The helper function recurisvely traverses through both of the
	 *        trees at the same time, going through each node.
	 */
	bool operator!=(const Tree &other);
	/*
	 * Const version of !=
	 */
	bool operator!=(const Tree &other) const;

	/*
	 * This constructor creates a new tree that is a copy of the other tree.
	 *
	 * Arguments
	 * ---------
	 *      - const Tree &other
	 *              - The tree to be copied
	 *
	 * Returns
	 * -------
	 *      - Copied tree
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - This function uses a helper function which runs recursively
	 *              through all nodes.
	 */
	Tree(const Tree &other);

	/*
	 * This constructor creates a new tree that is a copy of the other tree.
	 *
	 * Arguments
	 * ---------
	 *      - const Tree &other
	 *              - The tree to be copied
	 *
	 * Returns
	 * -------
	 *      - Copied tree
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - This function uses a helper function which runs recursively
	 *              through all nodes.
	 */
	Tree &operator=(const Tree &other);

  private:
	/*
	 * Helper function for clear()
	 *
	 * This function recursively traverses the tree and deletes each node.
	 *
	 * Arguments
	 * ---------
	 *      - Node *node
	 *              - The starting node
	 *
	 * Returns
	 * -------
	 *      - None
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - Traverses the entire tree, going through each node once.
	 */
	void clearTraversal(Node *node);
	/*
	 * Get the size of the tree.
	 *
	 * Recursively traverses the tree and returns 1 for each new node, resulting
	 * in the size of the tree.
	 *
	 * Arguments
	 * ---------
	 *       - Node *node
	 *               - The beginning node.
	 * Returns
	 * -------
	 *       - The size of the tree from the node argument
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - Traverses the entire tree, going through each node once.
	 */
	int size_traversal(Node *node) const;
	/*
	 * The root of the tree. Set during insert() or default constructor
	 */
	Node *root;

	/*
	 * Compares two trees from the starting point of two nodes.
	 *
	 * This function recursively checks if both nodes have the same key and the
	 * same value. If not, `false` is immediately returned.
	 *
	 * Arguments
	 * ---------
	 *       - Node* root
	 *               - Root of tree one
	 *       - Node* otherRoot
	 *               - Root of other tree
	 *
	 * Returns
	 * -------
	 *      - true if both trees are equal from roots down
	 *      - false otherwise
	 *
	 * Time Complexity
	 * ---------------
	 *      - O(n)
	 *      - This function recursively goes through all nodes of a tree.
	 */
	bool compareTraversal(Node *root, Node *otherRoot);

	/*
	 * Helper function to copy tree from root node
	 *
	 * Arguments
	 * ---------
	 *       - root
	 *               - Used to specify which node it should start copying from.
	 * Will not copy from above this node
	 *
	 * Returns
	 * -------
	 *       - Node with all the properties of the other tree root node
	 *
	 * Time Complexity
	 * ---------------
	 *       - O(n)
	 *       - Goes through all nodes recursively
	 */
	Node *TreeCopy(Node *root);
};

} // namespace DM852

#endif // !TREE_H
