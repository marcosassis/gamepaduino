#include "SNES_gamepad.h"

namespace gamepad {


void SNES_gamepad::latch() {
  digitalWrite(latch_pin, HIGH);//latch pulse, lock the state of the buttons in the register
  delayMicroseconds(12);
  digitalWrite(latch_pin, LOW);
  delayMicroseconds(6);
}

void SNES_gamepad::read_imp() {
  //Serial.println(id);

  read_bit(0); //first bit read before clock.

  for (int i = 1; i < 16; i++) {//clock &read
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(6);
    read_bit(i); //the values are stored for each bit
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(6);
  }

  buttons = ~buttons; // a button pressed is read as a '0'(pull ups), gamepad uses positive logic
}

//                                            0    1    2         3        4     5       6       7        8    9   10   11
static const String SNES_gamepad::names[] = {"B", "Y", "select", "start", "up", "down", "left", "right", "A", "X", "L", "R"};


#ifndef _GAMEPAD_SNES_SINGLEPLAYER
  /* original is
       virtual void read() {
        gamepad_t::read();
        for(uint8_t i = 1; i<players.size(); ++i)
          players.get(i)->read();
      }
  */
//template<class gamepad_type>
void SNES_multiplayer::read() {
  for(uint8_t i = 0; i<players.size(); ++i) {
    Serial.print("_be_"); Serial.println(players.get(i)->get_id());
    this->players.get(i)->action_before_read();
  }
  noInterrupts();
  latch();
  read_imp();  // only first(this) will really latch and read all controllers, see read_bit override here)
  interrupts();
  for(uint8_t i = 0; i<players.size(); ++i) {
    Serial.print("_af_"); Serial.println(players.get(i)->get_id());
    this->players.get(i)->action_after_read();
  }
}
#endif // _GAMEPAD_SNES_SINGLEPLAYER


}