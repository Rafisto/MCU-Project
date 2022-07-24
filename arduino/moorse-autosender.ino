void setup() {
  pinMode(3, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
}

String tosend = "";
char received;
int timemultiplier=5;
void loop() {
  if(Serial.available() > 0){
    received = Serial.read();
    tosend+=received;
  }
  if (digitalRead(3) == LOW) {
    tosend += "1";
    digitalWrite(2, HIGH);
    delay(50);
    digitalWrite(2, LOW);
    delay(200);
  }
  else if (digitalRead(5) == LOW) {
    tosend += "2";
    /*digitalWrite(2, LOW);
      delay(300);
      digitalWrite(2, HIGH);
      delay(100);*/
    digitalWrite(2, HIGH);
    delay(150);
    digitalWrite(2, LOW);
    delay(200);
  }
  else if (digitalRead(6) == LOW) {
    tosend += "0";
    /*
      delay(100);*/
    delay(200);
  }
  else if (digitalRead(7) == LOW) {
    int str_len = tosend.length() + 1;
    Serial.println(tosend);
    for (int x = 0; x < str_len; x++) {
      Serial.println(tosend[x]);
      if (tosend[x] == '0') {
        delay(150*timemultiplier);
      }
      if (tosend[x] == '1') {
        digitalWrite(2, HIGH);
        digitalWrite(10,LOW);
        delay(50*timemultiplier);
        digitalWrite(2, LOW);
        digitalWrite(10,HIGH);
        delay(50*timemultiplier);
      }
      if (tosend[x] == '2') {
        digitalWrite(2, HIGH);
        digitalWrite(10,LOW);
        delay(150*timemultiplier);
        digitalWrite(2, LOW);
        digitalWrite(10,HIGH);
        delay(50*timemultiplier);
      }
    }
    delay(1000);
  }
}
