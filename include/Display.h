//SCL --->D22
//SDA --->D21
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  
String main_menu_items[] = {"Test Menu", "Rfid/Nfc", "test3", "test4"};
String PN532_menu_items[] = {"Read Card", "Write to Card", "Dump Card", " "};
int i_menu = 0;
int selected_item = 0;
int x = 0;
String current_menu = "main";
void display_init(){
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
}

void display_main_menu(int selected_item){
    lcd.clear();
    current_menu = "main";
    i_menu = 0;
    //if(selected_item>3){selected_item=0;}
    for(int increment=0; increment<4; increment++){
        lcd.setCursor(0, increment);
        if(increment == selected_item){
            lcd.print("> ");
            lcd.print(main_menu_items[i_menu]);
        }
        else{
            lcd.print(main_menu_items[i_menu]);
        }
        i_menu++;
    }
    selected_item = 0;
}

void Test_Menu(){
    current_menu = "Test_Menu";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     Test Menu");
    lcd.setCursor(0, 1);
    lcd.print(String(x++));
    lcd.setCursor(0, 2);
    lcd.print("Press left to exit");
    lcd.setCursor(0, 3);
    lcd.print("Press left to exit");
}


void PN532_Menu(int selected_item){
    lcd.clear();
    current_menu = "PN532_Menu";
    i_menu = 0;
    for(int increment=0; increment<4; increment++){
        lcd.setCursor(0, increment);
        if(increment == selected_item){
            lcd.print("> ");
            lcd.print(PN532_menu_items[i_menu]);
        }
        else{
            lcd.print(PN532_menu_items[i_menu]);
        }
        i_menu++;
    }
    selected_item = 0;
}

void Display_UID(String UID){
    current_menu = "Display_UID_Menu";
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    Card Found!    ");
    lcd.setCursor(0,1);
    lcd.print("UID: ");
    for (int i = 0; i < UID.length(); i++) { //fix so the UID is not displayed on the wrong line
        lcd.print(UID[i]);
        if (i == 14) {
            lcd.setCursor(0,2);
        }
    }
    lcd.setCursor(0,3);
    lcd.print("Press right to save");
}