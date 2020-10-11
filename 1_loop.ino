void loop() {
  if (flag_button_wake_up == false) {  // если кнопка прерывания не была нажата, то...
    count_sleepss_period = 0;          // обнулим количество периодов долгого сна
    measure();                         // опросим датчики
    update_history();                  // обновим историю
<<<<<<< HEAD
    send_param();                      // функция отладки
    digitalWrite(4, LOW);              // уберём питание с датчиков и экрана
    long_sleep();                      // и опять уйдём в сон
  } else {                                  // если кнопка прерывания была нажата, то
    if (millis() - last_times_1 > 5000) {   // каждые пять секунд меняем информацию
      last_times_1 = millis();              // на экране
      view_weather();
    }
    if (millis() - last_times_2 > 60000) {  //через минуту уходим в сон (60000)
=======
    send_param();
    digitalWrite(4, LOW);              // уберём питание с датчиков и экрана
    long_sleep();                      // и опять уйдём в сон
  } else {                                  // если кнопка прерывания была нажата, то
    if (millis() - last_times_1 > 5000) {  // каждые пять секунд меняем информацию
      flag_button_wake_up = false;  // сбросим флаг кнопки прерывания
      last_times_1 = millis();             // на экране
      view_weather();
    }
    if (millis() - last_times_2 > 60000) {  //через минуту уходим в сон (60000)
      //myOLED.clrScr();              // почистим экран
>>>>>>> f51afda13614fd8ecd70466866530d673dec91c7
      digitalWrite(4, LOW);         // уберём питание с датчиков и экрана
      flag_button_wake_up = false;  // сбросим флаг кнопки прерывания
      long_sleep();                 // уйдём в догий сон
      last_times_2 = millis();
      measure();
    }
  }
  // по короткому нажатию кнопки меняем инф на экране
  // длинное нажатие (3 секунды) переведёт нас на одну минуту в режим графиков
  button_plus.check(&view_weather, &history_measuring_as_graf);
  button_set.check(&settings, &history_measuring_as_data);
}

//-----------долгий сон-----------------
void long_sleep() {
  // когда ардуина выходит из сна она проверяет сколько
  // прошло периодов и уходит обратно в сон или дальше
  // на выполнение
  while (count_sleepss_period < period_sleep) {
    
<<<<<<< HEAD
    power.sleep(SLEEP_8192MS);
    //delay(8192);
=======
    //power.sleep(SLEEP_8192MS);
    delay(8192);
>>>>>>> f51afda13614fd8ecd70466866530d673dec91c7
    count_sleepss_period++;
    if(period_sleep - count_sleepss_period < 20){
      Serial.println("разбудим датчики");
      digitalWrite(4, HIGH);  // заранее включим питание на датчиках для прогрева
                              // и корректного ответа
    }
    if (flag_button_wake_up == true) {
      break;
    }
    Serial.println(count_sleepss_period);
  }
<<<<<<< HEAD
  digitalWrite(4, HIGH);             // включим датчики
  delay(2000);                       // выдержим паузу
=======
  //count_wake_up++;
  //Serial.println("re-re");
  digitalWrite(4, HIGH);
  delay(1000);
>>>>>>> f51afda13614fd8ecd70466866530d673dec91c7
}

//-------функция для прерывания которая возводит флаг
//-------и по этому флагу ардуина выходит из долгого сна-------------
void empty_func() {
  flag_button_wake_up = true;
  last_times_2 = millis();
  Serial.println("нажали кнопку");
}

//------------вывод паузы---------------
void pause(String str){
  myOLED.begin();
  myOLED.setFont(MediumFontRus);
  myOLED.setCursor(0, 1);
  myOLED.print(str);
  for (int i = 70; i > 0; i--) {
    myOLED.setCursor(20, 3);
    myOLED.print(i);
    myOLED.print(" ");
    //Serial.println(i);
    delay(1000);
  }
}
