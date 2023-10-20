#include <string>

using namespace std;

#ifndef DATA_H
#define DATA_H

// --- Functions
void viewData();
void searchData(string dataType, string dataToSearch, bool *isCommand);
void depositMoney();
void checkoutMoney();
void transferMoney();
void suspendAccount();

#endif