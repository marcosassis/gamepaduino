#ifndef _BUTTONSET_H
#define _BUTTONSET_H

#include "Arduino.h"

#ifdef USBCON
typedef Serial_         DefaultHardwareSerial;
#else
typedef HardwareSerial  DefaultHardwareSerial;
#endif


namespace gamepad {


/// helper function, linear search in an array
template<class Type=String>
int get_id_by_name(Type aname, Type* names, unsigned max_names)
{
  int ret = -1;
  for (unsigned i = 0; i < max_names; ++i) {
    if (names[i] == aname) {
      ret = i;
      break;
    }
  }
  return ret;
}


/// this abstract class is helper for gamepad (abstract class) interface
/// this is also derived from subset, which is not gamepad,
/// but they have things in common (directional is the most eminent ex. of subset)
class buttonset
{
protected:
  uint8_t n_buttons;
  uint8_t id;

public:
  
  buttonset(uint8_t id, uint8_t n_buttons)
  : id(id), n_buttons(n_buttons)
  {}

  virtual bool get_button_state(uint8_t index) const = 0;
  virtual bool button_is_pressed(uint8_t index) const {
    return get_button_state(index);
    // forcing positive logic semantics for library consistency and understandability
  }
  virtual void set_button_state(uint8_t index, bool bs) = 0;

  virtual bool any_button_state_has_changed() const = 0;
  virtual bool button_state_has_changed(uint8_t index) const = 0;

  virtual String* get_button_names() const = 0;

  uint8_t get_id() const {
    return id;
  }
  uint8_t get_n_buttons() const {
    return n_buttons;
  }

  /// consider using concrete_classes::bid (if context allows it (statically))
  int get_button_id_by_name(String aname) const {
    return get_id_by_name(aname, get_button_names(), n_buttons);
  }

  /**
    verbose:
    0: print buttonset buttons as bits
    1: same as before, but with spaces between bytes
    2: print all '{ button_name: button_state, ... }'
    3: print all 'button_name:\t\tbutton_state\n'
  */
  template<class SerialType=DefaultHardwareSerial>
  void print(int verbose=0, SerialType& theSerialPrinter = Serial) const;
  
  template<class SerialType=DefaultHardwareSerial>
  void println(int verbose=0, SerialType& theSerialPrinter = Serial) const {
    print(verbose, theSerialPrinter);
    theSerialPrinter.println();
  }
};


template<class SerialType>
void buttonset::print(int verbose, SerialType& theSerialPrinter) const
{
  uint8_t nb = get_n_buttons();
  if(verbose>=3) {
    for(uint8_t i=0; i<nb; ++i) {
      theSerialPrinter.print(get_button_names()[i]);
      theSerialPrinter.print(":\t\t");
      theSerialPrinter.print(get_button_state(i));
      theSerialPrinter.print("\n");
    }  
  }
  else if(verbose>=2) {
    theSerialPrinter.print("{");
    for(uint8_t i=0; i<nb; ++i) {
      theSerialPrinter.print(" ");
      theSerialPrinter.print(get_button_names()[i]);
      theSerialPrinter.print(": ");
      theSerialPrinter.print(get_button_state(i));
      if(i==nb-1)
        theSerialPrinter.print(" ");
      else
        theSerialPrinter.print(", ");
    }
    theSerialPrinter.print("}");
  }
  else {
    for(uint8_t i=0; i<nb; ++i) {
      if(verbose>=1 && (i%8)==0 && i!=0)
        theSerialPrinter.print(" ");
      theSerialPrinter.print(get_button_state(i));
    }
  }
}

}
#endif //_BUTTONSET_H