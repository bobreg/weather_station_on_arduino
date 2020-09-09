//------------режим графиков на экране----------------
void history_measuring_as_graf() {
  view_graf(&history_temperature[0], "Temperature");
  unsigned long timer_exit = millis();
  
  while (flag_history_graf == true) {
    flag_history_graf = button_set.check(flag_history_graf);
    button_plus.check(&change_graf);
    if (millis() - timer_exit > 60000) {
      flag_history_graf = !flag_history_graf;
    }
  }
  flag_history_graf = true;
  myOLED.setFont(MediumFontRus);
}

//------------переход между графиками по кнопке-------
void change_graf() {
  type_graf_on_oled = (type_graf_on_oled + 1) % 4;
  if (type_graf_on_oled == 0) {
    view_graf(&history_pressure[0], "Pressure");
  } else if (type_graf_on_oled == 1) {
    view_graf(&history_humidity[0], "Humidity");
  } else if (type_graf_on_oled == 2) {
    view_graf(&history_co2[0], "Level CO2");
  } else if (type_graf_on_oled == 3) {
    view_graf(&history_temperature[0], "Temperature");
  }
}

//--------отрисовка графиков на экране----------------
void view_graf(int (* arr), String param) {
  myOLED.clrScr();
  myOLED.setFont(SmallFontRus);
  myOLED.setCursor(0, 0);
  myOLED.print(param);

  int min_p = arr[0];
  for (int i = 1; i < count_points_history; i++) {
    if (arr[i] <= min_p) {
      min_p = arr[i];
    }
  }
  //-------------------
  int max_p = arr[0];

  for (int i = 1; i < count_points_history; i++) {
    if (arr[i] >= max_p) {
      max_p = arr[i];
    }
  }
  if (max_p == min_p) {
    max_p += 1;
  }
  for (int x = 0; x < count_points_history; x++) {
    byte y = 7 - map(arr[x], min_p, max_p, 0, 5);
    myOLED.setCursor(x * 4, y);
    myOLED.print(".");
    for (byte yn = y + 1; yn < 8; yn++) {
      myOLED.setCursor(x * 4, yn);
      myOLED.print("|");
    }
  }
  myOLED.setCursor(100, 7);
  myOLED.print(min_p);
  myOLED.setCursor(100, 3);
  myOLED.print(max_p);

}
