#pragma once
#include<iostream>

using std::cout;
using std::endl;

class Node  {
public:
	Node* parent = nullptr;
	virtual void setNumber(int number) {}
	virtual void setOperation(char op) {}
	virtual void setLeft(Node* left) {}
	virtual void setRight(Node* right) {}
	virtual int getNumber() const { return 0; }
	virtual char getOperation() const { return '\0'; }
	virtual Node* getLeft() const { return nullptr; }
	virtual Node* getRight() const { return nullptr; }
	bool isLeftChild() const {
		return parent->getLeft() == this;					
	}
	virtual bool isLeaf() const = 0;
};

class Leaf : public Node {
	int number;
public:
	Leaf(int data = 0) {
		number = data;
	}

	void setNumber(int number) {
		this->number = number;
	}

	int getNumber() const {
		return number;
	}

	bool isLeaf() const {
		return true;
	}
};

class OperationNode : public Node {
	char operation;
	Node* left;
	Node* right;
public:
	OperationNode(char operation = '\0', Node* left = nullptr, Node* right = nullptr) {
		this->operation = operation;
		this->left = left;
		this->right = right;
	}

	void setOperation(char op) {
		operation = op;
	}

	void setLeft(Node* left) {
		this->left = left;
		this->left->parent = this;
	}

	void setRight(Node* right) {
		this->right = right;
		this->right->parent = this;
	}

	char getOperation() const {
		return operation;
	}

	Node* getLeft() const {
		return left;
	}

	Node* getRight() const {
		return right;
	}

	bool isLeaf() const {
		return false;
	}

};

void printTree(Node* node, int intervals) {
	if (node == nullptr) {
		return;
	}
	printTree(node->getRight(), intervals+3);
	for (int i = 0; i < intervals; i++) {
		cout << " ";
	}
	if (node->isLeaf()) {
		cout << node->getNumber() << endl;
	}
	else {
		cout << node->getOperation()<< endl;
	}
	printTree(node->getLeft(), intervals+3);
}

