#include "SNES_gamepad.h"

#define _GAMEPAD_DEFINE_SNES_HID
#include "gamepad_joystick_adapter.h"
using gamepad::SNES_hid;

#ifdef _GAMEPAD_SNES_MULTIPLAYER
using gamepad::SNES_multiplayer;
//typedef gamepad::multiplayer<SNES_*> SNES_multiplayer; == naïve
#endif

// ok, now thats very easy