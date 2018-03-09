#ifndef _DIRECTIONAL_H
#define _DIRECTIONAL_H

#include "static_subset.h"

namespace gamepad {

/// directional is a subset very specific, but ubiquitous
/// so it's standard interface of gamepad template class
/// refer to base classes for subset semantics
struct directional: public static_subset<4>
{
  enum directions { up, down, left, right };
  typedef static_subset<4> base_type;
  
  directional(uint8_t up_id, uint8_t down_id, uint8_t left_id, uint8_t right_id, buttonset& aparent)
  : directional(up_id, down_id, left_id, right_id, aparent, aparent.get_id())
  {}
  directional(uint8_t up_id, uint8_t down_id, uint8_t left_id, uint8_t right_id,
              buttonset& aparent, uint8_t id)
  : base_type(4,aparent,id)
  {
    buttons_ids[directions::up]=up_id;
    buttons_ids[directions::down]=down_id;
    buttons_ids[directions::left]=left_id;
    buttons_ids[directions::right]=right_id;
  }

  virtual bool is_up() const {
    return button_is_pressed(directions::up);
  }
  virtual bool up_has_changed() {
    return button_state_has_changed(directions::up);
  }
  virtual bool is_down() const {
    return button_is_pressed(directions::down);
  }
  virtual bool down_has_changed() {
    return button_state_has_changed(directions::down);
  }
  virtual bool is_left() const {
    return button_is_pressed(directions::left);
  }
  virtual bool left_has_changed() {
    return button_state_has_changed(directions::left);
  }
  virtual bool is_right() const {
    return button_is_pressed(directions::right);
  }
  virtual bool right_has_changed() {
    return button_state_has_changed(directions::right);
  }

  virtual void set_up(bool bs) {
    set_button_state(directions::up, bs);
  }
  virtual void set_down(bool bs) {
    set_button_state(directions::down, bs);
  }
  virtual void set_left(bool bs) {
    set_button_state(directions::left, bs);
  }
  virtual void set_right(bool bs) {
    set_button_state(directions::right, bs);
  }
};

}
#endif // _DIRECTIONAL_H
