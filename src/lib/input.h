#include <string>
#include "namespaces.h"

using namespace std;
using namespace commands;

#ifndef INPUT_H
#define INPUT_H

// --- Functions
// void ignoreInput();
void wrongCommand(cmdStatus cmdStatus);
void pressEnterToCont(string message, bool warning);

#endif