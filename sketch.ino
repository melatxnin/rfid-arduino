/*
  RFID ACCES SYSTEM
  
  Controls:
  Click on the RFID reader and choose one of
  the available cards at the top of the simulator

  Author:
  Nicolas GOSSARD
*/

#include <SPI.h>
#include <MFRC522.h>

// pins
constexpr int ssPin = 10;
constexpr int rstPin = 9;
constexpr int greenLedPin = 6;
constexpr int redLedPin = 7;
constexpr int buzzerPin = 8;

// Frequency
constexpr int buzzerFrequencyGood = 1000;
constexpr int buzzerFrequencyBad = 200;

MFRC522 rfid(ssPin, rstPin); 

void setup()
{
  Serial.begin(9600);

  SPI.begin();

  rfid.PCD_Init();

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);
}

void loop()
{
  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print("UID: ");

  for (byte i = 0; i < rfid.uid.size; i++)
  {
    if (rfid.uid.uidByte[i] < 0x10)
      Serial.print("0");

    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  bool isAdmin =
    rfid.uid.size == 4 &&
    rfid.uid.uidByte[0] == 0x01 &&
    rfid.uid.uidByte[1] == 0x02 &&
    rfid.uid.uidByte[2] == 0x03 &&
    rfid.uid.uidByte[3] == 0x04;

  if (isAdmin)
  {
    Serial.println("Bonjour");
    digitalWrite(greenLedPin, HIGH);
    tone(buzzerPin, buzzerFrequencyGood);
  }
  else
  {
    Serial.println("Accès refusé");
    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, buzzerFrequencyBad);
  }

  delay(500);
  noTone(buzzerPin);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(500);
}