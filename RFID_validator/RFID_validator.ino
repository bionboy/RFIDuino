#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define RLED 7
#define GLED 8

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte password[16] = {'h','u','n','t','e','r','2',' ',' ',' ',' ',' ',' ',' ',' ',' '};

void setup()
{
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  digitalWrite(RLED, HIGH);
  digitalWrite(GLED, HIGH);
  
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  // Init keys to 0xFF (Factory default)
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  Serial.println("System locked");
  
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, LOW);
}

void loop()
{ 
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(500);
  
  while(!CardFound()) delay(250);

  byte block = 1;
  byte len;
  
  Serial.print("Data: ");

  byte dataBuffer[18];
  
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid))
    != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed: ");
    return;
  }

  // READ DATA
  if (mfrc522.MIFARE_Read(block, dataBuffer, &len) != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    return;
  }

  // PRINT DATA
  for (uint8_t i = 0; i < 16; i++)
  {
    Serial.write(dataBuffer[i]);
    Serial.print(" ");
  }
  Serial.println();

  // CHECK DATA and respond
  for (uint8_t i = 0; i < 16; i++)
  {
    if (dataBuffer[i] != password[i])
    {
      WRONG_KEY();
      return;
    }
  }
  CORRECT_KEY();
}

bool CardFound()
{
  // Look for and select card
  if (!mfrc522.PICC_IsNewCardPresent()) return false;
  if (!mfrc522.PICC_ReadCardSerial()) return false;
  Serial.print("**Card Detected:**\t");
  return true;
}

void WRONG_KEY()
{
  
  digitalWrite(RLED, HIGH);
  digitalWrite(GLED, LOW);
  delay(1000);
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, LOW);
}

void CORRECT_KEY()
{
  
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, HIGH);
  delay(1000);
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, LOW);
  Serial.println("System Unlocked!");
}

