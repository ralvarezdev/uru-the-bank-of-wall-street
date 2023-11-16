#include "..\namespaces.h"
#include "..\terminal\input.h"

using namespace std;
using namespace commands;
using namespace clients;

#ifndef CLIENTS_H
#define CLIENTS_H

// --- Functions
int getClients(Client clients[]);
void filterClients(Client clients[], int nClientsRead, string **params, bool fields[], int sortBy[]);
void addClientToFile(Client clients[], int *nClientsRead);
void sortClients(Client clients[], int m, int sortBy[], int n);
void clientsMergeSort(Client clients[], int n, int sortByIndex);
int getClientId(string message);

// --- Templates

// Function to Check if Client Unique Fields have been Ocuppied
template <typename T>
clientStatus checkClient(Client clients[], int nClientsRead, T unique, fieldCmds field, int *index)
{
  if (field != fieldId && field != fieldAccountNumber)
    return errorStatus;
  else if (field == fieldId)
    clientsMergeSort(clients, nClientsRead, sortByIdA); // Sort Clients by Id
  else if (field == fieldAccountNumber)
    clientsMergeSort(clients, nClientsRead, sortByAccountA); // Sort Clients by Account Number

  bool found = false;
  T value;
  int mid, start = 0, end = nClientsRead - 1;
  string line;

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    try
    {
      if (field == fieldId)
        value = clients[mid].id;
      else if (field == fieldAccountNumber)
        value = clients[mid].account;

      found = (value == unique);
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
      if (field == fieldId)
        *index = mid;
      else if (field == fieldAccountNumber)
        checkClient(clients, nClientsRead, clients[mid].id, fieldId, index); // Get Index of Client with that Account Number when the Array is Sorted by Id

      if (clients[*index].suspended)
        return clientSuspended;
      else
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