#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include "../namespaces.h"
#include "../clients/clientsOp.h"
#include "../clients/transactionsOp.h"
#include "../data/dataOp.h"
#include "../datatables/output.h"
#include "../terminal/ansiEsc.h"
#include "../terminal/input.h"

using namespace std;
using namespace files;
using namespace commands;
using namespace terminal;

// --- Extern Variables Declaration
extern bool validFieldsToFilter[];
extern int fieldCmdsChar[];
extern char *fieldCmdsStr[], *accountStr[];

// --- Function Prototypes
int isCharOnArray(int character, int array[], int n);
void addClients(Clients *clients);
void removeClient(Clients *clients);
void viewClients(Clients *clients, bool fields[], int sortBy[]);
void filterClients(Clients *clients, string **params, int sortBy[]);
void validParameters(int nCharTitle);
void fields();
void sortByParameters();
void howToUseViewClients();
void howToUseFilterClients();
void getBalance(Clients *clients);
void depositMoney(Clients *clients);
void cashoutMoney(Clients *clients);
void sendMoney(Clients *clients);
void changeStatus(Clients *clients);
bool clientActionConfirm(clientActions action);
int getSortByStr(int sortBy[], string sortByStr[], int n);

// --- Functions

// Function to check if the Integer that Represents a Character is on Char Array
int isCharOnArray(int character, int array[], int n)
{ // Linear Search
  for (int i = 0; i < n; i++)
    if (array[i] == character)
      return i;
  return -1;
}

// Function to Add Clients to clients.csv
void addClients(Clients *clients)
{
  while (true)
  {
    cout << clear;
    printTitle("Add Client", applyBgColor, applyFgColor, false);

    cout << '\n';
    addClientToFile(clients);

    if (!booleanQuestion("Do you want to Add more Clients?"))
      break;
  }
}

// Function to Remove Client from clients.csv
void removeClient(Clients *clients)
{
  bool deleteClient;
  clientStatus check;
  int id, index;
  string temp;

  cout << clear;
  printTitle("Remove Client", applyBgColor, applyFgColor, false);

  cout << '\n';
  check = getClientId(clients, &id, &index, "ID");

  if (check == clientNotFound) // The Id hasn't been Added to that File
    checkClientStatus(check);
  else
  {

    printClientInfo((*clients).getClient(index), true); // Print Client Info

    if (booleanQuestion("Is this the Client Account you want to Delete?"))
      if (booleanQuestion("Are you 100% Sure?"))
        deleteClient = true;

    if (deleteClient)
    {
      (*clients).deleteAt(index);       // Delete Client
      deleteClientHistory(clients, id); // Delete Client Transaction and Movements History
      overwriteBalance(clients);        // OverWrite Balance
      overwriteClients(clients);        // Overwrite Clients

      pressEnterToCont("Client Deleted", true);
    }
  }
}

