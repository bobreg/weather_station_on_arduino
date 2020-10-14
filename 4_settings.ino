//------------настройка-------------------------------
void settings() {
  myOLED.clrScr();
  myOLED.print("Settings", 0, 1);
  delay(1000);
  bool flag = true;
  bool flag_cal = false;
  byte c = 0;
  while (flag == true) {    // калибровка датчика СО2
    flag = button_set.check(flag);
    flag_cal = button_plus.check(flag_cal);
    for (; c < 1; c++) {
      myOLED.print("Calibrate", 0, 1);
      myOLED.print("CO_2 ?", 0, 3);
      myOLED.setFont(SmallFontRus);
      myOLED.print("Please press +", 0, 5);
      myOLED.setFont(MediumFontRus);
    }
    if (flag_cal == true) {
      myOLED.clrScr();
      bool flag_point = true;  // флаг для мигания точки на экране раз в сек.
      bool flag_no_cal = false;
      for (unsigned int i = 1200; i > 0; i--) {
        flag_no_cal = button_set.check(flag_no_cal); // если нажать, то калибровки не будет
        flag_point = !flag_point;
        //myOLED.setCursor(0, 1);
        myOLED.print("wait ", 0, 1);
        myOLED.print(i/60);
        myOLED.print(" ");
        myOLED.print("minutes", 0, 3);
        flag_point == true ? myOLED.print(" ") : myOLED.print(".");
        myOLED.setFont(SmallFontRus);
        myOLED.print("Press \"set\" for break", 0, 5);
        myOLED.setFont(MediumFontRus);
        delay(1000);
        if(flag_no_cal == true){
          break;
        }
      }
      if(flag_no_cal == false){
        mySerial.write(cmd_calibrate, 9);
      }
      flag = false;
    }
  }
  flag = true;
  flag_cal = false;
  c = 0;
  while (flag == true) {    // сброс истории датчика СО2 если он передавал хрень
    flag = button_set.check(flag);
    flag_cal = button_plus.check(flag_cal);
    for (; c < 1; c++) {
      myOLED.clrScr();
      myOLED.print("Reset CO_2", 0, 1);
      myOLED.print("history ?", 0, 3);
      myOLED.setFont(SmallFontRus);
      myOLED.print("Please press +", 0, 5);
      myOLED.setFont(MediumFontRus);
    }
    if (flag_cal == true) {
      myOLED.clrScr();
      myOLED.print("OK!", 0, 1);
      history_co2[0] = 900;
      for (byte i = 1; i < count_points_history; i++) {
        history_co2[i] = 1000;
      }
      delay(1000);
      flag = false;
    }
  }
  flag = true;
  flag_cal = false;
  c = 0;
  while (flag == true) {    // запрос всех параметров прямо сейчас
    flag = button_set.check(flag);
    flag_cal = button_plus.check(flag_cal);
    for (; c < 1; c++) {
      myOLED.clrScr();
      myOLED.setFont(SmallFontRus);
      myOLED.print("Request all", 0, 1);
      myOLED.print("Please press +", 0, 3);
    }
    if (flag_cal == true) {
      myOLED.print("Please wait 3 min", 0, 3);
      myOLED.print("....", 0, 5);
      measure();
      update_history();
      myOLED.print(level_co2, 0, 3);
      myOLED.print("ppm");
      myOLED.print("    ");
      myOLED.print(temperature);
      myOLED.print("grad");
      myOLED.print("   ");
      myOLED.print(pressure, 0, 5);
      myOLED.print("kPa");
      myOLED.print("    ");
      myOLED.print(humidity);
      myOLED.print("%");
      myOLED.print("   ");
      delay(1000);
      flag_cal = false;
    }
  }
  myOLED.setFont(MediumFontRus);
  myOLED.clrScr();
}
