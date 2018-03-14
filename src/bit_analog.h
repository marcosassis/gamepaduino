#ifndef _GAMEPAD_BIT_ANALOG_H
#define _GAMEPAD_BIT_ANALOG_H

//#include "buttonset.h"
#include "analog.h"

namespace gamepad {


template<typename value_type, value_type BITMASK>
struct bit_analog_mask_traits {
  typedef value_type value_t;
  static const value_t BIT_MASK = BITMASK;

};
  

/// bitmap representation for analog types
template<typename value_type, uint8_t Dimensions=2>
struct bit_analog: public analog_abstract {
  typedef value_type value_t;
  static const uint8_t N_AXES = Dimensions;
  
  STATIC_CHECK(N_AXES>=1);
  
  virtual value_type get(uint8_t i) const {
    return values[i];
  }
  virtual void set(uint8_t i, value_type v) {
    return values[i]=v;
  }
};


template<class ButtonSetType, typename value_type, uint8_t Dimensions=2>
struct has_analog: public ButtonSetType {
  typedef ButtonSetType button_set_base;
  typedef value_type    value_t;
  
protected:
  analog_t analog;
}

}
#endif // _GAMEPAD_BIT_ANALOG_H