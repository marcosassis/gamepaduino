#include "N64_gamepad.h"

#define _GAMEPAD_DEFINE_N64_HID
#include "gamepad_joystick_adapter.h"
using gamepad::N64_hid;

#ifdef _GAMEPAD_N64_MULTIPLAYER
typedef gamepad::multiplayer<gamepad::N64_gamepad>  N64_multiplayer;
// NO custom imp. for N64 multiplayer interface, default is already optimal, see multiplayer.h
#endif

// ok, now thats very easy