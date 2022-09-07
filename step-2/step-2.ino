int GREEN_LED = 12;
int RED_LED = 14;
int TRIG_PIN = 13;
int ECHO_PIN = 15;

int MAX_DISTANCE = 100;

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, ESP32!");
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

float readDistanceCM() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    int duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}


void switchLed(bool busy) {
  if(busy) { 
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
  } else { 
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
  }
}

void loop() {
    float distance = readDistanceCM();

    Serial.print("Measured distance: ");
    Serial.println(distance);

    bool isSpotBusy = distance < MAX_DISTANCE;
    switchLed(isSpotBusy);
    delay(60);
}
