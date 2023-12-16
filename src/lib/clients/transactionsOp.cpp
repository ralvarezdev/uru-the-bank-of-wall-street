#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

// #define NDEBUG
#include <assert.h>

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
void deleteClientHistory(Clients *clients, int clientId);
string getCurrentTime();

// Function to Store Client Movements
void storeMovement(string time, clientActions action, Clients *clients, int indexFrom, float amount, int indexTo)
{
  Client clientFrom = (*clients).getClient(indexFrom); // Get Client
  double balanceFrom;

  ofstream movementsCSV(movementsFilename, ios::app);

  // Client ID, Account Number, Action, Amount, Time when the Transaction was Done
  movementsCSV << clientFrom.id << sep // Write to File at Last Line
               << setw(maxAccountDigits) << setfill('0') << fixed << setprecision(0) << clientFrom.account
               << sep << actionsStr[action]
               << sep << amount
               << sep << time << "\n";
  movementsCSV.close();

#ifndef NDEBUG
  balanceFrom = clientFrom.balance;
#endif

  if (action == clientDeposit)
    clientFrom.balance += amount;
  else
    clientFrom.balance -= amount;

  (*clients).updateBalance(indexFrom, clientFrom.balance);

  if (action == clientCashout || action == clientSend) // Check if the Client have the Money
    assert(balanceFrom >= amount);

  if (action == clientSend && indexTo != -1)
  {
    Client clientTo = (*clients).getClient(indexTo); // Get Client

    clientTo.balance += amount; // Update Balance
    (*clients).updateBalance(indexTo, clientTo.balance);
  }

  overwriteBalance(clients); // Overwrite Client Balances
}

// Function to Store Client Transactions
void storeTransactions(string time, int clientIdFrom, double accountFrom, float amount, int clientIdTo)
{
  ofstream transactionsCSV(transactionsFilename, ios::app);

  // Client ID that Sends the Money, Account Number, Amount, Account that Receives the Money, Time when the Transaction was Done
  transactionsCSV << clientIdFrom << sep // Write to File at Last Line
                  << setw(maxAccountDigits) << setfill('0') << fixed << setprecision(0) << accountFrom
                  << sep << amount
                  << sep << clientIdTo
                  << sep << time << "\n";
  transactionsCSV.close();
}

// Function to Delete Client Balance and Transaction History
void deleteClientHistory(Clients *clients, int clientId)
{
  string line, word;
  int tempId, count, movementsCounter = 0, transactionsCounter = 0;

  ostringstream movements, transactions;
  ifstream ifmovementsCSV(movementsFilename), iftransactionsCSV(transactionsFilename);

  while (getline(ifmovementsCSV, line)) // Get Client Movements
    try
    {
      if (line.length() == 0)
        continue;

      stringstream file(line);

      count = 0;
      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            try
            {
              tempId = stoi(word);

              if (tempId == clientId)
                movements << deletedClient;
              else
                movements << tempId;
            }
            catch (...)
            {
              movements << deletedClient;
            }
            break;
          default:
            movements << sep << word;
          }
        count++;
      }
      movements << '\n';
      movementsCounter++;
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from movements.csv
    }
  ifmovementsCSV.close();

  while (getline(iftransactionsCSV, line)) // Get Client Transactions
    try
    {
      if (line.length() == 0)
        continue;

      stringstream file(line);

      count = 0;
      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
          case 3:
            if (count != 0)
              transactions << sep;

            try
            {
              tempId = stoi(word);

              if (tempId == clientId)
                transactions << deletedClient;
              else
                transactions << tempId;
            }
            catch (...)
            {
              transactions << deletedClient;
            }
            break;
          default:
            transactions << sep << word;
          }
        count++;
      }
      transactions << '\n';
      transactionsCounter++;
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from transactions.csv
    }
  iftransactionsCSV.close();

  ofstream ofmovementsCSV(movementsFilename), oftransactionsCSV(transactionsFilename);

  assert(movementsCounter == 0 || movements.str().length() > 0); // Check if the File Content Could be Added to the Streams
  assert(transactionsCounter == 0 || transactions.str().length() > 0);

  ofmovementsCSV << movements.str(); // Write Movements Stream Content to movements.csv
  ofmovementsCSV.close();
  oftransactionsCSV << transactions.str(); // Write Transactions Stream Content to transactions.csv
  oftransactionsCSV.close();
}

// Function to Get Current Time
string getCurrentTime()
{
  time_t t = time(nullptr); // Get Time since Epoch as a Textual Representation
  tm tm = *localtime(&t);   // Get tm Structure

  ostringstream stream; // Get Date as a String

  stream << put_time(&tm, "%Y-%m-%d %H:%M:%S");
  assert(stream.str().length() > 11); // Check Stream Length

  return stream.str(); // Return Stream as a String
}
