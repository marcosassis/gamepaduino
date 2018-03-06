// define this macro to opt-out mutiplayer template class (interface) definition
// this is optional, since singleplayer will work anyway
#define _GAMEPAD_N64_SINGLEPLAYER // before include
#include "N64_gamepad_usb.h"

// ow, meaningful defines and typedefs/using are here to really encapsulate these
// options (that still available and exampled here for advanced users)
// so *noob* please: just use this as in examples, and it's really easy. have fun!
// and super users should derive some of these nice classes the same way SNES and N64
// gamepad (concrete) classes,,, and remember always: KISS!