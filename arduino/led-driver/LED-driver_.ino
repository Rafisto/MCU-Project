
#include <LiquidCrystal.h>
#include "RGBdriver.h"

#define CLK A0   
#define DIO A1

RGBdriver Driver(CLK, DIO);

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


void setup() {
  Serial.begin(9600);
  Driver.begin(); // begin   
  Driver.SetColor(0,0,0);
  Driver.end();
  lcd.begin(16, 2);
}

int red_v = 0;
int gre_v = 0;
int blu_v = 0;

void loop() {

  if (Serial.available() >= 0 )
  {
    unsigned int color1 = Serial.read();
    unsigned int color2 = Serial.read();
    unsigned int color = (color2 * 256) + color1;

    if (color >= 1000 && color < 1255) {
      int blue = color;
      blue = map(blue, 1000, 1255, 0, 255);
      blu_v = blue;
      Driver.begin();
      Driver.SetColor(red_v, gre_v, blu_v);
      Driver.end();
      delay(10);
    }

    if (color >= 2000 && color < 2255) {
      int green = color;
      green = map(green, 2000, 2255, 0, 255);
      gre_v = green;
      Driver.begin();
      Driver.SetColor(red_v, gre_v, blu_v);
      Driver.end();
      delay(10);

    }

    if (color >= 3000 && color < 3255) {
      int red = color;
      red = map(red, 3000, 3255, 0, 255);
      red_v = red;
      Driver.begin();
      Driver.SetColor(red_v, gre_v, blu_v);
      Driver.end();
      delay(10);
    } 

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("R" + String(red_v) + " G" + String(gre_v) + " B" + String(blu_v));
  }
}
