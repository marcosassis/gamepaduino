#ifndef _BUTTONSET_H
#define _BUTTONSET_H

#include "Arduino.h"

namespace gamepad {

/// helper function, linear search in an array
template<class Type=String>
int get_id_by_name(Type aname, Type* names, unsigned max_names) {
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
/// but they have things in common
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

  virtual int get_button_id_by_name(String aname) const {
    return get_id_by_name(aname, get_button_names(), n_buttons);
  }

  virtual void print() {
    for(uint8_t i=0; i<n_buttons; ++i) {
      Serial.print(get_button_names()[i]); Serial.print(":\t"); Serial.println(get_button_state(i));
    }
  }

};

}
#endif //_BUTTONSET_H