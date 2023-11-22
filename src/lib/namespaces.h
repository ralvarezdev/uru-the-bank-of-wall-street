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

  // --- Color Command Created Using the ralvarezdev's Minigrep Version. URL:https://github.com/ralvarezdev/minigrep

  // - Normal State
  const string sgrBgCmd = "\x1b[48;2;153;255;204m"; // Command to Change the Text Background Color
  const string sgrFgCmd = "\x1b[38;2;32;32;32m";    // Command to Change the Text Foreground Color

  // - Warning (On Error)
  const string sgrBgCmdError = "\x1b[48;2;201;42;42m";
  const string sgrFgCmdError = "\x1b[38;2;255;255;255m";

  // - Number of Characters
  const int nChar = 100;                                      // - Max Number of Characters being Printed in Each Line
  const int paramPerLine = 3;                                 // Number of Parameters Printed by Line
  const int maxSpacing = 4;                                   // If the Maximum Number Characters is Reached this is the Spacing between Paratemeters
  const int nCharField = 15;                                  // Number of Characters for Field Title
  const int nCharTitle = 30;                                  // Number of Characters of the Parameter Title Printed
  const int nCharParam = (nChar - nCharTitle) / paramPerLine; // Number of Maximum Characters per Parameter
}

namespace clients
{ // Enums Should be at the Beginning
  // - Clients Fields
  // Used to Select which Fields to Print in View Clients Command
  // or Used to Specify where to Filter the Parameter from, if it's
  // Typed Right After the Field
  enum fieldCmds
  {
    fieldId,
    fieldName,
    fieldAccountType,
    fieldAccountStatus,
    fieldAccountNumber,
    fieldBalance,
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
    sortByBalanceA,
    sortByBalanceD,
    sortByEnd // To get the number of Sort By Commands. SHOULD BE AT THE END
  };

  // - Invalid Client Data
  enum invalidClient
  {
    invalidClientId,
    invalidClientAccountNumber,
    clientExists,
    invalidArgument // Can't Search on Fields that doesn't have Unique Values
  };

  // - Client Status
  enum clientStatus
  {
    clientFound,
    clientNotFound,
    clientSuspended,
    errorStatus
  };

  // - Client Account Type
  enum accountType
  {
    accountCurrent,
    accountDebit,
    errorAccount,
    accountEnd // To get the number of Account Types. SHOULD BE AT THE END
  };

  // - Client Actions
  enum clientActions
  {
    clientDeposit,
    clientCashout,
    clientSend,
    clientEnd // To get the number of Client Actions. SHOULD BE AT THE END
  };

  /*
    // - Transaction Status
    enum transactionStatus
    {
      invalidAmount,
      transactionSuccessful,
      transactionEnd
    }; // To get the number of Transaction Statuses. SHOULD BE AT THE END
  */

  // - Client Structure
  struct Client
  {
    int id;         // Client ID
    string name;    // Client Name
    double account; // Client Account Number
    int type;       // Client Account Type
    bool suspended; // If the Account was Suspended
    float balance;  // Client Balance
  };

  // Clients Dynamic Array Class
  class Clients
  {
  private:
    Client *array = NULL; // Initializes as Null Pointer
    int capacity;
    int occupied; // Number of Elements that have been Added to the Array

  public:
    Clients()
    { // Default Constructor
      this->capacity = 2;
      this->occupied = 0;
      this->array = new Client[capacity];
    }

    Clients(int inputCapacity)
    { // Constructor with Capacity Given by the User
      this->capacity = inputCapacity;
      this->array = new Client[capacity];
    }

    int getCapacity() { return this->capacity; } // Return Array Capacity

    int getNumberClients() { return this->occupied; } // Return Number of Clients Appended to the Array

    Client getClient(int index) { return this->array[index]; } // Return Client Structure at the Given Index

    void changeClientStatus(int index)
    { // Activate the Client if it was Suspended. Suspend the Client if it was Active
      if (index < this->occupied)
        this->array[index].suspended = !this->array[index].suspended; // Change Status of Client
    }

    void pushBack(Client newClient)
    {
      if (this->occupied == this->capacity)
        increaseArrayCapacity(); // Double the Capacity of the Array

      this->array[this->occupied] = newClient;
      this->occupied++;
    }

