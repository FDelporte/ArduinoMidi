#include "MIDIUSB.h"
#include "PitchToNote.h"

const byte MIDI_CHANNEL = 1;
const int MINIMAL_VALUE_NEEDED = 50;
const int MAXIMUM_PLAY_TIME = 90;
const int VELOCITY = 127;

boolean padActive[6] = {0,0,0,0,0,0};
int padPlayTime[6] = {0,0,0,0,0,0};
byte padNote[6] = {NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4}; 

int pinValue = 0;

void setup() {
  // Nothing needed here
}

void loop() {
  for (int pin = 0; pin <= 5; pin++) {
    pinValue = analogRead(pin); 

    if((pinValue > MINIMAL_VALUE_NEEDED)) { 
      //Serial.print("Pin ");
      //Serial.print(pin);
      //Serial.print(", value: ");
      //Serial.println(pinValue);   
    
      if((padActive[pin] == false)) {
        sendNote(true, padNote[pin], VELOCITY);

        padPlayTime[pin] = 0;
        padActive[pin] = true;
      } else {
        padPlayTime[pin] = padPlayTime[pin] + 1;
      }
    } else if((padActive[pin] == true)) {
      padPlayTime[pin] = padPlayTime[pin] + 1;
      
      if(padPlayTime[pin] > MAXIMUM_PLAY_TIME) {
        padActive[pin] = false;
        sendNote(false, padNote[pin], 0); 
      }
    }
  } 
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void sendNote(boolean on, byte note, byte velocity) {
  // First parameter is the event type (0x09 = note on, 0x08 = note off).
  // Second parameter is note-on/note-off, combined with the channel.
  // Channel can be anything between 0-15. Typically reported to the user as 1-16.
  // Third parameter is the note number (48 = middle C).
  // Fourth parameter is the velocity (64 = normal, 127 = fastest).
  byte event = on ? 0x09 : 0x08;
  byte onOff = on ? 0x90 : 0x80;
  byte onOffChannel = onOff | MIDI_CHANNEL;

  Serial.print("Event: ");
  Serial.print(event, HEX);
  Serial.print(", On/Off: ");
  Serial.print(onOff, HEX);
  Serial.print(", OnOffChannel: ");
  Serial.print(onOffChannel, HEX);
  Serial.print(", Note: ");
  Serial.print(note, HEX);
  Serial.print(", Velocity: ");
  Serial.println(velocity, HEX);
  
  midiEventPacket_t noteOn = {event, onOffChannel, note, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}
