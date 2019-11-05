#include "ZrKeypad.h"

#include <Keypad.h>

static const byte KEYPAD_ROWS = 4;  // four rows
static const byte KEYPAD_COLS = 4;  // four columns
// define the cymbols on the buttons of the keypads

static const char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {{'1', '2', '3', 'A'},
                                                        {'4', '5', '6', 'B'},
                                                        {'7', '8', '9', 'C'},
                                                        {'E', '0', 'F', 'D'}};

// connect to the row pinouts of the keypad
byte rowPins[KEYPAD_ROWS] = {23, 19, 22, 21};

// connect to the column pinouts of the keypad
byte colPins[KEYPAD_COLS] = {2, 17, 5, 18};

// initialize an instance of class NewKeypad
Keypad zrKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
