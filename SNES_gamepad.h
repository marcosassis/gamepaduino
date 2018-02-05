// SNES_gamepad.h

#ifndef _SNES_GAMEPAD_h
#define _SNES_GAMEPAD_h

#include "gamepad.h"
#ifndef _GAMEPAD_SNES_SINGLEPLAYER
#include "multiplayer.h"
#endif

class SNES_gamepad: public bit_gamepad<uint16_t> {
  uint8_t latch_pin;
  uint8_t clock_pin;
  uint8_t data_pin;

protected:

  directional dpad;

  virtual void latch();
  virtual void read_imp();

public:

  static const uint8_t N_BUTTONS = 12;
  static const String names[N_BUTTONS]; // both of these should have in any concrete class of gamepads
  //      0  1  2       3      4   5     6     7      8  9  10 11
  enum b {B, Y, select, start, up, down, left, right, A, X, L, R};


  SNES_gamepad(const SNES_gamepad& other)
  : SNES_gamepad(other.id, other.data_pin, other.clock_pin, other.latch_pin)
  {}

  SNES_gamepad(uint8_t id, uint8_t data_pin = 7, uint8_t clock_pin = 4, uint8_t latch_pin = 5)
  : dpad(4,5,6,7, *this), bit_gamepad<uint16_t>(id, 12, &dpad, 1),
    latch_pin(latch_pin), clock_pin(clock_pin), data_pin(data_pin)
  {
    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, INPUT);
  }

  virtual String* get_button_names() const {
    return names;
  }

#ifndef _GAMEPAD_SNES_SINGLEPLAYER
  friend struct multiplayerSNES;
#endif
  
protected:
  
  virtual void read_bit(uint8_t i) {
    buttons |= (digitalRead(data_pin) << i);
  }
};


#ifndef _GAMEPAD_SNES_SINGLEPLAYER

struct multiplayerSNES: public multiplayer<SNES_gamepad>
{
  typedef multiplayer<SNES_gamepad> multiplayer_base;

  multiplayerSNES(const gamepad_t& p1)
  : multiplayer_base(p1) {
  }
  multiplayerSNES(const gamepad_t& p1, const gamepad_t& p2)
  : multiplayer_base(p1) {
  }
  multiplayerSNES(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3)
  : multiplayer_base(p1,p2) {
  }
  multiplayerSNES(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3, const gamepad_t& p4)
  : multiplayer_base(p1,p2,p3) {
  }

protected:

  // override SNES_gamepad::read_bit to read all controller buttons at once
  // clock and latch logic only once (first controller = this)
  virtual void read_bit(uint8_t i) {
    SNES_gamepad::read_bit(i);
    for(uint8_t i=1; i<players.size(); ++i)
      players.get(i)->read_bit(i);
  }


  /* original is
      virtual void read() {
        for(uint8_t i = 0; i<size(); ++i)
        players.get(i)->read();
      }
  */
  virtual void read() {
    for(uint8_t i = players.size()-1; i>=0; --i) {
      players.get(i)->action_before_read();
    }
    SNES_gamepad::read();  // only first(this) will really read (all bits, see read_bit override here)
    for(uint8_t i = 1; i<players.size(); ++i) {
      players.get(i)->action_after_read();
    }
  }

};

typedef multiplayer<SNES_gamepad> SNES_multiplayer;

#endif

#endif

