/* / **************************************
simpleMidiOutput

Control a PWM output with MIDI messages and a Teensy
the MIDI velocity message (0:127) is mapped linearly to a PWM output
uses Teensyduino: https://www.pjrc.com/teensy/teensyduino.html

variables:
myOutput - which pin to toggle
midiNote - determines the MIDI note that will control the output
MIDI velocity is mapped to PWM rate to control motor power

by Jiffer Harriman - modular-muse.com
************************************** */

// use PWM pin 3
int myOutput = 3;

// myOutput will by controlled by midiNote
int midiNote = 60; // MIDI note 60 (a.k.a. C-3)

// Teensy MIDI callbacks
void OnNoteOn(byte channel, byte note, byte velocity);
void OnNoteOff(byte channel, byte note, byte velocity);


void setup() {
  // initialize output pins
  pinMode(myOutput, OUTPUT);

  // init MIDI callbacks
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
}

// main loop
void loop() {
  // flush the MIDI read buffer
  while (usbMIDI.read()) {
  }

  // nothing to do here...
  // it's all in the callbacks below
}


// When a new "note on" message is recieved
void OnNoteOn(byte channel, byte note, byte velocity) {

  // check if the note is the one we care about
  if (note == midiNote) {

    // bit shift 7 bits to 12
    analogWrite(myOutput, velocity << 5); // this is the same as map(velocity, 0, 127, 0, 4095)

  }
}

// do nothing on "note off" messages since they are
// automatically turned off after onTime
void OnNoteOff(byte channel, byte note, byte velocity) {
  // check if the note is the one we care about
  if (note == midiNote) {

    // turn it off
    analogWrite(myOutput, 0);
  }
}





