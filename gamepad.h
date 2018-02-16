#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#include "Arduino.h"

//TODO
//#define SETBIT(port, abit) ( (port) |= (1 << (abit)) )
//#define CLEARBIT(port, abit) ( (port) &= ~(1 << (abit)) )
//#define ISSETBIT(value, bit) ((value) & _BV(bit))
//#define ISCLEARBIT(value, bit) (~value) & _BV(bit)))



int get_id_by_name(String aname, String* names, unsigned max_names);

template<typename Type> void print_bits(Type var) {
  bool bit;
  for(int i=(sizeof(Type)*8)-1; i>=0; --i) {
    bit = var & (Type(1) << i);
    Serial.print(unsigned(bit));
  }
}

///// BUTTONSET
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
    // forcing positive logic for library consistency and understandability
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

};//end buttonset



///// SUBSET
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
  virtual bool any_button_state_has_changed() const {
    return parent.any_button_state_has_changed();
  }
  virtual bool button_state_has_changed(uint8_t subsetindex) const {
    return parent.button_state_has_changed(get_button_parent_id(subsetindex));
  }
  virtual int get_button_id_by_name(String aname) const {
    int return_i = parent.get_button_id_by_name(aname);
    if(return_i>=0) {
      for(uint8_t i=0; i<get_n_buttons(); ++i) {
        if(get_button_parent_id(i)==return_i) {
          return_i = i;
          break;
        }
      }
    }
    return return_i;
  }
  virtual uint8_t get_button_parent_id(uint8_t subsetindex) const = 0;

};//end subset


////// STATIC_SUBSET
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
};//end static_subset



////// DIRECTIONAL
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
};//end directional

//TODO
////// ANALOG
/*
template<typename value_type>
struct analog: public directional {
  typedef value_type value_t;

  //analog(const directional& ids)
};//end analog
*/

class gamepad: public buttonset
{
protected:

  directional* dpads;
  uint8_t n_dpads;

  virtual void latch() = 0;
  virtual void read_imp() = 0;

  virtual void action_before_read() {}
  virtual void action_after_read() {}

public:

  gamepad(uint8_t id, uint8_t n_buttons, directional* dpads=NULL, uint8_t n_dpads=0)
  : buttonset(id,n_buttons), dpads(dpads), n_dpads(n_dpads)
  {}

  virtual void read() {
    action_before_read();
    noInterrupts();
    latch();
    read_imp();
    interrupts();
    action_after_read();
  }

  virtual const directional* get_dpad(uint8_t i=0)
  {
    if(dpads && n_dpads && i<n_dpads)
      return dpads+i;

    return dpads;
  }
  virtual void set_dpad(directional& newdpad, uint8_t i=0)
  {
    if(dpads && n_dpads && i<n_dpads)
      dpads[i] = newdpad;
  }
};



// bit representation for gamepads that use it
template<typename uint_type>
class bit_gamepad: public gamepad
{
protected:
  uint_type buttons = 0; // each bit is a button, positive logic
  uint_type buttons_last = 0;

  virtual void action_before_read() {
    buttons_last = buttons;
    buttons = 0;
  }

public:

  typedef uint_type uint_t;

  //bit_gamepad(bit_gamepad& other)
  //: bit_gamepad(other.id, other.n_buttons, other.dpads, other.n_dpads)
  //{}

  bit_gamepad(uint8_t id, uint8_t n_buttons, directional* dpads=NULL, uint8_t n_dpads=0)
  : gamepad(id, n_buttons, dpads, n_dpads)
  {}

  virtual bool get_button_state(uint8_t index) const {
    //return bit_is_set(buttons, index);
    return buttons & (uint_t(1) << index);
  }

  virtual void set_button_state(uint8_t index, bool bs) {
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
  
  virtual void print() {
    print_bits(buttons);
  }
};



template<class gamepad_type>
struct active_gamepad: public gamepad_type {
  
  typedef gamepad_type gamepad_t;

  active_gamepad(const gamepad_t& base) // concrete classes should have deep copy semantics
  : gamepad_t(base)
  {}

  virtual void action_after_read() { // or even this one, maybe
    gamepad_t::action_after_read();
    //Serial.println(get_id());
    if(any_button_state_has_changed()) // if this question is fast (as with bit_gamepad) = ok this imp.
      action_any_button_changed();
  }
  virtual void action_any_button_changed() { // maybe you'd override this
    for(uint8_t i=0; i<get_n_buttons(); ++i) // if this for is really needed = ok this imp.
      if(button_state_has_changed(i))
        action_button_changed(i);
  }
  virtual void action_button_changed(uint8_t i) {} // you have to override this one or one of the others
};


#endif // _GAMEPAD_H


