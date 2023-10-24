#ifndef INPUT_H
#define INPUT_H

// --- Enums

// - Command Status if it's Valid or not
enum cmdStatus
{
  validCmd,
  notACmd,
  wrongMainCmd,
  missingSearchParams,
  wrongSearchCmd
};

// --- Functions
void ignoreInput();
void wrongCommand(int commandStatus);
void pressEnterToContinue();

#endif