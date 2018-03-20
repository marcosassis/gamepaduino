#ifndef _SUBSET_H
#define _SUBSET_H

#include "buttonset.h"

namespace gamepad {
  
/// subset is not gamepad
/// but is almost, it's parent can be a gamepad(or not)
/// so a subset does nothing 'per se', except mapping
/// one set of (button)IDs onto another
///
/// directional is the most eminent ex. of subset
/// 
class subset: public buttonset
{
protected:
  
  virtual String* get_button_names() const {
    return parent.get_button_names();
  }

  buttonset& parent;

public:

  subset(uint8_t n_buttons, buttonset& aparent, uint8_t id)
  : buttonset(id, n_buttons), parent(aparent)
  {}
  subset(uint8_t n_buttons, buttonset& aparent)
  : buttonset(aparent.get_id(), n_buttons), parent(aparent)
  {}

  virtual bool button_is_pressed(uint8_t subsetindex) const {
    return get_button_state(subsetindex);
  }
  virtual bool get_button_state(uint8_t subsetindex) const {
    return parent.get_button_state(get_button_parent_id(subsetindex));
  }
  virtual void set_button_state(uint8_t subsetindex, bool bs) {
    parent.set_button_state(get_button_parent_id(subsetindex), bs);
  }
  virtual bool any_state_has_changed() const {
    return parent.any_state_has_changed();
  }
  virtual bool button_state_has_changed(uint8_t subsetindex) const {
    return parent.button_state_has_changed(get_button_parent_id(subsetindex));
  }
  virtual int get_button_id_by_name(String aname) const {
    int ret = parent.get_button_id_by_name(aname);
    if(ret<0)
      return ret;
    
    for(uint8_t i=0; i<get_n_buttons(); ++i)
      if(get_button_parent_id(i)==ret)
        return i;
        
    return -1;
  }
  virtual uint8_t get_button_parent_id(uint8_t subsetindex) const = 0;

};

}
#endif //_SUBSET_H