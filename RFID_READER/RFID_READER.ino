#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

void print_uid(void);
bool authenticate_card(byte sectorToAuth);
void print_one_block(byte BlockToRead);
void dump_card(void);

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

  dump_card();
}

void dump_card(void)
{
  print_uid();
  for (byte block = 0; block < 64; block++)
  {
    byte sector = block / 4;
    bool auth_status = authenticate_card(sector);
    if (auth_status)
    {
      print_one_block(block);
    }
    else
    {
      break;
    }
  }
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

bool authenticate_card(byte sectorToAuth)
{
  MFRC522::MIFARE_Key key;
  byte i;
  for (i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  byte trailerBlock = 3 + (4 * sectorToAuth);
  byte status;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
  Serial.print("PCD_Authenticate() failed: ");
  Serial.print(mfrc522.GetStatusCodeName(status));
  return false;
  }

  return true;
}

void print_one_block(byte blockToRead)
{
  byte buffer[18];
  byte bufferSize = sizeof(buffer);
  byte status = mfrc522.MIFARE_Read(blockToRead, buffer, &bufferSize);
  if (status == MFRC522::STATUS_OK)
  {
    Serial.print("Block ");
    if (blockToRead < 10)
    {
      Serial.print("0");
    }
    Serial.print(blockToRead);
    Serial.print(": ");
    for (byte i = 0; i < 16; i++)
    {
      if (buffer[i] < 0x10)
      {
        Serial.print("0");
      }
      Serial.print(buffer[i], HEX);
      Serial.print(" ");
      
    }
    Serial.print("\n");
  }
}