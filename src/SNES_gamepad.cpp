#include "SNES_gamepad.h"

namespace gamepad {

void SNES_gamepad::latch()
{
  digitalWrite(latch_pin, HIGH);//latch pulse, lock the state of the buttons in the register
  delayMicroseconds(12);
  digitalWrite(latch_pin, LOW);
}

/// hey, compare me to my "friend"'s SNES_multiplayer::read_imp()
void SNES_gamepad::read_imp()
{
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

static const String SNES_gamepad::names[] = 
//  0    1    2         3        4     5       6       7        8    9   10   11
  {"B", "Y", "select", "start", "up", "down", "left", "right", "A", "X", "L", "R"};


#ifdef _GAMEPAD_SNES_MULTIPLAYER

/// hey, compare us to my "friend"'s SNES_gamepad::read_imp()
void SNES_multiplayer::read_imp_all()
{
  delayMicroseconds(6);
  read_bit_all(0); //first bit read before clock.
  for (uint8_t i = 1; i < 16; i++) { // clock & read
    clock_read_bit_all(i);
  }
  gamepad_pointer pp;
  for(uint8_t p = 0; p<players.size(); ++p) {
    pp = this->players.get(p);
    pp->buttons = ~(pp->buttons);
  }
}

void SNES_multiplayer::clock_read_bit_all(uint8_t i)
{
  gamepad_pointer p1 = this->players.get(0);
  digitalWrite(p1->clock_pin, HIGH);
  delayMicroseconds(6);
  read_bit_all(i);
  digitalWrite(p1->clock_pin, LOW);
  delayMicroseconds(6);
}

#endif // _GAMEPAD_SNES_MULTIPLAYER


} // gamepad