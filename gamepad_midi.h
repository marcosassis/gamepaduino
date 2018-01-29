/* SOON

#ifndef _GAMEPAD_MIDI_H
#define _GAMEPAD_MIDI_H

#include "gamepad.h"


//#define SETBIT(port, abit) ( (port) |= (1 << (abit)) )
//#define CLEARBIT(port, abit) ( (port) &= ~(1 << (abit)) )
//#define ISSETBIT(value, bit) ((value) & _BV(bit))
//#define ISCLEARBIT(value, bit) (~value) & _BV(bit)))




int get_id_by_name(String aname, String* names, unsigned max_names);



class gamepad {
    uint8_t n_buttons;
    const uint8_t id;


  protected:

    virtual void latch() = 0;

  public:

    gamepad(uint8_t id, uint8_t n_buttons)
      : id(id), n_buttons(n_buttons)
    {}

    virtual bool get_button_state(uint8_t index) const = 0;
    virtual void set_button_state(uint8_t index, bool bs) = 0;

    virtual bool button_state_has_changed(uint8_t index) const = 0;

    virtual String* get_button_names() const = 0;

    virtual void read() = 0;
    /*
      {
      latch();
      for(uint8_t i = 0; i<n_buttons; ++i)
        set_button_state(i, read_next_bit() );
      }
    /

    uint8_t get_id() const {
      return id;
    }
    uint8_t get_n_buttons() const {
      return n_buttons;
    }

    virtual int get_button_id_by_name(String aname) const {
      return get_id_by_name(aname, get_button_names(), n_buttons);
    }
};




// bit representation for gamepads that use it
template<typename uint_type>
class bit_gamepad: public gamepad {

  protected:
    uint_type buttons = 0; // each bit is a button, positive logic
    uint_type buttons_last = 0;

    virtual void read_imp() = 0;

  public:

    bit_gamepad(uint8_t id, uint8_t n_buttons)
      : gamepad(id, n_buttons)
    {}

    virtual bool get_button_state(uint8_t index) const {
      //return bit_is_set(buttons, index);
      return buttons & (1 << index);
    }

    virtual void set_button_state(uint8_t index, bool bs) {
      if (bs)
        //setb(buttons,index);
        //SETBIT(buttons,index);
        buttons |= (1 << (index));
      else
        //clrb(buttons,index);
        //CLEARBIT(buttons,index);
        buttons &= ~(1 << (index));
    }

    virtual uint_type buttons_changed_mask() {
      return buttons_last ^ buttons;
    }

    virtual bool button_state_has_changed(uint8_t index) const {
      //return ((buttons_changed_mask()) & _BV(index));
      return buttons_changed_mask() & (1 << index);
    }

    virtual void read() {
      buttons_last = buttons;
      buttons = 0;
      noInterrupts();
      latch();
      read_imp();
      interrupts();
    }

    uint_type get_buttons() { // each bit is a button, positive logic
      return buttons;
    }
    uint_type get_buttons_last() {
      return buttons_last;
    }
};


class SNES_gamepad: public bit_gamepad<uint16_t> {
  uint8_t latch_pin;
  uint8_t clock_pin;
  uint8_t data_pin;

  protected:

    virtual void latch();
    virtual void read_imp();
    
  public:

    static const String names[12];

    SNES_gamepad(uint8_t id, uint8_t data_pin = 7, uint8_t clock_pin = 4, uint8_t latch_pin = 5)
      : bit_gamepad<uint16_t>(id, 12), latch_pin(latch_pin), clock_pin(clock_pin), data_pin(data_pin)
    {}

    virtual String* get_button_names() const {
      return names;
    }
};


#endif // _GAMEPAD_MIDI_H

*/
