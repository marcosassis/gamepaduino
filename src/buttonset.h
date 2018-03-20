#ifndef _BUTTONSET_H
#define _BUTTONSET_H

#include "Arduino.h"

#ifdef USBCON
typedef Serial_         DefaultHardwareSerial;
#else
typedef HardwareSerial  DefaultHardwareSerial;
#endif


namespace gamepad {

//#define _GAMEPAD_DEBUG_PRINT_

#ifdef _GAMEPAD_DEBUG_PRINT_
  #define _GAMEPAD_DEBUG(messg) Serial.print(get_id()); Serial.print(":\t"); Serial.println((messg));
  #define _GAMEPAD_DEBUG_noid(messg) Serial.print("\t"); Serial.println((messg));
#else
  #define _GAMEPAD_DEBUG(messg)
  #define _GAMEPAD_DEBUG_noid(messg)
#endif  


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

  virtual bool any_state_has_changed() const = 0;
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

  
  /// below on: printing
  
  template<class SerialType=DefaultHardwareSerial>
  void print_button_as_bit
    (uint8_t i, SerialType& theSerialPrinter = Serial) const
  {
    theSerialPrinter.print(get_button_state(i));
  }
  
  template<class SerialType=DefaultHardwareSerial>
  void print_button_name_state
    (uint8_t i, String div1=": ", String div2=", ", SerialType& theSerialPrinter = Serial) const
  {
    theSerialPrinter.print(get_button_names()[i]);
    if(div1.length())
      theSerialPrinter.print(div1);
    print_button_as_bit(i,theSerialPrinter);
    if(div2.length())
      theSerialPrinter.print(div2);
  }  
  
  template<class SerialType=DefaultHardwareSerial>
  void print_all_buttons
    (bool print_name=true, String begs="", String ends="",
    String div1=": ", String div2=", ", uint8_t sepeach=8, String sep=" ",
    SerialType& theSerialPrinter = Serial) const;
  
  /**
    verbose:
    0: print buttonset buttons as bits
    1: same as before, but with spaces between bytes
    2: print all '{ button_name: button_state, ... }'
    3: print all 'button_name:\t\tbutton_state\n'
  */  
  template<class SerialType=DefaultHardwareSerial>
  void print(int verbose=0, SerialType& theSerialPrinter = Serial) const;
  
  /// you can override print (for use in template functions)
  /// but you should override this too, so just use this macro inside class:
  #define _GAMEPAD_AUX_IMPLEMENT_PRINTLN \
  template<class SerialType=DefaultHardwareSerial> \
  void println(int verbose=0, SerialType& theSerialPrinter = Serial) const { \
    print(verbose, theSerialPrinter); \
    theSerialPrinter.println(); \
  }
  _GAMEPAD_AUX_IMPLEMENT_PRINTLN
};


template<class SerialType>
void buttonset::print(int verbose, SerialType& theSerialPrinter) const
{
  //_GAMEPAD_DEBUG("buttonset::print");
  if(verbose>=3)
    print_all_buttons(true,"\n","",":\t\t","\n",0,"",theSerialPrinter);
  else if(verbose==2)
    print_all_buttons(true,"{"," }",": ",", ",0,"",theSerialPrinter);
  else if(verbose==1)
    print_all_buttons(false,"","","","",8," ",theSerialPrinter);
  else
    print_all_buttons(false,"","","","",0,"",theSerialPrinter);
}

template<class SerialType>
void buttonset::print_all_buttons
  (bool print_name, String begs, String ends, String div1, String div2,
  uint8_t sepeach, String sep, SerialType& theSerialPrinter) const
{
  //_GAMEPAD_DEBUG("buttonset::print_all_buttons");
  
  if(begs.length())
    theSerialPrinter.print(begs);
  
  for(uint8_t i=0; i<n_buttons; ++i) {
    if(sepeach && sep.length() && (i%sepeach)==0 && i!=0)
      theSerialPrinter.print(sep);

    if(i==(n_buttons-1))
      div2 = ends;
    
    if(print_name)
      print_button_name_state(i,div1,div2,theSerialPrinter);
    else
      print_button_as_bit(i,theSerialPrinter);
  }
}

}
#endif //_BUTTONSET_H