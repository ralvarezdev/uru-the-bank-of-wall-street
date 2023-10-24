#include <string>

using namespace std;

#ifndef DATA_H
#define DATA_H

// --- Enums

// - Main Commands
enum cmds
{
  cmdViewData,
  cmdSearchData,
  cmdSearchParams,
  cmdDepositMoney,
  cmdCheckoutMoney,
  cmdTransferMoney,
  cmdHelp,
  cmdExit,
  cmdSuspendAccount,
  cmdEnd // To get the number of Commands. SHOULD BE AT THE END
};

// - Search Data SubCommands
enum searchCmds
{
  searchClientId,
  searchClientName,
  searchAccountNumber,
  searchEnd // To get the number of Search Data SubCommands. SHOULD BE AT THE END
};

// --- Functions
int isCharOnArray(char character, char array[], int n);
void viewData();
void searchData(string dataType, string dataToSearch, bool *isCommand);
void searchDataParameters();
void depositMoney();
void checkoutMoney();
void transferMoney();
void suspendAccount();

#endif