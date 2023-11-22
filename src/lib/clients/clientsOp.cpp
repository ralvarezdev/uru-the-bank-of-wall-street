#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "clientsOp.h"
#include "../namespaces.h"
#include "../datatables/output.h"
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
void filterClients(Clients *clients, string **params, bool fields[], int sortBy[]);
void addClientToFile(Clients *clients);
void sortClients(Clients *clients, int sortBy[], int n);
void clientsMergeSort(Clients *clients, int sortByIndex);
void clientsMerge(Clients *clients, Client sorted[], int low, int mid, int high, int sortByIndex);
int getClientId(string message);

// --- Functions

// Function to Get an Array of Clients from clients.csv
void getClients(Clients *clients)
{
  int count = 0;
  string line, word;

  ifstream infile(clientsFilename);

  if (!infile.is_open())
  { // Couldn't Access to infile
    infile.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
  }

  while (getline(infile, line))
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
      (*clients).pushBack(newClient);
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from clients.csv
    }

  clientsMergeSort(clients, sortByIdA); // Sort Clients by Id
  infile.close();
}

// Function that Returns Clients Indexes that Matched with the Parameters
void filterClients(Clients *clients, string **params, bool fields[], int sortBy[])
{
  clientStatus clientStatus;
  double account;
  int i, id, index, counter, nClientsRead = (*clients).getNumberClients();
  string nameLower;

  bool *filteredIndexes = new bool[nClientsRead];               // Allocate Memory
  fill(filteredIndexes, filteredIndexes + nClientsRead, false); // Fill Array with False Values

  for (int field = 0; field < fieldEnd - 1; field++)
  {
    if (validFieldsToFilter[field] && params[field][0] == "null") // Check if the Function can Filter that Field, and if there are Parameters
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

  sortClients(&filteredClients, sortBy, sortByEnd); // Sort Clients
  printClients(&filteredClients, fields);           // Print Clients

  filteredClients.deallocate(); // Deallocate Memory
  delete[] filteredIndexes;
}

// Function to Add Client
void addClientToFile(Clients *clients)
{
  Client newClient = Client();

  int iter, check, index;
  string temp, date, accountType;

  newClient.suspended = true; // New Clients will have to wait for an Admin to Remove their Suspension
  string suspended = (newClient.suspended) ? "true" : "false";

  while (true) // Get Client ID
    try
    {
      cout << "ID: ";
      getline(cin, temp);
      newClient.id = stoi(temp);

      check = checkClient(clients, newClient.id, fieldId, &index);
      if (newClient.id <= 0)
        throw(-1); // ID Must be in the Range 0<ID<n
      break;
    }
    catch (...)
    {
      wrongClientData(invalidClientId);
    }

  if (check != clientNotFound) // The Id has been Added to that File
    wrongClientData(clientExists);
  else
  {
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
    accountType = accountStr[newClient.type]; // Get Account Type

    (*clients).pushBack(newClient); // Added the Client Right After the Last One, and Increase the Counter of Occupied Indexes

    ofstream outfile(clientsFilename, ios::app); // Write to File
    outfile << newClient.id << sep << newClient.name << sep
            << setw(maxAccountDigits) << setfill('0') << right << fixed << setprecision(0) << newClient.account << left
            << sep << accountType << sep << suspended << '\n';

    outfile.close();

    pressEnterToCont("Client Added Successfully!", false);
  }
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
int getClientId(string message)
{
  string temp;

  while (true)
    try // Get Client ID
    {
      cout << message << ": ";
      getline(cin, temp);
      return stoi(temp);
    }
    catch (...)
    {
      wrongClientData(invalidClientId);
    }
}