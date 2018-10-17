
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
    analogWrite(pins[note], velocity << 5);

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





