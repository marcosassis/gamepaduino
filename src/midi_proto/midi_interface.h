#ifndef _MIDI_INTERFACE_H
#define _MIDI_INTERFACE_H

#include "Arduino.h" // this can be more generic, but binary "literals" are useful for readability

namespace meta { namespace midi {

typedef uint8_t data_byte; // maybe union with int8_t
typedef uint8_t status_byte;

union voice_message_t {
  status_byte mmmm____;
  enum event {
//  Channel Voice Messages [nnnn = 0-15 (MIDI Channel Number 1-16)] 
    note_off        = B10000000,
    note_on         = B10010000,
    aftertouch      = B10100000,
    control_change  = B10110000
    // etc: todo
  };
  
  /// convert from data_byte mask out last 4 bits
  voice_message_t(data_byte datavm) {
    mmmm____ = datavm & B11110000;
  }
  
  data_byte midi_event_type() const {
    return mmmm____ >> 4;
  }
};

union channel_t {
  status_byte ____nnnn;
  enum {
    MIDI_channel_1,
    MIDI_channel_2,
    MIDI_channel_3,
    MIDI_channel_4,
    MIDI_channel_5,
    MIDI_channel_6,
    MIDI_channel_7,
    MIDI_channel_8,
    MIDI_channel_9,
    MIDI_channel_10,
    MIDI_channel_11,
    MIDI_channel_12,
    MIDI_channel_13,
    MIDI_channel_14,
    MIDI_channel_15
  };
  
  /// convert from data_byte mask out first 4 bits
  channel_t(data_byte datac) {
    ____nnnn = datac & B00001111;
  }
  
  status_byte assembly_status_byte(voice_message_t message) const {
    return message.mmmm____ |
                   ____nnnn;
  }
};


/// https://www.midi.org/specifications/item/table-1-summary-of-midi-message
struct midi_send_interface
{
  virtual void flush() = 0;
  
  virtual void send       (status_byte data1,       data_byte data2,          data_byte data3)      = 0;
  ///                                  status                 data byte                 data byte
  ///                                 [D7----D0]             [D7----D0]                [D7----D0]
  /// 
  /// channel voice messages
  /// 
  ///   [nnnn:                0..15
  ///    MIDI channel number: 1..16]
  ///                                 [    nnnn]             [        ]                [        ]
  ///                                  1000nnnn               0kkkkkkk                  0vvvvvvv
  virtual void note_off   (  channel_t  channel,    data_byte note_number,    data_byte velocity)
  {
    send( channel.assembly_status_byte(voice_message_t::note_off),
          note_number,
          velocity );
  }
  ///                                  1001nnnn               0kkkkkkk                  0vvvvvvv
  virtual void note_on    (  channel_t  channel,    data_byte note_number,    data_byte velocity)
  {
    send( channel.assembly_status_byte(voice_message_t::note_on),
          note_number,
          velocity );
  }
  
  //virtual void program_change(data_byte program_number,                           channel_byte channel_0_15)  {}
  //virtual void control_change(data_byte control_number, data_byte control_value,  channel_byte channel_0_15)  {}
  //virtual void pitch_bend     (int inPitchValue,                                    channel_byte channel_0_15)  {}
  //virtual void pitch_bend     (double inPitchValue,                                 channel_byte channel_0_15)  {}
  //virtual void poly_pressure  (data_byte note_number, data_byte inPressure, channel_byte channel_0_15) __attribute__ ((deprecated)) {}
  //virtual void after_touch    (                           data_byte pressure,       channel_byte channel_0_15)  {}
  //virtual void after_touch    (data_byte note_number,     data_byte pressure,       channel_byte channel_0_15)  {}
  //virtual void sys_ex         (unsigned inLength, const byte* inArray, bool inArrayContainsBoundaries = false)  {}
  //virtual void sendTimeCodeQuarterFrame(data_byte inTypeNibble, data_byte inValuesNibble);
  //virtual void sendTimeCodeQuarterFrame(data_byte inData);
  //inline void sendSongPosition(unsigned inBeats);
  //inline void sendSongSelect(data_byte inSongNumber);
  //inline void sendTuneRequest();
  //inline void sendRealTime(MidiType inType);
  //inline void beginRpn(unsigned inNumber,        channel_byte channel_0_15);
  //inline void sendRpnValue(unsigned inValue,            channel_byte channel_0_15);
  //inline void sendRpnValue(byte inMsb,                       byte inLsb,                channel_byte channel_0_15);
  //inline void sendRpnIncrement(byte inAmount,                     channel_byte channel_0_15);
  //inline void sendRpnDecrement(byte inAmount,                      channel_byte channel_0_15);
  //inline void endRpn(channel_byte channel_0_15);
  //inline void beginNrpn(unsigned inNumber,                     channel_byte channel_0_15);
  //inline void sendNrpnValue(unsigned inValue,                 channel_byte channel_0_15);
  //inline void sendNrpnValue(byte inMsb,                 byte inLsb,              channel_byte channel_0_15);
  //inline void sendNrpnIncrement(byte inAmount,                        channel_byte channel_0_15);
  //inline void sendNrpnDecrement(byte inAmount,                           channel_byte channel_0_15);
  //inline void endNrpn(channel_byte channel_0_15);
};

}}

#endif//_MIDI_INTERFACE_H
