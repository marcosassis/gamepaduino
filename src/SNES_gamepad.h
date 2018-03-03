#ifndef _SNES_GAMEPAD_H
#define _SNES_GAMEPAD_H

#include "bit_gamepad.h"

namespace gamepad {


//#ifndef _GAMEPAD_SNES_SINGLEPLAYER
//#include "multiplayer.h"
//#endif


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
  enum b {B, Y, select, start, up, down, left, right, A, X, L, R};


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
  //template<class gamepad_type>
  friend struct SNES_multiplayer;
#endif
  
protected:
  
  virtual void read_bit(uint8_t i) {
    //Serial.print(id); Serial.print("\t"); Serial.println(i);
    buttons |= (digitalRead(data_pin) << i);
  }
};



#define _GAMEPAD_SNES_SINGLEPLAYER
#ifndef _GAMEPAD_SNES_SINGLEPLAYER

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

protected:

  // override SNES_gamepad::read_bit to read all controller buttons at once
  // clock and latch logic only once (first controller = this)
  virtual void read_bit(uint8_t i) {
    //Serial.print(players.size()); Serial.print("\t"); Serial.println(i);
    SNES_gamepad::read_bit(i);
    for(uint8_t p=1; p<players.size(); ++p)
      players.get(p)->read_bit(i);
  }


public:

  virtual void read();

};

#endif // _GAMEPAD_SNES_SINGLEPLAYER


}
#endif // _SNES_GAMEPAD_H


