#include <string>
#include <typeinfo>
#include "namespaces.h"

using namespace std;
using namespace clients;

// --- Extern Variables Declaration
extern int *cmdsPtr, *subCmdsPtr, *fieldCmdsCharPtr, *sortByCmdsPtr;
extern string *fieldCmdsStrPtr, *accountPtr;

#ifndef DATA_H
#define DATA_H

// --- Templates

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
void viewClients(Client clients[], int nClientsRead, bool fields[], int m, int sortBy[], int n);
void filterClients(Client clients[], int nMoviesRead, string **fieldParams, int l, int m, int sortBy[], int n);
void fields(); // As a Parameter or as a Subcommand
void sortByParameters();
void howToUseViewClients();
void howToUseFilterClients();
void depositMoney();
void checkoutMoney();
void transferMoney();
void suspendAccount();

#endif