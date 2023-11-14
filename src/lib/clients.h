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
void sortClients(Client *clients, int m, int sortBy[], int n);

#endif