#ifndef _GAMEPAD_JOYSTICK_HID_H
#define _GAMEPAD_JOYSTICK_HID_H

#include "gamepad.h"
#include "Joystick.h"


template<class gamepad_base_type>
class gamepad_joystick: public gamepad_base_type {
  public:

    typedef Joystick_ joystick_type;
    joystick_type usb_joystick;
    // this is intented to be public, so the user can directly set keys - still has its 'const getter'

    typedef gamepad_base_type gamepad_base;


    gamepad_joystick(const gamepad_base& base, const Joystick_& usb_joystick)
      : gamepad_base(base), usb_joystick(usb_joystick)
    {
      this->usb_joystick.begin(false); // this autosend is not our policy
    }
    
    gamepad_joystick(const gamepad_base& base)
      : gamepad_base(base), usb_joystick(Joystick_(base.get_id(), JOYSTICK_TYPE_GAMEPAD, base.get_n_buttons(), 0,
                                                   false, false, false, false, false, false, false, false, false, false, false))
                                                 // true, true, false, false, false, false, false, false, false, false, false)))
    {
      //this.usb_joystick.setXAxisRange(-127, 127);
      //this.usb_joystick.setYAxisRange(-127, 127);
      this->usb_joystick.begin(false); // this autosend is not our policy
    }

    const joystick_type& get_usb_joystick() const {
      return usb_joystick;
    }

    virtual void read() {
      gamepad_base::read();
      for (uint8_t i = 0; i < get_n_buttons(); ++i) {
        if (button_state_has_changed(i)) {
          if (get_button_state(i)) {
            usb_joystick.pressButton(i);
            //Serial.print(i); Serial.print("\t"); 
            //Serial.println("usb_joystick.pressButton");
          }
          else {
            usb_joystick.releaseButton(i);
            //Serial.print(i); Serial.print("\t"); 
            //Serial.println("usb_joystick.releaseButton");
          }
        }
      }
      usb_joystick.sendState();
    }
};



class SNES_hid: public gamepad_joystick<SNES_gamepad> {
  public:
    typedef gamepad_joystick<SNES_gamepad> gamepad_base;
    
    SNES_hid(uint8_t id, uint8_t data_pin = 7, uint8_t clock_pin = 4, uint8_t latch_pin = 5)
      : gamepad_base(SNES_gamepad(id, data_pin, clock_pin, latch_pin))
    {}
};


#endif // _GAMEPAD_JOYSTICK_HID_H
