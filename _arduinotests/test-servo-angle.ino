#include <Servo.h>
#include <TM1637.h>

Servo servo;


int CLK = 5;
int DIO = 6;

TM1637 tm(CLK, DIO);

int val = 0;
void setup() {
  pinMode(3, OUTPUT);
  pinMode(A0, INPUT);
  servo.attach(3);
  pinMode(4, INPUT_PULLUP);
  tm.init();
}


void loop() {
  val = analogRead(A0);
  val = map(val, 0, 1024, 0, 180);
  tm.display(val);
  delay(200);
  if (digitalRead(4) == LOW) {
    servo.write(val);
  }
}
