#include "SNES_gamepad.h"
#define _GAMEPAD_DEFINE_SNES_HID
#include "gamepad_joystick_adapter.h"
 // comment these out if you're like me (respect scopes, don't pollute)
using gamepad::SNES_hid;

#ifdef _GAMEPAD_SNES_MULTIPLAYER
using gamepad::SNES_multiplayer;
//typedef gamepad::multiplayer<SNES_hid> SNES_multiplayer;
#endif

// ok, now thats very easy