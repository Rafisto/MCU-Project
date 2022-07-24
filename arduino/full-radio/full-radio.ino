///
/// \file  TestRDA5807M.ino
/// \brief An Arduino sketch to operate a SI4705 chip based radio using the Radio library.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2014 by Matthias Hertel.\n
/// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
///
/// \details
/// This sketch implements a "as simple as possible" radio without any possibility to modify the settings after initializing the chip.\n
/// The radio chip is initialized and setup to a fixed band and frequency. These settings can be changed by modifying the
/// FIX_BAND and FIX_STATION definitions.
///
/// Open the Serial console with 57600 baud to see the current radio information.
///
/// Wiring
/// ------
/// The RDA5807M board/chip has to be connected by using the following connections:
/// | Arduino UNO pin    | Radio chip signal  |
/// | -------------------| -------------------|
/// | 3.3V (red)         | VCC                |
/// | GND (black)        | GND                |
/// | A5 or SCL (yellow) | SCLK               |
/// | A4 or SDA (blue)   | SDIO               |
/// The locations of the pins on the UNO board are written on the PCB.
/// The locations of the signals on the RDA5807M side depend on the board you use.
///
/// More documentation and source code is available at http://www.mathertel.de/Arduino
///
/// ChangeLog:
/// ----------
/// * 05.12.2014 created.
/// * 19.05.2015 extended.

#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <RDA5807M.h>
#include <TM1637Display.h>
#include <EEPROM.h>

// Module connection pins (Digital Pins)
#define CLK 6
#define DIO 4
TM1637Display display(CLK, DIO);

// ----- Fixed settings here. -----

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
//#define FIX_STATION  8980          ///< The station that will be tuned by this sketch is 89.30 MHz.
#define FIX_VOLUME   4               ///< The volume that will be set by this sketch is level 4.

RDA5807M radio;    // Create an instance of Class for RDA5807M Chip


int station = 10490;

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup() {
  pinMode(9, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  display.setBrightness(0x0f);
  // Initialize the Radio
  radio.init();
  // Set all radio setting to the fixed values.
  radio.setVolume(FIX_VOLUME);
  radio.setMono(true);
  radio.setMute(false);
  load_station();
} // setup
void save_station() {
  EEPROM.write(0, station%100);
  EEPROM.write(1, (station-station%100)/100);
} // save station to eeprom
void load_station() {
  int p_one = EEPROM.read(0);
  int p_two = EEPROM.read(1);
  station = p_one+p_two*100;
  radio.setBandFrequency(FIX_BAND, station);
} // load station from eeprom
int counter = 0;
void loop() {
  if (digitalRead(9) == LOW) {
    if (counter > 10) {
      station += 50;
    }
    station += 10;
    radio.setBandFrequency(FIX_BAND, station);
    save_station();
    counter += 1;
    delay(210);
  }
  else if (digitalRead(12) == LOW) {
    if (counter > 10) {
      station -= 50;
    }
    station -= 10;
    save_station();
    radio.setBandFrequency(FIX_BAND, station);
    counter += 1;
    delay(100);
  }
  else {
    counter = 0;
  }
  if (station > 10000) {
    display.showNumberDec(station - 10000, true);
  }
  else {
    display.showNumberDec(station, true);
  }
  if (station < 8750) {
    station = 8750;
  }
  else if (station > 10800) {
    station = 10800;
  }
} // loop
// End.
