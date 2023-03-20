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
		const int key;
		std::string value;
		Node *parent;
		Node *left;
		Node *right;
		Node *next();
		const Node *next() const;
		Node *prev();
		const Node *prev() const;
		Node();
		Node(int key, const std::string &value);
	};
	int size() const;
	bool empty() const;
	std::pair<Node *, bool> insert(int key, const std::string &value);
	Node *find(int key);
	const Node *find(int key) const;
	void clear();
	std::string &front();
	const std::string &front() const;
	std::string &back();
	const std::string &back() const;
	Node *begin();
	const Node *begin() const;
	Node *end();
	const Node *end() const;
	~Tree();

  private:
	void clearTraversal(Node *node);
	int size_traversal(Node *node) const;
	void getRoot();
	Node *root;
	Node *current_node;
};

} // namespace DM852

#endif // !TREE_H
