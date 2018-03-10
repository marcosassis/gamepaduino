#ifndef _GAMEPAD_JOYSTICK_ADAPTER_H
#define _GAMEPAD_JOYSTICK_ADAPTER_H

// adapter class for this cool USB HID joystick library:
// https://github.com/MHeironimus/ArduinoJoystickLibrary

#include "active_gamepad.h"
#include "Joystick.h"

namespace gamepad {

/*
Joystick_(uint8_t hidReportId = JOYSTICK_DEFAULT_REPORT_ID,
          uint8_t joystickType = JOYSTICK_TYPE_JOYSTICK,
          uint8_t buttonCount = JOYSTICK_DEFAULT_BUTTON_COUNT,
          uint8_t hatSwitchCount = JOYSTICK_DEFAULT_HATSWITCH_COUNT,
          bool includeXAxis = true,
          bool includeYAxis = true,
          bool includeZAxis = true,
          bool includeRxAxis = true,
          bool includeRyAxis = true,
          bool includeRzAxis = true,
          bool includeRudder = true,
          bool includeThrottle = true,
          bool includeAccelerator = true,
          bool includeBrake = true,
          bool includeSteering = true);
*/

/// adapter class pattern
template<class gamepad_type>
struct gamepad_joystick: public active_gamepad<gamepad_type> {
  
    typedef active_gamepad<gamepad_type> gamepad_base;
    typedef gamepad_type gamepad_t;
    typedef Joystick_ joystick_type;

  protected:
    joystick_type& usb_joystick;

  public:

    gamepad_joystick(const gamepad_t& base, joystick_type& joystick_instance)
      : gamepad_base(base), usb_joystick(joystick_instance)
    {
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
      if (get_button_state(i))
        usb_joystick.pressButton(i);
      else
        usb_joystick.releaseButton(i);
    }
};


#ifdef _GAMEPAD_DEFINE_N64_HID
/// out-of-the box Nintendo 64 controller adapter functionality (working great on Projetc 64)
struct N64_hid: public gamepad_joystick<N64_gamepad> {
    typedef gamepad_joystick<N64_gamepad> gamepad_base;
    joystick_type usb_joystick;

    N64_hid(const N64_gamepad& other)
    : N64_hid(other.get_id(), other.get_N64_pin(), false)
    {}
    
    N64_hid(uint8_t id, uint8_t N64_pin=3, bool init=true)
      : usb_joystick(joystick_type(id+2, JOYSTICK_TYPE_JOYSTICK, N_BUTTONS, 0,
                                   true, true, false, false, false, false, false, false, false, false, false)),
        gamepad_base(N64_gamepad(id, N64_pin, true), usb_joystick)
    {
      this->usb_joystick.setXAxisRange(-127, 127);
      this->usb_joystick.setYAxisRange(-127, 127);
    }

    virtual void action_any_button_changed() {
      this->usb_joystick.setXAxisRange(analog_range.xmin, analog_range.xmax);//this is gambiarra, soon (todo) will be generic
      this->usb_joystick.setXAxisRange(analog_range.ymin, analog_range.ymax);//todo: investigate more this behavior
      this->usb_joystick.setXAxis(get_x());//*1.05);//_cal()*127);
      this->usb_joystick.setYAxis(get_y());//*1.05);//_cal()*127);
      // really, I still don't know what is the "most default" behavior for a usb joystick that IS ALSO a N64 controller...
      gamepad_base::action_any_button_changed();
    }
};
#endif // _GAMEPAD_DEFINE_N64_HID


#ifdef _GAMEPAD_DEFINE_SNES_HID
/// out-of-the box Super Nintendo controller adapter functionality (working great on ZSNES)
struct SNES_hid: public gamepad_joystick<SNES_gamepad> {
    typedef gamepad_joystick<SNES_gamepad> gamepad_base;
    joystick_type usb_joystick;
    
    SNES_hid(const SNES_gamepad& other)
      : SNES_hid(other.get_id(),other.get_data_pin(),other.get_clock_pin(),other.get_latch_pin())
    {}
    
    SNES_hid(uint8_t id, uint8_t data_pin = 7, uint8_t clock_pin = 4, uint8_t latch_pin = 5)
      : usb_joystick(joystick_type(id+2, JOYSTICK_TYPE_GAMEPAD, N_BUTTONS, 0,
                                   false, false, false, false, false, false, false, false, false, false, false)),
        gamepad_base(SNES_gamepad(id, data_pin, clock_pin, latch_pin), usb_joystick)
    {} // this is almost a typedef with constructors mapping
};
#endif // _GAMEPAD_DEFINE_SNES_HID


}
#endif // _GAMEPAD_JOYSTICK_ADAPTER_H


