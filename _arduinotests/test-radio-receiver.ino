#include <LiquidCrystal.h>
#include <Wire.h>
#include <RDA5807M.h>

// ----- Fixed settings here. -----

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION  9880            ///< The station that will be tuned by this sketch is 89.30 MHz.
#define FIX_VOLUME   4               ///< The volume that will be set by this sketch is level 4.

RDA5807M radio;    // Create an instance of Class for RDA5807M Chip

int station = 10490;

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup() {
  pinMode(A0, INPUT);
  pinMode(12, INPUT_PULLUP);
  delay(200);
  radio.init();
  //radio.setBandFrequency(FIX_BAND, station);
  radio.setVolume(FIX_VOLUME);
  radio.setMono(true);
  radio.setMute(false);
}


float before = 0;
/// show the current chip data every 3 seconds.
void loop() {
  int value = digitalRead(12);
  float val = analogRead(A0) / 5.1f * 10;
  if (value == LOW) {
    radio.setBandFrequency(FIX_BAND, 8800 + val);
    delay(1000);
  }
} 
