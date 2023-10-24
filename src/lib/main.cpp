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
  bool exit = false;
  int commandsExecuted = 0;          // Number of times the Main While Loop has been executed
  int isCommand = validCmd;          // Variable to Use for Checking if the Command is Valid or Not. If not, it Stores the Reason
  string command, subCommand, param; // Command, SubCommand and Parameter

  while (!exit)
  {
    if (isCommand != validCmd)
    { // If in the last execution the user entered a wrong command
      wrongCommand(isCommand);
      if (isCommand == missingSearchParams || isCommand == wrongSearchCmd)
        searchDataParameters(); // Print the Valid Search Data Parameters
      isCommand = validCmd;
    }

    if (commandsExecuted == 0 && argc > 1)
    { // Checks if it's a Command
      commandsExecuted++;

      if (argv[1][0] != '-')
      {
        isCommand = notACmd;
        continue; // Execute the while-loop again
      }
      else
      {
        int commandIndex, subCommandIndex;

        command = argv[1];
        commandIndex = isCharOnArray(command[1], cmdsChar, cmdEnd); // Check if the Command is on the Array of Main Commands. Returns -1 if it doesn't exist

        if (commandIndex == -1 || (commandIndex == cmdSearchData && argc <= 2))
        {
          isCommand = (commandIndex == -1) ? notACmd : missingSearchParams;
          continue;
        }
        else if (commandIndex == cmdSearchData)
        { // Checks if it's a Search Command
          subCommand = argv[2];
          subCommandIndex = isCharOnArray(subCommand[2], searchCmdsChar, searchEnd);

          if (subCommand[0] == '-' && subCommandIndex != -1 && argc >= 4)
          {                  // Check if it's a Subcommand. If so, Checks if it's Valid
            param = argv[3]; // Add Search Data Parameter
          }
          else
          {
            isCommand = wrongSearchCmd;
            continue;
          }
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
        isCommand = notACmd; // Wrong Command

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
      searchData(subCommand, param);
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