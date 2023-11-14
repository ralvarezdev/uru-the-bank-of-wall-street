#include "input.h"
#include "namespaces.h"

using namespace std;
using namespace commands;
using namespace clients;

// --- Extern Variables Declaration
extern string *accountPtr;

#ifndef CLIENTS_H
#define CLIENTS_H

// --- Functions
int getClients(Client clients[]);
int addClientToFile(Client clients[], int nClientsRead);
void sortClients(Client *clients, int m, int sortBy[], int n);
void clientsMergeSort(Client *clients, int n, int sortByIndex);

// --- Templates

// Function to Check if Client Unique Fields have been Ocuppied
template <typename T>
int checkClient(Client clients[], int nClientsRead, T unique, fieldCmds field, int *index)
{
  if (field != fieldId && field != fieldAccountNumber)
    return invalidArgument;
  else if (field == fieldId)
    clientsMergeSort(clients, nClientsRead, sortByIdA); // Sort Clients by Id
  else if (field == fieldAccountNumber)
    clientsMergeSort(clients, nClientsRead, sortByAccountA); // Sort Clients by Account Number

  bool found;
  int value, mid, start = 0, end = nClientsRead - 1;
  string line;

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    try
    {
      if (field == fieldId)
      {
        value = clients[mid].id;
        found = (value == unique);
      }
      else if (field == fieldAccountNumber)
      {
        value = clients[mid].account;
        found = (value == unique);
      }
    }
    catch (...)
    {
      if (field == fieldId)
        wrongClientData(invalidClientId);
      else if (field == fieldAccountNumber)
        wrongClientData(invalidClientAccountNumber);
    }

    if (found)
    {
      *index = mid;
      return clientFound;
    }
    else if (unique > value)
      start = mid + 1;
    else
      end = mid - 1;
  }
  return clientNotFound;
}

#endif