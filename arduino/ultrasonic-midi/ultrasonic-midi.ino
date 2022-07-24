#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 7, d5 = 6, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

#define echoPin 2
#define trigPin 3

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
}

float tf = 0.1;
int olddistance = distance;
int oldnote = 0;
int note = 0;
int notes[] = {48, 50, 52, 53, 55, 57, 59, 60, 61};
String strnotes[] = {"C", "D", "E", "F", "G", "A", "B", "C"};
void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  distance =  tf * olddistance + (1 - tf) * distance;
  if (distance > 49) {
    return;
  }
  if (olddistance != distance) {
    Serial.print("Sending note ");
    Serial.println(distance);
    note = map(distance, 0, 50, 0, 8);
    if (oldnote != note) {
      noteOff(0, notes[oldnote], 64);
      noteOn(0, notes[note], 64);
    }
    MidiUSB.flush();
    olddistance = distance;
    oldnote = note;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(strnotes[note]);
    lcd.setCursor(0, 1);
    lcd.print(distance);
  }

}
