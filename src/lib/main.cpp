#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
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
extern char *cmdsPtr, *searchCmdsPtr;

// --- Functions
void helpMessage();

int main(int argc, char **argv)
{
  const int maxParamPerSubCmd = 10; // Max Number of Parameters per Subcommand

  bool exit = false;                      // Tells the Program if the User wants to Exit the Program
  char command, subCommand;               // Command, SubCommand
  int isCommand = validCmd;               // Used for Checking if the Command is Valid or Not. If not, it Stores the Reason
  int commandIndex, subCommandIndex;      // Used to Save the Index of the Command on its Corresponding Array
  int paramsCounter[searchEnd];           // Search Parameters Counter
  int commandsExecuted = 0;               // Number of times the Main While Loop has been executed
  string inputLine, inputWord, inputLong; // Saves the Input of the User, before being Processed by the Program

  // Main While Loop of the Program
  while (!exit)
  {
    string params[searchEnd][maxParamPerSubCmd]; // 2D String Array of Parameters
    string *pointerParams[searchEnd];            // 1D Pointer Array to to the 2D Array

    for (int i = 0; i < searchEnd; i++)
      pointerParams[i] = params[i];

    for (int i = 0; i < searchEnd; i++)
      paramsCounter[i] = 0; // Reset Parameter Counters to 0

    if (isCommand != validCmd)
    { // If in the Last Execution the User Typed a Wrong Command
      wrongCommand(isCommand);
      if (isCommand == missingSearchParams || isCommand == wrongSearchCmd)
        searchDataParameters(); // Print the Valid Search Data Parameters
      isCommand = validCmd;
    }

    if (commandsExecuted == 0 && argc > 1)
    { // Checks if it's a Command
      for (int i = 1; i < argc; i++)
      {
        inputWord = argv[i];

        size_t found = inputWord.find(' '); // Check if the string contains Whitespaces
        if (found != string::npos)
        {                                               // Whitespace Found
          inputWord.insert(0, 1, '"');                  // Insert a Double Quote to the Beggining of the Input
          inputWord.insert(inputWord.length(), 1, '"'); // Insert a Double Quote at the End
        }

        if (i != argc - 1) // Insert Whitespace between Parameters
          inputWord.insert(inputWord.length(), 1, ' ');

        inputLine.append(inputWord); // Append the Parameter
      }
    }
    else
    {
      helpMessage();
      cout << '\n';
      printTitle("ENTER A COMMAND", applyBgColor, applyFgColor, false);

      getline(cin, inputLine); // Get User Input
    }

    stringstream stream(inputLine); // Stream the Input
    commandsExecuted++;             // Increase the Counter of Commands Executed

    if (!getline(stream, inputWord, ' '))
    { // Couldn't Get the Command
      isCommand = noCmd;
      continue;
    }

    command = inputWord[0];
    commandIndex = isCharOnArray(command, cmdsPtr, cmdEnd); // Check if the Command is on the Array of Main Commands. Returns -1 if it doesn't exist

    if (commandIndex == -1)
    { // If it's not a Valid Command
      isCommand = wrongMainCmd;
    }
    else if (commandIndex == cmdSearchData)
    {                           // Checks if the Search Command is Typed Correctly
      bool isSubcommand = true; // Boolean to Check if there's More Input

      getline(stream, inputWord, ' '); // Get Subcommand

      while (isSubcommand)
      {
        isSubcommand = false;

        if (inputWord.length() < 3)
        { // Check if the Command has the Minimum String Length, which is 3
          isCommand = wrongSearchCmd;
          break; // Break the for-loop
        }

        subCommand = inputWord[2];
        subCommandIndex = isCharOnArray(subCommand, searchCmdsPtr, searchEnd); // Check if the Command is on the Array of subCommands

        if (inputWord[0] != '-' || subCommandIndex == -1)
        { // Wrong Search Subcommand
          isCommand = wrongSearchCmd;
          break;
        }

        int paramCounter = paramsCounter[subCommandIndex]; // Counter
        while (paramCounter < maxParamPerSubCmd && getline(stream, inputWord, ' '))
        { // Iterate while there's a Parameter and the Parameters Typed for that Subcommand are Less than maxParamPerSubCmds
          if (inputWord[0] == '"')
          { // If it Starts with Double Quote, it's a Long Sentence (a Parameter with Multiple Words)
            if (!getline(stream, inputLong, '"'))
            {
              isCommand = wrongSearchCmd; // Incomplete Long Parameter
              break;
            }
            inputWord.insert(inputWord.length(), 1, ' ');                 // Insert Whitespace at the End
            inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Insert the Parameter at the Beggining of the String, without the Double Quote

            params[subCommandIndex][paramCounter] = inputLong; // Add Sentence Search Data Parameter
          }
          else
          {                              // Get Word Input
            if (inputWord.length() == 0) // To Prevent Adding Whitespaces as Parameters
              continue;

            if (inputWord[0] == '-')
            {
              isSubcommand = true;
              break; // If it's a Command break this For-loop
            }

            params[subCommandIndex][paramCounter] = inputWord; // Add Word Search Data Parameter
          }
          paramCounter++; // Parameter Counter
        }
        paramsCounter[subCommandIndex] = paramCounter; // Update Parameter Counter

        if (isCommand == wrongSearchCmd)
          break; // Exit this While-loop

        while (!isSubcommand && getline(stream, inputWord, ' '))
        {
          if (inputWord[0] == '-')
            isSubcommand = true; // First Parameter Must be a Command
        }
      }
    }

    if (isCommand != validCmd)
    { // Chekc if the Command is Valid
      if (commandsExecuted > 1 || (commandsExecuted == 0 && argc == 1))
        cout << '\n'; // Print a New Line
      continue;
    }

    switch (isCharOnArray(command, cmdsPtr, cmdEnd))
    { // Get Index Position of Character on Command[1] in cmdsChar Array
    case cmdViewData:
      viewData();
      break;
    case cmdSearchData:
      searchData(pointerParams, searchEnd, maxParamPerSubCmd);
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
  printTitle("WELCOME TO THE BANK OF WALL STREET", applyBgColor, applyFgColor, false);
  cout << "Database Commands:\n"
       << tab1 << "[" << cmdsPtr[cmdViewData] << "] View Data\n"
       << tab1 << "[" << cmdsPtr[cmdSearchData] << "] Search Data\n"
       << tab1 << "[" << cmdsPtr[cmdSearchParams] << "] Valid Search Data Subcommands and Parameters\n"
       << "Transaction Commands:\n"
       << tab1 << "[" << cmdsPtr[cmdDepositMoney] << "] Deposit Money\n"
       << tab1 << "[" << cmdsPtr[cmdCheckoutMoney] << "] Checkout Money\n"
       << tab1 << "[" << cmdsPtr[cmdTransferMoney] << "] Transfer Money\n"
       << "Other Commands:\n"
       << tab1 << "[" << cmdsPtr[cmdHelp] << "] Help\n"
       << tab1 << "[" << cmdsPtr[cmdExit] << "] Exit\n"
       << "Admin Privileges:\n"
       << tab1 << "[" << cmdsPtr[cmdSuspendAccount] << "] Suspend Accounts\n";
}