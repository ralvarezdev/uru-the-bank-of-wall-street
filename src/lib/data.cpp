#include <string>
#include <iostream>
#include <iomanip>
#include "data.h"
#include "input.h"
#include "ansiEsc.h"

using namespace std;

// --- Valid Commands

// The Index of each Character is Related to the Enum Value of the Same Command or Subcommand
char cmdsChar[cmdEnd] = {'v', 's', 'p', 'd', 'c', 't', 'h', 'e', 'S'};
char searchCmdsChar[searchEnd] = {'i', 'n', 'a'};
string paramsTitle[searchEnd] = {"Id", "Client Name", "Account Number"};

// --- Extern Variables and Constants Assignment
char *cmdsPtr = cmdsChar;
char *searchCmdsPtr = searchCmdsChar;

// Function to check if the Character is on Char Array
int isCharOnArray(char character, char array[], int n)
{ // Linear Search
  for (int i = 0; i < n; i++)
  {
    if (array[i] == character)
      return i;
  }
  return -1;
}

/*
// Function to Get the Length of a Char Array with its Pointer
int getLengthCharArrayPointer(char *array)
{
  string arrayStr = array; // Converts Char Array to String
  return arrayStr.length();
}
*/

// Show Search Command Possible Parameters
void searchDataParameters()
{
  bool warning = false;

  cout << clear; // Clear Terminal

  // Valid Subcommands for Search Command
  printTitle("Valid Subcommands for Search Command", applyBgColor, applyFgColor, warning);
  cout << tab1 << "[--" << searchCmdsChar[searchClientId] << "] Client ID\n"
       << tab1 << "[--" << searchCmdsChar[searchClientName] << "] Client Name\n"
       << tab1 << "[--" << searchCmdsChar[searchAccountNumber] << "] Account Number\n\n";

  // Valid Parameters for Search Command
  printTitle("Valid Parameters", applyBgColor, applyFgColor, warning);
  cout << tab1 << "[word] Word to Search on Data\n"
       << tab1 << '[' << '"' << "sentence to search" << '"' << "] Sentence to Search on Data\n\n";

  pressEnterToContinue("Press ENTER to Continue", warning);
}

// TO DEVELOP

void viewData()
{
  cout << "v";
}

// Function to Filter Search Data. M is the number of Parameters. N is the number of Elements per Parameter
void searchData(string **params, int m, int n)
{
  const int paramPerLine = 3;                               // Number of Parameters Printed by Line
  const int nCharTitle = 20;                                // Number of Maximum Characters per Title
  const int nCharParam = (nWp - nCharTitle) / paramPerLine; // Number of Maximum Characters per Parameter
  const int maxSpacing = 7;                                 // If the Maximum Number Characters is Reached this is the Spacing between Paratemeters
  string param;                                             // Temp String

  // Show Parameters
  cout << clear << '\n';
  printTitle("Parameters", applyBgColor, applyFgColor, false);
  for (int i = 0; i < m; i++)
  {
    cout << setw(nCharTitle) << setfill(' ') << left << paramsTitle[i];
    for (int j = 0; j < n && params[i][j].length() != 0; j++)
    {
      param = params[i][j];

      if (j % paramPerLine == 0 && j != 0)
        cout << '\n' // Break the Line, and Add Some Indentation if there's more than 3 Parameters on the Current Line
             << string(nCharTitle, ' ');

      if (param.length() < nCharParam - maxSpacing)
      {
        cout << setw(nCharParam) << setfill(' ') << left << param;
        continue;
      }

      param = param.substr(0, nCharParam - maxSpacing); // Substract from String
      param.append("...");                              // Append to the String
      cout << setw(nCharParam) << setfill(' ') << left << param;
    }
    cout << '\n';
  }

  cout << '\n';
  pressEnterToContinue("Press ENTER to Continue", false);
}

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