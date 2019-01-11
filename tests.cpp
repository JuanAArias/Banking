// tests.cpp
// Tests for BSTree & Account classes
// Author: Juan Arias

#include <cassert>
#include <iostream>
#include "bstree.h"

// Test Deposit & RecordTransaction
void TestDeposit(Account* acctPtr) {
	
	for (int amount(1000), fund(Account::MONEY_MARKET);
		 fund < Account::MAX_FUNDS; amount += 1000, ++fund) {
	
		assert(acctPtr->Deposit(fund, amount));

		std::string transaction("D " + std::to_string(acctPtr->GetID()) +
			std::to_string(fund) + " " + std::to_string(amount));

		// Test transaction recording
		acctPtr->RecordTransaction(transaction, fund);
	}
}

// Test Transfer
void TestTransfer(Account* acctPtr, int fund1, int fund2, int amount) {

	std::string transaction("T " + std::to_string(acctPtr->GetID()) +
		std::to_string(fund1) + " " + std::to_string(amount) + " " +
		std::to_string(acctPtr->GetID()) + std::to_string(fund2));

	if (acctPtr->Transfer(acctPtr, fund1, fund2, amount)) {

		acctPtr->RecordTransaction(transaction, fund1);
		acctPtr->RecordTransaction(transaction, fund2);

	} else {
		
		acctPtr->RecordFailedTransaction(transaction, fund1);
		acctPtr->RecordFailedTransaction(transaction, fund2);
	}
}
// Test Withdraw for valid & invalid amounts, covering between linked funds,
// RecordTransaction, RecordFailedTransaction
void TestWithdraw(Account* acctPtr) {

	for (int amount(1000), fund(Account::MONEY_MARKET);
		fund < Account::MAX_FUNDS; amount += 1000, ++fund) {

		bool linkedFunds(fund == Account::PRIME_MONEY_MARKET ||
					   fund == Account::LONG_TERM_BOND);

		int testAmount = (linkedFunds) ? amount + 100: amount;

		std::string transaction("W " + std::to_string(acctPtr->GetID()) +
			std::to_string(fund) + " " + std::to_string(testAmount));

		// Test transaction recording
		if (acctPtr->Withdraw(fund, testAmount)) {
			
			acctPtr->RecordTransaction(transaction, fund);

		} else {
		
			acctPtr->RecordFailedTransaction(transaction, fund);
		}
	}
}

// Test Transfer between two different Accounts
void TestTransfer2Accounts(Account* acctPtr) {

	Account otherAcct("Yusuf Pisan", 3420);

	otherAcct.Deposit(Account::VALUE_FUND, 1000500);
	otherAcct.RecordTransaction("D 34208 1000500", Account::VALUE_FUND);

	assert(otherAcct.Transfer(acctPtr, Account::VALUE_FUND,
									   Account::SHORT_TERM_BOND, 450));

	std::string transaction1("T 34208 450 " + std::to_string(acctPtr->GetID())
							+ std::to_string(Account::SHORT_TERM_BOND));

	otherAcct.RecordTransaction(transaction1, Account::VALUE_FUND);
	acctPtr-> RecordTransaction(transaction1, Account::SHORT_TERM_BOND);

	assert(!otherAcct.Transfer(acctPtr, Account::MONEY_MARKET,
										Account::SHORT_TERM_BOND, 4000000));

	std::string transaction2("T 34200 40000000 " +
							 std::to_string(acctPtr->GetID()) +
							 std::to_string(Account::SHORT_TERM_BOND));

	otherAcct.RecordFailedTransaction(transaction2, Account::MONEY_MARKET);
	acctPtr-> RecordFailedTransaction(transaction2, Account::SHORT_TERM_BOND);

	otherAcct.DisplayHistory();
	acctPtr->DisplayHistory();

	// Test DisplayBalance
	acctPtr->DisplayBalances();
	otherAcct.DisplayBalances();

}

