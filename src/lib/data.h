#include <string>
#include <typeinfo>
#include "namespaces.h"

using namespace std;
using namespace clients;

// --- Extern Variables Declaration
extern int *cmdsPtr, *subCmdsPtr, *fieldCmdsCharPtr, *sortByCmdsPtr;
extern string *fieldCmdsStrPtr, *actionsPtr, *accountPtr;

#ifndef DATA_H
#define DATA_H

// --- Templates

// Template to Return a String that's Surrounded by Brackets
template <typename T>
string addBrackets(T message)
{
  string addedBrackets;

  if (typeid(T) == typeid(const int)) // Checks if it's a Char
    addedBrackets += message;         // Works fine if the  Message is a Char
  else
    addedBrackets = message; // Message is of type String

  addedBrackets.insert(0, 1, '[');

  return addedBrackets.insert(addedBrackets.length(), 1, ']');
}

// --- Functions
int isCharOnArray(int character, int array[], int n);
void addClients(Client clients[], int *nClientsRead);
void viewClients(Client clients[], int nClientsRead, bool fields[], int sortBy[]);
void filterClients(Client clients[], int nMoviesRead, string **params, int sortBy[]);
void fields(); // As a Parameter or as a Subcommand
void sortByParameters();
void howToUseViewClients();
void howToUseFilterClients();
void depositMoney(Client clients[], int nClientsRead);
void cashoutMoney(Client clients[], int nClientsRead);
void sendMoney(Client clients[], int nClientsRead);
void printClientInfo(Client client, bool censoreInfo);
void changeStatus(Client clients[], int nClientsRead);

#endif