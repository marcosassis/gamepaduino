#include "N64_gamepad.h"
#define _GAMEPAD_DEFINE_N64_HID
#include "gamepad_joystick_adapter.h"
#include "multiplayer.h"
// comment these out if you're like me (respect scopes, don't pollute)
using gamepad::N64_hid;
using gamepad::multiplayer<N64_hid>;
// ok, now thats very easy