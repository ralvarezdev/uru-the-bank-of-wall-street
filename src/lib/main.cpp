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

// --- Function Prototypes
void helpMessage();

int main(int argc, char **argv)
{
  const int maxParamPerSubCmd = 9; // Max Number of Parameters per Subcommand

  bool exit = false;                      // Tells the Program if the User wants to Exit the Program
  char cmd, subCmd;                       // Command, SubCommand
  int isCmd = validCmd;                   // Used for Checking if the Command is Valid or Not. If not, it Stores the Reason
  int cmdIndex, subCmdIndex;              // Used to Save the Index of the Command on its Corresponding Array
  int paramsCounter[searchEnd];           // Search Parameters Counter
  int timesExec = 0;                      // Number of times the Main While Loop has been executed
  string inputLine, inputWord, inputLong; // Saves the Input of the User, before being Processed by the Program

  // Main While Loop of the Program
  while (!exit)
  {
    string params[searchEnd][maxParamPerSubCmd]; // 2D String Array of Parameters
    string *ptrParams[searchEnd];                // 1D Pointer Array to to the 2D Array

    for (int i = 0; i < searchEnd; i++)
    {
      ptrParams[i] = params[i];
      paramsCounter[i] = 0; // Reset Parameter Counters to 0
    }

    if (isCmd != validCmd)
    { // If in the Last Execution the User Typed a Wrong Command
      wrongCommand(isCmd);
      if (isCmd == wrongSearchDataCmd)
      {
        searchDataParameters(); // Print the Valid Search Data Parameters
        howToUseSearchData();   // Print the How-To-Use guide of the Search Data Command
      }
      isCmd = validCmd;
    }

    if (timesExec == 0 && argc > 1)
    { // Checks if it's a Command
      for (int i = 1; i < argc; i++)
      {
        inputWord = argv[i];

        size_t found = inputWord.find(' '); // Check if the string contains Whitespaces
        if (found != string::npos)
        {                                               // Whitespace Found
          inputWord.insert(0, 1, '"');                  // Insert a Double Quote at the Beginning of the Input
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

      string temp;                    // Temporary String
      stringstream stream(inputLine); // Stream the Input to Remove Empty Input (that only has Whitespaces)

      while (getline(stream, inputWord, ' '))
      {
        if (temp.length() != 0 && inputWord.length() > 0)
          inputWord.insert(0, 1, ' '); // Insert a Whitespace at the Beginning of the Input

        temp.append(inputWord); // Append Input
      }
      inputLine = temp;
    }

    stringstream stream(inputLine); // Stream the Input
    timesExec++;                    // Increase the Counter of Commands Executed

    if (!getline(stream, inputWord, ' '))
    { // Couldn't Get the Command
      isCmd = noCmd;
      continue;
    }

    cmd = inputWord[0];
    cmdIndex = isCharOnArray(cmd, cmdsPtr, cmdEnd); // Check if the Command is on the Array of Main Commands. Returns -1 if it doesn't exist

    if (cmdIndex == -1) // If it's not a Valid Command
      isCmd = wrongMainCmd;
    else if (cmdIndex == cmdSearchData)
    {                           // Checks if the Search Command is Typed Correctly
      bool isSubcommand = true; // Boolean to Check if there's More Input

      getline(stream, inputWord, ' '); // Get Subcommand

      while (isSubcommand)
      {
        isSubcommand = false;

        if (inputWord.length() < 3)
        { // Check if the Command has the Minimum String Length, which is 3
          isCmd = wrongSearchDataCmd;
          break; // Break the for-loop
        }

        subCmd = inputWord[2];
        subCmdIndex = isCharOnArray(subCmd, searchCmdsPtr, searchEnd); // Check if the Command is on the Array of subCommands

        if (inputWord[0] != '-' || subCmdIndex == -1)
        { // Wrong Search Subcommand
          isCmd = wrongSearchDataCmd;
          break;
        }

        int paramCounter = paramsCounter[subCmdIndex]; // Counter
        while (paramCounter < maxParamPerSubCmd && getline(stream, inputWord, ' '))
        { // Iterate while there's a Parameter and the Parameters Typed for that Subcommand are Less than maxParamPerSubCmds
          if (inputWord[0] == '"')
          { // If it Starts with Double Quote, it's a Long Sentence (a Parameter with Multiple Words)
            if (!getline(stream, inputLong, '"'))
            {
              isCmd = wrongSearchDataCmd; // Incomplete Long Parameter
              break;
            }
            inputWord.insert(inputWord.length(), 1, ' ');                 // Insert Whitespace at the End
            inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Insert the Parameter at the Beginning of the String, without the Double Quote

            params[subCmdIndex][paramCounter] = inputLong; // Add Sentence Search Data Parameter
          }
          else if (inputWord[0] != '-') // Get Word Input
          {
            params[subCmdIndex][paramCounter] = inputWord; // Add Word Search Data Parameter
          }
          else
          {
            isSubcommand = true;
            break; // If it's a Command break this For-loop
          }
          paramCounter++; // Parameter Counter
        }
        paramsCounter[subCmdIndex] = paramCounter; // Update Parameter Counter

        if (isCmd == wrongSearchDataCmd)
          break; // Exit this While-loop

        while (!isSubcommand && getline(stream, inputWord, ' '))
          if (inputWord[0] == '-')
            isSubcommand = true; // First Parameter Must be a Command
      }
    }

    if (isCmd != validCmd)
    { // Chekc if the Command is Valid
      if (timesExec > 1 || (timesExec == 0 && argc == 1))
        cout << '\n'; // Print a New Line
      continue;
    }

    switch (isCharOnArray(cmd, cmdsPtr, cmdEnd))
    { // Get Index Position of Character on Command[1] in cmdsChar Array
    case cmdViewData:
      viewData();
      break;
    case cmdSearchData:
      searchData(ptrParams, searchEnd, maxParamPerSubCmd);
      break;
    case cmdSearchParams:
      searchDataParameters();
      break;
    case cmdHowToUseSearchData:
      howToUseSearchData();
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
      timesExec++;
    }
  }
}

// --- Functions

// Function to Output Help Message in the Terminal
void helpMessage()
{
  cout << clear;
  printTitle("WELCOME TO THE BANK OF WALL STREET", applyBgColor, applyFgColor, false);
  cout << "Database Commands:\n"
       << tab1 << "[" << cmdsPtr[cmdViewData] << "] View Data\n"
       << tab1 << "[" << cmdsPtr[cmdSearchData] << "] Search Data\n"
       << tab1 << "[" << cmdsPtr[cmdSearchParams] << "] Valid Search Data Subcommands and Parameters\n"
       << tab1 << "[" << cmdsPtr[cmdHowToUseSearchData] << "] How to Use the Search Data Command\n"
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