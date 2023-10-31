#include <string>

using namespace std;

// --- Extern Variables Declaration
extern char *cmdsPtr, *fieldCmdsPtr;
extern const string clear, tab1, tab2;
extern const bool applyBgColor, applyFgColor;
extern const int nWp;

#ifndef DATA_H
#define DATA_H

// --- Enums

// - Main Commands
enum cmds
{
  cmdViewData,
  cmdSearchData,
  cmdSearchParams,
  cmdHowToUseSearchData,
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
// int getLengthCharArrayPointer(char *array);
void viewData();
void searchData(string **params, int m, int n);
void searchDataParameters();
void howToUseSearchData();
void depositMoney();
void checkoutMoney();
void transferMoney();
void suspendAccount();

#endif