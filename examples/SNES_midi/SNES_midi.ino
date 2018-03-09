#include <MIDI.h>

#if defined(USBCON)
#include <midi_UsbTransport.h>

static const unsigned sUsbTransportBufferSize = 16;
typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;

UsbTransport sUsbTransport;

MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, MIDI);

#else // No USB available, fallback to Serial
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

#include "SNES_gamepad.h"
using namespace gamepad;

// any digital pin will work for this SNES gamepad interface
const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;

// ID values: 1 to 4 (tested on pro micro (ATmega32U4))
SNES_gamepad p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
// tested [so far] up to 4 SNES gamepads simultaneously (same 32U4)

SNES_multiplayer multi(p1,p2);

void setup() {}

void loop() {
  multi.read();
  if(p1.button_state_has_changed(SNES_gamepad::b::B)) { // done: change b name to more significative
    if(p1.button_is_pressed(SNES_gamepad::b::B))
      MIDI.sendNoteOn(42, 127, 0);
    else
      MIDI.sendNoteOff(42, 127, 0);
  }
}