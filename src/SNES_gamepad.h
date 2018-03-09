#ifndef _SNES_GAMEPAD_H
#define _SNES_GAMEPAD_H

#include "bit_gamepad.h"

namespace gamepad {


class SNES_gamepad: public bit_gamepad<uint16_t> {
protected:

  uint8_t latch_pin;
  uint8_t clock_pin;
  uint8_t data_pin;

  directional dpad;

  virtual void latch();
  virtual void read_imp();

public:

  typedef bit_gamepad<uint16_t> gamepad_base;
  static const uint8_t N_BUTTONS = 12;
  static const String names[N_BUTTONS]; // both of these should have in any concrete class of gamepads
  //      0  1  2       3      4   5     6     7      8  9  10 11
  enum bid {B, Y, select, start, up, down, left, right, A, X, L, R};


  SNES_gamepad(const SNES_gamepad& other)
  : SNES_gamepad(other.id, other.data_pin, other.clock_pin, other.latch_pin)
  {}

  SNES_gamepad(uint8_t id, uint8_t data_pin = 7, uint8_t clock_pin = 4, uint8_t latch_pin = 5)
  : dpad(4,5,6,7, *this), gamepad_base(id, 12, &dpad, 1),
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
  friend struct SNES_multiplayer;
#endif
  
protected:
  
  virtual void read_bit(uint8_t i) {
    buttons |= (digitalRead(data_pin) << i);
  }
};


// if you want to TURN OFF this optimization class for SNES controllers protocol,
// just define _GAMEPAD_SINGLEPLAYER before include this file
// (if you want muliplayer, never mind, for default it will be compiled)
#ifndef _GAMEPAD_SINGLEPLAYER
#define _GAMEPAD_SNES_MULTIPLAYER
}//end namespace gamepad
#include "multiplayer.h"
namespace gamepad {


//template<class gamepad_type>
struct SNES_multiplayer: public multiplayer<SNES_gamepad>//<gamepad_type>
{
  //typedef gamepad_type gamepad_t;
  typedef multiplayer<gamepad_t> multiplayer_base;

  SNES_multiplayer(const gamepad_t& p1)
  : multiplayer_base(p1) {
  }
  SNES_multiplayer(const gamepad_t& p1, const gamepad_t& p2)
  : multiplayer_base(p1,p2) {
  }
  SNES_multiplayer(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3)
  : multiplayer_base(p1,p2,p3) {
  }
  SNES_multiplayer(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3, const gamepad_t& p4)
  : multiplayer_base(p1,p2,p3,p4) {
  }
  SNES_multiplayer(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3, const gamepad_t& p4, const gamepad_t& p5)
  : multiplayer_base(p1,p2,p3,p4) {
    players.add(&p5); // é nois joga bomberman fi 
  }

  // clock and latch logic only once (first controller)
  virtual void read();
  virtual void latch_all();
  virtual void read_all();
  virtual void read_bit_all(uint8_t i);
  virtual void clock_read_bit_all(uint8_t i);
};

#endif // #ifndef _GAMEPAD_SINGLEPLAYER

}//end namespace gamepad

#endif // _SNES_GAMEPAD_H