// Function to View Clients Stored in clients.csv
void viewClients(Clients *clients, bool fields[], int sortBy[])
{
  int l, m = fieldEnd - 1, n = sortByEnd / 2;
  string fieldsStr[m], sortByStr[n], applied;

  if (fields[fieldAll])
    for (int i = 0; i < m; i++)
      fields[i] = true;

  for (int i = 0; i < m; i++)
  {
    applied = fields[i] ? "[Y] " : "[N] ";
    fieldsStr[i] = applied.append(fieldCmdsStr[i]); // Data to Print in the Field Parameters Row
  }

  l = getSortByStr(sortBy, sortByStr, n); // Get Sort By Array Length

  cout << clear;
  printTitle("Clients Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, l, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  sortClients(clients, sortBy, n); // Sort Clients
  printClients(clients, fields);   // Print Clients

  pressEnterToCont("Press ENTER to Continue", false);

  clientsMergeSort(clients, sortByIdA); // Return Clients Array to Initial Order
}

// Function to Filter Clients
void filterClients(Clients *clients, string **params, int sortBy[])
{
  int l = fieldEnd - 1, m = maxParamPerSubCmd, n = sortByEnd / 2, nClientsFiltered;
  bool fields[l];
  string sortByStr[n];

  fill(fields, fields + l, true); // Client Fields to Print (All)
  n = getSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Client Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(params, l, m, fieldCmdsStr);
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  filterClients(clients, params, fields, sortBy); // Filter Clients
  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print the Two Types of Parameters (Used in Filter Clients Command)
void validParameters(int nCharTitle)
{
  string temp = "\"sentence to search\"";

  cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets("word") << "Word to Search\n"
       << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(temp) << "Sentence to Search\n\n";
}

// Function to Print Field as a Parameter and as a Command
void fields()
{
  string temp;

  cout << clear; // Clear Terminal
  printTitle("Field as a Parameter (for View Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
    cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(fieldCmdsChar[i]) << fieldCmdsStr[i] << '\n';
  cout << '\n';

  printTitle("Field as a Command (for Filter Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
    if (validFieldsToFilter[i])
    {
      temp = addBrackets(fieldCmdsChar[i]).append(" [param...]");
      cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Client's " << fieldCmdsStr[i] << '\n';
    }
  cout << '\n';

  printTitle("Valid Field Parameters (for Filter Clients)", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Possible Parameters for Sort By Subcommand
void sortByParameters()
{
  string ascending, descending;

  cout << clear; // Clear Terminal
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  for (int i = 0; i < sortByEnd / 2; i++)
  {
    ascending = addBrackets(fieldCmdsChar[i]).append(" Ascending");
    descending = addBrackets(toupper(fieldCmdsChar[i])).append(" Descending");

    cout << tab1 << setw(nCharTitle) << setfill(' ') << fieldCmdsStr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';

  pressEnterToCont("Press ENTER to Continue", false);
}

// Print Usage Examples of the View Clients Command
void howToUseViewClients()
{
  const int nCmds = 3; // Number of Code Examples

  cmdExplanation examples[nCmds] = {
      cmdExplanation{"v -f . -s i", "View All Client Fields. Sort them by Id in Ascending Order"},
      cmdExplanation{"v -f i n -s I", "View Id and Client Name Field. Sort them by Id in Descending Order"},
      cmdExplanation{"v -f i t -s S", "View Id and Account Type Fields. Sort them by Suspended Status in Descending Order"}};

  printExamples(examples, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Print Usage Examples of the Filter Clients Command
void howToUseFilterClients()
{
  const int nCmds = 4; // Number of Code Examples

  cmdExplanation examples[nCmds] = {
      cmdExplanation{
          "f -f --n Ramon Ronald -s i", "Search for Clients Named as Ramon or Ronald. Sort them by Id in Ascending Order"},
      cmdExplanation{
          "f -f --n \"Ramon Alvarez\" \"Ronald Lopez\" -s I", "Search for Clients Named as Ramon Alvarez or Ronald Lopez. Sort them by Id in Descending Order"},
      cmdExplanation{
          "f -f --n Ronald --i 123456789 -s a", "Search for Clients Named as Ronald and the Client with Id 123456789. Sort them by Account Number"},
      cmdExplanation{
          "f -f --n Ramon --a 100200300 -s N", "Search for Clients Named as Ramon and the Client with Account Number 100200300. Sort them by Name in Descending Order"}};

  printExamples(examples, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function for Clients to Deposit Money in their Accounts
void depositMoney(Clients *clients)
{
  bool suspended;
  clientStatus check;
  Client client;
  float amount;
  int id, index;
  string time, message;

  cout << clear; // Clear Terminal
  printTitle("Deposit Money", applyBgColor, applyFgColor, false);
  cout << '\n';

  while (true)
  {
    check = getClientId(clients, &id, &index, "Client ID"); // Get Client Id and Check if it Exists

    if (check == clientSuspended) // The Client Cannot Deposit Any Money while his Account is Suspended
      suspended = true;

    if (check != clientFound)
    {
      checkClientStatus(check);
      break;
    }

    printClientInfo((*clients).getClient(index), true); // Print Client Info

    if (booleanQuestion("Is this your Client Account?"))
      break;
  }

  if (check != clientNotFound && !suspended)
  { // Check if the Client isn't Suspended
    amount = getFloat("Enter the Amount to Deposit", minDeposit, maxDeposit);

    if (clientActionConfirm(clientDeposit)) // Asks the Client for Confirmation
    {
      message = "You Have Successfully Deposited $";
      message.append(toStringWithPrecision(amount, precision));

      time = getCurrentTime();

      storeMovement(time, clientDeposit, clients, index, amount); // Save Deposit

      pressEnterToCont(message, false);
    }
  }
}

// Function to Get Client Balance
void getBalance(Clients *clients)
{
  clientStatus check;
  Client client;
  int id, index;
  string message;

  cout << clear; // Clear Terminal
  printTitle("Client Balance", applyBgColor, applyFgColor, false);
  cout << '\n';

  while (true)
  {
    check = getClientId(clients, &id, &index, "Client ID"); // Get Client Id and Check if it Exists

    if (check != clientFound)
    {
      checkClientStatus(check);
      break;
    }

    client = (*clients).getClient(index); // Get Client
    printClientInfo(client, true);        // Print Client Info

    if (booleanQuestion("Is this your Client Account?"))
      break;
  }

  ostringstream stream;

  stream << "Balance: $" << client.balance;
  pressEnterToCont(stream.str(), (client.balance < warningBalance) ? true : false);
}

// Function for Clients to Cashout Money from their Accounts
void cashoutMoney(Clients *clients)
{
  bool suspended;
  clientStatus check;
  Client client;
  int id, index;
  float amount;
  string time, message;

  cout << clear; // Clear Terminal
  printTitle("Cash Out Money", applyBgColor, applyFgColor, false);
  cout << '\n';

  while (true)
  {
    check = getClientId(clients, &id, &index, "Client ID"); // Get Client Id and Check if it Exists

    if (check == clientSuspended) // The Client Cannot Deposit Any Money while his Account is Suspended
      suspended = true;

    if (check != clientFound)
    {
      checkClientStatus(check);
      break;
    }

    client = (*clients).getClient(index); // Get Client
    printClientInfo(client, true);        // Print Client Info

    if (booleanQuestion("Is this your Client Account?"))
      break;
  }

  if (check != clientNotFound && !suspended)
  {                             // Check if the Client isn't Suspended
    printClientBalance(client); // Print Client Balance
    amount = getFloat("Enter the Amount to Cash Out", minDeposit, maxDeposit);

    if ((*clients).getClient(index).balance < amount)
      pressEnterToCont("Insufficient Funds", true);
    else if (clientActionConfirm(clientCashout)) // Asks the Client for Confirmation
    {
      message = "You Have Successfully Cashed Out $";
      message.append(toStringWithPrecision(amount, precision));

      time = getCurrentTime();

      storeMovement(time, clientCashout, clients, index, amount); // Save Cash Out

      pressEnterToCont(message, false);
    }
  }
}

// Function for Clients to Transfer Money from their Balance to other Client Accounts
void sendMoney(Clients *clients)
{
  bool suspended;
  clientStatus check;
  Client clientFrom;
  float amount;
  int idFrom, idTo, indexFrom, indexTo;
  string time, message;

  cout << clear; // Clear Terminal
  printTitle("Send Money", applyBgColor, applyFgColor, false);
  cout << '\n';

  while (true)
  {
    check = getClientId(clients, &idFrom, &indexFrom, "Client ID"); // Get Client Id and Check if it Exists

    if (check == clientSuspended) // The Client Cannot Deposit Any Money while his Account is Suspended
      suspended = true;

    if (check != clientFound)
    {
      checkClientStatus(check);
      break;
    }

    clientFrom = (*clients).getClient(indexFrom); // Get Client
    printClientInfo(clientFrom, true);            // Print Client Info

    if (booleanQuestion("Is this your Client Account?"))
      break;
  }

  if (check != clientNotFound && !suspended)
  { // Check if the Client isn't Suspended
    while (true)
    {
      check = getClientId(clients, &idTo, &indexTo, "Send to Client ID"); // Get Client Id and Check if it Exists

      if (check == clientNotFound)
      {
        checkClientStatus(check);
        break;
      }
      else if (idFrom == idTo) // Client Cannot Send to Himself
      {
        pressEnterToCont("Error: You Cannot Send to Yourself", false);
        continue;
      }

      printClientInfo((*clients).getClient(indexTo), true); // Print Client Info

      if (booleanQuestion("Is this the Account you Want to Send the Money to?"))
        break;
    }

    if (check != clientNotFound)
    {
      printClientBalance(clientFrom); // Print Client Balance
      amount = getFloat("Enter the Amount to Send", minDeposit, maxDeposit);

      if (clientFrom.balance < amount)
        pressEnterToCont("Insufficient Funds", true);
      else if (clientActionConfirm(clientSend)) // Asks the Client for Confirmation
      {
        message = "You Have Successfully Sent $";
        message.append(toStringWithPrecision(amount, precision));

        time = getCurrentTime();

        storeTransactions(time, idFrom, clientFrom.account, amount, idTo);    // Store Transaction
        storeMovement(time, clientSend, clients, indexFrom, amount, indexTo); // Update Balances

        pressEnterToCont(message, false);
      }
    }
  }
}

// Function to Change the Status of a Client
void changeStatus(Clients *clients)
{
  bool change;
  string temp;
  clientStatus clientStatus;
  int id, index;
  string message;

  cout << clear;
  printTitle("Change Account Status", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  cout << '\n';

  clientStatus = getClientId(clients, &id, &index, "Client ID to Change Status"); // Get Client Id and Check if it Exists

  if (clientStatus == clientNotFound)
    checkClientStatus(clientStatus);
  else if (clientStatus != errorStatus)
  {
    if (clientStatus == clientSuspended) // Ask wether to Suspend or Active Account
      change = booleanQuestion("Do you want to Activate the Client?");
    else
      change = booleanQuestion("Do you want to Suspend the Client?");
    cout << '\n';

    printClientInfo((*clients).getClient(index), true);
    if (!change)
      message = "Client Found: Nothing to Change";
    else
    {
      message = "Client Found: Changed Status";
      (*clients).changeClientStatus(index);

      overwriteClients(clients); // Overwrite clients.csv
    }
    pressEnterToCont(message, false);
  }
}

// Function to Print Message Confirmation when a Client Tries to make any Financial Transaction
bool clientActionConfirm(clientActions action)
{
  string message;

  switch (action)
  {
  case clientDeposit:
    message = "Do you Want to Deposit this Amount?";
    break;
  case clientCashout:
    message = "Do you Want to Cashout this Amount?";
    break;
  case clientSend:
    message = "Do you Want to Send this Amount?";
    break;
  }
  return booleanQuestion(message);
}

// Function to Get a String Array from a Int Array of the Sort By Commands that will be Applied to the Clients
int getSortByStr(int sortBy[], string sortByStr[], int n)
{
  bool nullParam;
  int charIndex, nParams = 0;
  string order;

  for (int j = 0; j < n; j++)
  {
    charIndex = sortBy[j];
    nullParam = false;

    if (charIndex == -1)
      nullParam = true;
    else if (charIndex % 2 == 0)
      order = "[A] ";
    else
      order = "[D] ";

    if (!nullParam)
    {
      sortByStr[nParams] = order.append(fieldCmdsStr[charIndex / 2]); // Data to Print in the Sort By Parameters Row
      nParams++;
    }
  }
  return nParams;
}