#include "namespaces.h"

using namespace clients;

// --- Extern Variables Declaration
extern string *actionsPtr;

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

// --- Functions
void storeBalance(clientActions action, int clientId, double account, float amount);

#endif