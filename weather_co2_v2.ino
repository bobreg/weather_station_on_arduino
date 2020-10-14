#include <iarduino_OLED_txt.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <GyverPower.h>

#define count_points_history 24

//------инициируем библиотеку экранчика---------

iarduino_OLED_txt myOLED(0x3C);
extern uint8_t MediumFontRus[];
extern uint8_t SmallFontRus[];
//---- инициируем данные для СО2 датчика------

SoftwareSerial mySerial(A0, A1); // A0 - к TX сенсора, A1 - к RX
byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte cmd_no_abc[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};
byte cmd_calibrate[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
unsigned char response[9];

//------временные переменные------------------
unsigned long last_times_1 = 0;
unsigned long last_times_2 = 0;
unsigned int count_sleepss_period = 0;
const int period_sleep = 423;
// 846 примерно 2 часа
// 423 примерно 1 час

//---------SEALEVELPRESSURE_HPA (константа для вычисления высоты)-------
float const_level = 101325;
//---------переменные для хранения измеренных значений-------------
unsigned int level_co2 = 0;
int history_co2[count_points_history] = {900, 1000, 1000, 1000, 1000, 
                       1000, 1000, 1000, 1000, 1000,
                       1000, 1000, 1000, 1000, 1000,
                       1000, 1000, 1000, 1000, 1000,
                       1000, 1000, 1000, 1000};

byte temperature = 0;
int history_temperature[count_points_history] = {22, 23, 23, 23, 23,
                               23, 23, 23, 23, 23,
                               23, 23, 23, 23, 23,
                               23, 23, 23, 23, 23,
                               23, 23, 23, 23};

int pressure = 0;
int history_pressure[count_points_history] = {739, 740, 740, 740, 740,
                            740, 740, 740, 740, 740,
                            740, 740, 740, 740, 740,
                            740, 740, 740, 740, 740,
                            740, 740, 740, 740};

byte humidity = 0;
int history_humidity[count_points_history] = {19, 20, 20, 20, 20,
                            20, 20, 20, 20, 20,
                            20, 20, 20, 20, 20,
                            20, 20, 20, 20, 20,
                            20, 20, 20, 20};

int altitude = 0;

//-----иничиализация датчика BME-----------------
Adafruit_BME280 bme;  //создание экземпляра класса bme280
bool status_bme;

//----------объявление параметров кнопок--------------------
class Buttons_drive {  //класс кнопки
  public:
    Buttons_drive(int pin) {  //инициализация
      _pin = pin;
      pinMode(_pin, INPUT);
    }
    void check(void (*func)()) {  // обработка однократного нажатия
      delay(20);
      state = digitalRead(_pin);
      if (state == HIGH && flag == false) {
        flag = true;
        (*func)();
      }   
      state = digitalRead(_pin);
      if (state == LOW) {
        flag = false;
      }
    }
    bool check(bool flag_in) {  // обработка однократного нажатия и возврат !true или !false
      delay(20);
      state = digitalRead(_pin);
      if (state == HIGH && flag == false) {
        flag = true;
        return !flag_in;
      }
      state = digitalRead(_pin);
      if (state == LOW) {
        flag = false;
      }
      return flag_in;
    }
    void check(void (*func1)(), void (*func2)()) {  // обработка нажатия с удержанием
      state = digitalRead(_pin);
      delay(20);
      if (state == HIGH && flag == false) {
        flag = true;
        int timer_butt = millis() % 10000;
        while(1){
          state = digitalRead(_pin);
          if (millis() % 10000 - timer_butt < 3000 && state == LOW) {
            (*func1)();
            break;
          }
          if (millis() % 10000 - timer_butt > 3000) {
            (*func2)();
            break;
          }
        }
      }
      state = digitalRead(_pin);
      if (state == LOW) {
        flag = false;
      }
    }
    void check(void (*func)(int ), int param) { // передаём ссылку на функцию
      state = digitalRead(_pin);               // и ещё параметр для функции
      if (state == HIGH && flag == false) {
        flag = true;
        (*func)(param);
      }
      delay(20);
      state = digitalRead(_pin);
      if (state == LOW) {
        flag = false;
      }
    }
  private:
    int _pin;
    bool state = LOW;
    bool flag = false;
};

Buttons_drive button_plus(10);
Buttons_drive button_set(11);
bool flag_button_wake_up = false;
bool flag_history_graf = true; // флаг для входа в режим графиков
bool flag_history_data = true; // флаг для входа в режим отображения истории


byte type_info_on_oled = 4;
byte type_history_on_oled = 0;
byte type_graf_on_oled = 0;

void setup() {
  power.autoCalibrate();
// инициализация обмена
  Serial.begin(9600);
  mySerial.begin(9600);
// инициализация главных таймеров
  last_times_1 = millis();
  last_times_2 = millis();

// контакт на прерывание для выхода из сна
  pinMode(2, INPUT);
  attachInterrupt(0, empty_func, RISING);
// контакт управления транзистора
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  while (mySerial.available()) { // необходимо для отчистки регистров порта
      mySerial.read();
  }
  //mySerial.write(cmd_no_abc, 9);
}
