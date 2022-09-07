#include <OneWire.h>
#include <DallasTemperature.h>

int GREEN_LED = 12;
int RED_LED = 14;
int TRIG_PIN = 13;
int ECHO_PIN = 15;
int ANALOG_TEMP_PIN = 0;

int MAX_DISTANCE = 100;
int TEMP_BUS=5;
int BUZZER_PIN=4;


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(TEMP_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

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

void alarm() {
   tone (BUZZER_PIN, 554);
   delay(100);
   tone(BUZZER_PIN, 440);
   delay(400);
}

void stopAlarm() {
  noTone(BUZZER_PIN);
}

void loop() {
    // SPOT OCCUPATION
    float distance = readDistanceCM();
    Serial.print("Measured distance: ");
    Serial.println(distance);

    bool isSpotBusy = distance < MAX_DISTANCE;
    switchLed(isSpotBusy);

    // SPOT TEMPERATURE
    sensors.requestTemperatures(); 
    Serial.print("Celsius temperature: ");
    Serial.println(sensors.getTempCByIndex(0)); 
    delay(60);
    if(sensors.getTempCByIndex(0) > 50) {
        alarm();
    } else { 
        stopAlarm();
    }

}
