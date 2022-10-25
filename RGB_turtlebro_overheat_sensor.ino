#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <FastLED.h>166626 11
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Bool.h>
/********************************************************************************************/
int counterofheat = 0;// количество найденных нагревательных элементов
int massedge = 0;// сообщение для ручного включения
int amg88xx_delay  = 1000; //ms задержка для сенсора
int sub_delay = 10; //ms хз, походу это задержка для serial1
int sum = 0;
float maxx = 0;
float avgtemp; // средняя температура с модуля AMG88
int Rled, Gled, Bled;
unsigned long previousMillis = 0;
#define Num 24 // количество светодиодов 
#define Pin 30 // пин ленты на главной плате черепахи
CRGB leds[Num]; // массив для работы ленты
Adafruit_AMG88xx amg;
float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
/********************************************************************************************/
// Функция для нахождения средней температуры
/*int summ() {
  amg.readPixels(pixels);
  for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; ++i) {
    sum += pixels[i];
    avgtemp = sum / 64;
  }
  }
*/
int high() {
  amg.readPixels(pixels);
  for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; ++i) {
    if (maxx < pixels[i]) {
      maxx = pixels[i];
    }
  }
}
/********************************************************************************************
  //Функция для проверки сообщений из терминала в ручном режиме
  int massedgeChek() {
  if (massedge > 0) {
    for (int i = 0; i < 23; i++) {
      leds[i] = CRGB(255, 255, 255);
      FastLED.show();
      delay(50);
    }
  }
  }
  /********************************************************************************************/
// Функция выбора цвета и работы с лентой
int LedlentAmg() {
  //Serial.println(avgtemp);
  if (maxx > 20) {
    if (counterofheat > 0) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 0: Rled = 0; Gled = 0; Bled = 255; counterofheat++;
        break;
    }
  }
  if (maxx > 25) {
    if (counterofheat > 1) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 1: Rled = 23; Gled = 0; Bled = 232; counterofheat++;
        break;
    }
  }
  if (maxx > 30) {
    if (counterofheat > 2) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 2: Rled = 46; Gled = 0; Bled = 209; counterofheat++;
        break;
    }
  }
  if (maxx > 35) {
    if (counterofheat > 3) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 3: Rled = 69; Gled = 0; Bled = 186; counterofheat++;
        break;
    }
  }
  if (maxx > 40) {
    if (counterofheat > 4) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 4: Rled = 91; Gled = 0; Bled = 163; counterofheat++;
        break;
    }
  }
  if (maxx > 45) {
    if (counterofheat > 5) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 5: Rled = 114; Gled = 0; Bled = 140; counterofheat++;
        break;
    }
  }
  if (maxx > 50) {
    if (counterofheat > 6) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 6: Rled = 137; Gled = 0; Bled = 117; counterofheat++;
        break;
    }
  }
  if (maxx > 55) {
    if (counterofheat > 7) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 7: Rled = 160; Gled = 0; Bled = 94; counterofheat++;
        break;
    }
  }
  if (maxx > 60) {
    if (counterofheat > 8) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 0: Rled = 183; Gled = 0; Bled = 71; counterofheat++;
        break;
    }
  }
  if (maxx > 65) {
    if (counterofheat > 9) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 9: Rled = 206; Gled = 0; Bled = 48; counterofheat++;
        break;
    }
  }
    if (maxx > 70) {
    if (counterofheat > 10) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 0: Rled = 229; Gled = 0; Bled = 25; counterofheat++;
        break;
    }
  }
    if (maxx > 75) {
    if (counterofheat > 11) {
      counterofheat = 0;
    }
    switch (counterofheat) {
      case 0: Rled = 252; Gled = 0; Bled = 2; counterofheat++;
        break;
    }
  }
    for (int i = 0; i < 23; i++) {
      leds[i] = CRGB(Rled, Gled, Bled);
      FastLED.show();
      delay(10);
    }
    delay(50);
  }

/********************************************************************************************/
// #СЛИШКОМСЛОЖНОНЕЛЮБЛЮРОС
class NewHardware : public ArduinoHardware
{
  public:
    NewHardware(): ArduinoHardware(&Serial1, 115200) {};
};
ros::NodeHandle_<NewHardware>  nh;

std_msgs::Float32MultiArray f_array_msg;
std_msgs::Bool toggle_msg;

ros::Publisher amg88xx_pixels_pub("amg88xx_pixels", & f_array_msg);

void messageCb(const std_msgs::Bool & toggle_msg)
{
  Serial.print("toggle_msg.data:");
  Serial.println(toggle_msg.data);
  if (toggle_msg.data == 1) {
    massedge++;
  }
}
// sub
ros::Subscriber<std_msgs::Bool> sub("alarm_led", &messageCb );
/********************************************************************************************/
void setup() {
  Serial.begin(115200); // скорасть аппаратного сериала
  FastLED.addLeds <WS2812, Pin, GRB>(leds, Num).setCorrection(TypicalLEDStrip); // настройка ленты
  FastLED.setBrightness(255);
  bool status;
  status = amg.begin();
  if (!status) {
    while (1);
  }
  // сброс ленты
  for (int i = 0; i < 23; i++) {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(10);
  }
  delay(100);
  // #СЛИШКОМСЛОЖНОНЕЛЮБЛЮРОС
  f_array_msg.data = (float*)malloc(sizeof(float) * AMG88xx_PIXEL_ARRAY_SIZE);
  f_array_msg.data_length = AMG88xx_PIXEL_ARRAY_SIZE;
  nh.initNode();
  nh.advertise(amg88xx_pixels_pub);
  nh.subscribe(sub);
}
/********************************************************************************************/
void loop() {
  // #СЛИШКОМСЛОЖНОНЕЛЮБЛЮРОС
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= amg88xx_delay)
  {
    previousMillis = currentMillis;
    amg.readPixels(pixels);
    for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; ++i)
    {
      f_array_msg.data[i] = pixels[i];
    }
    amg88xx_pixels_pub.publish(&f_array_msg);
  }
  high();
  LedlentAmg();
  Serial.println(maxx);
  /*Serial.println(avgtemp);*/
  delay(100);
  nh.spinOnce();
  maxx = 0;
  sum = 0;
  avgtemp = 0;
  Rled = 0;
  Gled = 0;
  Bled = 0;
  massedge = 0;
  delay(sub_delay);
}
/********************************************************************************************/
