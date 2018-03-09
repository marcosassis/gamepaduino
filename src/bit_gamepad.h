#ifndef _BIT_GAMEPAD_H
#define _BIT_GAMEPAD_H

#include "gamepad.h"

namespace gamepad {

/// print all the bits of any primitive type variable
template<typename Type> void print_bits(Type var) {
  bool bit;
  for(int i=(sizeof(Type)*8)-1; i>=0; --i) {
    bit = var & (Type(1) << i);
    Serial.print(unsigned(bit));
  }
}

/// bit representation for gamepads that use it
template<typename uint_type>
class bit_gamepad: public gamepad
{
protected:
  uint_type buttons = 0; // each bit is a button, positive logic
  uint_type buttons_last = 0;

public:

  typedef uint_type uint_t;

  bit_gamepad(bit_gamepad& other)
  : bit_gamepad(other.id, other.n_buttons, other.dpads, other.n_dpads)
  {}

  bit_gamepad(uint8_t id, uint8_t n_buttons, directional* dpads=NULL, uint8_t n_dpads=0)
  : gamepad(id, n_buttons, dpads, n_dpads)
  {}

  virtual bool get_button_state(uint8_t index) const {
    //return bit_is_set(buttons, index);
    return buttons & (uint_t(1) << index); // OK
  }

  virtual void set_button_state(uint8_t index, bool bs) {
    //bitWrite(buttons,index,bs);
    
    if (bs)
      //setb(buttons,index);
      //SETBIT(buttons,index);
      buttons |= (uint_t(1) << (index));
    else
      //clrb(buttons,index);
      //CLEARBIT(buttons,index);
      buttons &= ~(uint_t(1) << (index));
  }

  virtual uint_type buttons_changed_mask() {
    return buttons_last ^ buttons;
  }
  virtual bool any_button_state_has_changed() const {
    return buttons_changed_mask();
  }
  virtual bool button_state_has_changed(uint8_t index) const {
    //return ((buttons_changed_mask()) & _BV(index));
    return buttons_changed_mask() & (uint_t(1) << index);
  }

  uint_type get_buttons() { // each bit is a button, positive logic
    return buttons;
  }
  uint_type get_buttons_last() {
    return buttons_last;
  }
  
  virtual void print(bool verbose=false) {
    if(verbose)
      gamepad::print();
    else
      print_bits(buttons);
  }

  /// override of gamepad:: , you can override but must call 
  /// this bit_gamepad::action_before_read inside yours.
  virtual void action_before_read() {
    buttons_last = buttons;
    buttons = 0;
  }
};

}
#endif // _BIT_GAMEPAD_H
