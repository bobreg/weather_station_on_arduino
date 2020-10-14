void loop() {
  if (flag_button_wake_up == false) {  // если кнопка прерывания не была нажата, то...
    count_sleepss_period = 0;          // обнулим количество периодов долгого сна
    measure();                         // опросим датчики
    update_history();                  // обновим историю
    //Serial.println("всё измерили и обновили историю");
    digitalWrite(4, LOW);              // уберём питание с датчиков и экрана
    long_sleep();                      // и опять уйдём в сон
  } else {                                  // если кнопка прерывания была нажата, то
    if (millis() - last_times_1 > 5000) {   // каждые пять секунд меняем информацию
      last_times_1 = millis();              // на экране
      view_weather();
    }
    if (millis() - last_times_2 > 60000) {  //через минуту уходим в сон (60000)
      flag_button_wake_up = false;  // сбросим флаг кнопки прерывания
      digitalWrite(4, LOW);         // уберём питание с датчиков и экрана
      long_sleep();                 // уйдём в догий сон
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
    delay(300); //перед сном нужно выдержать паузу, чтобы отработала переферия
    power.sleep(SLEEP_8192MS);
    count_sleepss_period++;
    if (flag_button_wake_up == true) {
      break;
    }
    Serial.println(count_sleepss_period);
  }
  digitalWrite(4, HIGH);             // включим датчики
  last_times_1 = millis();
  last_times_2 = millis();
  delay(1000);                       // выдержим паузу
}

//-------функция для прерывания которая возводит флаг
//-------и по этому флагу ардуина выходит из долгого сна-------------
void empty_func() {
  flag_button_wake_up = true;
  //Serial.println("нажали кнопку");
}
