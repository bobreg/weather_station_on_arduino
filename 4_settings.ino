//------------настройка-------------------------------
void settings() {
  myOLED.clrScr();
  myOLED.setCursor(0, 1);
  myOLED.print("Settings");
  delay(1000);
  bool flag = true;
  bool flag_cal = false;
  byte c = 0;
  while (flag == true) {    // калибровка датчика СО2
    flag = button_set.check(flag);
    flag_cal = button_plus.check(flag_cal);
    for (; c < 1; c++) {
      myOLED.setCursor(0, 1);
      myOLED.print("Calibrate");
      myOLED.setCursor(0, 3);
      myOLED.print("CO_2 ?");
      myOLED.setFont(SmallFontRus);
      myOLED.setCursor(0, 5);
      myOLED.print("Please press +");
      myOLED.setFont(MediumFontRus);
    }
    if (flag_cal == true) {
      //mySerial.write(cmd_calibrate, 9);
      myOLED.clrScr();
      myOLED.setCursor(0, 1);
      myOLED.print("wait!");
      for (byte i = 9; i > 0; i--) {
        myOLED.setCursor(20, 3);
        myOLED.print(i);
        delay(1000);
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
      myOLED.setCursor(0, 1);
      myOLED.print("Reset CO_2");
      myOLED.setCursor(0, 3);
      myOLED.print("history ?");
      myOLED.setFont(SmallFontRus);
      myOLED.setCursor(0, 5);
      myOLED.print("Please press +");
      myOLED.setFont(MediumFontRus);
    }
    if (flag_cal == true) {
      myOLED.clrScr();
      myOLED.setCursor(0, 1);
      myOLED.print("OK!");
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
  while (flag == true) {    // запрос со2 прямо сейчас
    flag = button_set.check(flag);
    flag_cal = button_plus.check(flag_cal);
    for (; c < 1; c++) {
      myOLED.clrScr();
      myOLED.setCursor(0, 1);
      myOLED.print("Request ");
      myOLED.setCursor(0, 3);
      myOLED.print("CO_2?");
      myOLED.setFont(SmallFontRus);
      myOLED.setCursor(0, 5);
      myOLED.print("Please press +");
    }
    if (flag_cal == true) {
      myOLED.setCursor(0, 5);
      myOLED.print("_____________");
      mySerial.write(cmd, 9);
      memset(response, 0, 9);  // заполняет память ответа от датчика нулями
      mySerial.readBytes(response, 9);
      if (response[0] == 0xFF && response[1] == 0x86)  {
        level_co2 = 256 * (unsigned int)response[2] + (unsigned int)response[3];
        myOLED.setCursor(0, 5);
        myOLED.print(level_co2);
        history_co2[0] = level_co2;
      }
      delay(1000);
      flag_cal = false;
    }
  }
  myOLED.setFont(MediumFontRus);
  myOLED.clrScr();
}
