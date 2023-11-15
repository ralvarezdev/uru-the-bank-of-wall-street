#include <string>
#include <iomanip>
#include "namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;

#ifndef INPUT_H
#define INPUT_H

// --- Templates

// Template to Convert Float to String with a Set Precision of N Digits
template <typename T>
string toStringWithPrecision(T number, int precision)
{
  ostringstream stream;
  stream << fixed << setprecision(precision) << number;
  return stream.str();
}

// --- Functions
// void ignoreInput();
bool booleanQuestion(string message);
void wrongCommand(cmdStatus cmdStatus);
void wrongClientData(invalidClient wrongData);
void checkClientStatus(clientStatus clientStatus);
void pressEnterToCont(string message, bool warning);
int getClientId(string message);
float getFloat(string message, float low, float high);

#endif