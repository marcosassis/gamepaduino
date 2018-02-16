#ifndef _GAMEPAD_JOYSTICK_HID_H
#define _GAMEPAD_JOYSTICK_HID_H

#include "gamepad.h"
#include "Joystick.h"


template<class gamepad_type>
class gamepad_joystick: public active_gamepad<gamepad_type> {
  public:

    typedef active_gamepad<gamepad_type> gamepad_base;
    typedef gamepad_type gamepad_t;
    typedef Joystick_ joystick_type;
    joystick_type usb_joystick;
    // this is intended to be public, so the user can directly set keys - still has its 'const getter'

    //gamepad_joystick(const gamepad_t& base, const joystick_type& usb_joystick)
    //  : gamepad_base(base), usb_joystick(usb_joystick)
    //{
    //  this->usb_joystick.begin(false); // this autosend is not our policy
    //}
    
    gamepad_joystick(const gamepad_base& base)
      : gamepad_base(base), usb_joystick(Joystick_(base.get_id()+2, JOYSTICK_TYPE_GAMEPAD, base.get_n_buttons(), 0,
                                                 //  false, false, false, false, false, false, false, false, false, false, false))
                                                  true, true, false, false, false, false, false, false, false, false, false))
    {
      // uint8_t hidReportId - Default: 0x03 - Indicates the joystick's HID report ID.
      // Do not use 0x01 or 0x02 as they are used by the built-in Arduino Keyboard and Mouse libraries.

      //this->usb_joystick.setXAxisRange(-127, 127);
      //this->usb_joystick.setYAxisRange(-127, 127);
      this->usb_joystick.begin(false); // this autosend is not our policy
    }


    const joystick_type& get_usb_joystick() const {
      return usb_joystick;
    }

    virtual void action_any_button_changed() {
      gamepad_base::action_any_button_changed();
      usb_joystick.sendState();
    }

    virtual void action_button_changed(uint8_t i) {
      if (get_button_state(i)) {
        usb_joystick.pressButton(i);
#ifdef _GAMEPAD_JOYSTICK_DEBUG
        Serial.print(get_id()); Serial.print("\t"); 
        Serial.print(i); Serial.print("\t"); 
        Serial.println("usb_joystick.pressButton");
#endif
      }
      else {
        usb_joystick.releaseButton(i);
#ifdef _GAMEPAD_JOYSTICK_DEBUG
        Serial.print(get_id()); Serial.print("\t"); 
        Serial.print(i); Serial.print("\t"); 
        Serial.println("usb_joystick.releaseButton");
#endif
      }
    }
};

#endif // _GAMEPAD_JOYSTICK_HID_H


