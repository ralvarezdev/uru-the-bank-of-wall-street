#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "clientsOp.h"
#include "..\namespaces.h"
#include "..\datatables\output.h"
#include "..\terminal\input.h"

using namespace std;
using namespace commands;
using namespace files;
using namespace terminal;
using namespace clients;

// --- Extern Variables Declaration
extern bool *fieldValidCmdsPtr;
extern string *accountPtr;

// --- Functions Prototypes
int getClients(Client clients[]);
void filterClients(Client clients[], int nClientsRead, string **params, bool fields[], int sortBy[]);
void addClientToFile(Client clients[], int *nClientsRead);
void sortClients(Client clients[], int m, int sortBy[], int n);
void clientsMergeSort(Client clients[], int n, int sortByIndex);
void clientsMerge(Client clients[], Client sorted[], int low, int mid, int high, int sortByIndex);
string getLower(string word);
int getClientId(string message);

// --- Functions

// Function to Get an Array of Clients from clients.csv
int getClients(Client clients[])
{
  int count = 0, nline = 0, nClientsRead;
  string line, word;

  ifstream infile(clientsFilename);

  if (!infile.is_open())
  { // Couldn't Access to infile
    infile.close();
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
    return -1;
  }

  while (getline(infile, line) && nClients >= nClientsRead)
    try
    {
      stringstream file(line);

      count = 0;
      while (getline(file, word, sep))
      {
        if (word.length() != 0)
          switch (count)
          {
          case 0:
            clients[nline].id = stoi(word);
            break;
          case 1:
            clients[nline].name = word;
            break;
          case 2:
            clients[nline].account = stod(word);
            break;
          case 3:
            for (int i = 0; i < accountEnd; i++)
              if (getLower(accountPtr[i]) == getLower(word) || i == errorAccount)
              {
                clients[nline].type = i;
                break;
              }
            break;
          case 4:
            clients[nline].suspended = (word == "false") ? false : true;
            break;
          }
        count++;
      }
      nline++;
      nClientsRead = nline;

      clientsMergeSort(clients, nClientsRead, sortByIdA); // Sort Clients by Id
    }
    catch (...)
    {
      // It will Ignore the Line that was Read from clients.csv
    }

  infile.close();

  return nClientsRead;
}

// Function that Returns Clients Indexes that Matched with the Parameters
void filterClients(Client clients[], int nClientsRead, string **params, bool fields[], int sortBy[])
{
  clientStatus clientStatus;
  double account;
  int i, id, index, counter;
  string nameLower;

  bool *filteredIndexes = new bool[nClientsRead];               // Allocate Memory
  fill(filteredIndexes, filteredIndexes + nClientsRead, false); // Fill Array with False Values

  for (int field = 0; field < fieldEnd - 1; field++)
  {
    if (fieldValidCmdsPtr[field] && params[field][0] == "null") // Check if the Function can Filter that Field, and if there are Parameters
      continue;

    for (int param = 0; param < maxParamPerSubCmd && params[field][param] != "null"; param++)
    {
      if (field != fieldName)
      {
        if (field == fieldId)
        {
          id = stoi(params[field][param]);
          clientStatus = checkClient(clients, nClientsRead, id, fieldId, &index); // Binary Search
        }
        else if (field == fieldAccountNumber)
        {
          account = stod(params[field][param]);
          clientStatus = checkClient(clients, nClientsRead, account, fieldAccountNumber, &index); // Binary Search
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

        clientsMergeSort(clients, nClientsRead, sortByIdA); // Sort Clients by Id
        for (i = 0; i < nClientsRead; i++)
          if (!filteredIndexes[i] && getLower(clients[i].name).find(nameLower) != string::npos)
          {                            // Checks if the Client Name in Lowercase Contains the Parameter that is being Searched by Linear Search
            filteredIndexes[i] = true; // Save Id
            counter++;
          }
      }
    }
  }

  Client *filteredClients = new Client[counter];

  counter = 0;
  for (int i = 0; i < nClientsRead; i++)
    if (filteredIndexes[i])
      filteredClients[counter++] = clients[i]; // Save Client that has been Filtered to Array

  sortClients(filteredClients, counter, sortBy, sortByEnd); // Sort Clients
  printClients(filteredClients, counter, fields);           // Print Clients

  delete[] filteredClients;
  delete[] filteredIndexes;
}

// Function to Add Client
void addClientToFile(Client clients[], int *nClientsRead)
{
  if (*nClientsRead >= nClients)
    pressEnterToCont("The Maximum Number of Clients has been Reached", true);
  else
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

        check = checkClient(clients, *nClientsRead, newClient.id, fieldId, &index);
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

          check = checkClient(clients, *nClientsRead, newClient.account, fieldAccountNumber, &index);
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
      accountType = accountPtr[newClient.type]; // Get Account Type

      clients[*nClientsRead] = newClient;
      *nClientsRead = *nClientsRead + 1;

      ofstream outfile(clientsFilename, ios::app); // Write to File
      outfile << newClient.id << sep << newClient.name << sep
              << setw(maxAccountDigits) << setfill('0') << right << fixed << setprecision(0) << newClient.account << left
              << sep << accountType << sep << suspended << '\n';

      outfile.close();

      pressEnterToCont("Client Added Successfully!", false);
    }
  }
}

// Function to Sort Clients (Uses Merge Sort)
void sortClients(Client clients[], int m, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      clientsMergeSort(clients, m, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void clientsMergeSort(Client clients[], int n, int sortByIndex)
{
  int pass, low, high, mid, i;
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
  {
    int j = 0;
    Client temp;

    for (i = n - 1; i > (n - 1) / 2; i--)
    {
      temp = clients[j];
      clients[j] = clients[i];
      clients[i] = temp;
      j++;
    }
  }
}

// Function to Merge Clients Subarrays
void clientsMerge(Client clients[], Client sorted[], int low, int mid, int high, int sortByIndex)
{
  int i = low, j = mid + 1, k = low;

  switch (sortByIndex / 2)
  {
  case sortByIdA / 2:
    while (i <= mid && j <= high)
      if (clients[i].id < clients[j].id)
        sorted[k++] = clients[i++];
      else
        sorted[k++] = clients[j++];
    break;

  case sortByNameA / 2:
    while (i <= mid && j <= high)
      if (clients[i].name.compare(clients[j].name) < 0)
        sorted[k++] = clients[i++];
      else
        sorted[k++] = clients[j++];
    break;

  case sortByAccountA / 2:
    while (i <= mid && j <= high)
      if (clients[i].account < clients[j].account)
        sorted[k++] = clients[i++];
      else
        sorted[k++] = clients[j++];
    break;

  case sortByTypeA / 2:
    while (i <= mid && j <= high)
      if (clients[i].suspended && !clients[j].suspended)
        sorted[k++] = clients[i++];
      else
        sorted[k++] = clients[j++];
    break;
  }

  while (i <= mid)
    sorted[k++] = clients[i++];
  while (j <= high)
    sorted[k++] = clients[j++];

  for (i = low; i <= high; i++)
    clients[i] = sorted[i];
}

// Function to Get a Lowercase String
string getLower(string word)
{
  string wordToLower;

  for (int i = 0; i < word.length(); i++)
    wordToLower += tolower(word[i]); // Append Character in Lowercase

  return wordToLower;
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