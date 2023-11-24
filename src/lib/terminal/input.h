#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <typeinfo>

#include "../namespaces.h"

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

// Template to Get the N Last Digits from a Number
template <typename T>
string getLastDigits(T number, int numberDigits)
{
  string lastDigits;
  double numberDouble, dDivision;

  if (typeid(number) != typeid(const double))
    numberDouble = double(number);
  else
    numberDouble = number;

  for (int i = numberDigits; i > 0; i--)
  {
    if (i > 1)
      dDivision = numberDouble / pow(10, i - 1);
    else
      dDivision = numberDouble;
    lastDigits.append(to_string(abs(int(dDivision) % 10)));
  }

  return lastDigits;
}

// --- Functions
// void ignoreInput();
bool booleanQuestion(string message);
void wrongCommand(cmdStatus cmdStatus);
void wrongClientData(invalidClient wrongData);
void checkClientStatus(clientStatus clientStatus);
void pressEnterToCont(string message, bool warning = false);
string getLower(string word);
float getFloat(string message, float low, float high);

#endif