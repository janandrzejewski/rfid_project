#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

void print_uid(void);
void authenticate_card(void);

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup(void) 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop(void)
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  print_uid();
  authenticate_card();
  mfrc522.PCD_StopCrypto1();
}

void print_uid(void)
{
  byte i = 0;
  byte size = mfrc522.uid.size;
  Serial.print("Card id: ");
  for (i = 0; i < size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("\nPICC type: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
}

void authenticate_card(void)
{
  MFRC522::MIFARE_Key key;
  byte i;
  for (i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  byte trailerBlock = 7;
  byte status;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
  Serial.print("PCD_Authenticate() failed: ");
  Serial.println(mfrc522.GetStatusCodeName(status));
  return;
  }
}