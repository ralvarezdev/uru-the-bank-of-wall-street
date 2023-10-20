#include <iostream>
#include <string>
#include "ansiEsc.h"
#include "data.h"
#include "input.h"

using namespace std;

/*
--- NOTES
- For this proyect we're not allowed to use maps, vectors, classes, linked lists, tuples.
That's the reason why the program is written like this
*/

// --- Global variables
string tab1 = string(3, ' '); // String with 3 whitespaces
string tab2 = string(8, ' '); // String with 8 whitespaces
bool applyBgColor = true;     // Add Customed Background Color
bool applyFgColor = true;     // Add Customed Foreground Color

// --- Functions
void helpMessage();

int main(int argc, char **argv)
{
  bool exit = false, isCommand = true;
  int commandsExecuted = 0;          // Number of times the Main While Loop has been executed
  string command, subCommand, param; // Command, SubCommand and Parameter

  while (!exit)
  {
    if (isCommand == false)
    { // If in the last execution the user entered a wrong command
      wrongCommand();
      isCommand = true;
    }

    if (commandsExecuted == 0 && argc > 1)
    {
      // Checks if it's a Command
      if (argv[1][0] != '-')
      {
        isCommand = false;
        commandsExecuted++;
        continue;
      }
      else
      {
        command = argv[1]; // Get Command

        if (command[1] == 's')
        { // Search Data Command
          subCommand = argv[2];
          param = argv[3];
        }
      }
    }
    else
    {
      helpMessage();
      printTitle("ENTER A COMMAND", applyBgColor, applyFgColor);

      // Get User Input
      cin >> command;

      if (command[0] != '-')
        isCommand = false; // Wrong Command

      if (command[1] == 's')
      { // Search Data Command
        cin >> subCommand;
        cin >> param;
      }
      ignoreInput();
    }

    switch (command[1])
    {
    case 'v':
      viewData();
      break;
    case 's':
      searchData(subCommand, param, &isCommand);
      break;
    case 'd':
      depositMoney();
      break;
    case 'c':
      checkoutMoney();
      break;
    case 't':
      transferMoney();
      break;
    case 'e':
      exit = true;
      break;
    case 'S':
      suspendAccount();
      break;
    default: // Help Message
      commandsExecuted++;
    }
  }
}

// Function to Output Help Message in the Terminal
void helpMessage()
{
  // Filter Commands Possible Parameters
  string filterCommands[3] = {"[--i] Client ID", "[--n] Client Name", "[--a] Account Number"};

  cout << ANSI_CLEAR;
  printTitle("WELCOME TO THE BANK OF WALL STREET", applyBgColor, applyFgColor);
  cout << "Avalaible Commands:\n"
       << tab1 << "[-h] Help\n"
       << tab1 << "[-v] View Data\n"
       << tab1 << "[-s] Search Data\n";
  for (int i = 0; i < sizeof(filterCommands) / sizeof(string); i++)
  {
    cout << tab2 << filterCommands[i] << '\n';
  }
  cout << tab1 << "[-d] Deposit Money\n"
       << tab1 << "[-c] Checkout Money\n"
       << tab1 << "[-t] Transfer Money\n"
       << tab1 << "[-e] Exit\n"
       << "Admin Privileges:\n"
       << tab1 << "[-S] Suspend Accounts\n";
}