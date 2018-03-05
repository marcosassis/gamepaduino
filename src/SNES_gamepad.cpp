#include "SNES_gamepad.h"
#include "stopwatch.h"

namespace gamepad {


void SNES_gamepad::latch() {
  stopwatch t(12000);
  digitalWrite(latch_pin, HIGH);//latch pulse, lock the state of the buttons in the register
  t.wait();
  digitalWrite(latch_pin, LOW);
}

void SNES_gamepad::read_imp() {

  stopwatch t(6000);
  while(t.stop_if_elapsed()) {}  
  read_bit(0); //first bit read before clock.

  for (int i = 1; i < 16; i++) { // clock & read
    digitalWrite(clock_pin, HIGH);
    t.start(6000); t.wait();
    read_bit(i); //the values are stored for each bit
    digitalWrite(clock_pin, LOW);
    t.start(); t.wait(6000); // different ways here just for demo purposes
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
  
  this->players.get(0)->latch(); // only first will really latch all controllers
  
  stopwatch t;
  t.wait(6000);
  for(uint8_t p = 0; p<players.size(); ++p) {
    this->players.get(p)->read_bit(0); //first bit read before clock.
  }
  for (int i = 1; i < 16; i++) { // clock & read
    digitalWrite(this->players.get(0)->clock_pin, HIGH);
    t.start();
    t.wait(6000);
    for(uint8_t p = 0; p<players.size(); ++p) {
      this->players.get(p)->read_bit(i);
    }
    digitalWrite(this->players.get(0)->clock_pin, LOW);
    t.start();
    t.wait(6000);
  }
  for(uint8_t p = 0; p<players.size(); ++p) {
    this->players.get(p)->buttons = ~(this->players.get(p)->buttons);
  } // ok(ufa) of course i'm gonna generalize this

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
#endif // _GAMEPAD_SNES_SINGLEPLAYER


}