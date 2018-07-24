#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

byte block;
byte dataBuffer[18];
byte len;

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  // Factory keys are all set to 0xFF
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  len = sizeof(dataBuffer);
  block = 1;
  Serial.println("Ready to write data");
}

void loop()
{
  if (!CardFound()) return;

  // Reset data buffer
  for (int i = 0; i < len; i++) dataBuffer[i] = 0;

  ReadCard();
  delay(2000);
  ResetScanner();
  while (!CardFound())delay(250);
  if (!WriteCard()) Serial.println("Failed to write, start over.");
  ResetScanner();
}

bool CardFound()
{
  // Look for and select card
  if ( ! mfrc522.PICC_IsNewCardPresent())return false;
  if ( ! mfrc522.PICC_ReadCardSerial()) return false;
  Serial.println("*** Found card ***");
  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) Serial.print(mfrc522.uid.uidByte[i], HEX);
  Serial.println();
  return true;
}

void ReadCard()
{
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid))
      != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed: ");
    return;
  }

  MFRC522::StatusCode status = mfrc522.MIFARE_Read(block, dataBuffer, &len);
  if ( status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //PRINT DATA (Use Serial.Write to print as ASCII)
  for (uint8_t i = 0; i < 16; i++)
  {
    Serial.write(dataBuffer[i]);
    Serial.print(" ");
  }
  Serial.println();
}

bool WriteCard()
{
  Serial.println("Authenticating...");
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid))
      != MFRC522::STATUS_OK)
  {
    Serial.print("Failed to authenticate");
    return false;
  }
  else Serial.println("Authentication Success!");

  // Write block
  if (mfrc522.MIFARE_Write(block, dataBuffer, 16) != MFRC522::STATUS_OK)
  {
    Serial.print("Failed to write");
    return false;
  }
  else Serial.println("Write success!");
  Serial.println();
  return true;
}

void ResetScanner()
{
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

