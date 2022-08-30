int GREEN_LED = 13;
int RED_LED = 12;

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, ESP32!");
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
}

void loop() {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(500);
}