// Run Account Tests
void RunAccountTests() {
	
	Account acct("Juan Arias", 2569);

	// Test Deposit
	std::cout << "-----Testing Deposit------" << std::endl;

	TestDeposit(&acct);

	// Test DisplayHistory
	acct.DisplayHistory(Account::CAPITAL_VALUE_FUND);
	acct.DisplayHistory();

	std::cout << std::endl;

	// Test Transfer
	std::cout << "-----Testing Transfer------" << std::endl;

	TestTransfer(&acct, Account::CAPITAL_VALUE_FUND, Account::INDEX_FUND_500,
		50);

	TestTransfer(&acct, Account::GROWTH_EQUITY_FUND,
		Account::GROWTH_INDEX_FUND, 950500);

	acct.DisplayHistory();

	std::cout << std::endl;

	// Test Withdraw
	std::cout << "-----Testing Withdraw------" << std::endl;

	TestWithdraw(&acct);

	acct.DisplayHistory(Account::GROWTH_EQUITY_FUND);
	acct.DisplayHistory();

	std::cout << std::endl;

	// Test Transfer with other Account
	std::cout << "-----Testing Transfer with two Accounts------" << std::endl;

	TestTransfer2Accounts(&acct);
}

// Test Insert, check for inserting duplicate Ids
void TestInsert(BSTree* treePtr) {

	assert(treePtr->Insert(new Account("Kobe Bryant", 5824)));
	assert(treePtr->Insert(new Account("LeBron James", 3600)));
	assert(treePtr->Insert(new Account("Allen Iverson", 2901)));
	assert(treePtr->Insert(new Account("Dwayne Wade", 7549)));
	assert(treePtr->Insert(new Account("Tim Duncan", 6491)));
	assert(treePtr->Insert(new Account("Michael Jordan", 2360)));

	// Test Insert with same Id number, should be false and print error
	assert(!treePtr->Insert(new Account("James Harden", 5824)));
	assert(!treePtr->Insert(new Account("Chris Paul", 2360)));
}

// Test Retrieve, check for Accounts with ids not stored
void TestRetrieve(BSTree* treePtr) {

	Account* kobePtr,
		   * lebronPtr,
		   * aiPtr,
		   * dWadePtr,
		   * duncanPtr,
		   * mjPtr;

	assert(treePtr->Retrieve(5824, kobePtr) && kobePtr != nullptr);
	assert(treePtr->Retrieve(3600, lebronPtr) && lebronPtr != nullptr);
	assert(treePtr->Retrieve(2901, aiPtr) && aiPtr != nullptr);
	assert(treePtr->Retrieve(7549, dWadePtr) && dWadePtr != nullptr);
	assert(treePtr->Retrieve(6491, duncanPtr) && duncanPtr != nullptr);
	assert(treePtr->Retrieve(2360, mjPtr) && mjPtr != nullptr);

	// Test for ids not stored
	Account* beardPtr,
		   * cp3Ptr;

	assert(!treePtr->Retrieve(5825, beardPtr) && beardPtr == nullptr);
	assert(!treePtr->Retrieve(2359, cp3Ptr) && cp3Ptr == nullptr);
}

// Run BSTree tests
void RunBSTreeTests() {

	BSTree tree;

	// Test Insert
	TestInsert(&tree);
	
	// Test Retrieve
	TestRetrieve(&tree);
	
	// Test Display, should visit Nodes in sorted order (inorder traversal)
	tree.Display();

	// Test isEmpty & Empty
	assert(!tree.isEmpty());

	tree.Empty();

	assert(tree.isEmpty());
}

// Run all tests for each class
void RunAllTests() {

	std::cout <<"-------------------Running Account Tests------------------\n";
	RunAccountTests();
	std::cout << std::endl << std::endl <<
		"------------------Running BSTree Tests-------------------\n";
	RunBSTreeTests();
}

// Tests classes
int main() {
	
	RunAllTests();

	return 0;
}