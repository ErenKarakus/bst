#include "bst.h"
#include <iostream>
#include <cassert>

struct BST::Node {
	Node(BST::KeyType, BST::ItemType);
	BST::KeyType key;
	BST::ItemType item;
	Node* leftChild;
	Node* rightChild;
};

BST::Node* BST::leaf() {
	return nullptr;
}

bool BST::isLeaf(Node* n) {
	return (n == leaf());
}

BST::Node::Node(KeyType key, ItemType item) {
	this->key = key;
	this->item = item;
	this->leftChild = nullptr;
	this->rightChild = nullptr;	
}

BST::ItemType* BST::lookup(KeyType soughtKey) {
	return lookupRec(soughtKey, root);
}

BST::ItemType* BST::lookupRec(KeyType soughtKey, Node* currentNode) {
	if (isLeaf(currentNode)) return nullptr;
	else if (soughtKey == currentNode->key) return &(currentNode->item);
	else if (currentNode->key > soughtKey) return lookupRec(soughtKey, currentNode->leftChild);
	else if (currentNode->key < soughtKey) return lookupRec(soughtKey, currentNode->rightChild);
}

void BST::insert(KeyType k, ItemType i) {
	insertRec(k, i, this->root);
}

void BST::insertRec(KeyType k, ItemType i, Node*& currentNode) {
	if (isLeaf(currentNode)) currentNode = new Node(k, i);
	else if (currentNode->key == k) currentNode->item = i;
	else if (currentNode->key > k) insertRec(k, i, currentNode->leftChild);
	else if (currentNode->key < k) insertRec(k, i, currentNode->rightChild);
}

void BST::displayEntries() {
	displayEntriesRec(root);
}

void BST::displayTree() {
	displayTreeRec(root, 0);
}

void BST::displayTreeRec(Node*& currentNode, int depth) {
	std::string tabs;
	for (size_t i = 0; i < depth; ++i) tabs += "\t";
	std::cout << tabs << currentNode->key << std::endl;

	if (!isLeaf(currentNode->leftChild)) displayTreeRec(currentNode->leftChild, depth + 1);
	if (!isLeaf(currentNode->rightChild)) displayTreeRec(currentNode->rightChild, depth + 1);
}

void BST::displayEntriesRec(Node*& currentNode) {
	// pre
	 std::cout << currentNode->key << std::endl;
	 if (!isLeaf(currentNode->leftChild)) displayEntriesRec(currentNode->leftChild);
	 if (!isLeaf(currentNode->rightChild)) displayEntriesRec(currentNode->rightChild);

	// in order
	if (!isLeaf(currentNode->leftChild)) displayEntriesRec(currentNode->leftChild);
	std::cout << currentNode->key << std::endl;
	if (!isLeaf(currentNode->rightChild)) displayEntriesRec(currentNode->rightChild);

	// post
	 if (!isLeaf(currentNode->leftChild)) displayEntriesRec(currentNode->leftChild);
	 if (!isLeaf(currentNode->rightChild)) displayEntriesRec(currentNode->rightChild);
	 std::cout << currentNode->key << std::endl;
}

BST::Node* BST::detachMinimumNode(Node*& current) {
	if (!isLeaf(current)) {
		if (isLeaf(current->leftChild)) return current;
		else detachMinimumNode(current->leftChild);
	}
}

void BST::remove(KeyType keyToRemove) {
	removeRec(root, keyToRemove);
}

void BST::removeRec(Node*& currentNode, KeyType k) {
	if (isLeaf(currentNode)) return;
	if (currentNode->key == k) {
		if (isLeaf(currentNode->leftChild) && isLeaf(currentNode->rightChild)) {
			delete currentNode;
			currentNode = leaf();
		}

		else if (isLeaf(currentNode->leftChild)) {
			Node* temp = currentNode;
			currentNode = currentNode->rightChild;
			delete temp;
		}

		else if (isLeaf(currentNode->rightChild)) {
			Node* temp = currentNode;
			currentNode = currentNode->leftChild;
			delete temp;
		}

		else if (!isLeaf(currentNode->leftChild) && !isLeaf(currentNode->rightChild)) {
			Node* minimumNode = detachMinimumNode(currentNode->rightChild);
			currentNode->key = minimumNode->key;
			currentNode->item = minimumNode->item;

			if (!isLeaf(minimumNode->rightChild)) {
				minimumNode = minimumNode->rightChild;
				delete minimumNode->rightChild;
			}
		}
	}
	else if (currentNode->key > k) removeRec(currentNode->leftChild, k);
	else if (currentNode->key < k) removeRec(currentNode->rightChild, k);
}

void BST::removeIf(std::function<bool(int)> f) {

	std::vector<int> keys;

	doFindIf(root, keys, f);

	for (int i = 0; i < keys.size(); i++) {
		remove(keys[i]);
	}
}

void BST::doFindIf(BST::Node* root, std::vector<int>& keys, std::function<bool(int)> f) {

	if (root == nullptr)
		return;

	if (f(root->key)) {
		keys.push_back(root->key);
	}

	doFindIf(root->leftChild, keys, f);


	doFindIf(root->rightChild, keys, f);

}


BST::~BST() {
	deepDelete(root);
}

void BST::deepDelete(Node* currentNode) {
	if (isLeaf(currentNode)) return;
	deepDelete(currentNode->leftChild);
	deepDelete(currentNode->rightChild);
	delete currentNode;
	currentNode = leaf();
}

BST::BST(const BST& bstToCopy) {
	this->root = deepCopy(bstToCopy.root);
}

BST::Node* BST::deepCopy(Node* nodeToCopy) {
	if (isLeaf(nodeToCopy)) return nullptr;
	Node* copiedNode = new Node(nodeToCopy->key, nodeToCopy->item);
	copiedNode->leftChild = deepCopy(nodeToCopy->leftChild);
	copiedNode->rightChild = deepCopy(nodeToCopy->rightChild);
	return copiedNode;
}

BST& BST::operator=(const BST& bstToCopy) {
	if (this == &bstToCopy)return*this;
	deepDelete(this->root);
	this->root = deepCopy(bstToCopy.root);
	return *this;
}

BST::BST(BST&& bstToMove) {
	deepDelete(this->root);
	this->root = bstToMove.root;
	bstToMove.root = leaf();
}

BST& BST::operator=(BST&& bstToMove) {
	if (this == &bstToMove) return *this;
	deepDelete(this->root);
	this->root = bstToMove.root;
	bstToMove.root = leaf();
	return *this;
}

void BST::rotateRight(Node*& localRoot) {
	assert(!isLeaf(localRoot));
	Node* a = localRoot->leftChild;
	assert(!isLeaf(a));
	Node* beta = a->rightChild;

	localRoot->leftChild = beta;
	a->rightChild = localRoot;
	localRoot = a;
}

void BST::rotateLeft(Node*& localRoot) {
	assert(!isLeaf(localRoot));
	Node* b = localRoot->rightChild;
	assert(!isLeaf(b));
	Node* beta = b->leftChild;

	localRoot->rightChild = beta;
	b->leftChild = localRoot;
	localRoot = b;
}
