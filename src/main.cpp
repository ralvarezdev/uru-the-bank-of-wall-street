#include <algorithm>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

// #define NDEBUG
#include <assert.h>

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
- For this proyect we're not allowed to use maps, vectors, linked lists, tuples.
That's the reason why the program is written like this
*/

// --- Extern Variables Declaration (ASSIGNMENT AT THE END OF THIS FILE)
extern bool validFieldsToFilter[];
extern int cmdsChar[], subCmdsChar[], fieldCmdsChar[];
extern char *fieldCmdsStr[], *actionsStr[], *accountStr[];

// --- Templates
template <typename T>
void overwriteSortByParam(T *cmdStruct, int *counter, int sortByOrder[], int indexParam)
{ // It will Overwrite the Previous Sorting for this Parameter, if it was Specified
  if ((*cmdStruct).sortBy[indexParam / 2] != -1)
  {
    sortByOrder[*counter] = indexParam;
    *counter = *counter + 1;
  }

  (*cmdStruct).sortBy[indexParam / 2] = indexParam;
}

// --- Function Prototypes
void helpMessage();
void changeCwdToData();
void checkCmd(bool isViewClientsCmd, string input, bool *moreInput, bool *isField);
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

  changeCwdToData(); // Change Current Working Path to 'src/data'

  Clients clients = Clients(); // Allocate Memory
  getClients(&clients);        // Get Clients

  if (clients.getNumberClients() < 0)
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

    if (timesExec == 0 && argc > 1) // Checks if it's a Command
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

    timesExec++; // Increase the Counter of Commands Executed
    assert(timesExec > 0);

    if (!getline(stream, inputWord, ' '))
    { // Couldn't Get the Command
      isCmd = noCmd;
      continue;
    }

    cmd.main = inputWord[0];
    index.main = isCharOnArray(cmd.main, cmdsChar, cmdEnd); // Check if the Command is on the Array of Main Commands. Returns -1 if it doesn't exist

    if (index.main == -1) // If it's not a Valid Command
      isCmd = wrongMainCmd;
    else if (index.main == cmdViewClients || index.main == cmdFilterClients)
    {                                                         // Checks if the View Clients or Filter Clients Command is Typed Correctly
      bool isViewClientsCmd = (index.main == cmdViewClients); // Boolean to Check if the Current Command is View Clients

      int nSortBy = sortByEnd / 2;
      assert(nSortBy > 0); // Check if Enum sortByEnd is Greater than 0

      int sortByOrder[nSortBy], sortByCounter = 0; // Save Sorting Order
      fill(sortByOrder, sortByOrder + nSortBy, -1);

      if (isViewClientsCmd)
      {
        viewClientsCmd = ViewClientsCmd();                                // Initialize viewClients Sruct
        fill(viewClientsCmd.sortBy, viewClientsCmd.sortBy + nSortBy, -1); // Initialize Sort By Array
      }
      else
      {
        filterClientsCmd = FilterClientsCmd();
        for (int i = 0; i < fieldEnd - 1; i++) // Initialize Params to Filter Array
          fill(filterClientsCmd.params[i], filterClientsCmd.params[i] + maxParamPerSubCmd, "null");
        initPtrArray(filterClientsCmd.paramsPtr, filterClientsCmd.params, filterClientsCmd.counter, fieldEnd); // Copy Array to Pointer Array
        fill(filterClientsCmd.sortBy, filterClientsCmd.sortBy + nSortBy, -1);                                  // Initialize Sort By Array
      }

      try
      {
        int fieldsCounter = 0, sortByCounter = 0;

        while (true)
        {
          while (!moreInput && getline(stream, inputWord, ' '))
            if (inputWord[0] == '-')
              moreInput = true; // First Parameter Must be a Command

          if (!moreInput)
            break; // Exit this While-loop
          moreInput = false;
          isField = true;

          cmd.sub = inputWord[1]; // Check if the Command is in the Array of subCommands
          index.sub = isCharOnArray(cmd.sub, subCmdsChar, subCmdEnd);

          if (index.sub == -1) // Wrong Subcommand
            throw(wrongSubCmd);

          if (index.sub == subCmdSortBy)
          { // Get Sort By Parameters
            while (getline(stream, inputWord, ' '))
            {
              if (inputWord[0] == '-') // It's not a Sort By Parameter
              {
                moreInput = true;
                break;
              }

              // Check if the Command is in the Sort By Array
              cmd.param = inputWord[0];
              index.param = isCharOnArray(tolower(cmd.param), fieldCmdsChar, fieldEnd) * 2;

              // If the Character is Uppercase, Increase the Index by One to Match with the Descending Order Command Index
              if (isupper(cmd.param))
                index.param++;

              if (index.param == -1 || index.param == fieldAll) // Wrong Sort By Command Parameter
                throw(wrongSortByParam);
              sortByCounter++;

              if (isViewClientsCmd)
                overwriteSortByParam(&viewClientsCmd, &sortByCounter, sortByOrder, index.param);
              else
                overwriteSortByParam(&filterClientsCmd, &sortByCounter, sortByOrder, index.param);
            }
            continue;
          }

          if (!getline(stream, inputWord, ' '))
            throw((isViewClientsCmd) ? wrongField : wrongFieldParam); // Missing Parameter

          while (isField) // Get Field Parameters
          {
            isField = false;

            checkCmd(isViewClientsCmd, inputWord, &moreInput, &isField); // Check Command
            if (moreInput)                                               // It's not a Field Command or a Parameter
              break;

            cmd.field = (isViewClientsCmd) ? inputWord[0] : inputWord[2];
            index.field = isCharOnArray(cmd.field, fieldCmdsChar, fieldEnd); // Check if the Command is in the Field Parameters Array

            if (index.field == -1 || (!isViewClientsCmd && (index.field == fieldAll || !validFieldsToFilter[index.field]))) // Wrong Field Parameter or Field Command
              throw((isViewClientsCmd) ? wrongFieldParam : wrongField);

            if (isViewClientsCmd)
            {
              viewClientsCmd.params[index.field] = true;
              fieldsCounter++;

              if (getline(stream, inputWord, ' '))                           // Get the Next Argument
                checkCmd(isViewClientsCmd, inputWord, &moreInput, &isField); // Check Command
              else if (sortByCounter == 0)
                throw(wrongSortByParam); // Missing Sort By Param

              continue;
            }

            int *paramCounter; // Counter

            paramCounter = &filterClientsCmd.counter[index.field];

            while (*paramCounter < maxParamPerSubCmd && getline(stream, inputWord, ' '))
            { // Iterate while there's a Parameter and can be Added to the Array

              if (inputWord.length() == 0) // To Prevent Adding Whitespaces as Parameters-
                continue;
              else if (inputWord[0] != '"')
              {
                checkCmd(isViewClientsCmd, inputWord, &moreInput, &isField); // Check Command
                if (moreInput)
                  break;
              }
              else
              { // If it Starts with Double Quote, it's a Long Sentence (a Parameter with Multiple Words)
                if (!getline(stream, inputLong, '"'))
                  throw(wrongFilterClientsCmd); // Incomplete Long Parameter

                inputWord.insert(inputWord.length(), 1, ' ');                 // Insert Whitespace at the End
                inputLong.insert(0, inputWord.substr(1, inputWord.length())); // Remove Double Quote
                inputWord = inputLong;
              }

              try
              {                                  // Add Parameter to Search Client
                assert(inputWord.length() != 0); // Check inputWord String Length

                if (index.field == fieldId)
                  stoi(inputWord); // Check if the String can be Converted into an Integer

                filterClientsCmd.params[index.field][*paramCounter] = inputWord;
                fieldsCounter++;
              }
              catch (...)
              {
                continue; // Ignore the Parameter
              }

              *paramCounter += 1; // Parameter Counter
            }

            if (*paramCounter == 0) // Missing Field Param
              throw(wrongFieldParam);

            // Check if *paramCounter doesn't have Invalid Values
            assert(*paramCounter > 0 && *paramCounter <= maxParamPerSubCmd);

            while (!isField && !moreInput) // Reached Max Number of Parameters for Command
              if (!getline(stream, inputWord, ' '))
                break;
              else
                checkCmd(isViewClientsCmd, inputWord, &moreInput, &isField); // Check Command
          }
        }

        for (int i = 0; i < nSortBy; i++) // Save the Sort By Array based on the Order they were Introduced
          if (sortByOrder[i] != -1)
            if (isViewClientsCmd)
              viewClientsCmd.sortBy[i] = sortByOrder[i];
            else
              filterClientsCmd.sortBy[i] = sortByOrder[i];

        if (fieldsCounter == 0)
          throw((isViewClientsCmd) ? wrongField : wrongFieldParam);
        else if (sortByCounter == 0)
          throw(wrongSortByParam);
      }
      catch (cmdStatus cmdStatus)
      {
        isCmd = cmdStatus;
        assert(isCmd != validCmd); // Check if the Command is Invalid
      }
    }

    if (isCmd != validCmd)
    { // Chekc if the Command is Valid
      if (timesExec > 1 || (timesExec == 0 && argc == 1))
        cout << '\n'; // Print a New Line
      continue;
    }

    assert(index.main >= 0 && index.main < cmdEnd); // Check if the Command is Valid

    switch (index.main)
    {
    case cmdViewClients:
      viewClients(&clients, viewClientsCmd.params, viewClientsCmd.sortBy);
      break;
    case cmdFilterClients:
      filterClients(&clients, filterClientsCmd.paramsPtr, filterClientsCmd.sortBy);
      break;
    case cmdGetBalance:
      getBalance(&clients);
      break;
    case cmdAddClient:
      addClients(&clients);
      break;
    case cmdRemoveClient:
      removeClient(&clients);
      break;
    case cmdChangeStatus:
      changeStatus(&clients);
      break;
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
      depositMoney(&clients);
      break;
    case cmdCashoutMoney:
      cashoutMoney(&clients);
      break;
    case cmdSendMoney:
      sendMoney(&clients);
      break;
    case cmdExit:
      exit = true;
      break;
    }
  }

  clients.deallocate(); // Deallocate Memory
}

