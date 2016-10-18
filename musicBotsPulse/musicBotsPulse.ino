/* / **************************************
musicBotsPulse
Pulse the PWM pins specified in pins[] when 
MIDI "note on" messages are received
automatically turn off after set amount of time 

Use with 5 channel modular-muse motor driver board:
https://oshpark.com/shared_projects/uq5EsZsM
Made for a Teensy LC

or a 4 channel modular-muse motor driver board and some jumper wires
https://oshpark.com/shared_projects/1uvL8iDM

NOTE: this code uses Teensyduino:
https://www.pjrc.com/teensy/teensyduino.html

variables:
pins[] - this array determines output pins to use (assumes PWM)
lowNote - determines the starting MIDI note for the first motor channel, the rest count up one note per channel
onTime - length time motor is pulsed on (in ms)
MIDI velocity is mapped to PWM rate to control motor power

by Jiffer Harriman - modular-muse.com
************************************** */

// use PWM pins 3, 4, 6, 9, 10
int pins[] = {3, 4, 6, 9, 10};
#define numNotes 5

// motor channel 0 will be triggered by lowNote
// motor channel 1 will be triggered by lowNote + 1 etc.
int lowNote = 60; // MIDI note 60 (a.k.a. C-3)

// how long to pulse in ms
unsigned int onTime = 10;

// keep track of the last time a motor channel (note) was triggered
unsigned long lastHit[numNotes];

// keep track if a note is currently on (high) or if it has been released
bool noteHigh[numNotes];

// Teensy MIDI callbacks
void OnNoteOn(byte channel, byte note, byte velocity);
void OnNoteOff(byte channel, byte note, byte velocity);


void setup() {

  // initialize output pins to control the motors
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // no notes are on to start
  for (int i = 0; i < numNotes; i++) {
    noteHigh[i] = false;
    lastHit[i] = 0;
  }

  // init MIDI callbacks
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
}

// main loop
void loop() {
  // flush the MIDI read buffer
  while (usbMIDI.read()) {
  }

  // for all motor driver pins
  // check if onTime since lastHit has elapsed
  for (int i = 0; i < numNotes; i++) {
    // if so, turn the motor off
    if (noteHigh[i] && (millis() - lastHit[i] > onTime)) {
      noteHigh[i] = false;
      analogWrite(pins[i], 0);
    }
  }
}


// When a new "note on" message is recieved
void OnNoteOn(byte channel, byte note, byte velocity) {
  
  // check if the note is in range of our possible motor outputs
  if (note >= lowNote && note < lowNote + numNotes) {
    // adjust for note # offset
    int notePin = note - lowNote;
    
    // bit shift 7 bits to 12
    analogWrite(pins[notePin], velocity << 5);
    
    // keep track of which note is high and reset the time since the lastHit to now
    noteHigh[note] = true;
    lastHit[note] = millis();
  }

}

// do nothing on "note off" messages since they are 
// automatically turned off after onTime
void OnNoteOff(byte channel, byte note, byte velocity) {

}





