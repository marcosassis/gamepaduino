#ifndef _STATIC_SUBSET_H
#define _STATIC_SUBSET_H

#include "subset.h"

namespace gamepad {


template<typename Type> void print_bits(Type var) {
  bool bit;
  for(int i=(sizeof(Type)*8)-1; i>=0; --i) {
    bit = var & (Type(1) << i);
    Serial.print(unsigned(bit));
  }
}


template<uint8_t N>
struct static_subset: public subset
{
  static const uint8_t N_BUTTONS = N;
  uint8_t buttons_ids[N_BUTTONS];

  static_subset(const static_subset& other)
  : static_subset(other.n_buttons, other.parent, other.get_id(), other.buttons_ids)
  {}
  static_subset(uint8_t n_buttons, buttonset& aparent)
  : subset(n_buttons, aparent, id, aparent.get_id())
  {}
  static_subset(uint8_t n_buttons, buttonset& aparent, uint8_t id)
  : subset(n_buttons, aparent, id)
  {}
  static_subset(uint8_t n_buttons, buttonset& aparent, const uint8_t buttons_ids[])
  : static_subset(n_buttons, aparent, aparent.get_id(), buttons_ids)
  {}
  static_subset(uint8_t n_buttons, buttonset& aparent, uint8_t id, const uint8_t buttons_ids[])
  : subset(n_buttons, aparent, id)
  {
    set_ids(buttons_ids);
  }

  virtual uint8_t get_button_parent_id(uint8_t subsetindex) const {
    if(subsetindex>=N_BUTTONS)
      return -1;
    
    return buttons_ids[subsetindex];
  }

  static_subset<N>& operator= (static_subset<N>& other)	{
    this->n_buttons = other.n_buttons;
    this->parent = other.parent;
    set_ids(other.buttons_ids);
    return *this;
  }

  void set_ids(const uint8_t buttons_ids[])	{
    for(uint8_t i=0; i<N_BUTTONS; ++i)
      this->buttons_ids[i] = buttons_ids[i];    
  }
};

}
#endif //_STATIC_SUBSET_H