#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int GREEN_LED = 12;
int RED_LED = 14;
int TRIG_PIN = 13;
int ECHO_PIN = 15;

int MAX_DISTANCE = 100;
bool isSpotBusy = false;

const char* ssid     = "2GHZ4_JLT";
const char* password = "abi23nafk8fis73czrp8";

const char* host = "srv2.juleslaurent.fr";
const uint16_t port = 1883;
const char* client_id = "PROF"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, ESP32!");
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    initWifi();

    client.setServer(host, port);
}

void initWifi() { 
    Serial.print("[WIFI] Connecting to ");
    Serial.println(ssid);
  
    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
       would try to act as both a client and an access-point and could cause
       network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
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

void sendMessage(char const* measurement,char const* fieldName,char const* fieldValue) {
  if(!client.connected()) {
    if(client.connect(client_id")) { 
      Serial.println("Connected to MQTT broker"); 
    } else { 
      Serial.println("Failed to connect to MQTT broker"); 
    }
  }

  char* message;
  sprintf(message, "%s,spot=1 %s=%s",measurement, fieldName, fieldValue);
 

  client.publish("parking", message);
}

void loop() {
    float distance = readDistanceCM();

    Serial.print("Measured distance: ");
    Serial.println(distance);

    bool newStatus = distance < MAX_DISTANCE;
    if(newStatus != isSpotBusy) { 
        switchLed(newStatus);
        sendMessage("status","status", newStatus ? "1" : "0");
        isSpotBusy = newStatus;
    }
    delay(60);
}
