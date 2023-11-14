#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include "ansiEsc.h"
#include "data.h"
#include "input.h"
#include "namespaces.h"

using namespace std;
using namespace commands;
using namespace terminal;

// --- Valid Commands

// The Index of each Character is Related to the Enum Value of the Same Command or Subcommand
int cmdsChar[cmdEnd] = {'v', 'f', 'd', 'c', 't', 'F', 'S', 'x', 'y', 'h', 'e', 'a', 's'};
int subCmdsChar[subCmdEnd] = {'f', 's'};
int fieldCmdsChar[fieldEnd] = {'i', 'n', 'a', 't', 's', '.'};
int sortByCmdsChar[sortByEnd] = {'i', 'I', 'n', 'N', 'a', 'A', 't', 'T', 's', 'S'};

// Command Title
string fieldCmdsStr[fieldEnd] = {"Id", "Client Name", "Account Number", "Account Type", "Suspended Status"};

// --- Extern Variables and Constants Assignment
int *cmdsPtr = cmdsChar;
int *subCmdsPtr = subCmdsChar;
int *fieldCmdsCharPtr = fieldCmdsChar;
string *fieldCmdsStrPtr = fieldCmdsStr;
int *sortByCmdsPtr = sortByCmdsChar;

// --- Function Prototypes
int isCharOnArray(int character, int array[], int n);
void viewClients(Client clients[], int nClientsRead, bool fields[], int m, int sortBy[], int n);
void filterClients(Client clients[], int nMoviesRead, string **fieldParams, int l, int m, int sortBy[], int n);
void validParameters(int nCharTitle);
void fields();
void sortByParameters();
void printExamples(string cmds[], string explanations[], int n);
void howToUseViewClients();
void howToUseFilterClients();
void depositMoney();
void checkoutMoney();
void transferMoney();
void suspendAccount();
void printArray(string *params, int m, string paramTitle);
void print2DArray(string **params, int m, int n, string paramsTitle[]);
void printClients(Client clients[], int m, bool *fields, int n);
int getSortByStr(int sortBy[], string sortByStr[], int n);

// --- Functions

// Function to check if the Integer that Represents a Character is on Char Array
int isCharOnArray(int character, int array[], int n)
{ // Linear Search
  for (int i = 0; i < n; i++)
    if (array[i] == character)
      return i;
  return -1;
}

// TO DEVELOP

