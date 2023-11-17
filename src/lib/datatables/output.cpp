#include <iomanip>
#include <iostream>
#include "..\terminal\ansiEsc.h"
#include "..\terminal\input.h"
#include "..\namespaces.h"

using namespace clients;
using namespace commands;
using namespace terminal;

// --- Extern Variables Declaration
extern bool *fieldValidCmdsPtr;
extern string *fieldCmdsStrPtr, *accountPtr;

// --- Function Prototypes
void printClientInfo(Client client, bool censoreInfo);
void printArray(string *params, int m, string paramTitle);
void print2DArray(string **params, int m, int n, string paramsTitle[]);
void printClients(Client clients[], int m, bool *fields);

// --- Functions For Output Styling

// Function to Print the Commands and its Explanations while Taking Care of nChar
void printExamples(string cmds[], string explanations[], int n)
{
  const int nCharLine = nChar - tab1.length(); // Number of Characters for Each Line

  int printedChar, messageLength; // Number of Characters Printed in the Current Line and the Length of Message String
  string message, lastIter;       // Temp Data

  cout << clear;                                             // Clear Terminal
  printTitle("Examples", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  for (int i = 0; i < n; i++)
  {
    printedChar = 0;

    for (int j = 0; j < cmds[i].length(); j += nCharLine)
    {
      message = cmds[i].substr(j, nCharLine + j);
      cout << '\n'
           << tab1 << message << '\n';
    }

    cout << tab1 << string(nCharTitle, '-') << '\n';

    stringstream stream(explanations[i]); // To Print the Message with New Line each time it Reaches nCharLine
    while (getline(stream, message, ' '))
    {
      messageLength = message.length();

      if (printedChar + messageLength >= nCharLine)
      { // Exceeds the Maximum Number of Characters per Line
        cout << '\n'
             << tab1 << message;
        printedChar = messageLength;
        continue;
      }

      if (printedChar != 0)
      {
        cout << ' ' << message;
        printedChar++;
      }
      else
        cout << tab1 << message;

      printedChar += messageLength;
    }
    cout << '\n';
  }
}

// Function to Print a 1D Array
void printArray(string *params, int n, string paramTitle)
{
  string param;

  cout << setw(nCharTitle) << setfill(' ') << left << paramTitle;
  for (int i = 0; i < n && params[i] != "null"; i++)
  {
    param = params[i];

    if (i % paramPerLine == 0 && i != 0)
      cout << '\n' // Break the Line, and Add Some Indentation if there's more than N Parameters on the Current Line
           << string(nCharTitle, ' ');

    if (param.length() <= nCharParam - maxSpacing)
    {
      cout << setw(nCharParam) << setfill(' ') << left << param;
      continue;
    }

    param = param.substr(0, nCharParam - maxSpacing); // Substract from String
    param.append("...");                              // Append to the String
    cout << setw(nCharParam) << setfill(' ') << left << param;
  }
  cout << "\n\n";
}

// Function to Print a 2D Data Table
void print2DArray(string **params, int m, int n, string paramsTitle[])
{
  for (int i = 0; i < m; i++)
    if (params[i][0] != "null")
      printArray(params[i], n, paramsTitle[i]);
}

// Function to Print Client Info
void printClientInfo(Client client, bool censoreInfo)
{
  int nCharContent = nChar - nCharField;
  int nAccountCensored = 7;                                       // Number of Characters from Account Number that are Censored
  string accountType = accountPtr[client.type];                   // Get Client Type
  string suspended = (client.suspended) ? "Suspended" : "Active"; // Get Client Status

  printTitle("Client Info", applyBgColor, applyFgColor, false);

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStrPtr[fieldId] << client.id << '\n'; // Print Client Id

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStrPtr[fieldName]; // Print Client Name
  if (client.name.length() < nCharContent)
    cout << setw(nCharContent) << setfill(' ') << client.name << '\n';
  else
    cout << client.name.substr(0, nCharContent - 4) << "... " << '\n';

  if (!censoreInfo)
    cout << setw(nCharField) << setfill(' ') << fieldCmdsStrPtr[fieldAccountNumber]
         << setfill(' ') << fixed << setprecision(0) << client.account << '\n'                           // Print Client Account Number
         << setw(nCharField) << setfill(' ') << fieldCmdsStrPtr[fieldAccountType] << accountType << '\n' // Print Client Account Type
         << setw(nCharField) << setfill(' ') << fieldCmdsStrPtr[fieldSuspended] << suspended << '\n';    // Print Client Suspend Status
  else
    cout << setw(nCharField) << setfill(' ') << fieldCmdsStrPtr[fieldAccountNumber]
         << string(nAccountCensored, 'X') << getLastDigits(client.account, maxAccountDigits - nAccountCensored) << '\n' // Print Client Account Number
         << setw(nCharField) << setfill(' ') << fieldCmdsStrPtr[fieldAccountType] << accountType << '\n';               // Print Client Account Type
}

// Function to Print Clients
void printClients(Client clients[], int m, bool *fields)
{
  const int nId = 15;            // Number of Characters for Id
  const int nAccountType = 15;   // ... for Account Type
  const int nSuspended = 18;     // ... for Suspended Status
  const int nAccountNumber = 20; // ... for Account Number

  int nName = nChar; // Decrease the Number of Characters Used by the Name Field
  int n = fieldEnd - 1;

  int fieldsNChar[n] = {nId, 0, nAccountType, nSuspended, nAccountNumber}; // Number of Characters per Field
  for (int i = 0; i < n; i++)
    if (fields[i] && i != fieldName)
      nName -= fieldsNChar[i]; // Decrease Number of Characters for Movie's Name FIeld
  fieldsNChar[fieldName] = nName;

  cout << clear << sgrBgCmd << sgrFgCmd;
  for (int i = 0; i < n; i++)
    if (fields[i])
      cout << setw(fieldsNChar[i]) << setfill(' ') << fieldCmdsStrPtr[i]; // Field Title
  cout << reset << '\n';

  // Print Clients
  string temp;

  for (int i = 0; i < m; i++)
  {
    // Client Id
    if (fields[fieldId])
      cout << setw(nId) << setfill(' ') << clients[i].id;

    // Client Title
    if (fields[fieldName])
      if (clients[i].name.length() < nName)
        cout << setw(nName) << setfill(' ') << clients[i].name;
      else
        cout << clients[i].name.substr(0, nName - 4) << "... ";

    // Client Account Type
    if (fields[fieldAccountType])
      cout << setw(nAccountType) << setfill(' ') << accountPtr[clients[i].type];

    // Client Suspended Status
    if (fields[fieldSuspended])
    {
      temp = (clients[i].suspended) ? "Suspended" : "Active";
      cout << setw(nSuspended) << setfill(' ') << temp;
    }

    // Client Account Number
    if (fields[fieldAccountNumber])
      cout << fixed << setprecision(0) << setw(nAccountNumber) << setfill(' ') << clients[i].account;

    cout << '\n';
  }
}