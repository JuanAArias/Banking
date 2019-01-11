// account.cpp
// Implementations for Account class
// Author: Juan Arias
//
// The Account class is a client banking account. It has the first & last name
// of the client, the account ID number and contains assets held in up
// to the following ten funds:
//	 0: Money Market
//	 1: Prime Money Market
//	 2: Long-Term Bond
//	 3: Short-Term Bond
//	 4: 500 Index Fund
//	 5: Capital Value Fund
//	 6: Growth Equity Fund
//	 7: Growth Index Fund
//	 8: Value Fund
//	 9: Value Stock Index
//
// The operations of an Account include:
//	 -construct a new client account with ID number
//	 -deposit assets into a fund
//	 -withdraw assets from a fund
//	 -transfer assets between funds
//	 -transfer assets to another Account
//	 -display the history of all transactions for a single fund
//	 -display the history of all account transactions

#include <iostream>
#include <iomanip>
#include "account.h"

// Static function
// Returns name of Fund indexed by parameter fund 
std::string Account::FundName(int fund) {

	switch (fund) {

	case MONEY_MARKET:

		return "Money Market";

	case PRIME_MONEY_MARKET:

		return "Prime Money Market";

	case LONG_TERM_BOND:

		return "Long-Term Bond";

	case SHORT_TERM_BOND:

		return "Short-Term Bond";

	case INDEX_FUND_500:

		return "500 Index Fund";

	case CAPITAL_VALUE_FUND:

		return "Capital Value Fund";

	case GROWTH_EQUITY_FUND:

		return "Growth Equity Fund";

	case GROWTH_INDEX_FUND:

		return "Growth Index Fund";

	case VALUE_FUND:

		return "Value Fund";

	default:

		return "Value Stock Index";

	}
}

// Static function
// Returns true if parameter fund is a valid fund index, false otherwise
bool Account::ValidFund(int fund) {

	return (NONE < fund) && (fund < MAX_FUNDS);
}

// Constructs Account with CLIENT as parameter name & ID as paremter num
Account::Account(const std::string& name, int num) :CLIENT(name), ID(num) {}

// Destroys Account
Account::~Account() {}

// Records parameter transaction for parameter fund
void Account::RecordTransaction(const std::string& transaction, int fund) {

	if (ValidFund(fund)) {

		funds[fund].transactions.push_back(transaction);
	}
}

// Records parameter transaction for parameter fund if failed
void Account::RecordFailedTransaction(const std::string& transaction,
															int fund) {

	std::string failedTransaction(transaction + " (Failed)");
	RecordTransaction(failedTransaction, fund);
}

// Displays history of all transactions for parameter fund or
// history of all transactions in Account if no fund specified
void Account::DisplayHistory(int fund) const {

	std::cout << "Transaction history for " + CLIENT + " ";

	if (ValidFund(fund)) {

		displayFundHistory(fund);

	} else {

		std::cout << "by fund." << std::endl;

		displayAll();
	}
}

// Displays balances of all funds in Account
void Account::DisplayBalances() const {

	std::cout << CLIENT << " Account ID: " << ID << std::endl;

	for (int fund(MONEY_MARKET); fund < MAX_FUNDS; ++fund) {
	
		std::cout << "    " << fundInfo(fund) << std::endl;
	
	}

	std::cout << std::endl;
}

// Deposits parameter assets into parameter fund,
// returns true if successful, false otherwise
bool Account::Deposit(int fund, int amount) {

	if (!ValidFund(fund) || amount <= NONE) {

		std::cout << "DEPOSIT ERROR" << std::endl;

		return false;
	}

	funds[fund].balance += amount;

	return true;
}

// Withdrawals parameter assets from parameter fund,
// returns true if successful, false otherwise
bool Account::Withdraw(int fund, int amount) {

	if (!ValidFund(fund) || amount <= NONE) {

		std::cout << "WITHDRAW ERROR" << std::endl;

		return false;
	}

	int overdraft(funds[fund].balance - amount);

	bool wentThrough(false);

	if (overdraft > NONE) {
		
		funds[fund].balance -= amount;

		wentThrough =  true;
		
	} else if (fund == MONEY_MARKET || fund == PRIME_MONEY_MARKET) {

		int otherFund = (fund == MONEY_MARKET) ? PRIME_MONEY_MARKET:
													   MONEY_MARKET;
	
		wentThrough = cover(fund, otherFund, amount, overdraft);
	
	} else if (fund == LONG_TERM_BOND || fund == SHORT_TERM_BOND) {

		int otherFund = (fund == LONG_TERM_BOND) ? SHORT_TERM_BOND:
													LONG_TERM_BOND;

		wentThrough = cover(fund, otherFund, amount, overdraft);
	}

	return wentThrough;
}

// 
bool Account::Transfer(Account* otherPtr, int fund, int otherFund, int amount){

	bool canTransfer(Withdraw(fund, amount));

	if (canTransfer) {
		
		otherPtr->Deposit(otherFund, amount);
		return true;
	}

	return false;
}

// Returns name of client
std::string Account::GetName() const {

	return CLIENT;
}

// Returns the ID number of client
int Account::GetID() const {

	return ID;
}

// Helper method to display transaction of Fund indexed by parameter fund
void Account::displayFundHistory(int fund) const {

	std::cout << fundInfo(fund) << std::endl;

	for (std::string transaction : funds[fund].transactions) {
		
		std::cout << "  " << transaction << std::endl;
	
	}
}

// Returns fund name with balance
std::string Account::fundInfo(int fund) const {

	return FundName(fund) + ": $" + std::to_string(funds[fund].balance);

}

// Helper method to display transactions of all Funds in Account
void Account::displayAll() const {

	for (int fund(MONEY_MARKET); fund < MAX_FUNDS; ++fund) {
	
		displayFundHistory(fund);
	}
}

// Covers overdraft Withdraws for linked Accounts,
// returns true if successful, false otherwise
bool Account::cover(int fund, int otherFund, int amount, int overdraft) {

	if (funds[otherFund].balance + overdraft > NONE) {

		overdraft *= -1;

		Transfer(this, otherFund, fund, overdraft);

		Withdraw(fund, amount);

		recordCover(fund, otherFund, overdraft);

		return true;
	}
	
	return false;
}

// Records cover transaction for linked Accounts
void Account::recordCover(int fund, int otherFund, int overdraft){

	std::string transaction("Transfered " + std::to_string(overdraft));

	RecordTransaction(transaction + " from " + FundName(otherFund), fund);
	RecordTransaction(transaction + " to "   + FundName(fund), otherFund);
}

// Constructs empty fund
Account::Fund::Fund() :balance(NONE + 1) {}
