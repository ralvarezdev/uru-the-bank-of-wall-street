#include <iostream>
#include <fstream>
#include <sstream>
#include "input.h"
#include <iomanip>
#include "clients.h"
#include "namespaces.h"

using namespace std;
using namespace commands;
using namespace files;
using namespace terminal;
using namespace clients;

// --- Functions Prototypes
int getClients(Client clients[]);
int addClientToFile(Client clients[], int nClientsRead);
void sortClients(Client *clients, int m, int sortBy[], int n);
void clientsMergeSort(Client *clients, int n, int sortByIndex);
void clientsMerge(Client *clients, Client *sorted, int low, int mid, int high, int sortByIndex);
string getLower(string word);

// Function to Get an Array of Clients from clients.csv
int getClients(Client clients[])
{
  string director[2];
  int count = 0, nline = 0, nClientsRead;
  string line, word;

  ifstream infile(clientsFilename);

  if (!infile.is_open())
  { // Couldn't Access to infile
    pressEnterToCont("Error: File Not Found. Press ENTER to go Back to Main Menu", true);
    return -1;
  }

  try
  {
    getline(infile, line); // Remove First Line, which is Used as a Title

    while (getline(infile, line) && nClients >= nClientsRead)
    {
      stringstream file(line);

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
      count = 0;
    }
    infile.close();
  }
  catch (...)
  {
    pressEnterToCont("An Error Ocurred when Loading movies.csv. It has been Manipulated", true);
    return -1;
  }
  return nClientsRead;
}

// Function to Add Client
int addClientToFile(Client clients[], int nClientsRead)
{
  if (nClientsRead >= nClients)
    pressEnterToCont("The Maximum Number of Clients has been Reached", true);
  else
  {
    Client newClient = Client();

    bool check;
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

        // The Id has already been Added to that File
        check = checkClient(clients, nClientsRead, newClient.id, fieldId, &index);
        if (check != clientNotFound)
          throw(-1);
        else
          break;
      }
      catch (...)
      {
        wrongClientData(invalidClientId);
      }

    cout << "Name: "; // Get Client Name
    getline(cin, newClient.name);

    while (true) // Get Client Account Number
      try
      {
        cout << "Account Number: ";
        getline(cin, temp);
        newClient.account = stoi(temp);

        // The Account Number has already been Added to that File
        check = checkClient(clients, nClientsRead, newClient.account, fieldAccountNumber, &index);
        if (check != clientNotFound)
          throw(-1);
        else
          break;
      }
      catch (...)
      {
        wrongClientData(invalidClientId);
      }

    check = booleanQuestion("Do you Want to Create a Debit (Y) or a Current (N) Account?");
    newClient.type = (check) ? accountDebit : accountCurrent;
    accountType = accountPtr[newClient.type]; // Get Account Type

    clients[nClientsRead++] = newClient;

    ofstream outfile(clientsFilename, ios::app); // Write to File
    outfile << newClient.id << sep << newClient.name << sep
            << setw(10) << setfill('0') << right << fixed << setprecision(0) << newClient.account << left
            << sep << accountType << sep << suspended << '\n';

    outfile.close();

    pressEnterToCont("Client Added Successfully!", false);
  }
  return nClientsRead;
}

// Function to Sort Clients (Uses Merge Sort)
void sortClients(Client *clients, int m, int sortBy[], int n)
{
  for (int i = 0; i < n; i++)
    if (sortBy[i] != -1)
      clientsMergeSort(clients, m, sortBy[i]);
}

// - Merge Sort
// O(n*logn)
// Stable
void clientsMergeSort(Client *clients, int n, int sortByIndex)
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
void clientsMerge(Client *clients, Client *sorted, int low, int mid, int high, int sortByIndex)
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