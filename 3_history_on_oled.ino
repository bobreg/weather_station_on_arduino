//---------режим отображения истории измерений -------
void history_measuring_as_data() {
  view_history(&history_temperature[0], "Temperature");
  unsigned long timer_exit = millis();
  
  while (flag_history_data == true) {
    flag_history_data = button_set.check(flag_history_data);
    button_plus.check(&change_history_data);
    if (millis() - timer_exit > 150000) {
      flag_history_data = !flag_history_data;
    }
  }
  flag_history_data = true;
  myOLED.setFont(MediumFontRus);
}

//---------переход между историями на экране---------
void change_history_data(){
  type_history_on_oled = (type_history_on_oled + 1) % 4;
  if (type_history_on_oled == 0) {
    view_history(&history_pressure[0], "Pressure");
  } else if (type_history_on_oled == 1) {
    view_history(&history_humidity[0], "Humidity");
  } else if (type_history_on_oled == 2) {
    view_history(&history_co2[0], "Level CO2");
  } else if (type_history_on_oled == 3) {
    view_history(&history_temperature[0], "Temperature");
  }
}

//---------отображение историй на экране--------------
void view_history(int (* arr), String param){
  myOLED.clrScr();
  myOLED.setFont(SmallFontRus);
  myOLED.print(param, 0, 0);
  byte count = 1;
  for(byte i = 1; i < 8; i++){
    for(byte j = 0; j < 120; j+=40){
      myOLED.print(count, j, i);
      myOLED.print(".");
      myOLED.print(arr[count-1]);
      count++;
    }
  }
}
