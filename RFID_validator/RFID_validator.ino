#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  // Init keys to 0xFF (Factory default)
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  Serial.println("Ready to read");  
}

void loop()
{ 
  if (!CardFound()) return;

  byte block = 1;
  byte len;
  
  Serial.print("Data: ");

  byte dataBuffer[18]; // = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};

  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid))
    != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed: ");
    return;
  }

//  if (status = mfrc522.MIFARE_Write(block, dataBuffer, 16) != MFRC522::STATUS_OK)
  if (mfrc522.MIFARE_Read(block, dataBuffer, &len) != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    return;
  }

  //PRINT DATA (Use Serial.Write to print as ASCII)
  for (uint8_t i = 0; i < 16; i++)
  {
    Serial.print(dataBuffer[i]);
    Serial.print(" ");
  }
  Serial.println();

  delay(500);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

bool CardFound()
{
  // Look for and select card
  if (!mfrc522.PICC_IsNewCardPresent()) return false;
  if (!mfrc522.PICC_ReadCardSerial()) return false;
  Serial.println("**Card Detected:**");
  return true;
}

