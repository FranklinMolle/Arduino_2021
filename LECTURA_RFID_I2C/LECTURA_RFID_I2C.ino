#include <Wire.h>
#include "MFRC522_I2C.h"

MFRC522 mfrc522(0x28, 6);   // Create MFRC522 instance.
String BufferID = "";
uint32_t valor = 0 ;

void setup() {
  Serial.begin(9600);           // Initialize serial communications with the PC
  Wire.begin();                   // Initialize I2C
  mfrc522.PCD_Init();             // Init MFRC522
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  BufferID = "";
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
    BufferID = BufferID + String(mfrc522.uid.uidByte[i], DEC);
    valor = BufferID.toInt();
  } 
  Serial.println();
  Serial.println(BufferID);
  Serial.println(valor);
  mfrc522.PICC_HaltA();
  
}
