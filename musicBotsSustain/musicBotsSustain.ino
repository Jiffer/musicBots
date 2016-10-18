/* / **************************************
musicBotsSustain
Turn on the PWM pins specified in pins[] when 
MIDI "note on" messages are received. Turn off when 
corresponding "note off" message is received.

For use with 5 channel modular-muse motor driver board:
https://oshpark.com/shared_projects/uq5EsZsM
Made for a Teensy LC

or a 4 channel modular-muse motor driver board and some jumper wires
https://oshpark.com/shared_projects/1uvL8iDM

NOTE: this code uses Teensyduino:
https://www.pjrc.com/teensy/teensyduino.html

variables:
pins[] - this array determines output pins to use (assumes PWM)
lowNote - determines the starting MIDI note for the first motor channel, 
          the rest count up one note per channel
MIDI velocity is mapped to PWM rate to control motor power/speed

by Jiffer Harriman - modular-muse.com
************************************** */

// use PWM pins 3, 4, 6, 9, 10
int pins[] = {3, 4, 6, 9, 10};
#define numNotes 5

// motor channel 0 will be triggered by lowNote
// motor channel 1 will be triggered by lowNote + 1 etc.
int lowNote = 60; // MIDI note 60 (C-2) // change to use different note triggers

// Teensy MIDI callbacks
void OnNoteOn(byte channel, byte note, byte velocity);
void OnNoteOff(byte channel, byte note, byte velocity);

// initialization
void setup() {
  // output pins
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], OUTPUT);
  }
  
  // init MIDI callbacks
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
}

// main loop
void loop() {
  while (usbMIDI.read()) {
  }
  
  // nothing to do here...

}

// OnNoteOn() callback
// When a new "note on" message is recieved
void OnNoteOn(byte channel, byte note, byte velocity) {
  
  // check if the note is in range for our possible motor outputs
  if (note >= lowNote && note < lowNote + numNotes) {
    // adjust for note # offset
    int notePin = note - lowNote;
    // linear map from MIDI's 7 bits of resolution to Teensy's 12
    analogWrite(pins[notePin], velocity << 5 ); 
  }

}

// OnNoteOff() callback
// When a new "note off" message is received
void OnNoteOff(byte channel, byte note, byte velocity) {
  
  // check that it is in the range
  if (note >= lowNote && note < lowNote + numNotes) {
    int notePin = note - lowNote; // offset
    analogWrite(pins[notePin], 0); // turn it off
  }
}





