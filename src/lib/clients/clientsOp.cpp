#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// #define NDEBUG
#include <assert.h>

#include "clientsOp.h"
#include "../namespaces.h"
#include "../datatables/output.h"
#include "../terminal/ansiEsc.h"
#include "../terminal/input.h"

using namespace std;
using namespace commands;
using namespace files;
using namespace terminal;
using namespace clients;

// --- Extern Variables Declaration
extern bool validFieldsToFilter[];
extern char *accountStr[];

// --- Functions Prototypes
void getClients(Clients *clients);
void overwriteClients(Clients *clients);
void overwriteBalance(Clients *clients);
void filterClients(Clients *clients, string **params, bool fields[], int sortBy[]);
void addClientToFile(Clients *clients);
void sortClients(Clients *clients, int sortBy[], int n);
void clientsMergeSort(Clients *clients, int sortByIndex);
void clientsMerge(Clients *clients, Client sorted[], int low, int mid, int high, int sortByIndex);
clientStatus getClientId(Clients *clients, int *id, int *index, string message);

// --- Functions

// Function to Get an Array of Clients from clients.csv
void getClients(Clients *clients)
{
  Client client;
  int id, nClients, clientCounter = 0, count = 0;
  float balance;
  string line, word;

  ifstream clientsCSV(clientsFilename);

  if (!clientsCSV.is_open())
  { // Couldn't Access to clientsCSV
    clientsCSV.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
  }

  while (getline(clientsCSV, line)) // Get Clients
    try
    {
      if (line.length() == 0)
        continue;

      stringstream file(line);
      Client newClient;

      count = 0;
      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            newClient.id = stoi(word);
            break;
          case 1:
            newClient.name = word;
            break;
          case 2:
            newClient.account = stod(word);
            break;
          case 3:
            for (int i = 0; i < accountEnd; i++)
              if (getLower(accountStr[i]) == getLower(word) || i == errorAccount)
              {
                newClient.type = i;
                break;
              }
            break;
          case 4:
            newClient.suspended = (word == "false") ? false : true;
            break;
          }
        count++;
      }

#ifndef NDEBUG
      nClients = (*clients).getNumberClients();
#endif

      (*clients).pushBack(newClient);
      assert(nClients == (*clients).getNumberClients() - 1); // Check if the Number of Clients Gets Increased
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from clients.csv
    }
  clientsCSV.close();

  ifstream balanceCSV(balanceFilename);
  clientsMergeSort(clients, sortByIdA); // Sort Clients by Id

  nClients = (*clients).getNumberClients();
  while (getline(balanceCSV, line)) // Get Clients Balance
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
            id = stoi(word);
            break;
          case 1:
            balance = stof(word);
            break;
          }
        count++;
      }

      for (; clientCounter < nClients; clientCounter++)
      {
        client = (*clients).getClient(clientCounter); // Get Client

        if (client.id == id)
          (*clients).updateBalance(clientCounter, balance); // Update Client Balance
        else
          continue; // To Prevent Wrong Client Balance Assignment if balance.csv was Manipulated

        break;
      }
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from balance.csv
    }
  balanceCSV.close();
}

// Function to Overwite clients.csv
void overwriteClients(Clients *clients)
{
  Client client;
  string suspended;

  ostringstream content;
  ofstream clientsCSV(clientsFilename);

  content << "ci, client, account_number, account_type, suspend\n"; // Overwrite Header
  for (int i = 0; i < (*clients).getNumberClients(); i++)
  {
    client = (*clients).getClient(i); // Get Client
    suspended = (client.suspended) ? "true" : "false";

    content << client.id << sep
            << client.name << sep
            << setw(maxAccountDigits) << setfill('0') << fixed << setprecision(0) << client.account << sep
            << accountStr[client.type] << sep
            << suspended << '\n';
  }

  assert(content.str().length() > 0); // Check if the Content Could be Added to the Stream
  clientsCSV << content.str();        // Write Content to clients.csv
  clientsCSV.close();
}

