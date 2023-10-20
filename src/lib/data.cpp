#include <string>
#include <iostream>

using namespace std;

// TO DEVELOP

void viewData()
{
  cout << "v";
}

void searchData(string dataType, string dataToSearch, bool*isCommand)
{
  cout << "s\n"
       << dataType << '\n'
       << dataToSearch;
}

void depositMoney()
{
  cout << "d";
}

void checkoutMoney()
{
  cout << "c";
}

void transferMoney()
{
  cout << "t";
}

void suspendAccount()
{
  cout << "S";
}