    void increaseArrayCapacity() // Function to Double the Array Capacity
    {
      Client *temp = new Client[2 * this->capacity];
      for (int i = 0; i < this->occupied; i++)
        temp[i] = this->array[i]; // Copy Pointer to Client Structures

      delete[] this->array; // Delete Old Array

      this->array = temp; // Assign New Array
      this->capacity *= 2;
    }

    void insertAt(int index, Client client)
    { // Function to Insert Client. If the Index hasn't been Occupied. The Client will be Pushed Back
      if (index >= this->occupied)
        pushBack(client);
      else
        this->array[index] = client; // Insert Client at Given Index
    }

    void deleteAt(int index) // Function to Delete Client at Given Index
    {
      for (int i = index; i < this->occupied; i++)
        this->array[i] = this->array[i + 1]; // Move Clients

      this->occupied--; // Reduce Occupied Variable
    }

    void reverse() // Function to Swap the Order of the Array
    {
      int j = 0, n = this->getNumberClients();
      Client temp;

      for (int i = n - 1; i > (n - 1) / 2; i-- && j++)
      {
        temp = this->array[j];

        this->array[j] = this->array[i];
        this->array[i] = temp;
      }
    }

    Client compare(int *i, int *j, fieldCmds field, int increaseIndexBy = 0) // Function to Compare Client Fields
    {
      bool isI = false;

      switch (field)
      {
      case fieldCmds::fieldId:
        isI = (this->array[*i].id < this->array[*j].id);
        break;
      case fieldCmds::fieldName:
        isI = (this->array[*i].name.compare(this->array[*j].name) < 0);
        break;
      case fieldCmds::fieldAccountNumber:
        isI = (this->array[*i].account < this->array[*j].account);
        break;
      case fieldCmds::fieldAccountType:
        isI = (this->array[*i].type < this->array[*j].type);
        break;
      case fieldCmds::fieldBalance:
        isI = (this->array[*i].balance < this->array[*j].balance);
        break;
      }

      if (increaseIndexBy != 0)
        if (isI)
          *i = *i + 1;
        else
          *j = *j + 1;

      return (isI) ? this->array[*i - 1] : this->array[*j - 1];
    }

    void deallocate() // Functin to Deallocate Memory
    {
      delete[] this->array;
    }
  };

  const int precision = 2;                                // Precision for Floats and Doubles
  const string clientsFilename = "clients.csv";           // Clients Filename
  const string transactionsFilename = "transactions.csv"; // Transactions Filename (STORES TRANSACTIONS BETWEEN CLIENTS)
  const string balancesFilename = "balances.csv";         // Clients Movements Filename (STORES DEPOSITS, CASHOUTS AND TRANSACTIONS)
  const int maxAccountDigits = 10;                        // Maximum Number of Digits for Account Number
  const float minDeposit = 0;                             // Minimum Amount to Deposit
  const float maxDeposit = 1000000;                       // Maximum Amount to Deposit
}

namespace commands
{ // - Main Commands
  enum cmds
  {
    cmdViewClients,
    cmdFilterClients,
    cmdAddClient,
    cmdRemoveClient,
    cmdChangeStatus,
    cmdDepositMoney,
    cmdCashoutMoney,
    cmdSendMoney,
    cmdFieldParameters,
    cmdSortByParameters,
    cmdHowToUseViewClients,
    cmdHowToUseFilterClients,
    cmdHelp,
    cmdExit,
    cmdEnd // To get the number of Commands. SHOULD BE AT THE END
  };

  // - Subcommands
  enum subCmds
  {
    subCmdField,
    subCmdSortBy,
    subCmdEnd // To get the number of Subcommands. SHOULD BE AT THE END
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

  const int maxParamPerSubCmd = 3; // Max Number of Parameters per Subcommand

  // - View Clients Command Parameters Structure
  struct ViewClientsCmd
  {
    bool params[clients::fieldEnd];     // 1D Array to Save the Fields to Show in View Clients
    int sortBy[clients::sortByEnd / 2]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
  };

  // - Filter Clients Command Parameters Structure
  struct FilterClientsCmd
  {
    string params[clients::fieldEnd][maxParamPerSubCmd]; // 2D String Array of Clients Parameters
    string *paramsPtr[clients::fieldEnd];                // 1D Pointer Array to to the 2D Array
    int counter[clients::fieldEnd];
    int sortBy[clients::sortByEnd / 2]; // For a Field, only Allowed Ascending or Descending Order, not Both at the Same Time
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