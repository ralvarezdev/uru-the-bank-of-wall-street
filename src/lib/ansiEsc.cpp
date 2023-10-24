#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// --- Extern Variables and Constants Definition

// - ANSI Escape Code
extern const string reset = "\x1b[0m"; // Reset Terminal Text Format to Default
extern const string clear = "\033c";   // Clear Terminal

// - Format Constants
extern const string tab1 = string(3, ' '); // String with 3 whitespaces
extern const string tab2 = string(8, ' '); // String with 8 whitespaces
extern const bool applyBgColor = true;     // Add Customed Background Color
extern const bool applyFgColor = true;     // Add Customed Foreground Color

// --- Color Command Created Using the ralvarezdev's Minigrep Version. URL:https://github.com/ralvarezdev/minigrep
const string sgrBgCommand = "\x1b[48;2;153;255;204m"; // Command to Change the Text Background Color
const string sgrFgCommand = "\x1b[38;2;32;32;32m";    // Command to Change the Text Foreground Color

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
       << message << reset << '\n';
}