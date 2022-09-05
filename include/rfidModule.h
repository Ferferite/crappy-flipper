//https://arduino.stackexchange.com/questions/86545/esp32-not-detecting-pn532-module-running-in-spi-mode
//Module running in VPSI mode
//MOSI-GPIO 23
//MISO-GPIO 19
//SCK-GPIO 18
//SS-GPIO 5
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SCK  (18)
#define PN532_MISO (19)
#define PN532_MOSI (23)
#define PN532_SS   5  
Adafruit_PN532 nfc(PN532_SS);  
int j = 0;
void PN532_init(){
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }
  nfc.SAMConfig();
}
String card_uid_string;
String read_uid(){
    uint32_t card_uid[4];
    uint8_t card_uid_length;
    card_uid_string = "";
    j = 0;
    if (! nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, (uint8_t*) card_uid, &card_uid_length)) {
        Serial.println("No Card Found");
    }
    else{
      for (uint8_t i = 0; i < card_uid_length; i++) {
        card_uid_string = card_uid_string + String(card_uid[i], HEX);
        card_uid_string = card_uid_string + " ";
        Serial.println(String(card_uid[i], HEX));
        }
    }
  return card_uid_string;  
}


void write_card(){
  Serial.println("Write to card");
}



