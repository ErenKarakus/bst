#pragma once
#include <string>
#include <functional>
#include <vector>

class BST {
public:
	BST() = default;
	BST(const BST&);
	BST& operator=(const BST&);
	BST(BST&&);
	BST& operator=(BST&&);
	~BST();
	
	using KeyType = int;
	using ItemType = std::string;
	
	ItemType* lookup(KeyType);
	void insert(KeyType, ItemType);
	void remove(KeyType);
	void removeIf(std::function<bool(KeyType)>);
	void displayEntries();
	void displayTree();

private:
	struct Node;
	Node* root = leaf();
	static Node* leaf();
	static bool isLeaf(Node*);
	void doFindIf(BST::Node* KeyType, std::vector<int>& keys, std::function<bool(int)> f);
	ItemType* lookupRec(KeyType, Node*);
	void insertRec(KeyType, ItemType, Node*&);
	void removeRec(Node*&, KeyType);
	Node* detachMinimumNode(Node*&);
	void deepDelete(Node*);
	Node* deepCopy(Node*);
	void displayEntriesRec(Node*&);
	void displayTreeRec(Node*&, int);
	void rotateRight(Node*&);
	void rotateLeft(Node*&);
};