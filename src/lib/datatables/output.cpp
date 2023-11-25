#include <iomanip>
#include <iostream>

// #define NDEBUG
#include <assert.h>

#include "../terminal/ansiEsc.h"
#include "../terminal/input.h"
#include "../namespaces.h"

using namespace clients;
using namespace commands;
using namespace terminal;

// --- Extern Variables Declaration
extern bool validFieldsToFilter[];
extern char *fieldCmdsStr[], *accountStr[];

// --- Function Prototypes
void printExamples(cmdExplanation examples[], int n);
void printClientInfo(Client client, bool censoreInfo);
void printClientBalance(Client client);
void printArray(string *params, int m, string paramTitle);
void print2DArray(string **params, int m, int n, char **paramsTitle);
void printClients(Clients *clients, bool *fields);

// --- Functions For Output Styling

// Function to Print the Command Examples and its Explanations while Taking Care of nChar
void printExamples(cmdExplanation examples[], int n)
{
  const int nCharLine = nChar - tab1.length(); // Number of Characters for Each Line

  int printedChar, messageLength; // Number of Characters Printed in the Current Line and the Length of Message String
  string message;                 // Temp Data

  cout << clear;                                             // Clear Terminal
  printTitle("Examples", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  for (int i = 0; i < n; i++)
  {
    printedChar = 0;

    for (int j = 0; j < examples[i].cmd.length(); j += nCharLine)
    {
      message = examples[i].cmd.substr(j, nCharLine + j);
      assert(message.length() <= nCharLine); // Check message String Length

      cout << '\n'
           << tab1 << message << '\n';
    }

    cout << tab1 << string(nCharTitle, '-') << '\n';

    stringstream stream(examples[i].explanation); // To Print the Message with New Line each time it Reaches nCharLine

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

  assert(nCharTitle > 0 && nCharParam > 0 && maxSpacing > 0); // Check nCharTitle, nCharParam and maxSpacing

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
void print2DArray(string **params, int m, int n, char **paramsTitle)
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
  string accountType = accountStr[client.type];                   // Get Client Type
  string suspended = (client.suspended) ? "Suspended" : "Active"; // Get Client Status

  assert(nCharContent > 0); // Check nCharContent

  cout << '\n';
  printTitle("Client Info", applyBgColor, applyFgColor, false);

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldId] << client.id << '\n'; // Print Client Id

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldName]; // Print Client Name
  if (client.name.length() < nCharContent)
    cout << setw(nCharContent) << setfill(' ') << client.name << '\n';
  else
    cout << client.name.substr(0, nCharContent - 4) << "... " << '\n';

  if (!censoreInfo)
    cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountNumber]
         << setfill(' ') << fixed << setprecision(0) << client.account << '\n'                         // Print Client Account Number
         << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountType] << accountType << '\n'  // Print Client Account Type
         << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountStatus] << suspended << '\n'; // Print Client Suspend Status
  else
    cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountNumber]
         << string(nAccountCensored, 'X') << getLastDigits(client.account, maxAccountDigits - nAccountCensored) << '\n' // Print Client Account Number
         << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountType] << accountType << '\n';                  // Print Client Account Type
  cout << '\n';
}

// Function to Print Client Balance
void printClientBalance(Client client)
{
  cout << "Balance: $" << client.balance << '\n';
}

// Function to Print Clients
void printClients(Clients *clients, bool *fields)
{
  const int nId = 15;            // Number of Characters for Id
  const int nAccountType = 15;   // ... for Account Type
  const int nSuspended = 18;     // ... for Suspended Status
  const int nAccountNumber = 20; // ... for Account Number
  const int nBalance = 12;       // ... for Balance

  int nName = nChar; // Decrease the Number of Characters Used by the Name Field
  int n = fieldEnd - 1;

  int fieldsNChar[n] = {nId, 0, nAccountType, nSuspended, nAccountNumber, nBalance}; // Number of Characters per Field
  for (int i = 0; i < n; i++)
    if (fields[i] && i != fieldName)
      nName -= fieldsNChar[i]; // Decrease Number of Characters for Movie's Name Field

  assert(nName > 0); // Check nName
  fieldsNChar[fieldName] = nName;

  cout << clear << sgrBgCmd << sgrFgCmd;
  for (int i = 0; i < n; i++)
    if (fields[i])
      cout << setw(fieldsNChar[i]) << setfill(' ') << fieldCmdsStr[i]; // Field Title
  cout << reset << '\n';

  // Print Clients
  string temp;
  Client client;

  for (int i = 0; i < (*clients).getNumberClients(); i++)
  {
    client = (*clients).getClient(i); // Get Client

    if (fields[fieldId]) // Client Id
      cout << setw(nId) << setfill(' ') << client.id;

    if (fields[fieldName]) // Client Title
      if (client.name.length() < nName)
        cout << setw(nName) << setfill(' ') << client.name;
      else
        cout << client.name.substr(0, nName - 4) << "... ";

    if (fields[fieldAccountType]) // Client Account Type
      cout << setw(nAccountType) << setfill(' ') << accountStr[client.type];

    if (fields[fieldAccountStatus]) // Client Suspended Status
    {
      temp = (client.suspended) ? "Suspended" : "Active";
      cout << setw(nSuspended) << setfill(' ') << temp;
    }

    if (fields[fieldAccountNumber]) // Client Account Number
      cout << fixed << setprecision(0) << setw(nAccountNumber) << setfill(' ') << client.account;

    if (fields[fieldAccountNumber]) // Client Balance
      cout << fixed << setprecision(0) << setw(nBalance) << setfill(' ') << client.balance;

    cout << '\n';
  }
}