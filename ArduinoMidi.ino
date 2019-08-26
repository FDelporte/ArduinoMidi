#include "MIDIUSB.h"
#include "PitchToNote.h"

const byte MIDI_CHANNEL = 0;
const int MINIMAL_VALUE_NEEDED = 200;
const int MAXIMUM_PLAY_TIME = 90;

boolean padActive[6] = {0,0,0,0,0,0};
int padPlayTime[6] = {0,0,0,0,0,0};
byte padNote[6] = {NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4}; 

int pinValue = 0;

void setup() {
  Serial.begin(57600);
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
        sendNote(true, padNote[pin], 64);

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

void sendNote(boolean on, byte note, byte velocity) {
  Serial.print(on ? "ON:": "OFF:");
  Serial.print(" Note: ");
  Serial.print(note);
  Serial.print(", Velocity: ");
  Serial.println(velocity);
  
  midiEventPacket_t noteOn = {on ? 0x09 : 0x08, (on ? 0x90 : 0x80) | MIDI_CHANNEL, note, velocity};
  MidiUSB.sendMIDI(noteOn);
}
