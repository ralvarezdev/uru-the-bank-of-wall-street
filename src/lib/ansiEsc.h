#include <string>

using std::string;

#ifndef ANSI_ESC_H
#define ANSI_ESC_H

// --- ANSI Escape Codes

// - Reset Terminal Text Format to Default
#define ANSI_RESET "\x1b[0m"

// - Clear Terminal
#define ANSI_CLEAR "\033c"

// --- Functions
void printTitle(string message, bool applySgrBgCommand, bool applySgrFgCommand);

#endif