// bstree.h
// Specifications for BSTree class
// Authors: Yusuf Pisan & Juan Arias
//
// The BSTree class is a binary-search tree for objects of the Account class,
// which can:
//	-insert an Account
//	-retrieve an Account
//	-display info of all stored Accounts
//	-clear all stored Accounts
//	-check if it is empty

#ifndef BSTREE_H
#define BSTREE_H

#include "account.h"

class BSTree {

public:

	// Constructs BSTree
	BSTree();

	// Destroys BSTree
	virtual ~BSTree();

	// Inserts Account object referenced by parameter acctPtr,
	// returns true if successful, false otherwise
	bool Insert(Account* acctPtr);

	// Points parameter acctPtr to Account object with ID given as a parameter
	// returns true if found, otherwise will point to nullptr then return false
	bool Retrieve(const int& ID, Account*& acctPtr) const;

	// Displays info of all stored Accounts to output
	void Display() const;

	// Clears all stored Accounts
	void Empty();

	// Returns true if BSTree is empty, false otherwise
	bool isEmpty() const;

private:

	// Nodes of BSTree
	struct Node {

		// Construct Node with given pointer to Account
		explicit Node(Account* acctPtr);

		// Pointer to Account object
		Account* acctPtr;

		// Left child of current Node
		Node* left;

		// Right child of current Node
		Node* right;

	};

	// Root Node of BSTree
	Node* root;

	// Recursive helper for Insert, uses parameter curr to traverse
	bool insertNode(Node* curr, Account* newPtr);

	// Recursive helper for Retrieve, uses parameter curr to traverse
	bool retrieveNode(Node* curr, const int& ID, Account*& acct) const;

	// Recursive helper for Display, uses parameter curr to traverse
	void displayNode(Node* curr) const;

	// Recursive helper for Empty, uses parameter curr to traverse
	void deleteNode(Node* curr);

};
#endif
