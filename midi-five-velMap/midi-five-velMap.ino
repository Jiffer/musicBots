
// use PWM pins 3, 4, 6, 9
int pins[] = {3, 4, 6, 8, 9}; // updated for v2 w/ pinout fix // old one uses 8, 9
#define numNotes 5

int lowNote = 60; // MIDI note 16 (E-2)
unsigned int onTime = 15;
int trimPin = A0;
boolean pulse = true;

void OnNoteOn(byte channel, byte note, byte velocity);
void OnNoteOff(byte channel, byte note, byte velocity);
unsigned long lastHit[numNotes];
bool noteHigh[numNotes];


// Gamma correction improves appearance of midrange colors
/////////////////////////
// velocity table //
// maps from 127 to 4096 nonlinearly 
const int velocityMap[] = {
      0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  3,  // 0-15
      2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  5,  // 16-31
      2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  8,  // 32-47
      3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  11,  // 48-63
      12,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 32,  // 64-79
     13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 512, 512, 256,  // 80-95
     20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 1024, 1024, 1024, 1024,  // 96-111
     30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 4096, 4096, 4096, 4096  // 112-127
};

/////////////////////////
// Main Loop ////////////
/////////////////////////
void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], OUTPUT);
  }
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);

  // no notes are on to start
  for (int i = 0; i < numNotes; i++) {
    noteHigh[i] = false;
    lastHit[i] = 0;
  }

}


void loop() {
  int knob0 = analogRead(trimPin);
  if (knob0 <= 800) {
    pulse = true;
    onTime = map(knob0, 0, 800, 2, 30); // between 2 and 30 milliseconds
  }
  else {
    pulse = false;
    onTime = 200; // if greater than 800 then...
  }
  // flush the MIDI read buffer
  while (usbMIDI.read()) {

  }

  // check if its time to turn a note off
  if (pulse) {
    checkPulseTime();
  }
  else {
    // leave notes on
    // sustain the note until OnNoteOff
    // is called by the system callback
  }
}

void checkPulseTime() {
  for (int i = 0; i < numNotes; i++) {
    if (noteHigh[i] && (millis() - lastHit[i] > onTime)) {
      // check if on time has passed and turn off
      noteHigh[i] = false;
      analogWrite(pins[i], 0);
    }
  }
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  if (note >= lowNote && note < lowNote + 5) {
    // adjust fir note # offset
    note = note - lowNote;
    // bit shift 7 bits to 12
    analogWrite(pins[note], velocityMap[velocity]);  // between 0-127

    noteHigh[note] = true;
    lastHit[note] = millis();
  }

}

void OnNoteOff(byte channel, byte note, byte velocity) {
  if (!pulse) {
    if (note >= lowNote && note < lowNote + numNotes) {
      // adjust fir note # offset
      note = note - lowNote;

      analogWrite(pins[note], 0);
    }
  }

}





