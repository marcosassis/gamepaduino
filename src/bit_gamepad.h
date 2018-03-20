#ifndef _BIT_GAMEPAD_H
#define _BIT_GAMEPAD_H

#include "gamepad.h"

namespace gamepad {


/// print all the bits of any primitive type variable
template<typename Type, class SerialType=DefaultHardwareSerial>
void print_bits(Type var, int verbose=0, SerialType& theSerialPrinter = Serial) {
  bool bit;
  
  for(int i=(sizeof(Type)*8)-1; i>=0; --i) {
    if(verbose>0 && (i%8)==0 && i!=0)
      theSerialPrinter.print(" ");

    bit = var & (Type(1) << i);
    theSerialPrinter.print(unsigned(bit));    
  }
}


/// bitmap representation for gamepads that use it
template<typename uint_Type>
struct bit_gamepad: public gamepad
{
  typedef uint_Type uint_type;
  
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

  bool get_bit_state(uint8_t index) const {
    return buttons & (uint_t(1) << index);
  }
  void set_bit_state(uint8_t index, bool bs) {
    if (bs)
      buttons |= (uint_t(1) << (index));
    else
      buttons &= ~(uint_t(1) << (index));
  }
  virtual bool get_button_state(uint8_t index) const {
    return get_bit_state(index);
  }
  virtual void set_button_state(uint8_t index, bool bs) {
    set_bit_state(index,bs);
  }

  virtual uint_type state_changed_mask() {
    return buttons_last ^ buttons;
  }
  virtual bool any_state_has_changed() const {
    return state_changed_mask();
  }
  virtual bool button_state_has_changed(uint8_t index) const {
    return bit_state_has_changed(index);
  }
  bool bit_state_has_changed(uint8_t index) const {
    //return ((state_changed_mask()) & _BV(index));
    return state_changed_mask() & (uint_t(1) << index);
  }

  /// each bit is a button, positive logic
  
  /// this is virtual because: maybe you want to mask-out non-buttons states (but default is this)
  virtual uint_type get_buttons() const {
    return get_state();
  }
  virtual void set_buttons(uint_type bu) {
    set_state(bu);
  }
  uint_type get_state() const {
    return buttons;
  }
  void set_state(uint_type bu) {
    buttons = bu;
  }
  uint_type get_state_last() const {
    return buttons_last;
  }
  
  /// override of gamepad:: 
  /// you must call this bit_gamepad::action_before_read inside yours.
  virtual void action_before_read() {
    _GAMEPAD_DEBUG("bit_gamepad::action_before_read");
    buttons_last = buttons;
    //buttons = 0;
    //gamepad::action_before_read();
  }
  
  /// override of buttonset:: 
  template<class SerialType=DefaultHardwareSerial>
  void print(int verbose=0, SerialType& theSerialPrinter = Serial) const {
    _GAMEPAD_DEBUG("bit_gamepad::print");
    if(verbose>1)
      buttonset::print(verbose, theSerialPrinter);
    else
      print_bits(buttons, verbose, theSerialPrinter); // here: print all state (maybe has more than buttons)
  }
  /// override of buttonset::println
  /// same thing as buttonset::print, but assures we call bit_gamepad::print
  _GAMEPAD_AUX_IMPLEMENT_PRINTLN 
};

}
#endif // _BIT_GAMEPAD_H
