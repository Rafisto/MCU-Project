#include <HID-Project.h>
#include <HID-Settings.h>
#include <EEPROM.h>

int vol = 0;
int svol = 0;
int lsCLK = 0;
int crCLK = 0;
int crPOW = 0;
bool muted = false;

unsigned long lastButtonPress = 0;
unsigned long lastBlink = 0;
unsigned long lastMuteBlink = 0;
bool diode = false;

void setup() {
  DDRD = B10010011;
  DDRC = B01000000;
  DDRF = B11100000;
  DDRB = B00000000;
  PORTB = B01100000;
  resetLED(1);
  lsCLK = (PINE >> 6) & 1;
  Consumer.begin();
  Keyboard.begin();
  delay(1000);
  resetLED(0);
  mute();
  vol = EEPROM.read(0);
  muted = EEPROM.read(1);
  if (muted) {
    mute();
    resetLED(0);
  }
  else {
    unmute();
    updateLED();
  }
}

int _step = 2;
void loop() {
  crCLK = (PINE >> 6) & 1;
  crPOW = (PINB >> 5) & 1;
  if (crPOW == 0) {
    Keyboard.press(KEY_SCROLL_LOCK);
    resetLED(1);
    delay(5000);
    Keyboard.release(KEY_SCROLL_LOCK);
    resetLED(0);
    updateLED();
  }
  if (muted) {
    if (millis() - lastMuteBlink > 1000) {
      lastMuteBlink = millis();
      diode = !diode;
      digitalWrite(A1, diode);
    }
  }
  if (millis() - lastBlink > 1000) {
    digitalWrite(A0, 0);
    digitalWrite(A2, 0);
    lastBlink = millis();
  }
  if (((PINB >> 6) & 1) == 0) {
    if (millis() - lastButtonPress > 50) {
      if (muted) {
        muted = false;
        EEPROM.write(1, 0);
        unmute();
        resetLED(0);
        updateLED();
      }
      else {
        muted = true;
        EEPROM.write(1, 1);
        mute();
        resetLED(0);
      }
    }
    lastButtonPress = millis();
  }
  if (crCLK != lsCLK  && crCLK == 1) {
    if (muted) {
      muted = false;
      EEPROM.write(1, 0);
      unmute();
      updateLED();
      delay(100);
    }
    else if (((PINB >> 4) & 1) != crCLK) {
      vol += _step;
      lastBlink = millis();
      digitalWrite(A0, HIGH);
      digitalWrite(A2, LOW);
      Consumer.write(MEDIA_VOLUME_UP);
    }
    else {
      vol -= _step;
      lastBlink = millis();
      digitalWrite(A0, LOW);
      digitalWrite(A2, HIGH);
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    EEPROM.write(0, vol);
    updateLED();
  }
  lsCLK = crCLK;
  delay(1);
}

void mute() {
  for (int a = 0; a < 52; a++) {
    Consumer.write(MEDIA_VOLUME_DOWN);
    delay(2);
  }
}

void unmute() {
  for (int a = 0; a < vol / 2; a++) {
    Consumer.write(MEDIA_VOLUME_UP);
    delay(2);
  }
}

void resetLED(int v) {
  digitalWrite(A0, v);
  digitalWrite(A1, v);
  digitalWrite(A2, v);
  digitalWrite(2, v);
  digitalWrite(3, v);
  digitalWrite(4, v);
  digitalWrite(5, v);
  digitalWrite(6, v);
}

void updateLED() {
  if (vol < 0) vol = 0;
  if (vol > 100) vol = 100;
  if (vol >= 20) digitalWrite(6, HIGH);
  else digitalWrite(6, LOW);
  if (vol >= 40) digitalWrite(5, HIGH);
  else digitalWrite(5, LOW);
  if (vol >= 60) digitalWrite(4, HIGH);
  else digitalWrite(4, LOW);
  if (vol >= 80) digitalWrite(3, HIGH);
  else digitalWrite(3, LOW);
  if (vol >= 100) digitalWrite(2, HIGH);
  else digitalWrite(2, LOW);
}
