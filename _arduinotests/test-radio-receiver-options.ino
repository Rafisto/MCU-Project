#include <LiquidCrystal.h>
#include <Wire.h>
#include <radio.h>
#include <RDA5807M.h>

// ----- Fixed settings here. -----

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION  9880            ///< The station that will be tuned by this sketch is 89.30 MHz.
#define FIX_VOLUME   4               ///< The volume that will be set by this sketch is level 4.

RDA5807M radio;    // Create an instance of Class for RDA5807M Chip

int station = 10490;

LiquidCrystal lcd(9, 10, 2, 3, 4, 5);

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup() {
  // open the Serial port
  pinMode(A0, INPUT);
  pinMode(13, INPUT_PULLUP);
  Serial.begin(57600);
  Serial.println("Radio...");
  Serial.println(analogRead(A0));
  delay(200);
  lcd.begin(16, 2);
  lcd.print("Working");
  // Initialize the Radio
  radio.init();
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  // Enable information to the Serial port
  radio.debugEnable();

  // Set all radio setting to the fixed values.
  //radio.setBandFrequency(FIX_BAND, station);
  radio.setVolume(FIX_VOLUME);
  //radio.setMono(false);
  //radio.setMute(false);
 // analogWrite(A7, 800);
} // setup


float before = 0;
/// show the current chip data every 3 seconds.
void loop() {
  
  float val = analogRead(A0) / 5.1f * 10;
  if (digitalRead(12) == HIGH) {
    radio.setBandFrequency(FIX_BAND, 8800 + val);
    Serial.println("station changed");
  }
  delay(100);
} // loop

// End.
