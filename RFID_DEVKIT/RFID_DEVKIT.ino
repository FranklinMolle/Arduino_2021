#include <Wire.h>
#include <Ufox.h>
#include "MFRC522_I2C.h"

#define RXLED  17

Ufox wisol;
MFRC522 mfrc522(0x28, 4);     //Creamos el objeto "mfrc522" y el pin 4 para el reset del i2c  
String StringID = "";         //creamos un string para el dato leido
uint32_t valor = 0 ;          //creamos el entero para que posteriormente convertir un string a entero

void setup() {
  Wire.begin(); 
  pinMode(RXLED,OUTPUT);
  Serial.begin(9600);          
  mfrc522.PCD_Init();         //inicializa el modulo
  wisol.begin(9600);            
}

void loop() {
  /* preguntamos si hay una tarjeta o llavero presente y si se pude obtener los datos en caso sea falso retorar al loop */
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  StringID = "";              //limpiamos el valor del string
  /*************** OBTENEMOS EL VALOR DEL UID DE LA TARJETA ***********/
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
    StringID = StringID + String(mfrc522.uid.uidByte[i], DEC);
    valor = StringID.toInt();   //conevrtimos el valor(string) de la tarjeta a un valor entero para el envio del sigfox
  } 
  /***************  ****************************************************/
  Serial.println();
  Serial.println(valor);      
  mfrc522.PICC_HaltA();       //detenemos la comunicacion del modulo
  //**************** ENVIO POR SIGFOX ***********
  digitalWrite(RXLED,LOW);
  wisol.RST();                        //reinicia y despierta al modulo
  Serial.println(wisol.SEND(valor));  //enviamos el valor obtenido
  digitalWrite(RXLED,HIGH);
  wisol.SLEEP();                      //modo sleep
  delay(3000);
  Serial.println("Enviado");          //enviamos un mensaje de confirmacion 
  //************
}
