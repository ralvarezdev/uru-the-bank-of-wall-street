#include <string>
#include <iostream>
#include "data.h"
#include "input.h"
#include "ansiEsc.h"

using namespace std;

// --- Extern Variables Definition
extern char cmdsChar[cmdEnd] = {'v', 's', 'p', 'd', 'c', 't', 'h', 'e', 'S'};
extern char searchCmdsChar[searchEnd] = {'i', 'n', 'a'};

// --- Extern Variables and Constants Declaration
extern const string clear, tab1;
extern const bool applyBgColor, applyFgColor;

// TO DEVELOP

// Function to check if the Character is on Char Array
int isCharOnArray(char character, char array[], int n)
{
  for (int i = 0; i < n; i++)
  {
    if (array[i] == character)
      return i;
  }
  return -1;
}

void viewData()
{
  cout << "v";
}

void searchData(string dataType, string dataToSearch, bool *isCommand)
{
  cout << "s\n"
       << dataType << '\n'
       << dataToSearch;
}

// Show Search Command Possible Parameters
void searchDataParameters()
{
  cout << clear; // Clear Terminal
  printTitle("Search Command Possible Parameters", applyBgColor, applyFgColor);
  cout << tab1 << "[--" << searchCmdsChar[searchClientId] << "] Client ID\n"
       << tab1 << "[--" << searchCmdsChar[searchClientName] << "] Client Name\n"
       << tab1 << "[--" << searchCmdsChar[searchAccountNumber] << "] Account Number\n";
  pressEnterToContinue();
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