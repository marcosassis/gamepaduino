#ifndef _META_MIDI_CONTROLLER_H
#define _META_MIDI_CONTROLLER_H

#include "../midi_proto/midi_send_interface.h"
//#include "../gamepad.h" // gamepad models this `pad_type` concept

namespace meta { namespace midi { namespace luthier {

template<class padbase>
struct midi_controller: public padbase
{  
  typedef padbase pad_base;
  typedef pad_base pad_type;
  
  midi_send_interface& midi_sender;
  
  midi_controller(pad_type const& base, midi_send_interface& _midi_instance)
    : pad_type(base), midi_sender(_midi_instance)
  {}
  
  midi_send_interface& midi_instance() {
    return _midi_;
  }

  /// most important functionss...
  ///
  /// """read everything then do stuff [then flush()]"""
  /// 
  inline virtual void read() {
    pad_type::read();
  }

  /// ... about this info flow =]
  /// 
  /// understand these both and you'll be blessed
  /// 
  virtual void flush() {
    midi_interface().flush();
  }
};



}}}
#endif // _META_MIDI_CONTROLLER_H