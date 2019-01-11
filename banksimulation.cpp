// banksimulation.cpp
// Implementations for class BankSimulation
// Author: Juan Arias
//
// The BankSimulation class simulates transactions in a bank. It takes
// predetermined transactions from a textfile and then proccesses them.

#include <iostream>
#include <sstream>
#include "banksimulation.h"

// Destroys BankSimulation
BankSimulation::~BankSimulation() {}

// Starts simulation with parameter fileName
void BankSimulation::Start(const std::string& fileName) {

	if (!tree.isEmpty()) {
	
		tree.Empty();
	}

	std::ifstream inFile(fileName);

	phase1(inFile);
}

// Runs phase1 of simulation,
// parameter inFile indicating file with predetermined transactions
void BankSimulation::phase1(std::ifstream& inFile) {

	std::queue<std::string> transactionQ;

	std::string line;

	while (getline(inFile, line)) {

		transactionQ.push(line);
	}

	inFile.close();

	phase2(transactionQ);
}

// Runs phase2 of simulation,
// parameter q filled with transactions
void BankSimulation::phase2(std::queue<std::string>& transactionQ) {

	while (!transactionQ.empty()) {

		const std::string transaction(transactionQ.front());
		
		transactionQ.pop();

		std::stringstream stream(transaction);

		char type;

		stream >> type;

		analyzeTransaction(transaction, stream, type);
	}

	phase3();
}

// Runs phase3 of simulation
void BankSimulation::phase3() {

	std::cout << std::endl << "Processing Done. Final Balances" << std::endl;

	tree.Display();
}

// Analyzes parameter transaction, classified by parameter type,
// to get necessary data from parameter stream
void BankSimulation::analyzeTransaction(const std::string& transaction,
										      std::stringstream& stream,
	                                                          char type) {
	if (type == OPEN) {

		openAccount(stream);

		return;
	}

	Account* acct1Ptr = nullptr, * acct2Ptr = nullptr;

	int id1, amount(NONE), fund1(NONE), id2(NONE), fund2(NONE);

	bool validAccounts(fillData(acct1Ptr, acct2Ptr, stream, amount,
								id1, fund1, id2, fund2));

	if (validAccounts) {

		processTransaction(acct1Ptr, acct2Ptr, transaction, type, amount,
						   fund1, fund2);

	} else if (acct1Ptr == nullptr) {
	
		printAccountNotFound(id1);

	} else {
	
		printAccountNotFound(id2);
	}
}

// Fills all parameters with corresponding data from parameter stream
bool BankSimulation::fillData(Account *& acct1Ptr, Account *& acct2Ptr,
							  std::stringstream& stream, int& amount, int& id1,
							  int& fund1, int& id2, int& fund2) const {

	stream >> id1;

	if (id1 > Account::MAX_ID) {

		fillIdFund(id1, fund1);
	}

	bool validAccounts(tree.Retrieve(id1, acct1Ptr));

	if (!stream.eof()) {

		stream >> amount;

		if (!stream.eof()) {

			stream >> id2;

			fillIdFund(id2, fund2);

			validAccounts &= tree.Retrieve(id2, acct2Ptr);
		}
	}

	return validAccounts;
}

// Fills id and fund with correct numbers to proceed with transaction
void BankSimulation::fillIdFund(int& id, int& fund) const {

	fund = id % Account::MAX_FUNDS;
	id   = id / Account::MAX_FUNDS;

}

// Processes transaction with given data parameters
void BankSimulation::processTransaction(Account* acct1Ptr, Account* acct2Ptr,
										const std::string& transaction,
										char type, int amount, int fund1,
															   int fund2) {
	bool wentThrough(true);

	switch (type) {

	case HISTORY:

		acct1Ptr->DisplayHistory(fund1);
		break;

	case DEPOSIT:

		acct1Ptr->Deposit(fund1, amount);
		break;

	case WITHDRAW:

		wentThrough = acct1Ptr->Withdraw(fund1, amount);
		break;

	case TRANSFER:

		wentThrough = acct1Ptr->Transfer(acct2Ptr, fund1, fund2, amount);

		acct2Ptr = (acct2Ptr == nullptr) ? acct1Ptr : acct2Ptr;

		if (wentThrough) {

			acct2Ptr->RecordTransaction(transaction, fund2);

		} else {

			acct2Ptr->RecordFailedTransaction(transaction, fund2);
		}

		break;
	}

	if (wentThrough) {
		
		acct1Ptr->RecordTransaction(transaction, fund1);

	} else {
		
		printInsufficientFunds(acct1Ptr->GetName(), amount, fund1);

		acct1Ptr->RecordFailedTransaction(transaction, fund1);
	}
}

// Processes opening an Account with parameter stream
// containing transaction data
void BankSimulation::openAccount(std::stringstream& stream) {

	std::string name, lastName;
	int id;

	stream >> lastName >> name >> id;

	if (Account::MIN_ID <= id && id <= Account::MAX_ID) {
	
		name += " ";
		name += lastName;

		Account* newAcct = new Account(name, id);

		if (!tree.Insert(newAcct)) {
			
			printIdInUse(id);

			delete newAcct;
		}

	} else {
		
		printInvalidId(id);
	}
}

// Prints error message for transaction with
// an id not in any active Account
void BankSimulation::printAccountNotFound(int id) const {
	
	std::cout << "ERROR: Account " << id
			  << " not found. Transaction refused." << std::endl;

}

// Prints error message for opening an Account with
// an id that is already in use
void BankSimulation::printIdInUse(int id) const {

	std::cout << "ERROR: Account " << id
			  << " is already open. Transaction refused." << std::endl;
}

// Prints error message for opening an Account with
// an id that is not of valid syntax
void BankSimulation::printInvalidId(int id) const {

	std::cout << "ERROR: Invalid ID number " << id
			  << "Transaction refused." << std::endl;
}

// Prints error message for Withdraw or Transfer with
// an amount that would leave a fund in negative balance
void BankSimulation::printInsufficientFunds(const std::string& client,
										    int amount, int fund) const {

	std::cout << "ERROR: Not enough funds to withdraw " << amount << " from "
		      << client << " " << Account::FundName(fund) << std::endl;

}
