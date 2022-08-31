int ButtonPin = 34;
int ButtonVal = 0;

String get_input(){
    ButtonVal = analogRead(ButtonPin);
    //Serial.println(String(ButtonVal));
    if (ButtonVal>= 140 && ButtonVal<=300){return "down";}
    else if (ButtonVal>= 1200 && ButtonVal<=1400){return "right";}
    else if (ButtonVal == 4095){return "left";}
    else if (ButtonVal>= 1650 && ButtonVal<=1950){return "up";}
    else {return "none";}
}