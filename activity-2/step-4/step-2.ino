#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         0         // Configurable, see typical pin layout above
#define SS_PIN          15         // Configurable, see typical pin layout above
#define RED_LED         5
#define GREEN_LED       16
#define SERVO_PIN       4
#define BUTTON_PIN      10

Servo myservo;
int pos = 0;
int lastOpened = 0;
int barrierTimeout = 5000;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte authorized_cards[1][4] = {
  {0x0a, 0x55, 0x0f, 0xbe},
};

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);



  myservo.attach(SERVO_PIN, 500, 2400);

  
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

bool readUid() { 
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false; 
  }
  mfrc522.PICC_HaltA();
  return true;
  
}

void printUid(MFRC522::Uid *uid) { 
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < uid->size; i++) {
    if(uid->uidByte[i] < 0x10)
      Serial.print(F(" 0"));
    else
      Serial.print(F(" "));
    Serial.print(uid->uidByte[i], HEX);
  } 
  Serial.println();
}

bool isAuthorized(MFRC522::Uid *uid) {
  for (int i = 0; i < sizeof(authorized_cards) ; i++) {
    if(memcmp(uid->uidByte,authorized_cards[i],sizeof(uid)) == 0)
    {
      Serial.println("Acess granted");
      digitalWrite(GREEN_LED, HIGH);
      delay(1000);
      digitalWrite(GREEN_LED, LOW);
      return true;
    }
  }
  Serial.println("Access denied");
  digitalWrite(RED_LED, HIGH);
  delay(1000);
  digitalWrite(RED_LED, LOW);
  return false;
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
  
  if(readUid()) { 
   printUid(&(mfrc522.uid));
   if(isAuthorized(&(mfrc522.uid))) { 
      open();
   }
 }

 if(lastOpened != 0 && (millis() - lastOpened) > barrierTimeout) {
    close();
 }
}