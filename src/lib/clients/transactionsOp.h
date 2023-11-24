#include "../namespaces.h"

using namespace clients;

// --- Extern Variables Declaration
extern string *actionsPtr;

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

// --- Functions
void storeMovement(string time, clientActions action, Clients *clients, int indexFrom, float amount, int indexTo = -1);
void storeTransactions(string time, int clientIdFrom, double accountFrom, float amount, int clientIdTo);
void deleteClientHistory(Clients *clients, int clientId);
string getCurrentTime();

#endif