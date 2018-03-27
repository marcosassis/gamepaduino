#ifndef _META_MIDI_SERIAL_SENDER_H
#define _META_MIDI_SERIAL_SENDER_H

#include "midi_interface.h"

namespace meta { namespace midi {

template<class SerialType=DefaultHardwareSerial>
struct midi_serial_sender: public midi_send_interface
{
  typedef SerialType serial_type;
  serial_type& the_serial_writer;
  
  midi_serial_sender(serial_type& theSerialWriter)
  : the_serial_writer(theSerialWriter)
  {}
  
  /// see midi_interface for high (user) level methods
  /// 
  /// (don't check first bits of data bytes)
  /// 
  ///                        status           data byte        data byte
  ///                        1_______         0_______         0_______
  virtual void send(status_byte data1, data_byte data2, data_byte data3) {
    the_serial_writer.write(data1);
    the_serial_writer.write(data2);
    the_serial_writer.write(data3);
  }
  
  virtual void flush() {
    the_serial_writer.flush();
  }
};

}}

#endif//_META_MIDI_SERIAL_SENDER_H