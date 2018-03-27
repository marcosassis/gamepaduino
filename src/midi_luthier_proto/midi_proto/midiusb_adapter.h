#ifndef _META_MIDIUSB_ADAPTER_H
#define _META_MIDIUSB_ADAPTER_H

#include "midi_interface.h"
#include "MIDIUSB.h"

namespace meta { namespace midi {

/// adapter for MIDIUSB library with meta::midi:: midi_interface methods inherited
struct midiusb_sender: public midi_send_interface
{
  union midi_event_packet_t {
    midiEventPacket_t midiusb_event;
    data_byte data[4];
  };
  
  midi_event_packet_t event;
  
  virtual void send(status_byte data1, data_byte data2, data_byte data3) {
    // todo: make generic for all different status (not only voice m.)
    event = { voice_message_t(data1).midi_event_type(), data1, data2, data3 };
    MidiUSB.sendMIDI(event.midiusb_event);
  }
  
  virtual void flush() {
    MidiUSB.flush();
  }
};

}}

#endif//_META_MIDIUSB_ADAPTER_H