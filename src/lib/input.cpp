#include <iostream>

using namespace std;

// This prevents the program to crash if there's more input
void ignoreInput()
{
  while (getchar() != '\n')
  {
    cin.clear();
  }
}

// Function to Check if the Command Entered by the User is Correct
void wrongCommand()
{
  cout << "ERROR: Wrong Command. Press ENTER to Display Help Message: ";
  ignoreInput();
}