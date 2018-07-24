#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);        
  SPI.begin();               
  mfrc522.PCD_Init();        
  // Factory keys are all set to 0xFF
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  Serial.println("Ready to write data");
}

void loop()
{
  if(!CardFound()) return;

  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  
  byte buffer[18];
  byte block;
  byte len;

  Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial
  Serial.println("Type data, ending with #");
  len = Serial.readBytesUntil('#', (char *) buffer, 16) ; // read family name from serial
  for (byte i = len; i < 16; i++) buffer[i] = ' ';     // pad with spaces

  block = 1;
  Serial.println(F("Authenticating..."));
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid))
    != MFRC522::STATUS_OK)
  {
    Serial.print(F("Failed to authenticate"));
    return;
  }
  else Serial.println(F("Authentication Success!"));

  // Write block
  if (mfrc522.MIFARE_Write(block, buffer, 16) != MFRC522::STATUS_OK)
  {
    Serial.print("Failed to write");
    return;
  }
  else Serial.println("Write success!");
  Serial.println();
  
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

bool CardFound()
{
  // Look for and select card
  if ( ! mfrc522.PICC_IsNewCardPresent()) return false;
  if ( ! mfrc522.PICC_ReadCardSerial()) return false;
  return true;
}


