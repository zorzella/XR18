#pragma once

#ifndef ZR_KEYPAD_h
#define ZR_KEYPAD_h

#include <Keypad.h>

/*
  The conceptual keyboard layout is:

  1 2 3 A
  4 5 6 B
  7 8 9 C
  D E F G

 */
static const char KEY_PAGE{'1'};

static const char KEY_UP{'6'};
static const char KEY_DOWN{'F'};
static const char KEY_RIGHT{'C'};
static const char KEY_LEFT{'8'};

static const char KEY_PLUS{'4'};
static const char KEY_MINUS{'7'};

extern Keypad zrKeypad;

#endif