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

// --- Extern Variables and Constants Declaration
extern const string clear, tab1;
extern const bool applyBgColor, applyFgColor;
extern char cmdsChar[], searchCmdsChar[];

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

        if (command[1] == cmdsChar[cmdSearchData])
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

      if (command[1] == cmdsChar[cmdSearchData])
      { // Search Data Command
        cin >> subCommand;
        cin >> param;
      }
      ignoreInput();
    }

    switch (isCharOnArray(command[1], cmdsChar, cmdEnd))
    { // Get Index Position of Character on Command[1] in cmdsChar Array
    case cmdViewData:
      viewData();
      break;
    case cmdSearchData:
      searchData(subCommand, param, &isCommand);
      break;
    case cmdSearchParams:
      searchDataParameters();
      break;
    case cmdDepositMoney:
      depositMoney();
      break;
    case cmdCheckoutMoney:
      checkoutMoney();
      break;
    case cmdTransferMoney:
      transferMoney();
      break;
    case cmdExit:
      exit = true;
      break;
    case cmdSuspendAccount:
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
  cout << clear;
  printTitle("WELCOME TO THE BANK OF WALL STREET", applyBgColor, applyFgColor);
  cout << "Database Commands:\n"
       << tab1 << "[-" << cmdsChar[cmdViewData] << "] View Data\n"
       << tab1 << "[-" << cmdsChar[cmdSearchData] << "] Search Data\n"
       << tab1 << "[-" << cmdsChar[cmdSearchParams] << "] Show Search Data Parameters\n"
       << "Transaction Commands:\n"
       << tab1 << "[-" << cmdsChar[cmdDepositMoney] << "] Deposit Money\n"
       << tab1 << "[-" << cmdsChar[cmdCheckoutMoney] << "] Checkout Money\n"
       << tab1 << "[-" << cmdsChar[cmdTransferMoney] << "] Transfer Money\n"
       << "Other Commands:\n"
       << tab1 << "[-" << cmdsChar[cmdHelp] << "] Help\n"
       << tab1 << "[-" << cmdsChar[cmdExit] << "] Exit\n"
       << "Admin Privileges:\n"
       << tab1 << "[-" << cmdsChar[cmdSuspendAccount] << "] Suspend Accounts\n";
}