#include <string>

using namespace std;

#ifndef NAMESPACES_H
#define NAMESPACES_H

namespace files
{
  const char sep = ','; // Separators
}

namespace terminal
{
  // - ANSI Escape Code
  const string reset = "\x1b[0m"; // Reset Terminal Text Format to Default
  const string clear = "\033c";   // Clear Terminal

  // - Format Constants
  const string tab1 = string(3, ' '); // String with 3 whitespaces
  const string tab2 = string(8, ' '); // String with 8 whitespaces
  const bool applyBgColor = true;     // Add Customed Background Color
  const bool applyFgColor = true;     // Add Customed Foreground Color

  // - Size of the Title
  const int nChar = 100; // If the string length is smaller, it'll be filled with whitespaces

  // --- Color Command Created Using the ralvarezdev's Minigrep Version. URL:https://github.com/ralvarezdev/minigrep

  // - Normal State
  const string sgrBgCmd = "\x1b[48;2;153;255;204m"; // Command to Change the Text Background Color
  const string sgrFgCmd = "\x1b[38;2;32;32;32m";    // Command to Change the Text Foreground Color

  // - Warning (On Error)
  const string sgrBgCmdError = "\x1b[48;2;201;42;42m";
  const string sgrFgCmdError = "\x1b[38;2;255;255;255m";

  // - Number of Characters
  const int paramPerLine = 3;                                 // Number of Parameters Printed by Line
  const int maxSpacing = 4;                                   // If the Maximum Number Characters is Reached this is the Spacing between Paratemeters
  const int nCharField = 15;                                  // Number of Characters for Field Title
  const int nCharTitle = 30;                                  // Number of Characters of the Parameter Title Printed
  const int nCharParam = (nChar - nCharTitle) / paramPerLine; // Number of Maximum Characters per Parameter
}

namespace clients
{ // Enums Should be at the Beginning
  // - Invalid Client Data
  enum invalidClient
  {
    invalidClientId,
  };

  // - Client Status
  enum clientStatus
  {
    clientFound,
    clientNotFound
  };

  // - Client Account Type
  enum accountType
  {
    accountCurrent,
    accountDebit,
    errorAccount,
    accountEnd // To get the number of Account Types. SHOULD BE AT THE END
  };

  // - Client Structure
  struct Client
  {
    int id;         // Client ID
    string name;    // Client Name
    double account; // Client Account Number
    int type;       // Client Account Type
    bool suspended; // If the Account was Suspended
  };

  const int nClients = 10000;                   // Max Number of Clients
  const string clientsFilename = "clients.csv"; // Clients Filename
}

namespace commands
{ // - Main Commands
  enum cmds
  {
    cmdViewClients,
    cmdFilterClients,
    cmdDepositMoney,
    cmdCheckoutMoney,
    cmdTransferMoney,
    cmdFieldParameters,
    cmdSortByParameters,
    cmdHowToUseViewClients,
    cmdHowToUseFilterClients,
    cmdHelp,
    cmdExit,
    cmdAddClient,
    cmdSuspendAccount,
    cmdEnd // To get the number of Commands. SHOULD BE AT THE END
  };

  // - Subcommands
  enum subCmds
  {
    subCmdField,
    subCmdSortBy,
    subCmdEnd // To get the number of Subcommands. SHOULD BE AT THE END
  };

  // - Clients Fields
  // Used to Select which Fields to Print in View Clients Command
  // or Used to Specify where to Filter the Parameter from, if it's
  // Typed Right After the Field
  enum fieldCmds
  {
    fieldId,
    fieldName,
    fieldAccountType,
    fieldSuspended,
    fieldAccountNumber,
    fieldAll,
    fieldEnd // To get the number of Fields. SHOULD BE AT THE END
  };

  // - Sort By Commands
  // A: Ascending
  // D: Descending
  enum sortByCmds
  {
    sortByIdA,
    sortByIdD,
    sortByNameA,
    sortByNameD,
    sortByTypeA,
    sortByTypeD,
    sortBySuspendedA,
    sortBySuspendedD,
    sortByAccountA,
    sortByAccountD,
    sortByEnd // To get the number of Sort By Commands. SHOULD BE AT THE END
  };

  // - Command Status if it's Valid or not
  enum cmdStatus
  {
    validCmd,
    noCmd,
    wrongMainCmd,
    wrongSubCmd,
    wrongViewClientsCmd,
    wrongFilterClientsCmd,
    wrongField,
    wrongFieldParam,
    wrongSortByParam,
    wrongBooleanAnswer
  };

  const int maxParamPerSubCmd = 6; // Max Number of Parameters per Subcommand

  // - View Clients Command Parameters Structure
  struct ViewClientsCmd
  {
    bool params[fieldEnd];     // 1D Array to Save the Fields to Show in View Clients
    int sortBy[sortByEnd / 2]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Filter Clients Command Parameters Structure
  struct FilterClientsCmd
  {
    string params[fieldEnd][maxParamPerSubCmd]; // 2D String Array of Clients Parameters
    string *paramsPtr[fieldEnd];                // 1D Pointer Array to to the 2D Array
    int counter[fieldEnd];
    int sortBy[sortByEnd / 2]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Command Structure
  struct Cmd
  { // Command, SubCommand Parameter
    int main;
    int sub;
    int field;
    int param;
  };

  // - Command Indexes Structure
  struct CmdIndex
  { // Used to Save the Index of the Command on its Corresponding Array
    int main = 0;
    int sub = 0;
    int field = 0;
    int param = 0;
  };
}

#endif