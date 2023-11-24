#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "transactionsOp.h"
#include "clientsOp.h"
#include "../namespaces.h"

using namespace std;
using namespace files;
using namespace clients;

// --- Extern Variables Declaration
extern char *actionsStr[];

// - Function Prototypes
void storeMovement(string time, clientActions action, Clients *clients, int indexFrom, float amount, int indexTo);
void storeTransactions(string time, int clientIdFrom, double accountFrom, float amount, int clientIdTo);
string getCurrentTime();

// Function to Store Client Movements
void storeMovement(string time, clientActions action, Clients *clients, int indexFrom, float amount, int indexTo)
{
    Client clientFrom = (*clients).getClient(indexFrom); // Get Client

    ofstream movementsCSV(movementsFilename, ios::app);

    // Client Id, Account Number, Action, Amount, Time when the Transaction was Done
    movementsCSV << clientFrom.id << sep // Write to File at Last Line
                 << setw(maxAccountDigits) << setfill('0') << fixed << setprecision(0) << clientFrom.account
                 << sep << actionsStr[action]
                 << sep << amount
                 << sep << time << "\n";
    movementsCSV.close();

    if (action == clientDeposit)
        clientFrom.balance += amount;
    else
        clientFrom.balance -= amount;
    (*clients).updateBalance(indexFrom, clientFrom.balance);

    if (action == clientSend && indexTo != -1)
    {
        Client clientTo = (*clients).getClient(indexTo); // Get Client
        clientTo.balance += amount;                      // Update Balance
        (*clients).updateBalance(indexTo, clientTo.balance);
    }

    overwriteBalance(clients); // Overwrite Client Balances
}

// Function to Store Client Transactions
void storeTransactions(string time, int clientIdFrom, double accountFrom, float amount, int clientIdTo)
{
    ofstream transactionsCSV(transactionsFilename, ios::app);

    // Client Id that Sends the Money, Account Number, Amount, Account that Receives the Money, Time when the Transaction was Done
    transactionsCSV << clientIdFrom << sep // Write to File at Last Line
                    << setw(maxAccountDigits) << setfill('0') << fixed << setprecision(0) << accountFrom
                    << sep << amount
                    << sep << clientIdTo
                    << sep << time << "\n";
    transactionsCSV.close();
}

// Function to Get Current Time
string getCurrentTime()
{
    time_t t = time(nullptr); // Get Time since Epoch as a Textual Representation
    tm tm = *localtime(&t);   // Get tm Structure

    ostringstream stream; // Get Date as a String

    stream << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return stream.str(); // Return Stream as a String
}
