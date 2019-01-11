// ass5.cpp
// Runs banking program with command line input for all files specified
// Author: Juan Arias

#include "banksimulation.h"

// Constant for test file name
const char FILENAME[] = "BankTransIn.txt";

// Runs simulation with specified file name
int main() {

	BankSimulation sim;

	sim.Start(FILENAME);

	return 0;
}