// bstree.cpp
// Implementations for BSTree class
// Author: Juan Arias
//
// The BSTree class is a binary-search tree for objects of the Account class,
// which can:
//	-insert an Account
//	-retrieve an Account
//	-display info of all stored Accounts
//	-clear all stored Accounts
//	-check if it is empty

#include <iostream>
#include "bstree.h"

// Constructs BSTree
// Initializes root to nullptr
BSTree::BSTree() :root(nullptr) {}

// Destroys BSTree
// Calls Empty to deallocate dynamic memory
BSTree::~BSTree() {

	Empty();
}

// Inserts Account object referenced by parameter acctPtr,
// returns true if successful, false otherwise
// Uses helper method insertNode
bool BSTree::Insert(Account* newPtr) {

	if (root == nullptr) {
	
		root = new Node(newPtr);
		
		return true;

	}

	return insertNode(root, newPtr);
}

// Points parameter acctPtr to Account object with ID given as a parameter
// returns true if found, otherwise will point to nullptr then return false
// Uses helper method retrieveNode
bool BSTree::Retrieve(const int& ID, Account *& acct) const {

	return retrieveNode(root, ID, acct);
}

// Displays info of all stored Accounts to output
// Uses helper method displayNode
void BSTree::Display() const {

	displayNode(root);

}

// Clears all stored Accounts
// Uses helper method deleteNode
void BSTree::Empty() {

	deleteNode(root);

	root = nullptr;
}

// Returns true if BSTree is empty, false otherwise
bool BSTree::isEmpty() const {

	return root == nullptr;
}

// Recursive helper for Insert, uses parameter curr to traverse
bool BSTree::insertNode(Node* curr, Account* newPtr) {

	if (newPtr->GetID() < curr->acctPtr->GetID()) {

		if (curr->left == nullptr) {
		
			curr->left = new Node(newPtr);
			
			return true;
		}

		return insertNode(curr->left, newPtr);

	} else if (newPtr->GetID() > curr->acctPtr->GetID()) {

		if (curr->right == nullptr) {

			curr->right = new Node(newPtr);

			return true;
		}

		return insertNode(curr->right, newPtr);
	}

	return false;
}

// Recursive helper for Retrieve, uses parameter curr to traverse
bool BSTree::retrieveNode(Node* curr, const int& ID, Account*& acct) const {

	if (curr != nullptr) {
		
		if (curr->acctPtr->GetID() == ID) {
		
			acct = curr->acctPtr;

			return true;

		} 
			
		Node* nextPtr = (ID < curr->acctPtr->GetID()) ? curr->left:
														curr->right;

		return retrieveNode(nextPtr, ID, acct);
	}

	acct = nullptr;

	return false;
}

// Recursive helper for Display, uses parameter curr to traverse
void BSTree::displayNode(Node* curr) const {

	if (curr != nullptr) {

		displayNode(curr->left);
		
		curr->acctPtr->DisplayBalances();

		displayNode(curr->right);
	}

}

// Recursive helper for Empty, uses parameter curr to traverse
void BSTree::deleteNode(Node* curr) {

	if (curr != nullptr) {
		
		deleteNode(curr->left);
		deleteNode(curr->right);
		
		delete curr->acctPtr;
		delete curr;
	}

}

// Construct Node with given pointer to Account
BSTree::Node::Node(Account* newPtr) :acctPtr(newPtr), left(nullptr),
													 right(nullptr) {}
