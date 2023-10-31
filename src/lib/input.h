#include <string>

using namespace std;

// --- Extern Variables and Constants Declaration
extern const bool applyBgColor, applyFgColor;

#ifndef INPUT_H
#define INPUT_H

// --- Enums

// - Command Status if it's Valid or not
enum cmdStatus
{
  validCmd,
  noCmd,
  wrongMainCmd,
  wrongSearchDataCmd
};

// --- Functions
void ignoreInput();
void wrongCommand(int commandStatus);
void pressEnterToContinue(string message, bool warning);

#endif