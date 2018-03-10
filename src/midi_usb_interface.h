#ifndef _MIDI_USB_INTERFACE_H
#define _MIDI_USB_INTERFACE_H

#include "midi_interface.h"
#include "MIDIUSB.h"

namespace meta { namespace midi {

/// https://www.midi.org/specifications/item/table-1-summary-of-midi-message
struct midi_usb_interface: public midi_send_interface
{
  union midi_event_packet_t {
    midiEventPacket_t midiusb_event;
    data_byte data[4];
  };
  
  midi_event_packet_t event;
  
  virtual void send(status_byte data1, data_byte data2, data_byte data3) {
     event = { voice_message_t(data1).midi_event_type(), // todo: generic for all different status (not only voice m.)
              data1, data2, data3 };
    MidiUSB.sendMIDI(event.midiusb_event);
  }
  
  virtual void flush() {
    MidiUSB.flush();
  }
};

}}

#endif//_MIDI_USB_INTERFACE_H
