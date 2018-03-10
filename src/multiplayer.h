#ifndef _MULTIPLAYER_H
#define _MULTIPLAYER_H

#include <LinkedList.h>

namespace gamepad {
  
/// multiplayer class is not a gamepad, it is a collection of gamepads (simplest)
/// friend this class instantiated for your <gamepad_type> inside gamepad_type definition
/// (as it uses read_imp and latch protected interface) see SNES_gamepad.h for an example
template<class gamepad_type>
struct multiplayer
{
  typedef gamepad_type gamepad_t;
  typedef gamepad_type* gamepad_pointer;
  typedef LinkedList<gamepad_pointer> gamepad_list_t;

  gamepad_list_t players;
  
  multiplayer(const gamepad_t& p1) {
    players.add(&p1);
  }
  multiplayer(const gamepad_t& p1, const gamepad_t& p2)
  : multiplayer(p1) {
    players.add(&p2);
  }
  multiplayer(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3)
  : multiplayer(p1,p2) {
    players.add(&p3);
  }
  multiplayer(const gamepad_t& p1, const gamepad_t& p2, const gamepad_t& p3, const gamepad_t& p4)
  : multiplayer(p1,p2,p3) {
    players.add(&p4);
  }

  /// please override this and/other methods for each specific
  /// gamepad protocol supporting multiplayer
  virtual void read() {
    //naive_read_all(); // unfortunately this is safer in generic
    read_all_without_interrupts();
  }
  
  void action_before_read_all() {
    for(uint8_t p = 0; p<players.size(); ++p)
      this->players.get(p)->action_before_read();
  }
    
  void action_after_read_all() {
    for(uint8_t p = 0; p<players.size(); ++p)
      this->players.get(p)->action_after_read();
  }
  
protected:
  
  void naive_read_all() {
    for(uint8_t i = 0; i<players.size(); ++i)
      players.get(i)->read();
  }

  /// don't override non-virtual members (ever), other points of extension are provided
  void read_all_without_interrupts() {
    action_before_read_all();
    noInterrupts();             // latch and read all on the same batch
    latch_all_read_imp_all();   // (it's not necessarily on the same latch,
    interrupts();               // but at least without interruptions in the middle)
    action_after_read_all();
  }
  /* gamepad::read is:
  virtual void read() {
    action_before_read();
    noInterrupts();
    latch();
    read_imp();
    interrupts();
    action_after_read();
  }
  */
  
  /// if your protocol is too time sensitive or
  /// it's architecture is different, override this
  /// or other methods of multiplayer (virtual) interface
  virtual void latch_all_read_imp_all() {
    latch_and_read_imp_all(); // possible paths
    //latch_all_then_read_imp_all();
  }

  /// latch AND read all
  void latch_and_read_imp_all() {
    for(uint8_t p = 0; p<players.size(); ++p) {
      gamepad_pointer pp = this->players.get(p);
      pp->latch();  
      pp->read_imp();  // ow damn it, (only) pointer dereferenced (again) in this middle cause N64 protocol to fail waw
    }
  }  
  
  /// latch_all THEN read_all
  void latch_all_then_read_imp_all() {
    latch_all();  
    read_imp_all();     
  }

  /// PLEASE derive from this class and override these methods for each
  /// multiplayer specific protocol (see SNES_multiplayer)  
  virtual void latch_all() {
    for(uint8_t p = 0; p<players.size(); ++p)
      this->players.get(p)->latch();   
  }
  virtual void read_imp_all() {
    for(uint8_t p = 0; p<players.size(); ++p)
      this->players.get(p)->read_imp();   
  }  

};

}
#endif //_MULTIPLAYER_H



