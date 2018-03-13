#include "SNES_gamepad.h"

#define _GAMEPAD_DEFINE_SNES_HID
#include "gamepad_joystick_adapter.h"
using gamepad::SNES_hid;

#ifdef _GAMEPAD_SNES_MULTIPLAYER
using gamepad::SNES_multiplayer;
// this is custom optimization for SNES multiplayer protocol, see SNES_gamepad.h
#endif

// ok, now thats very easy