#ifndef _META_GAMEPAD_MIDI_NOTEMAP_H
#define _META_GAMEPAD_MIDI_NOTEMAP_H

#include "midi_controller.h"
#include "notemap.h"

namespace meta { namespace midi { namespace luthier {

template<class padbase>
struct gamepad_midi_notemap: public midi_controller, public notemap
{  
  typedef padbase pad_base;
  typedef pad_base pad_type;
  
  

};
  
}}}
#endif // _META_GAMEPAD_MIDI_NOTEMAP_H