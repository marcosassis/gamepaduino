#include "SNES_gamepad.h"

namespace gamepad {


void SNES_gamepad::latch() {
  digitalWrite(latch_pin, HIGH);//latch pulse, lock the state of the buttons in the register
  delayMicroseconds(12);
  digitalWrite(latch_pin, LOW);
}

void SNES_gamepad::read_imp() {

  delayMicroseconds(12);
  read_bit(0); //first bit read before clock.

  for (int i = 1; i < 16; i++) { // clock & read
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
// clock and latch logic only once (first controller)
void SNES_multiplayer::read() {
  
  for(uint8_t p = 0; p<players.size(); ++p)
    this->players.get(p)->action_before_read();
  
  noInterrupts(); // latch and read all on the same batch
  
  latch_all(); // only first will really latch all controllers
  
  read_all();

  interrupts();
  
  for(uint8_t p = 0; p<players.size(); ++p)
    this->players.get(p)->action_after_read();
}
/* gamepad::read is:
virtual void read() {
  action_before_read();
  noInterrupts();
  latch();
  read_imp();
  interrupts();
  action_after_read();
}
*/

void SNES_multiplayer::latch_all() {
  this->players.get(0)->latch(); // only first will really latch all controllers
}

void SNES_multiplayer::read_bit_all(uint8_t i) {
  for(uint8_t p = 0; p<players.size(); ++p) {
    this->players.get(p)->read_bit(i);
  }
}

void SNES_multiplayer::clock_read_bit_all(uint8_t i) {
  digitalWrite(this->players.get(0)->clock_pin, HIGH);
  delayMicroseconds(6);
  read_bit_all(i);
  digitalWrite(this->players.get(0)->clock_pin, LOW);
  delayMicroseconds(6);
}

void SNES_multiplayer::read_all() {
  for(uint8_t p = 0; p<players.size(); ++p) {
    this->players.get(p)->buttons = 0;//~gamepad_t::uint_t(0);
  }
  delayMicroseconds(6);
  read_bit_all(0); //first bit read before clock.
  for (uint8_t i = 1; i < 16; i++) { // clock & read
    clock_read_bit_all(i);
  }
  for(uint8_t p = 0; p<players.size(); ++p) {
    this->players.get(p)->buttons = ~(this->players.get(p)->buttons);
    //this->players.get(p)->set_button_state(0,!(this->players.get(p)->get_button_state(0)));
  } // ok(ufa) of course i'm gonna generalize this
}

#endif // _GAMEPAD_SNES_SINGLEPLAYER


}