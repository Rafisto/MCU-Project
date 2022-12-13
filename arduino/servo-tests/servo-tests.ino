#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

float pos = 0;    // variable to store the servo position
bool cangoback = false;
int drag = 100;

void setup()   {
  // put your setup code here, to run once:
  pinMode(A0, OUTPUT);
  pinMode(A3, INPUT);
  Serial.begin(9600);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
}


void loop() {
  Serial.println(analogRead(A3));
  if (pos < 180 && cangoback) {
    pos += sq(analogRead(A3) / drag);
    myservo.write(pos);
    analogWrite(A0, 512);          // tell servo to go to position in variable 'pos'                 // waits 15ms for the servo to reach the position
    if (pos >= 180) {
      cangoback = false;
    }
  }
  else {
    pos -= sq(analogRead(A3) / drag);
    myservo.write(pos);
            // waits 15ms for the servo to reach the position
    analogWrite(A0, 0);
    if (pos <= 0) {
      cangoback = true;
    }
  }
}
