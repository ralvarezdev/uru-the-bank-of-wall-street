#include <iostream>
#include "ansiEsc.h"
#include "input.h"

using namespace std;

// --- Function Prototypes
void ignoreInput();
void wrongCommand(int commandStatus);
void pressEnterToContinue(string message, bool warning);

// --- Functions

// This prevents the program to crash if there's more input
void ignoreInput()
{
  while (getchar() != '\n')
    cin.clear();
}

// Function to Check if the Command Entered by the User is Correct
void wrongCommand(int commandStatus)
{
  string message = "ERROR: ", err;

  switch (commandStatus)
  {
  case noCmd:
  case wrongMainCmd:
    message.append((commandStatus == noCmd) ? "No" : "Wrong");
    message.append(" Command. Press ENTER to Display Help Message");
    break;
  case wrongSearchDataCmd:
    message.append("Wrong Command. Press ENTER to Display Search Data Parameters Message");
    break;
  }
  pressEnterToContinue(message, true);
}

// Function to Stop the Program Flow while the User doesn't press the ENTER key
void pressEnterToContinue(string message, bool warning)
{
  printTitle(message, applyBgColor, applyFgColor, warning);
  ignoreInput();
}