// Function to View Clients Stored in clients.csv
void viewClients(Client clients[], int nClientsRead, bool fields[], int m, int sortBy[], int n)
{
  string fieldsStr[m - 1], sortByStr[n], applied;

  if (fields[fieldAll])
    for (int i = 0; i < m - 1; i++)
      fields[i] = true;

  for (int i = 0; i < m - 1; i++)
  {
    applied = fields[i] ? "[Y] " : "[N] ";
    fieldsStr[i] = applied.append(fieldCmdsStr[i]); // Data to Print in the Field Parameters Row
  }

  m--;                                    // Get Field Array Length. We don't Count the '.' Command
  n = getSortByStr(sortBy, sortByStr, n); // Get Sort By Array Length

  cout << clear;
  printTitle("Clients Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Filter Clients
void filterClients(Client clients[], int nMoviesRead, string **fieldParams, int l, int m, int sortBy[], int n)
{
  string sortByStr[n];

  n = getSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Client Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(fieldParams, l, m, fieldCmdsStr);
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print the Two Types of Parameters (Used in Filter Clients Command)
void validParameters(int nCharTitle)
{
  string temp = "\"sentence to search\"";

  cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets<string>("word") << "Word to Search\n"
       << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets<string>(temp) << "Sentence to Search\n\n";
}

// Function to Print Field as a Parameter and as a Command
void fields()
{
  string temp;

  cout << clear; // Clear Terminal
  printTitle("Field as a Parameter (for View Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
    cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(fieldCmdsChar[i]) << fieldCmdsStr[i] << '\n';
  cout << '\n';

  printTitle("Field as a Command (for Filter Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
  {
    temp = addBrackets(fieldCmdsChar[i]).append(" [param...]");
    cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Client's " << fieldCmdsStr[i] << '\n';
  }
  cout << '\n';

  printTitle("Valid Field Parameters (for Filter Clients)", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Possible Parameters for Sort By Subcommand
void sortByParameters()
{
  string ascending, descending;

  cout << clear; // Clear Terminal
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  for (int i = 0; i < sortByEnd / 2; i++)
  {
    ascending = addBrackets(sortByCmdsChar[i * 2]).append(" Ascending");
    descending = addBrackets(sortByCmdsChar[i * 2 + 1]).append(" Descending");

    cout << tab1 << setw(nCharTitle) << setfill(' ') << fieldCmdsStr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';

  pressEnterToCont("Press ENTER to Continue", false);
}

// Print Usage Examples of the View Clients Command
void howToUseViewClients()
{
  const int nCmds = 3; // Number of Code Examples
  string cmds[nCmds] = {"v -f . -s i",
                        "v -f i n -s I", "v -f i t -s S"};

  string explanations[nCmds] = {"View All Client Fields. Sort them by Id in Ascending Order",
                                "View Id and Client Name Field. Sort them by Id in Descending Order", "View Id and Account Type Fields. Sort them by Suspended Status in Descending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Print Usage Examples of the Filter Clients Command
void howToUseFilterClients()
{
  const int nCmds = 4; // Number of Code Examples
  string cmds[nCmds] = {"f -f --n Ramon Ronald -s i",
                        "f -f --n \"Ramon Alvarez\" \"Ronald Lopez\" -s I",
                        "f -f --n Ronald --i 123456789 --t debit -s a",
                        "f -f --n Ramon --a 100200300 -s N"};

  string explanations[nCmds] = {
      "Search for Clients Named as Ramon or Ronald. Sort them by Id in Ascending Order",
      "Search for Clients Named as Ramon Alvarez or Ronald Lopez. Sort them by Id in Descending Order",
      "Search for Clients Named as Ronald, the Client with Id 123456789 and Debit Accounts. Sort them by Account Number",
      "Search for Clients Named as Ramon and the Client with Account Number 100200300. Sort them by Name in Descending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// To Add

void depositMoney()
{
  cout << "d";
}

void checkoutMoney()
{
  cout << "c";
}

void transferMoney()
{
  cout << "t";
}

void suspendAccount()
{
  cout << "S";
}

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
    message = ""; // Clear String

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

// Function to Print Client Info
void printClientInfo(Client client)
{
  int nCharContent = nChar - nCharField;

  printTitle("Client Info", applyBgColor, applyFgColor, false);

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldId] << client.id << '\n'; // Print Client Id

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldName]; // Print Client Name
  if (client.name.length() < nCharContent)
    cout << setw(nCharContent) << setfill(' ') << client.name << '\n';
  else
    cout << client.name.substr(0, nCharContent - 4) << "... " << '\n';

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountNumber] << client.account << '\n'; // Print Client Account Number
}

// Function to Print a 1D Array
void printArray(string *params, int n, string paramTitle)
{
  string param;

  cout << setw(nCharTitle) << setfill(' ') << left << paramTitle;
  for (int i = 0; i < n && params[i].length() != 0; i++)
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
    if (params[i][0].length() != 0)
      printArray(params[i], n, paramsTitle[i]);
}

// Function to Print Clients
void printClients(Client clients[], int m, bool *fields, int n)
{
  const int nId = 5;             // Number of Characters for Id
  const int nAccountNumber = 15; // ... for Duration
  int nName = nChar;             // Decrease the Number of Characters Used by the Name Field

  // Number of Characters per Field
  int fieldsNChar[fieldEnd - 1] = {nId, 0, nAccountNumber};

  for (int i = 0; i < n; i++)
    if (fields[i] && i != fieldName)
      nName -= fieldsNChar[i]; // Decrease Number of Characters for Movie's Name FIeld

  fieldsNChar[fieldName] = nName; // Assign Number of Characters for Movie's Name

  cout << clear << sgrBgCmd << sgrFgCmd;
  for (int i = 0; i < fieldEnd - 1; i++)
    if (fields[i])
      cout << setw(fieldsNChar[i]) << setfill(' ') << fieldCmdsStr[i]; // Field Title
  cout << reset << '\n';

  // Print Clients
  int *date;
  string title, director;

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

    // Client Account Number
    if (fields[fieldAccountNumber])
      cout << setw(nAccountNumber) << setfill(' ') << clients[i].account;

    cout << '\n';
  }
}

// Function to Get a String Array from a Int Array of the Sort By Commands that will be Applied to the Clients
int getSortByStr(int sortBy[], string sortByStr[], int n)
{
  bool nullParam;
  int charIndex, nParams = 0;
  string order;

  for (int j = 0; j < n; j++)
  {
    charIndex = sortBy[j];
    nullParam = false;

    if (charIndex == -1)
      nullParam = true;
    else if (isupper(sortByCmdsChar[charIndex]))
      order = "[D] ";
    else
      order = "[A] ";

    if (!nullParam)
    {
      sortByStr[nParams] = order.append(fieldCmdsStr[charIndex / 2]); // Data to Print in the Sort By Parameters Row
      nParams++;
    }
  }
  return nParams;
}