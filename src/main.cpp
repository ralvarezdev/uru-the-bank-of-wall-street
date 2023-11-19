#include <algorithm>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

#include "lib/clients/clientsOp.h"
#include "lib/data/dataOp.h"
#include "lib/terminal/ansiEsc.h"
#include "lib/terminal/input.h"
#include "lib/namespaces.h"

using namespace std;
using namespace clients;
using namespace commands;
using namespace terminal;

/*
--- NOTES
- For this proyect we're not allowed to use maps, vectors, classes, linked lists, tuples.
That's the reason why the program is written like this
*/

// --- Extern Variables Declaration (ASSIGNMENT AT THE END OF THIS FILE)
extern bool *fieldValidCmdsPtr;
extern int *cmdsPtr, *subCmdsPtr, *fieldCmdsCharPtr, *sortByCmdsPtr;
extern string *fieldCmdsStrPtr, *actionsPtr, *accountPtr;

// --- Global Variables
int nClientsRead = 0; // Number of Clients that had been Read and Copied from clients.csv

// --- Function Prototypes
void helpMessage();
void changeCwdToData(string path);
void initPtrArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n);

int main(int argc, char **argv)
{
  std::ios::sync_with_stdio(false); // Desynchronize C++ Streams from C I/O Operations to Increase Performance

  ViewClientsCmd viewClientsCmd; // Used to Save the Parameters Typed by the User for the Given Command
  FilterClientsCmd filterClientsCmd;
  Cmd cmd;
  CmdIndex index;

  bool exit = false;                      // Tells the Program if the User wants to Exit the Program
  bool moreInput, isField;                // Boolean to Check if there's More Input
  cmdStatus isCmd = validCmd;             // Used for Checking if the Command is Valid or Not. If not, it Stores the Reason
  int timesExec = 0;                      // Number of times the Main While Loop has been executed
  string inputLine, inputWord, inputLong; // Saves the Input of the User, before being Processed by the Program

  changeCwdToData(argv[0]); // Change Current Working Path to 'src/data'

  Client *clients = new Client[nClients]; // Allocate Memory
  nClientsRead = getClients(clients);     // Get Clients

  if (nClientsRead == -1)
    return -1; // An Error Ocurred

  while (!exit) // Main While Loop of the Program
  {
    cmd = Cmd();        // Initialize Command Structure to 0
    index = CmdIndex(); // Initialize Command Indexes to 0

    if (isCmd != validCmd)
    { // If in the Last Execution the User Typed a Wrong Command
      wrongCommand(isCmd);

      switch (isCmd)
      {
      case wrongField:
      case wrongFieldParam:
        fields(); // Print the Valid Field as Commands or as Parameters
        break;
      case wrongSortByParam:
        sortByParameters(); // Print the Valid Sort By Parameters
        break;
      case wrongViewClientsCmd:
        howToUseViewClients(); // Print the Usage Examples of the View Clients Command
        break;
      case wrongFilterClientsCmd:
        howToUseFilterClients(); // Print Usage Examples of the Filter Clients Commands
        break;
      }

      isCmd = validCmd;
    }

    if (timesExec == 0 && argc > 1)
    { // Checks if it's a Command
      for (int i = 1; i < argc; i++)
      {
        inputWord = argv[i];

        size_t found = inputWord.find(' ');                               // Check if the string contains Whitespaces
        if (found != string::npos)                                        // Whitespace Found
          inputWord.insert(0, 1, '"').insert(inputWord.length(), 1, '"'); // Insert a Double Quote at the Beginning and at the End of the Input

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

    cmd.main = inputWord[0];
    index.main = isCharOnArray(cmd.main, cmdsPtr, cmdEnd); // Check if the Command is on the Array of Main Commands. Returns -1 if it doesn't exist

    if (index.main == -1) // If it's not a Valid Command
      isCmd = wrongMainCmd;
    else if (index.main == cmdViewClients || index.main == cmdFilterClients)
    {                                                         // Checks if the View Clients or Filter Clients Command is Typed Correctly
      bool isViewClientsCmd = (index.main == cmdViewClients); // Boolean to Check if the Current Command is View Clients

      int nSortBy = sortByEnd / 2;
      int sortByOrder[nSortBy], sortByCounter = 0; // Save Sorting Order
      for (int i = 0; i < nSortBy; i++)
        sortByOrder[i] = -1;

      if (isViewClientsCmd)
      { // Initialize viewClients Sruct
        viewClientsCmd = ViewClientsCmd();
        fill(viewClientsCmd.sortBy, viewClientsCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }
      else
      {
        filterClientsCmd = FilterClientsCmd();
        for (int i = 0; i < fieldEnd - 1; i++)
          fill(filterClientsCmd.params[i], filterClientsCmd.params[i] + maxParamPerSubCmd, "null");            // Initialize Params to Filter Array
        initPtrArray(filterClientsCmd.paramsPtr, filterClientsCmd.params, filterClientsCmd.counter, fieldEnd); // Copy Array to Pointer Array
        fill(filterClientsCmd.sortBy, filterClientsCmd.sortBy + nSortBy, -1);                                  // Initialize Sort By Array
      }

      while (true)
      {
        if (isCmd != validCmd)
          break; // Exit this While-loop
        isCmd = validCmd;

        while (!moreInput && getline(stream, inputWord, ' '))
          if (inputWord[0] == '-')
            moreInput = true; // First Parameter Must be a Command

        if (!moreInput)
          break; // Exit this While-loop
        moreInput = false;
        isField = true;

        if (inputWord.length() < 2)
        { // Check if the Command has the Minimum String Length
          isCmd = wrongSubCmd;
          break;
        }

        cmd.sub = inputWord[1]; // Check if the Command is in the Array of subCommands
        index.sub = isCharOnArray(cmd.sub, subCmdsPtr, subCmdEnd);

        if (inputWord[0] != '-' && index.sub == -1)
        { // Wrong Subcommand
          isCmd = wrongSubCmd;
          break;
        }

        if (index.sub == subCmdSortBy)
        { // Get Sort By Parameters
          while (getline(stream, inputWord, ' '))
          {
            if (inputWord[0] == '-')
            { // It's not a Sort By Parameter
              moreInput = true;
              break;
            }

            cmd.param = inputWord[0];
            index.param = isCharOnArray(cmd.param, sortByCmdsPtr, sortByEnd); // Check if the Command is in the Sort By Array

            if (index.param == -1)
            { // Wrong Sort By Command Parameter
              isCmd = wrongSortByParam;
              break;
            }

            if (isViewClientsCmd)
            { // It will Overwrite the Previous Sorting for this Parameter, if it was Specified
              if (viewClientsCmd.sortBy[index.param / 2] != -1)
                sortByOrder[sortByCounter++] = index.param;

              viewClientsCmd.sortBy[index.param / 2] = index.param;
            }
            else
            {
              if (filterClientsCmd.sortBy[index.param / 2] != -1)
                sortByOrder[sortByCounter++] = index.param;

              filterClientsCmd.sortBy[index.param / 2] = index.param;
            }
          }
          continue;
        }

        getline(stream, inputWord, ' ');

        while (isField) // Get Field Parameters
        {
          isField = false;

          if (inputWord[0] == '-' && (isViewClientsCmd || (!isViewClientsCmd && inputWord.length() < 3)))
          { // It's not a Field Command or a Parameter
            moreInput = true;
            break;
          }

          cmd.field = (isViewClientsCmd) ? inputWord[0] : inputWord[2];
          index.field = isCharOnArray(cmd.field, fieldCmdsCharPtr, fieldEnd); // Check if the Command is in the Field Parameters Array

          if (index.field == -1 || (!isViewClientsCmd && (index.field == fieldAll || !fieldValidCmdsPtr[index.field])))
          { // Wrong Field Parameter or Field Command
            isCmd = (isViewClientsCmd) ? wrongFieldParam : wrongField;
            break;
          }

          if (isViewClientsCmd)
          {
            viewClientsCmd.params[index.field] = true;

            if (getline(stream, inputWord, ' '))                 // Get the Next Argument
              if (inputWord[0] == '-' && inputWord.length() < 3) // It's a Subcommand
                moreInput = true;
              else if (inputWord[0] != '-' && inputWord.length() > 0)
                isField = true;

            continue;
          }

          int *paramCounter; // Counter

          paramCounter = &filterClientsCmd.counter[index.field];

          while (*paramCounter < maxParamPerSubCmd && getline(stream, inputWord, ' '))
          { // Iterate while there's a Parameter and can be Added to the Array
            if (inputWord[0] == '"')
            { // If it Starts with Double Quote, it's a Long Sentence (a Parameter with Multiple Words)
              if (!getline(stream, inputLong, '"'))
              { // Incomplete Long Parameter
                isCmd = wrongFilterClientsCmd;
                break;
              }

              inputWord.insert(inputWord.length(), 1, ' ');                 // Insert Whitespace at the End
              inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Insert the Parameter at the Beginning of the String, without the Double Quote
              inputWord = inputLong;
            }
            else if (inputWord[0] == '-')
            { // If it's a Command break this For-loop
              if (inputWord.length() < 3)
                moreInput = true;
              else if (inputWord.length() > 2)
                isField = true;
              break;
            }
            else if (inputWord.length() == 0) // To Prevent Adding Whitespaces as Parameters-
              continue;

            try
            { // Add Parameter to Search Client
              if (index.field == fieldId)
                stoi(inputWord); // Check if the String can be Converted into an Integer

              filterClientsCmd.params[index.field][*paramCounter] = inputWord;
            }
            catch (...)
            {
              continue; // Ignore the Parameter
            }

            *paramCounter += 1; // Parameter Counter
          }

          while (!isField && !moreInput) // Reached Max Number of Parameters for Command
            if (!getline(stream, inputWord, ' '))
              break;
            else if (inputWord[0] == '-') // Got a Command
              if (inputWord.length() < 3)
                moreInput = true;
              else if (inputWord.length() > 2)
                isField = true;
        }
      }

      for (int i = 0; i < nSortBy; i++) // Save the Sort By Array based on the Order they were Introduced
        if (sortByOrder[i] != -1)
          if (isViewClientsCmd)
            viewClientsCmd.sortBy[i] = sortByOrder[i];
          else
            filterClientsCmd.sortBy[i] = sortByOrder[i];

      if (isCmd == validCmd)
        switch (index.main)
        {
        case cmdViewClients:
          viewClients(clients, nClientsRead, viewClientsCmd.params, viewClientsCmd.sortBy);
          break;
        case cmdFilterClients:
          filterClients(clients, nClientsRead, filterClientsCmd.paramsPtr, filterClientsCmd.sortBy);
          break;
        }
    }

    if (isCmd != validCmd)
    { // Chekc if the Command is Valid
      if (timesExec > 1 || (timesExec == 0 && argc == 1))
        cout << '\n'; // Print a New Line
      continue;
    }

    switch (index.main)
    {
    case cmdFieldParameters:
      fields();
      break;
    case cmdSortByParameters:
      sortByParameters();
      break;
    case cmdHowToUseViewClients:
      howToUseViewClients();
      break;
    case cmdHowToUseFilterClients:
      howToUseFilterClients();
      break;
    case cmdDepositMoney:
      depositMoney(clients, nClientsRead);
      break;
    case cmdCashoutMoney:
      cashoutMoney(clients, nClientsRead);
      break;
    case cmdSendMoney:
      sendMoney(clients, nClientsRead);
      break;
    case cmdExit:
      exit = true;
      break;
    case cmdAddClient:
      addClients(clients, &nClientsRead);
      break;
    case cmdSuspendAccount:
      changeStatus(clients, nClientsRead);
      break;
    }
  }
  delete[] clients; // Deallocate Memory
}

// --- Functions

// Function to Output Help Message in the Terminal
void helpMessage()
{
  cout << clear;
  printTitle("WELCOME TO THE BANK OF WALL STREET", applyBgColor, applyFgColor, false);
  cout << "Database Commands:\n"
       << tab1 << addBrackets(cmdsPtr[cmdViewClients]) << " View Clients\n"
       << tab1 << addBrackets(cmdsPtr[cmdFilterClients]) << " Filter Clients\n"
       << "Client Commands:\n"
       << tab1 << addBrackets(cmdsPtr[cmdDepositMoney]) << " Deposit Money\n"
       << tab1 << addBrackets(cmdsPtr[cmdCashoutMoney]) << " Cashout Money\n"
       << tab1 << addBrackets(cmdsPtr[cmdSendMoney]) << " Send Money\n"
       << "Command Parameters:\n"
       << tab1 << addBrackets(cmdsPtr[cmdFieldParameters]) << " Client Field Parameters\n"
       << tab1 << addBrackets(cmdsPtr[cmdSortByParameters]) << " Sort By Parameters\n"
       << "How-To:\n"
       << tab1 << addBrackets(cmdsPtr[cmdHowToUseViewClients]) << " How to Use the View Clients Command\n"
       << tab1 << addBrackets(cmdsPtr[cmdHowToUseFilterClients]) << " How to Use the Filter Clients Command\n"
       << "Other Commands:\n"
       << tab1 << addBrackets(cmdsPtr[cmdHelp]) << " Help\n"
       << tab1 << addBrackets(cmdsPtr[cmdExit]) << " Exit\n"
       << "Admin Privileges:\n"
       << tab1 << addBrackets(cmdsPtr[cmdAddClient]) << " Add Client\n"
       << tab1 << addBrackets(cmdsPtr[cmdSuspendAccount]) << " Change Client Status\n";
}

// Function to Assign 2D Array to 1D Pointer, and Reset the Counters
void initPtrArray(string **ptrArray, string array[][maxParamPerSubCmd], int arrayCounter[], int n)
{
  for (int i = 0; i < n; i++)
  {
    ptrArray[i] = array[i];
    arrayCounter[i] = 0;
  }
}

// Function to Change Current Working Directory to 'src/data'
void changeCwdToData(string path)
{
  try
  {
    filesystem::path mainPath = path;                                // Path to main.exe
    filesystem::path binPath = mainPath.parent_path().parent_path(); // Path to Main Folder

    filesystem::path dataDir = "src/data";
    filesystem::path dataPath = binPath / dataDir; // Concatenate binPath with DataDir to get the FUll Path to the .csv Files

    filesystem::current_path(dataPath); // Change cwd to '.../src/data'
  }
  catch (...)
  {
    pressEnterToCont("Error: Executable File is not Inside 'bin' Folder", true);
  }
}

// --- Extern Variables and Constants Assignment

// The Index of each Character is Related to the Enum Value of the Same Command or Subcommand
int cmdsChar[commands::cmdEnd] = {'v', 'f', 'd', 'c', 's', 'F', 'S', 'x', 'y', 'h', 'e', 'a', 'C'};
int subCmdsChar[commands::subCmdEnd] = {'f', 's'};
int fieldCmdsChar[commands::fieldEnd] = {'i', 'n', 't', 's', 'a', '.'};
int sortByCmdsChar[commands::sortByEnd] = {'i', 'I', 'n', 'N', 't', 'T', 's', 'S', 'a', 'A'};

// Fields that can be Used for Filter Clients Command
bool fieldValidCmds[commands::fieldEnd] = {true, true, false, false, true};

// Command Title
string accountStr[clients::accountEnd] = {"current", "debit"};
string actionsStr[clients::clientEnd] = {"deposit", "cashout", "send"};
string fieldCmdsStr[commands::fieldEnd] = {"Id", "Client Name", "Account Type", "Account Status", "Account Number"};

// --- Extern Variables and Constants Assignment
int *cmdsPtr = cmdsChar;
int *subCmdsPtr = subCmdsChar;
int *fieldCmdsCharPtr = fieldCmdsChar;

bool *fieldValidCmdsPtr = fieldValidCmds;

string *accountPtr = accountStr;
string *actionsPtr = actionsStr;
string *fieldCmdsStrPtr = fieldCmdsStr;
int *sortByCmdsPtr = sortByCmdsChar;