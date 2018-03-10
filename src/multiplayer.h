#ifndef _MULTIPLAYER_H
#define _MULTIPLAYER_H

#include <LinkedList.h>

namespace gamepad {
  
/// multiplayer class is not a gamepad, it is a collection of gamepads (simplest)
/**
  if the gamepad protocol you're implementing is multiplayer compatible, then:
  *you must* friend this class instantiated for your <gamepad_type> inside gamepad_type definition
  (as it uses read_imp and latch protected interface). see SNES_gamepad.h for an example
*/
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

  /// please override this or other (virtual) methods for each specific
  /// gamepad protocol supporting multiplayer
  /**
    default implementation (as in this multiplayer base):
    
    virtual void read() // VIRTUAL
        // but I HOPE next methods will suffice, and it'll be not necessary to override this
    `-> 
        // D E F A U L T  B E H A V I O R
        void read_all_without_interrupts() // NOT virtual (override previous)
        {
          action_before_read_all();
          noInterrupts();             // latch and read all on the same batch
          latch_all_read_imp_all();   // (it's not necessarily on the same latch (it depends on protocol),
          interrupts();               // but at least without interruptions in the middle)
          action_after_read_all();
        }
        `-> 
            virtual void latch_all_read_imp_all() // VIRTUAL
                // oh, now this is important
                // this function determines the ORDER of the commands
                // and reading will proceed between controllers and MCU, so: CONSIDER OVERRIDE THIS.
            `-> 
                // D E F A U L T  B E H A V I O R 
                void latch_AND_read_imp_all() // NOT virtual (override previous)
                {
                  for(uint8_t p = 0; p<players.size(); ++p) {
                    gamepad_pointer pp = this->players.get(p); // timing is crucial
                    pp->latch();     // latch and read EACH controller
                    pp->read_imp();  // these are protected, so please friend multiplayer<G> inside your G def.
                  }
                }
                // so, we consider (default) that latch and read_imp for each controller MUST be done together.
                // sometimes we don't have time even to dereference (again) a pointer...
    
                and this is so true for N64 controllers, so this default is great for them, see N64_gamepad.h
                
                SNES_multiplayer other way, has more than one communication pin, and share part of then with other
                controllers. so an specific derivative class had to be done. AND the most convenient behavior
                of virtual void latch_all_read_imp_all() for THIS architecture (my imp.) is
                    latch_all_THEN_read_imp_all()
                    [not latch_AND_read_imp_all() as in this multiplayer:: default implementation]
                
                
    if your protocol's architecture is different from this in anyway,
    override these (protected) virtual methods wherever makes sense
    (you have some options (inline methods) you can utilize from this class)
    
    virtual void read() interface is ultimately the important thing in the whole library (for user),
    as be gamepad or multiplayer template (virtual tabled) classes
    
    gamepad is only an abstract class (interface/template pattern), but library provides
    plenty of concrete options out-of-the-box
    
    so multiplayer class relies on these gamepad functionalities, and provides a structured way of
    commanding multiple controllers without waste, making latches and readings most close together possible
    
    that improves not only semantics (also essential),
    but these specific implementations showed to be the really margin between
    
        your latency-related-excuses-for-gameovers
                        vs
        a great multiplayer gameplay experience
    
    the whole point, many times too subtle to be immediately noticed, is that all controllers must be read,
    in the most precise possible sense of this: at same time. <- this is fair.
    
  */
  virtual void read() {
    //naive_read_all(); // maybe this can be safer in some cases, who knows
    read_all_without_interrupts();
  }
  
  /// only semantics (it's inline, practically free)
  void read_all() {
    read();
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
  
  /// no, please no
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
    latch_AND_read_imp_all(); // possible paths
    //latch_all_THEN_read_imp_all();
  }

  /// latch AND read all [not virtual, see latch_all_read_imp_all()]
  void latch_AND_read_imp_all() {
    for(uint8_t p = 0; p<players.size(); ++p) {
      gamepad_pointer pp = this->players.get(p);
      pp->latch();
      // oww damn it, (only 1 instruction) pointer be dereferenced (again)
      // in this middle caused N64 protocol to fail waw
      pp->read_imp();
    }
  }  
  
  /// latch_all THEN read_all [not virtual, see latch_all_read_imp_all()]
  void latch_all_THEN_read_imp_all() {
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



