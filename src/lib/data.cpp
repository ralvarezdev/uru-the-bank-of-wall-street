#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "clients.h"
#include "ansiEsc.h"
#include "data.h"
#include "input.h"
#include "namespaces.h"

using namespace std;
using namespace files;
using namespace commands;
using namespace terminal;

// --- Valid Commands

// The Index of each Character is Related to the Enum Value of the Same Command or Subcommand
int cmdsChar[cmdEnd] = {'v', 'f', 'd', 'c', 't', 'F', 'S', 'x', 'y', 'h', 'e', 'a', 's'};
int subCmdsChar[subCmdEnd] = {'f', 's'};
int fieldCmdsChar[fieldEnd] = {'i', 'n', 't', 's', 'a', '.'};
int sortByCmdsChar[sortByEnd] = {'i', 'I', 'n', 'N', 't', 'T', 's', 'S', 'a', 'A'};

// Command Title
string accountStr[accountEnd] = {"current", "debit"};
string fieldCmdsStr[fieldEnd] = {"Id", "Client Name", "Account Type", "Account Status", "Account Number"};

// --- Extern Variables and Constants Assignment
int *cmdsPtr = cmdsChar;
int *subCmdsPtr = subCmdsChar;
int *fieldCmdsCharPtr = fieldCmdsChar;
string *accountPtr = accountStr;
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
void changeStatus(Client clients[], int nClientsRead);
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

// Function to Add Clients to clients.csv
void addClients(Client clients[], int *nClientsRead)
{
  while (true)
  {
    cout << clear;
    printTitle("Add Client", applyBgColor, applyFgColor, false);

    cout << '\n';
    *nClientsRead = addClientToFile(clients, *nClientsRead);

    if (!booleanQuestion("Do you want to Add more Clients?"))
      break;
  }
}

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

  sortClients(clients, nClientsRead, sortBy, sortByEnd); // Sort Clients
  printClients(clients, nClientsRead, fields, m);        // Print Clients

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

// Function to Change the Status of a Client
void changeStatus(Client clients[], int nClientsRead)
{
  string temp;
  int id, index, clientStatus;
  string message;

  cout << clear;
  printTitle("Change Account Status", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  cout << '\n';

  while (true)
    try // Get Client ID
    {
      cout << "Client ID to Change Status: ";
      getline(cin, temp);
      id = stoi(temp);
      break;
    }
    catch (...)
    {
      wrongClientData(invalidClientId);
    }

  clientStatus = checkClient(clients, nClientsRead, id, fieldId, &index); // Check if the Clients Exists

  if (clientStatus != clientFound)
    message = "Error 404: Client Not Found. Run \"Add Client Command\"";
  else if (clientStatus != invalidArgument)
  {
    bool suspend = booleanQuestion("Do you want to Suspend a Client?"); // Ask wether to Suspend or Active Account
    cout << '\n';

    if (clients[index].suspended == suspend)
      message = "Client Found: Nothing to Change";
    else
    {
      message = "Client Found: Changed Status";

      clients[index].suspended = suspend; // Change Status of Client

      ifstream infile(clientsFilename);
      ofstream outfile(clientsFilename);

      string header;
      getline(infile, header); // Get Header line from clients.csv

      outfile << header << '\n'; // Write Header

      string accountType, suspended;

      for (int i = 0; i < nClientsRead; i++) // Write to File
      {
        accountType = (clients[i].type) ? "debit" : "current"; // Get Account Type
        suspended = (clients[i].suspended) ? "true" : "false"; // Get Account Status

        outfile << clients[i].id << sep << clients[i].name << sep
                << setw(10) << setfill('0') << right << fixed << setprecision(0) << clients[i].account << left
                << sep << accountType << sep << suspended << '\n';
      }
    }
    printClientInfo(clients[index]);
  }
  pressEnterToCont(message, (clientStatus == clientFound) ? false : true);
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
  string accountType = accountStr[client.type];                   // Get Client Type
  string suspended = (client.suspended) ? "Suspended" : "Active"; // Get Client Status

  printTitle("Client Info", applyBgColor, applyFgColor, false);

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldId] << client.id << '\n'; // Print Client Id

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldName]; // Print Client Name
  if (client.name.length() < nCharContent)
    cout << setw(nCharContent) << setfill(' ') << client.name << '\n';
  else
    cout << client.name.substr(0, nCharContent - 4) << "... " << '\n';

  cout << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountNumber] << client.account << '\n' // Print Client Account Number
       << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldAccountType] << accountType << '\n'      // Print Client Account Type
       << setw(nCharField) << setfill(' ') << fieldCmdsStr[fieldSuspended] << suspended << '\n';         // Print Client Suspend Status
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
  const int nId = 15;            // Number of Characters for Id
  const int nAccountType = 15;   // ... for Account Type
  const int nSuspended = 18;     // ... for Suspended Status
  const int nAccountNumber = 20; // ... for Account Number

  int nName = nChar; // Decrease the Number of Characters Used by the Name Field

  // Number of Characters per Field
  int fieldsNChar[fieldEnd - 1] = {nId, 0, nAccountType, nSuspended, nAccountNumber};

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
      cout << setw(nAccountNumber) << setfill(' ') << fixed
           << setprecision(0) << clients[i].account;

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