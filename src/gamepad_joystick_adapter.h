#ifndef _GAMEPAD_JOYSTICK_ADAPTER_H
#define _GAMEPAD_JOYSTICK_ADAPTER_H

// adapter class for this cool USB HID joystick library:
// https://github.com/MHeironimus/ArduinoJoystickLibrary

// #include "Joystick.h" // include ArduinoJoystickLibrary BEFORE include this file


#include "active_gamepad.h"

namespace gamepad {


/*

	Joystick_(
		uint8_t hidReportId = JOYSTICK_DEFAULT_REPORT_ID,
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


// adapter class pattern
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
    
	/*
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
	*/


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



// you can include "N64_gamepad.h" and define this macro BEFORE include this file
// for out-of-the box Nintendo 64 controller adapter functionality (working great on Projetc 64)
#ifdef _GAMEPAD_DEFINE_N64_HID

struct N64_hid: public gamepad_joystick<N64_gamepad> {
    typedef gamepad_joystick<N64_gamepad> gamepad_base;
    joystick_type usb_joystick;
    
    N64_hid(uint8_t id, uint8_t N64_pin=3, bool init=true)
      : usb_joystick(joystick_type(id+2, JOYSTICK_TYPE_JOYSTICK, N_BUTTONS, 0,
                                   true, true, false, false, false, false, false, false, false, false, false)),
        gamepad_base(N64_gamepad(id, N64_pin, false), usb_joystick)
    {
      if(init)  AndrewBrownInitialize();
      
      this->usb_joystick.setXAxisRange(-127, 127);
      this->usb_joystick.setYAxisRange(-127, 127);
    }

    virtual void action_any_button_changed() {
      this->usb_joystick.setXAxisRange(analog_range.xmin, analog_range.xmax);
      this->usb_joystick.setXAxisRange(analog_range.ymin, analog_range.ymax);
      this->usb_joystick.setXAxis(get_x());//*1.05);//_cal()*127);
      this->usb_joystick.setYAxis(get_y());//*1.05);//_cal()*127);
      // really, I still don't know what is the "most default" behavior for a usb joystick that is also a N64 controller...
      gamepad_base::action_any_button_changed();
    }
};
#endif // _GAMEPAD_DEFINE_N64_HID



#ifdef _GAMEPAD_DEFINE_SNES_HID

struct SNES_hid: public gamepad_joystick<SNES_gamepad> {
    typedef gamepad_joystick<SNES_gamepad> gamepad_base;
    joystick_type usb_joystick;
    
    SNES_hid(uint8_t id, uint8_t data_pin = 7, uint8_t clock_pin = 4, uint8_t latch_pin = 5)
      : usb_joystick(joystick_type(id+2, JOYSTICK_TYPE_JOYSTICK, N_BUTTONS, 0,
                                   false, false, false, false, false, false, false, false, false, false, false)),
        gamepad_base(SNES_gamepad(id, data_pin, clock_pin, latch_pin), usb_joystick)
    {}
};
#endif // _GAMEPAD_DEFINE_SNES_HID



}
#endif // _GAMEPAD_JOYSTICK_ADAPTER_H


