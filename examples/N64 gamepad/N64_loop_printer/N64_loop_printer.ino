// #define _GAMEPAD_SINGLEPLAYER // before include // if you din't install LinkedList yet, define this
// #include "N64_gamepad_usb.h" // if your board is USB enabled you can test this
#include "N64_gamepad.h"
// this defines multiplayer and gamepad interfaces/classes
// and implementation for N64 gamepad protocol

// using gamepad;
// using gamepad::SNES_gamepad;
// using gamepad::SNES_multiplayer;

// available pins for N64 controller interface
// on 'pro micro' (ATmega32U4): 2,3,4,6 [tested]
// on general arduino boards: 0,1,2,3,4,5,6,7
const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;

// if your board is USB enabled you can test these // print and loop AND (OO) joystick USB HID
//N64_hid p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
//N64_hid p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
// available IDs are 1..4 [for USB HID]

gamepad::N64_gamepad p1(1, N64_PIN1);
gamepad::N64_gamepad p2(2, N64_PIN2);
// [available IDs are any [general]

gamepad::N64_multiplayer multi(p1, p2); // just comment p2 and multiplayer stuff you are singleplayer

void setup() {
  Serial.begin(9600);
}

const int VERBOSE_LEVEL = 1;
  /**
    verbose:
    0: print buttonset buttons as bits
    1: same as before, but with spaces between bytes
    2: print all '{ button_name: button_state; ...}'
    3: print all 'button_name:\t\tbutton_state\n'
  */

void loop() {
  multi.read();

  //p1.read(); // call this if you are singleplayer
  //p2.read();
  // if multi, don't call these directly, mutiplayer class already does it on it's read()

  //multi.players.get(0)->print(); Serial.print("\t");
  //multi.players.get(1)->print(); Serial.println();

  if(VERBOSE_LEVEL>=3)
    Serial.print("p1:\n");
  else
    Serial.print("p1:\t");
  
  p1.print(VERBOSE_LEVEL); // print p1
  
  if(VERBOSE_LEVEL>=2)
    Serial.print("\n");
  else
    Serial.print("\t");
    
  if(VERBOSE_LEVEL>=3)
    Serial.print("p2:\n");
  else
    Serial.print("p2:\t");
  
  p2.print(VERBOSE_LEVEL); // print p2
  
  if(VERBOSE_LEVEL>=2)
    Serial.print("\n\n");
  else
    Serial.print("\n");

}