#include <Servo.h>

int SERVO_PIN = 4;
int BUTTON_PIN= 15;


Servo myservo;
int pos = 0;
int lastOpened = 0;
int barrierTimeout = 5000;

void setup() {
  myservo.attach(SERVO_PIN, 500, 2400);
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
}

void open() {
    myservo.write(0);
    lastOpened = millis();
}

void close() { 
  myservo.write(90);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  Serial.println(buttonState);
  if(buttonState == 1) { 
    open();
  }
  if(lastOpened != 0 && (millis() - lastOpened) > barrierTimeout) {
    close();
  }
}
