/*
Teensy 3.1
 5 -- GSCLK
 4 -- Blank
 3 -- XLAT
 7 -- SCLK
 6 -- SIN

 */

#include "Tlc5940.h"
#define numNotes 16


void OnNoteOn(byte channel, byte note  , byte velocity);
//void OnNoteOff(byte channel, byte note, byte velocity);

unsigned int onTime[numNotes];
unsigned long lastHit[numNotes];
bool noteHigh[numNotes];
int noteVel[numNotes];

int lowNote = 0;

void setup() {
  usbMIDI.setHandleNoteOn(OnNoteOn);
  //  usbMIDI.setHandleNoteOff(OnNoteOff);
  Tlc.init();
  Tlc.clear();
  // no notes are on to start
  for (int i = 0; i < numNotes; i++) {
    noteHigh[i] = false;
    lastHit[i] = 0;
    noteVel[i] = 0;
  }
  for (int i = 0; i < 16; i++){
     onTime[i] = 10; 
  }
  for (int i = 16; i < 32; i++){
     onTime[i] = 20; 
  }
  for (int i = 32; i < 48; i++){
     onTime[i] = 9; 
  }

}


void loop() {
  while (usbMIDI.read()) {
  }

  for (int i = 0; i < numNotes; i++) {
    if (noteHigh[i] && (millis() - lastHit[i] > onTime[i])) {
      // check if on time has passed and turn off
      noteHigh[i] = false;


    }

  }
  // write all values
  for (int i = 0; i < numNotes; i++) {
    if (noteHigh[i]) {
      Tlc.set(i, noteVel[i] << 5);
    }
    else
      Tlc.set(i, 0);
    Tlc.update();
  }

}

void OnNoteOn(byte channel, byte note, byte velocity) {
  note = note - lowNote;
  
  if (note >= 0 && note < numNotes)
  {

    noteHigh[note] = true;
    noteVel[note] = velocity;
    lastHit[note] = millis();
    Tlc.set(note, velocity << 5);
    Tlc.update();

  }
}

//void OnNoteOff(byte channel, byte note, byte velocity) {
//  Tlc.set(note, 0);
//  Tlc.update();
//}





