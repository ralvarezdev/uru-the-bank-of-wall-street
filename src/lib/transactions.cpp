#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "namespaces.h"
#include "transactions.h"

using namespace std;
using namespace files;
using namespace clients;

// Function to Store Updates of Clients Balance
void storeBalance(clientActions action, int clientId, double account, float amount)
{
    ofstream outfile(balancesFilename, ios::app);

    // Client Id, Account Number, Action, Amount
    outfile << clientId << sep // Write to File at Last Line
            << setw(maxAccountDigits) << setfill('0') << fixed << setprecision(0) << account
            << sep << actionsPtr[action]
            << sep << amount << "\n";
    outfile.close();
}

// Function to Store Client Transactions
void storeTransactions(int clientIdFrom, double accountFrom, float amount, int clientIdTo)
{
    ofstream outfile(transactionsFilename, ios::app);

    // Client Id that Sends the Money, Account Number, Amount, Account that Receives the Money
    outfile << clientIdFrom << sep // Write to File at Last Line
            << setw(maxAccountDigits) << setfill('0') << fixed << setprecision(0) << accountFrom
            << sep << amount
            << sep << clientIdTo << "\n";
    outfile.close();
}