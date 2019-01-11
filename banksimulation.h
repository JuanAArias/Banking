// banksimulation.h
// Specifications for class BankSimulation
// Author: Juan Arias
//
// The BankSimulation class simulates transactions in a bank. It takes
// predetermined transactions from a textfile and then proccesses them.

#ifndef BANKSIMULATION_H
#define BANKSIMULATION_H

#include <fstream>
#include <queue>
#include "bstree.h"

class BankSimulation {

public:

	// Destroys BankSimulation
	virtual ~BankSimulation();

	// Starts simulation with parameter fileName
	void Start(const std::string& fileName);

private:

	// Constant for no number
	static const int NONE = -1;

	// Constants for transaction types
	enum TRANSACTIONTYPE {

		OPEN      = 'O',
		HISTORY   = 'H',
		DEPOSIT   = 'D',
		WITHDRAW  = 'W',
		TRANSFER  = 'T'
	};

	// BSTree that stores Accounts
	BSTree tree;

	// Runs phase1 of simulation,
	// parameter inFile indicating file with predetermined transactions
	void phase1(std::ifstream& inFile);

	// Runs phase2 of simulation,
	// parameter transactionQ filled with transactions
	void phase2(std::queue<std::string>& transactionQ);

	// Runs phase3 of simulation
	void phase3();

	// Analyzes parameter transaction, classified by parameter type,
	// to get necessary data from parameter stream
	void analyzeTransaction(const std::string& transaction,
								  std::stringstream& stream, char type);

	// Fills all parameters with corresponding data from parameter stream
	bool fillData(Account *& acct1Ptr, Account *& acct2Ptr,
		          std::stringstream& stream, int& amount, int& id1, int& fund1,
				  int& id2, int& fund2) const;

	// Fills id and fund with correct numbers to proceed with transaction
	void fillIdFund(int& id, int& fund) const;

	// Processes transaction with given data parameters
	void processTransaction(Account* acct1Ptr, Account* acct2Ptr,
							const std::string& transaction, char type,
						    int amount, int fund1, int fund2);

	// Processes opening an Account with parameter stream
	// containing transaction data
	void openAccount(std::stringstream& stream);

	// Prints error message for transaction with
	// an id not in any active Account
	void printAccountNotFound(int id) const;

	// Prints error message for opening an Account with
	// an id that is already in use
	void printIdInUse(int id) const;

	// Prints error message for opening an Account with
	// an id that is not of valid syntax
	void printInvalidId(int id) const;

	// Prints error message for Withdraw or Transfer with
	// an amount that would leave a fund in negative balance
	void printInsufficientFunds(const std::string& client, int amount,
														   int fund) const;

};
#endif