// --- Functions

// Function to Output Help Message in the Terminal
void helpMessage()
{
  cout << clear;
  printTitle("WELCOME TO THE BANK OF WALL STREET", applyBgColor, applyFgColor, false);
  cout << "Database Commands:\n"
       << tab1 << addBrackets(cmdsChar[cmdAddClient]) << " Add Client\n"
       << tab1 << addBrackets(cmdsChar[cmdRemoveClient]) << " Remove Client\n"
       << tab1 << addBrackets(cmdsChar[cmdChangeStatus]) << " Change Client Status\n"
       << tab1 << addBrackets(cmdsChar[cmdViewClients]) << " View Clients\n"
       << tab1 << addBrackets(cmdsChar[cmdFilterClients]) << " Filter Clients\n"
       << "Client Commands:\n"
       << tab1 << addBrackets(cmdsChar[cmdGetBalance]) << " Get Balance\n"
       << tab1 << addBrackets(cmdsChar[cmdDepositMoney]) << " Deposit Money\n"
       << tab1 << addBrackets(cmdsChar[cmdCashoutMoney]) << " Cashout Money\n"
       << tab1 << addBrackets(cmdsChar[cmdSendMoney]) << " Send Money\n"
       << "Command Parameters:\n"
       << tab1 << addBrackets(cmdsChar[cmdFieldParameters]) << " Client Field Parameters\n"
       << tab1 << addBrackets(cmdsChar[cmdSortByParameters]) << " Sort By Parameters\n"
       << "How-To:\n"
       << tab1 << addBrackets(cmdsChar[cmdHowToUseViewClients]) << " How to Use the View Clients Command\n"
       << tab1 << addBrackets(cmdsChar[cmdHowToUseFilterClients]) << " How to Use the Filter Clients Command\n"
       << "Other Commands:\n"
       << tab1 << addBrackets(cmdsChar[cmdHelp]) << " Help\n"
       << tab1 << addBrackets(cmdsChar[cmdExit]) << " Exit\n";
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
void changeCwdToData()
{
  try
  {
    filesystem::path mainCppPath = __FILE__;              // Path to main.cpp File
    filesystem::path srcPath = mainCppPath.parent_path(); // Path to 'src' Folder

    filesystem::path dataDir = "data";
    filesystem::path dataPath = srcPath / dataDir; // Concatenate srcPath with DataDir to get the FUll Path to the .csv Files

    filesystem::current_path(dataPath); // Change cwd to '.../src/data'
  }
  catch (...)
  {
    pressEnterToCont("Error: main.cpp File is not Inside 'lib' Folder", true);
  }
}

// Function to Check Command Typed as Input
void checkCmd(bool isViewClientsCmd, string input, bool *moreInput, bool *isField)
{
  bool isCmd = input[0] == '-';
  int inputLength = input.length();

  if (isViewClientsCmd)
  {
    if (isCmd && inputLength < 3 && inputLength > 0) // It's a Subcommand
      *moreInput = true;
    else if (!isCmd && inputLength > 0) // It's a Field Parameter
      *isField = true;
  }
  else if (isCmd)        // Got a Command
    if (inputLength < 3) // It's a Subcommand
      *moreInput = true;
    else if (inputLength > 2) // It's a Field Parameter
      *isField = true;
}

// --- Extern Variables and Constants Assignment

int cmdsChar[cmdEnd] = { // Commands Character
    [cmdViewClients] = 'v',
    [cmdFilterClients] = 'f',
    [cmdAddClient] = 'a',
    [cmdRemoveClient] = 'r',
    [cmdChangeStatus] = 'C',
    [cmdGetBalance] = 'g',
    [cmdDepositMoney] = 'd',
    [cmdCashoutMoney] = 'c',
    [cmdSendMoney] = 's',
    [cmdFieldParameters] = 'F',
    [cmdSortByParameters] = 'S',
    [cmdHowToUseViewClients] = 'x',
    [cmdHowToUseFilterClients] = 'y',
    [cmdHelp] = 'h',
    [cmdExit] = 'e'};

int subCmdsChar[subCmdEnd] = { // Subcommands
    [subCmdField] = 'f',
    [subCmdSortBy] = 's'};

int fieldCmdsChar[fieldEnd] = { // Field Commands
    [fieldId] = 'i',
    [fieldName] = 'n',
    [fieldAccountType] = 't',
    [fieldAccountStatus] = 's',
    [fieldAccountNumber] = 'a',
    [fieldBalance] = 'b',
    [fieldAll] = '.'};

char *fieldCmdsStr[fieldEnd] = { // Field Names
    [fieldId] = "Id",
    [fieldName] = "Client Name",
    [fieldAccountType] = "Account Type",
    [fieldAccountStatus] = "Account Status",
    [fieldAccountNumber] = "Account Number",
    [fieldBalance] = "Balance"};

bool validFieldsToFilter[fieldEnd] = { // Fields that can be Used in Filter Clients Command
    [fieldId] = true,
    [fieldName] = true,
    [fieldAccountType] = false,
    [fieldAccountStatus] = false,
    [fieldAccountNumber] = true,
    [fieldBalance] = false};

char *accountStr[accountEnd] = { // Account Types
    [accountCurrent] = "current",
    [accountDebit] = "debit"};

char *actionsStr[clientEnd] = { // Client Possible Actions
    [clientDeposit] = "deposit",
    [clientCashout] = "cashout",
    [clientSend] = "send"};
