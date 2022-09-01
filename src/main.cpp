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
    }
    else if(current_menu=="PN532_Menu"){
      if(PN532_menu_items[selected_item] == "Read Card"){
        Display_UID(read_card());
      }
      else if(PN532_menu_items[selected_item] == "Write to Card"){
        write_card();
      }
      else if(PN532_menu_items[selected_item] == " "){
        write_card();
      }
      else if(PN532_menu_items[selected_item] == " "){
        write_card();
      }
    }
    else if(current_menu == "Display_UID_Menu"){
      if(get_input() == "left"){PN532_Menu(0);}
      else if(get_input() == "right"){save_card();}
    }
    
  }
  else if(get_input() == "left"){
    display_main_menu(0);
    selected_item = 0;
  }
  else{;} //dont remove or it aint going to work
  delay(100);
}
