#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include "data.h"
#include "input.h"
#include "ansiEsc.h"

using namespace std;

// --- Valid Commands

// The Index of each Character is Related to the Enum Value of the Same Command or Subcommand
char cmdsChar[cmdEnd] = {'v', 's', 'p', 'u', 'd', 'c', 't', 'h', 'e', 'S'};
char searchCmdsChar[searchEnd] = {'i', 'n', 'a'};
string paramsTitle[searchEnd] = {"Id", "Client Name", "Account Number"};

// --- Extern Variables and Constants Assignment
char *cmdsPtr = cmdsChar;
char *searchCmdsPtr = searchCmdsChar;

// --- Function Prototypes
int isCharOnArray(char character, char array[], int n);
void viewData();
void searchData(string **params, int m, int n);
void searchDataParameters();
void howToUseSearchData();
void depositMoney();
void checkoutMoney();
void transferMoney();
void suspendAccount();
void printTable(string **params, int m, int n, string title, int nCharTitle);

// --- Functions

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
  cout << clear; // Clear Terminal

  // Valid Subcommands for Search Command
  printTitle("Valid Subcommands for Search Command", applyBgColor, applyFgColor, false);
  cout << tab1 << "[--" << searchCmdsChar[searchClientId] << "] Client ID\n"
       << tab1 << "[--" << searchCmdsChar[searchClientName] << "] Client Name\n"
       << tab1 << "[--" << searchCmdsChar[searchAccountNumber] << "] Account Number\n\n";

  // Valid Parameters for Search Command
  printTitle("Valid Parameters", applyBgColor, applyFgColor, false);
  cout << tab1 << "[word] Word to Search on Data\n"
       << tab1 << '[' << '"' << "sentence to search" << '"' << "] Sentence to Search on Data\n\n";

  pressEnterToContinue("Press ENTER to Continue", false);
}

// Print Usage Examples of the Search Data Command
void howToUseSearchData()
{
  const int nCmds = 3;                          // Number of Code Examples
  const int nCharMessage = nWp - tab1.length(); // Number of Characters for Each Line
  string feCmds[nCmds] = {"s --c Ramon Ronald",
                          "s --c `Ramon Alvarez` `Ronald Lopez`",
                          "s --c Ramon --i 123456789 --n 100200300"};

  string feExplanations[nCmds] = {"Search for Ramon and/or Ronald Client",
                                  "Search for Ramon Alvarez and/or Ronald Lopez Client",
                                  "Search for Ramon and/or Ronald Client, the Client whose Id is 123456789 and the Client whose Account Number is 100200300"};

  string message, lastIter; // Temp Data

  cout << clear;                                             // Clear Terminal
  printTitle("Examples", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  for (int i = 0; i < nCmds; i++)
  {
    for (int j = 0; j < feCmds[i].length(); j++)
    {
      message = feCmds[i].substr(j, nCharMessage + j);
      replace(message.begin(), message.end(), '`', '"'); // Replace Backsticks with Double Quotes
      cout << tab1 << message << '\n';
      j += message.length();
    }
    message = ""; // Clear String

    cout << tab1 << string(nCharMessage, '-') << '\n';

    stringstream stream(feExplanations[i]);
    while (getline(stream, lastIter, ' ')) // To Print the Message with New Line each time it Reaches nCharMessage
      if (message.length() + lastIter.length() + 1 < nCharMessage)
      {
        if (message.length() != 0)
          message.append(" ").append(lastIter);
        else
          message.append(lastIter);
      }
      else
      {
        cout << tab1 << message << '\n';
        message = lastIter;
      }
    cout << tab1 << message << "\n\n";
  }
  pressEnterToContinue("Press ENTER to Continue", false);
}

// TO DEVELOP

void viewData()
{
  cout << "v";
}

// Function to Filter Search Data. M is the number of Parameters. N is the number of Elements per Parameter
void searchData(string **params, int m, int n)
{
  const int nCharTitle = 20; // Number of Maximum Characters per Title
  string param;              // Temp String

  cout << clear;
  printTable(params, m, n, "Parameters", nCharTitle);
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

// Function to Print a 2D Data Table
void printTable(string **params, int m, int n, string title, int nCharTitle)
{
  const int paramPerLine = 3;                               // Number of Parameters Printed by Line
  const int nCharParam = (nWp - nCharTitle) / paramPerLine; // Number of Maximum Characters per Parameter
  const int maxSpacing = 7;                                 // If the Maximum Number Characters is Reached this is the Spacing between Paratemeters
  string param;

  printTitle(title, applyBgColor, applyFgColor, false);
  for (int i = 0; i < m; i++)
  {
    cout << setw(nCharTitle) << setfill(' ') << left << paramsTitle[i];
    for (int j = 0; j < n && params[i][j].length() != 0; j++)
    {
      param = params[i][j];

      if (j % paramPerLine == 0 && j != 0)
        cout << '\n' // Break the Line, and Add Some Indentation if there's more than 3 Parameters on the Current Line
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
    cout << '\n';
  }
}