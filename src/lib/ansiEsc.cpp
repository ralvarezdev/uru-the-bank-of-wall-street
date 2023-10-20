#include <iostream>
#include <iomanip>
#include <string>
#include "ansiEsc.h"

using namespace std;

// --- Color Command Created Using the ralvarezdev's Minigrep Version
// url: https://github.com/ralvarezdev/minigrep

// - Command to Change the Text Background Color
string sgrBgCommand = "\x1b[48;2;153;255;204m";
// - Command to Change the Text Foreground Color
string sgrFgCommand = "\x1b[38;2;32;32;32m";

// Function to Print Title with Customed Colors
void printTitle(string message, bool applySgrBgCommand, bool applySgrFgCommand)
{
  int nWp = 60; // Size of the title. If the string length is smaller, it'll be filled with whitespaces
  string sgrCommand;
  if (applySgrBgCommand)
    sgrCommand.append(sgrBgCommand);
  if (applySgrFgCommand)
    sgrCommand.append(sgrFgCommand);

  cout << '\n'
       << sgrCommand
       << setw(nWp) << setfill(' ') << left
       << message << ANSI_RESET << '\n';
}