// Function to Store Updates of Clients Balance
void overwriteBalance(Clients *clients)
{
  Client client;

  ostringstream content;
  ofstream balanceCSV(balanceFilename);

  clientsMergeSort(clients, sortByIdA); // Sort Clients by Id
  for (int i = 0; i < (*clients).getNumberClients(); i++)
  {
    client = (*clients).getClient(i); // Get Client
    content << client.id << sep << fixed << setprecision(2) << client.balance << '\n';
  }

  assert(content.str().length() > 0); // Check if the Content Could be Added to the Stream
  balanceCSV << content.str();        // Write Content to balance.csv
  balanceCSV.close();
}

// Function that Returns Clients Indexes that Matched with the Parameters
void filterClients(Clients *clients, string **params, bool fields[], int sortBy[])
{
  clientStatus clientStatus;
  double account;
  int i, id, index, counter = 0, nClientsRead = (*clients).getNumberClients();
  string nameLower;

  assert(nClientsRead > 0); // Check if the Number of Clients is Greater than 0

  bool *filteredIndexes = new bool[nClientsRead];               // Allocate Memory
  fill(filteredIndexes, filteredIndexes + nClientsRead, false); // Fill Array with False Values

  for (int field = 0; field < fieldEnd - 1; field++)
  {
    // Check if the Function can Filter that Field, and if there are Parameters
    if (validFieldsToFilter[field] && params[field][0] == "null")
      continue;

    for (int param = 0; param < maxParamPerSubCmd && params[field][param] != "null"; param++)
    {
      if (field != fieldName)
      {
        if (field == fieldId)
        {
          id = stoi(params[field][param]);
          clientStatus = checkClient(clients, id, fieldId, &index); // Binary Search
        }
        else if (field == fieldAccountNumber)
        {
          account = stod(params[field][param]);
          clientStatus = checkClient(clients, account, fieldAccountNumber, &index); // Binary Search
        }
        assert(index >= 0); // Check the Client Index

        if (clientStatus != clientNotFound && !filteredIndexes[index])
        {                                // Checks if the Client has Already being Filtered
          filteredIndexes[index] = true; // Store Index
          counter++;
        }
      }
      else
      {
        nameLower = getLower(params[field][param]); // Get Client Name To Search for in Lowercase

        clientsMergeSort(clients, sortByIdA); // Sort Clients by Id
        for (i = 0; i < nClientsRead; i++)
          if (!filteredIndexes[i] && getLower((*clients).getClient(index).name).find(nameLower) != string::npos)
          {                            // Checks if the Client Name in Lowercase Contains the Parameter that is being Searched by Linear Search
            filteredIndexes[i] = true; // Save Id
            counter++;
          }
      }
    }
  }

  Clients filteredClients = Clients(counter); // Create Client Array

  for (int i = 0; i < nClientsRead; i++)
    if (filteredIndexes[i])
      filteredClients.pushBack((*clients).getClient(i)); // Save Client that has been Filtered to Array

  sortClients(&filteredClients, sortBy, sortByEnd / 2); // Sort Clients
  printClients(&filteredClients, fields);               // Print Clients

  string message = "Number of Coincidences: ";
  message.append(to_string(counter));

  if (counter == 0)
    cout << string(nChar, '_') << '\n';

  cout << '\n';
  printTitle(message, applyBgColor, applyFgColor, (counter == 0) ? true : false); // Print Number of Coincidences

  filteredClients.deallocate(); // Deallocate Memory
  delete[] filteredIndexes;
}

