//------измерение всех параметров---------------------
void measure() {
  //-----измерение и сохранение значения уровня СО2--------
  if (flag_button_wake_up == false) { // если кнопка анктивации была нажата, то
                               // не надо запрашивать со2 т.к. его надо ещё прогреть
    mySerial.write(cmd, 9);
    memset(response, 0, 9);  // заполняет память ответа от датчика нулями
    delay(10);
    mySerial.readBytes(response, 9);
    if (response[0] == 0xFF && response[1] == 0x86)  {
      level_co2 = 256 * (unsigned int)response[2] + (unsigned int)response[3];
    }
    while (mySerial.available()) { // необходимо для отчистки регистров порта
      mySerial.read();
    }
  }
  //-----измерение и сохранение температуры, влажности, давления и высоты---------
  status_bme = bme.begin(0x76);
  if (status_bme) {
    temperature = bme.readTemperature();
    pressure = bme.readPressure() / 133.322;
    humidity = bme.readHumidity();
    altitude = bme.readAltitude(const_level / 100);
  } else {
    temperature = 255;
    pressure = 255;
    humidity = 255;
    altitude = 255;
  }
}

//-------------режим данных на экране-----------------
void view_weather() {
  myOLED.begin();
  type_info_on_oled = (type_info_on_oled + 1) % 5;
  if (type_info_on_oled == 0) {
    myOLED.clrScr();
    myOLED.setCursor(0, 1);
    myOLED.print("Temp-re: ");
    //отладка
    //myOLED.print(count_wake_up);
    //
    myOLED.setCursor(2, 4);
    myOLED.print(temperature);
    myOLED.print(" ");
    myOLED.print("deg");
  } else if (type_info_on_oled == 1) {
    myOLED.clrScr();
    myOLED.setCursor(0, 1);
    myOLED.print("Pressure:");
    myOLED.setCursor(2, 4);
    myOLED.print(pressure);
    myOLED.print(" ");
    myOLED.print("mm");
    myOLED.setCursor(2, 6);
    myOLED.print((float)pressure * 0.133);
    myOLED.print(" ");
    myOLED.print("kPa");
  } else if (type_info_on_oled == 2) {
    myOLED.clrScr();
    myOLED.setCursor(0, 1);
    myOLED.print("Humidity:");
    myOLED.setCursor(2, 4);
    myOLED.print(humidity);
    myOLED.print(" ");
    myOLED.print("%");
  } else if (type_info_on_oled == 3) {
    myOLED.clrScr();
    myOLED.setCursor(0, 1);
    myOLED.print("Level_CO2");
    myOLED.setCursor(2, 4);
    myOLED.print(level_co2);
    myOLED.print(" ");
    myOLED.print("ppm");
  } else if (type_info_on_oled == 4) {
    myOLED.clrScr();
    myOLED.setCursor(0, 1);
    myOLED.print("Altitude:");
    myOLED.setCursor(2, 4);
    myOLED.print(altitude);
    myOLED.print(" ");
    myOLED.print("m");
  }
}

//------сдвиг значений и запись в историю последнего измерения--------
void update_history() {
  for (byte i = count_points_history - 1; i > 0 ; i--) {
    history_co2[i] = history_co2[i - 1];
  }
  for (byte i = count_points_history - 1; i > 0 ; i--) {
    history_temperature[i] = history_temperature[i - 1];
  }
  for (byte i = count_points_history - 1; i > 0 ; i--) {
    history_pressure[i] = history_pressure[i - 1];
  }
  for (byte i = count_points_history - 1; i > 0 ; i--) {
    history_humidity[i] = history_humidity[i - 1];
  }
  history_co2[0] = level_co2;
  history_temperature[0] = temperature;
  history_pressure[0] = pressure;
  history_humidity[0] = humidity;
}
