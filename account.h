// account.h
// Specifications for Account class
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

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>

class Account {

public:

	// MAX number of funds
	static const int MAX_FUNDS = 10;

	// MAX & MIN ID numbers
	static const int MAX_ID = 9999;
	static const int MIN_ID = 1000;

	// Constants to represent fund numbers
	enum FUNDNUM {

		NONE              = -1,
		MONEY_MARKET       = 0,
		PRIME_MONEY_MARKET = 1,
		LONG_TERM_BOND     = 2,
		SHORT_TERM_BOND    = 3,
		INDEX_FUND_500     = 4,
		CAPITAL_VALUE_FUND = 5,
		GROWTH_EQUITY_FUND = 6,
		GROWTH_INDEX_FUND  = 7,
		VALUE_FUND         = 8,
		VALUE_STOCK_INDEX  = 9
	};

	// Returns name of Fund indexed by parameter fund 
	static std::string FundName(int fund);

	// Returns true if parameter fund is a valid fund index, false otherwise
	static bool ValidFund(int fund);

	// Constructs Account with CLIENT as parameter name & ID as paremter num
	Account(const std::string& name, int num);

	// Destroys Account
	virtual ~Account();

	// Records parameter transaction for Fund indexed by parameter fund
	void RecordTransaction(const std::string& transaction, int fund);

	// Records failed parameter transaction for Fund indexed by parameter fund
	void RecordFailedTransaction(const std::string& transaction, int fund);

	// Displays history of all transactions for Fund indexed by parameter fund
	// or history of all transactions in Account if no fund specified
	void DisplayHistory(int fund = NONE) const;

	// Displays balances of all funds in Account
	void DisplayBalances() const;

	// Deposits parameter amount into Fund indexed by parameter fund,
	// returns true if successful, false otherwise
	bool Deposit(int fund, int amount);

	// Withdrawals parameter amount from Fund indexed by parameter fund,
	// returns true if successful, false otherwise
	bool Withdraw(int fund, int amount);

	// Transfers parameter amount from Fund indexed by parameter fund
	// to Fund indexed by parameter otherFund in the Account of parameter
	// otherPtr, returns true if successful, false otherwise
	bool Transfer(Account* otherPtr, int fund, int otherfund, int amount);

	// Returns name of client
	std::string GetName() const;

	// Returns the ID number of client
	int GetID() const;

private:

	// Funds of Account
	struct Fund {

		// Constructs empty fund
		Fund();

		// Balance of fund
		int balance;

		// History of transactions
		std::vector<std::string> transactions;

	};

	// Name of client
	const std::string CLIENT;

	// Account ID number
	const int ID;

	// Array for balances all ten funds of Account
	Fund funds[MAX_FUNDS];

	// Helper method to display transaction of Fund indexed by parameter fund
	void displayFundHistory(int fund) const;

	// Returns fund name with balance
	std::string fundInfo(int fund) const;

	// Helper method to display transactions of all Funds in Account
	void displayAll() const;

	// Covers overdraft Withdraws for linked Accounts,
	// returns true if successful, false otherwise
	bool cover(int fund, int otherFund, int amount, int overdraft);

	// Records cover transaction for linked Accounts
	void recordCover(int fund, int otherFund, int overdraft);
};
#endif
