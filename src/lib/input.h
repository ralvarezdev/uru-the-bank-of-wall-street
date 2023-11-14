#include <string>
#include "namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;

#ifndef INPUT_H
#define INPUT_H

// --- Functions
// void ignoreInput();
bool booleanQuestion(string message);
void wrongCommand(cmdStatus cmdStatus);
void wrongClientData(invalidClient wrongData);
void pressEnterToCont(string message, bool warning);

#endif