#include <iostream>
#include "ansiEsc.h"
#include "input.h"

using namespace std;

// --- Extern Variables and Constants Declaration
extern const bool applyBgColor, applyFgColor;

// This prevents the program to crash if there's more input
void ignoreInput()
{
  while (getchar() != '\n')
  {
    cin.clear();
  }
}

// Function to Check if the Command Entered by the User is Correct
void wrongCommand(int commandStatus)
{
  switch (commandStatus)
  {
  case wrongMainCmd:
  case notACmd:
    cout << "ERROR: Wrong Command. Press ENTER to Display Help Message: ";
    break;
  case wrongSearchCmd:
  case missingSearchParams:
    string errorDescription = (commandStatus == missingSearchParams) ? "Missing Parameters" : "Wrong Command";

    cout << "ERROR: " << errorDescription << ". Press ENTER to Display Search Data Parameters Message : ";
    break;
  }
  ignoreInput();
}

// Function to Stop the Program Flow while the User doesn't press the ENTER key
void pressEnterToContinue()
{
  printTitle("Press ENTER to Continue", applyBgColor, applyFgColor);
  ignoreInput();
}