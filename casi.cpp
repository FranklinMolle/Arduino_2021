#include <Wire.h>
#include <Ufox.h>
#include "MFRC522_I2C.h"

#define RXLED  17

Ufox wisol;
MFRC522 mfrc522(0x28, 4);     //CREAMOS EL OBJETO "mfrc522" Y EL PIN DE RESET '4'  
String StringID = "";         //CREAMOS EL STRING PARA EL DATO LEIDO
uint32_t valor = 0 ;          //CREAMOS EL ENTERO PARA EL DATO LEIDO

void setup() {
  Wire.begin(); 
  pinMode(RXLED,OUTPUT);
  Serial.begin(9600);          
  mfrc522.PCD_Init();         //INICIALIZA EL MODULO
  wisol.begin(9600);            
}

void loop() {
  /* PREGUNTAMOS SI HAY UNA TARJETA O LLAVERO PRESENTE Y SI SE PUEDE OBETNER LOS DATOS
     RETORNA AL LOOP*/ 
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  StringID = "";              //LIMPIAMOS EL VALOR DEL STRING
  /*************** OBTENEMOS EL VALOR DEL UID DE LA TARJETA ***********/
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
    StringID = StringID + String(mfrc522.uid.uidByte[i], DEC);
    valor = StringID.toInt();
  } 
  /********************************************************************/
  Serial.println();
  Serial.println(StringID);
  //Serial.println(valor);
  mfrc522.PICC_HaltA();       //DETENEMOS LA COMUNICACION DE LA TARJETA
  //**************** ENVIO POR SIGFOX ***********
  digitalWrite(RXLED,LOW);
  wisol.RST();
  Serial.println(wisol.SEND(valor));  //ENVIAMOS EL VALOR OBTENIDO
  digitalWrite(RXLED,HIGH);
  //wisol.SLEEP();
  delay(3000);
  Serial.println("Enviado");         //ENVIAMOS UN MENSAJE DE CONFIRMACION 
  //************
}
