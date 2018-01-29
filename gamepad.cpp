#include "gamepad.h"


int get_id_by_name(String aname, String* names, unsigned max_names) {
  int ret = -1;
  for (unsigned i = 0; i < max_names; ++i) {
    if (names[i] == aname) {
      ret = i;
      break;
    }
  }
  return ret;
}


void SNES_gamepad::latch() {
  digitalWrite(latch_pin, HIGH);//latch pulse, lock the state of the buttons in the register
  delayMicroseconds(12);
  digitalWrite(latch_pin, LOW);
  delayMicroseconds(6);
}

void SNES_gamepad::read_imp() {
  buttons |= (digitalRead(data_pin) << 0); //first bit read before clock.

  for (int i = 1; i < 16; i++) {//clock &read
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(6);
    buttons |= (digitalRead(data_pin) << i);//the values are stored for each bit
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(6);
  }

  buttons = ~buttons; //workaround, a button pressed is read as a '0'(pull ups), jus inversing them to my taste.
}

static const String SNES_gamepad::names[] = {"B", "Y", "select", "start", "up", "down", "left", "right", "A", "X", "L", "R"};



