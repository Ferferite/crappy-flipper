#include <Arduino.h>
#include <Display.h>
#include <Navigation.h>
#include <rfidModule.h>
#include <SDcard.h>


void save_card(){
  Serial.println("Save card");
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("     Card Saved");
  const char * c = card_uid_string.c_str();
  writeFile(SD, "/card.txt", c);
  delay(1000);
  PN532_Menu(0);
}
//appendFile(SD, "/hello.txt", "World!\n");
void dump_card(){
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16];   
  uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    appendFile(SD, "/hello.txt", "World!\n");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    Serial.println("");

    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ...
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      // Now we try to go through all 16 sectors (each having 4 blocks)
      // authenticating each sector, and then dumping the blocks
      for (currentblock = 0; currentblock < 64; currentblock++)
      {
        // Check if this is a new block so that we can reauthenticate
        if (nfc.mifareclassic_IsFirstBlock(currentblock)) authenticated = false;

        // If the sector hasn't been authenticated, do so first
        if (!authenticated)
        {
          // Starting of a new sector ... try to to authenticate
          Serial.print("------------------------Sector ");Serial.print(currentblock/4, DEC);Serial.println("-------------------------");
          if (currentblock == 0)
          {
              // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
              // or 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 for NDEF formatted cards using key a,
              // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
          }
          else
          {
              // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
              // or 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 for NDEF formatted cards using key a,
              // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
          }
          if (success)
          {
            authenticated = true;
          }
          else
          {
            Serial.println("Authentication error");
          }
        }
        // If we're still not authenticated just skip the block
        if (!authenticated)
        {
          Serial.print("Block ");Serial.print(currentblock, DEC);Serial.println(" unable to authenticate");
        }
        else
        {
          // Authenticated ... we should be able to read the block now
          // Dump the data into the 'data' array
          success = nfc.mifareclassic_ReadDataBlock(currentblock, data);
          if (success)
          {
            // Read successful
            Serial.print("Block ");Serial.print(currentblock, DEC);
            if (currentblock < 10)
            {
              Serial.print("  ");
            }
            else
            {
              Serial.print(" ");
            }
            // Dump the raw data
            nfc.PrintHexChar(data, 16);
          }
          else
          {
            // Oops ... something happened
            Serial.print("Block ");Serial.print(currentblock, DEC);
            Serial.println(" unable to read this block");
          }
        }
      }
    }
    else
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
    }
  }
}


void setup() {
  Serial.begin(921600);
  display_init();
  pinMode(34, INPUT);
  PN532_init();
  display_main_menu(0);
  SD_init();
}

void loop() {
  if(get_input() == "down"){
    selected_item++;
    if(selected_item>=5){selected_item=0;}
    else if( current_menu == "main"){
      display_main_menu(selected_item);
    }
    else if( current_menu == "PN532_Menu"){
      PN532_Menu(selected_item);
    }
  }
  else if(get_input() == "up"){
    selected_item--;
    if(selected_item<0){selected_item=4;}
    else if(current_menu == "main"){
      display_main_menu(selected_item);
    }
    else if(current_menu == "PN532_Menu"){
      PN532_Menu(selected_item);
    }
  }
  else if(get_input() == "right"){
    //======================== main menu ========================
    if(current_menu == "main"){
        if(main_menu_items[selected_item] == "Test Menu"){
        while(true){
          Test_Menu();
          if(get_input() == "left"){display_main_menu(0);break;} //exit menu if left is pressed
          delay(150);
        }
      }
      else if(main_menu_items[selected_item] == "Rfid/Nfc"){
          selected_item = 0;
          PN532_Menu(0);
      }
      else if(main_menu_items[selected_item] == "IR remote"){
        IR_Menu();
      }
    }
    //========================rfid/nfc menu and sub menus========================
    else if(current_menu=="PN532_Menu"){
      if(PN532_menu_items[selected_item] == "Read Card"){
        Display_UID(read_uid());
      }
      else if(PN532_menu_items[selected_item] == "Write to Card"){
        write_card();
      }
      else if(PN532_menu_items[selected_item] == "Dump Card"){
        Serial.println("===========Dump card=========");
        dump_card();
      }
      else if(PN532_menu_items[selected_item] == " "){
        write_card();
      }
    }
    else if(current_menu == "Display_UID_Menu"){
      if(get_input() == "left"){PN532_Menu(0);}
      else if(get_input() == "right"){save_card();}
    }
    //========================IR remote and sub menus========================
    else if(current_menu == "IR_Menu"){
      ;
    }
  }
  else if(get_input() == "left"){
    display_main_menu(0);
    selected_item = 0;
  }
  else{;} //dont remove or it aint going to work
  delay(100);
}
