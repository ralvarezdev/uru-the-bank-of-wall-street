#include "../namespaces.h"
#include "../terminal/input.h"

using namespace std;
using namespace commands;
using namespace clients;

#ifndef CLIENTS_H
#define CLIENTS_H

// --- Functions
void getClients(Clients *clients);
void filterClients(Clients *clients, string **params, bool fields[], int sortBy[]);
void addClientToFile(Clients *clients);
void sortClients(Clients *clients, int sortBy[], int n);
void clientsMergeSort(Clients *clients, int sortByIndex);
int getClientId(string message);

// --- Templates

// Function to Check if Client Unique Fields have been Ocuppied
template <typename T>
clientStatus checkClient(Clients *clients, T unique, fieldCmds field, int *index)
{
  if (field != fieldId && field != fieldAccountNumber)
    return errorStatus;
  else if (field == fieldId)
    clientsMergeSort(clients, sortByIdA); // Sort Clients by Id
  else if (field == fieldAccountNumber)
    clientsMergeSort(clients, sortByAccountA); // Sort Clients by Account Number

  Client client;

  bool found = false;
  T value;
  int mid, start = 0, end = (*clients).getNumberClients() - 1;
  string line;

  while (start <= end)
  { // Binary Search
    mid = start + (end - start) / 2;

    try
    {
      client = (*clients).getClient(mid); // Get Client at the Given Index

      if (field == fieldId)
        value = client.id;
      else if (field == fieldAccountNumber)
        value = client.account;

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
        checkClient(clients, client.id, fieldId, index); // Get Index of Client with that Account Number when the Array is Sorted by Id

      if ((*clients).getClient(*index).suspended)
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