// Function to Add Client
void addClientToFile(Clients *clients)
{
  Client newClient = Client();

  int check, id, index = -1;
  string temp, accountType;

  newClient.suspended = true; // New Clients will have to wait for an Admin to Remove their Suspension
  string suspended = (newClient.suspended) ? "true" : "false";

  check = getClientId(clients, &id, &index, "ID"); // Get Client Id

  if (check != clientNotFound) // The Id has been Added to that File
  {
    assert(id >= 0 && index >= 0); // Check Client Id and Index
    wrongClientData(clientExists);
    return; // End this Function
  }

  assert(id >= 0 && index == -1); // Check Client Id and Index
  newClient.id = id;              // Assign Client Id

  cout << "Name: "; // Get Client Name
  getline(cin, newClient.name);

  while (true) // Get Client Account Number
    try
    {
      cout << "Account Number: ";
      getline(cin, temp);
      newClient.account = stoi(temp);

      check = checkClient(clients, newClient.account, fieldAccountNumber, &index);
      if (check != clientNotFound) // The Account Number has already been Added to that File
        throw(-1);
      else
        break;
    }
    catch (...)
    {
      wrongClientData(invalidClientAccountNumber);
    }

  check = booleanQuestion("Do you Want to Create a Debit (Y) or a Current (N) Account?");
  newClient.type = (check) ? accountDebit : accountCurrent;
  newClient.balance = 0.0;                  // Set Balance to 0
  accountType = accountStr[newClient.type]; // Get Account Type

  (*clients).pushBack(newClient); // Added the Client Right After the Last One, and Increase the Counter of Occupied Indexes

  ofstream clientsCSV(clientsFilename, ios::app); // Write to File
  clientsCSV << newClient.id << sep << newClient.name << sep
             << setw(maxAccountDigits) << setfill('0') << right << fixed << setprecision(0) << newClient.account << left
             << sep << accountType << sep << suspended << '\n';

  clientsCSV.close();

  overwriteBalance(clients);

  pressEnterToCont("Client Added Successfully!", false);
}

// Function to Sort Clients (Uses Merge Sort)
void sortClients(Clients *clients, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      clientsMergeSort(clients, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void clientsMergeSort(Clients *clients, int sortByIndex)
{
  int pass, low, high, mid, i, n = (*clients).getNumberClients();
  assert(n >= 0); // Check that the Array Has Some Clients

  Client *sorted = new Client[n]; // Store the Array in the Heap Memory

  for (pass = 2; pass <= n; pass *= 2)
  {
    for (i = 0; i + pass - 1 < n; i += pass)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      clientsMerge(clients, sorted, low, mid, high, sortByIndex);
    }

    if (n - i > pass / 2)
    {
      low = i;
      high = i + pass - 1;
      mid = (low + high) / 2;
      clientsMerge(clients, sorted, low, mid, n - 1, sortByIndex);
    }
    assert(pass % 2 == 0); // Check that pass value is a Multiple of 2
  }

  if (pass / 2 < n)
    clientsMerge(clients, sorted, 0, pass / 2 - 1, n - 1, sortByIndex);

  delete[] sorted; // Deallocate Memory

  if (sortByIndex % 2 != 0) // True if it's an Odd Number, which is Used for Descending Order
    (*clients).reverse();   // Reverse Array
}

// Function to Merge Clients Subarrays
void clientsMerge(Clients *clients, Client sorted[], int low, int mid, int high, int sortByIndex)
{
  int i = low, j = mid + 1, k = low;
  assert(low <= high); // Check if value of low variable is less or equal to high

  switch (sortByIndex / 2)
  {
  case sortByIdA / 2:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, fieldId, 1);
    break;

  case sortByNameA / 2:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, fieldName, 1);
    break;

  case sortByAccountA / 2:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, fieldAccountNumber, 1);
    break;

  case sortByTypeA / 2:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, fieldAccountType, 1);
    break;

  case sortByBalanceA / 2:
    while (i <= mid && j <= high)
      sorted[k++] = (*clients).compare(&i, &j, fieldBalance, 1);
    break;
  }

  while (i <= mid)
    sorted[k++] = (*clients).getClient(i++);
  while (j <= high)
    sorted[k++] = (*clients).getClient(j++);

  for (i = low; i <= high; i++)
    (*clients).insertAt(i, sorted[i]);
}

// Function to Ask for Client Id
clientStatus getClientId(Clients *clients, int *id, int *index, string message)
{
  string temp;

  while (true) // Get Client ID
    try
    {
      cout << message << ": ";
      getline(cin, temp);
      *id = stoi(temp);

      if (*id <= 0)
        throw(-1); // ID Must be in the Range 0<ID<n

      return checkClient(clients, *id, fieldId, index);
    }
    catch (...)
    {
      wrongClientData(invalidClientId);
    }
}