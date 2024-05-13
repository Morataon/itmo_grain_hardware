#include <SoftwareSerial.h> // подключаем библиотеку SoftwareSerial
#define zummer 13 
#define RX_PIN 0 // RX для Bluetooth
#define TX_PIN 1 // TX для Bluetooth
SoftwareSerial bluetooth(RX_PIN, TX_PIN); // создаем объект Bluetooth в рамках библиотеки SoftwareSerial

byte out_pin[] = {3, 9, 5, 6, 10}; // !пины для системы 2.0! все однотипные переменные группируем в массив для циклической обработки (включая номера контактов)
byte Button[] = {12, 7, 4, 2, 8}; // !пины для системы 2.0!  
boolean last_button[] = {LOW, LOW, LOW, LOW, LOW}; 
boolean curent_button[] = {LOW, LOW, LOW, LOW, LOW}; // текущее состояние кнопки
boolean mode[] = {LOW, LOW, LOW, LOW, LOW}; // режимы кнопок 
boolean flag = false; // флаг для первого нажатия кнопки 
unsigned long time = 0; 
int brightness = 10; // яркость светодиода 
int fadeAmount = 1; // величина изменения яркости светодиода 
int led = 0;

void setup() 
{ 
 Serial.begin(9600); // последовательный порт для вывода сообщений
 bluetooth.begin(9600); // начало работы последовательного порта Bluetooth на скорости 9600

 for (byte i = 0; i < 5; i++) 
 { 
  pinMode(Button[i], INPUT_PULLUP); // использование внутренних подтягивающих резисторов 
  pinMode(out_pin[i], OUTPUT); 
 }

 pinMode(zummer, OUTPUT); 
 pinMode(11, OUTPUT); // светодиод активации аппаратной части
 analogWrite(11, 10); 
}

boolean debounce(boolean last, byte Button) 
{ 
 if (last != digitalRead(Button)) 
 {
  delay(5); 
 } 
 return !digitalRead(Button); 
}

void loop() 
{ 
 if (millis() - time >= 100) 
 { 
  for (byte i = 0; i < 5; i++) 
  { 
   curent_button[i] = debounce(last_button[i], Button[i]); 
   if (!last_button[i] && curent_button[i] && !flag) 
   { 
    mode[i] = !mode[i]; // переключение режима кнопки 
    Serial.print(String(i + 1)); 
  
    flag= true; 
    led = out_pin[i]; 
    tone(zummer, 1000, 500); 
   } 
   analogWrite(led, brightness); 
   brightness = brightness + fadeAmount; 
   if (brightness <= 10 || brightness >= 255) 
   { 
    fadeAmount = -fadeAmount; 
   } 
   last_button[i] = curent_button[i];

   if (Serial.available() > 0)
   {
    int incomingByte = Serial.read();
    if(incomingByte == '1' || incomingByte == 1) 
    {                              
       flag = true;               
    }
    if(incomingByte == '2' || incomingByte == 2) 
    {
       flag = false;
       led = 0;// выключение всех светодиодов
       for (byte i = 0; i < 5; i++) 
       {
        analogWrite(out_pin[i], 0);                  
       }
    }
   }
  }
 }
}
