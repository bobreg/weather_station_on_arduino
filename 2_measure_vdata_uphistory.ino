//------измерение всех параметров---------------------
void measure() {
  //Serial.println("---measure---");
  myOLED.begin();  // инициализация экрана  
  myOLED.setFont(MediumFontRus);
  myOLED.print(" Measure", 0, 1);
  measure_co2();
  //-----измерение и сохранение температуры, влажности, давления и высоты---------
  //Serial.println("и вот мы тут");
  status_bme = bme.begin(0x76);
  //Serial.print("status_bme: ");
  //Serial.println(status_bme);
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
  myOLED.clrScr();
}

//-------------режим данных на экране-----------------
void view_weather() {
  myOLED.begin();  // инициализация экрана  
  //Serial.println("показываю");
  myOLED.setFont(MediumFontRus);
  type_info_on_oled = (type_info_on_oled + 1) % 5;
  if (type_info_on_oled == 0) {
    myOLED.clrScr();
    myOLED.print("Temp-re: ", 0, 1);
    //отладка
    //myOLED.print(count_wake_up);
    //
    myOLED.print(temperature, 2, 4);
    myOLED.print(" ");
    myOLED.print("deg");
  } else if (type_info_on_oled == 1) {
    myOLED.clrScr();
    myOLED.print("Pressure:", 0, 1);
    myOLED.print(pressure, 2, 4);
    myOLED.print(" ");
    myOLED.print("mm");
    myOLED.print((float)pressure * 0.133, 2, 6);
    myOLED.print(" ");
    myOLED.print("kPa");
  } else if (type_info_on_oled == 2) {
    myOLED.clrScr();
    myOLED.print("Humidity:", 0, 1);
    myOLED.print(humidity, 2, 4);
    myOLED.print(" ");
    myOLED.print("%");
  } else if (type_info_on_oled == 3) {
    myOLED.clrScr();
    myOLED.print("Level_CO2", 0, 1);
    myOLED.print(level_co2, 2, 4);
    myOLED.print(" ");
    myOLED.print("ppm");
  } else if (type_info_on_oled == 4) {
    myOLED.clrScr();
    myOLED.print("Altitude:", 0, 1);
    myOLED.print(altitude, 2, 4);
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

// запрос со2 надо делать долго, ибо после включения датчик некоторое время даёт
// одинаковый ответ
void measure_co2() {
  Serial.println("запросим со2");
  unsigned long timer = millis();
  bool flag = true;
  int count_period = 20;
  while (flag == true) {
    if (millis() - timer > 10000) {
      //Serial.println(count_period);
      myOLED.setFont(MediumFontRus);
      myOLED.print("  ---", 0, 5);
      myOLED.print(count_period);
      myOLED.print("--- ");
      count_period--;
      timer = millis();
      mySerial.write(cmd, 9);
      memset(response, 0, 9);  // заполняет память ответа от датчика нулями
      delay(100);
      mySerial.readBytes(response, 9);
      if (response[0] == 0xFF && response[1] == 0x86)  {
        level_co2 = 256 * (unsigned int)response[2] + (unsigned int)response[3];
      }
      Serial.println(level_co2);
      myOLED.setFont(SmallFontRus);
      myOLED.print("Level CO_2: ", 0, 7);
      myOLED.print(level_co2);
      myOLED.print("--- ");
    }
    while (mySerial.available()) { // необходимо для отчистки регистров порта
      mySerial.read();
    }
    flag = button_set.check(flag);
    if (count_period == 0) {
      break;
    }
  }
  //Serial.println("намерили со2");
}
