#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

void print_uid(void);

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
}

void print_uid(void)
{
  int i = 0;
  int size = mfrc522.uid.size;
  Serial.print("Card id: ");
  for (i = 0; i < size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  int piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("\nPICC type: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
}
