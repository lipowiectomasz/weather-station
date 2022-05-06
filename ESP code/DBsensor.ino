//PROGRAM DZIAŁAJĄCY NA PŁYTCE ARDUINO UNO R3
//autor: Tomasz Lipowiec
//date: 18.06.2021r.
//code: Mikrokomputery Projekt
//chip: Arduino Uno R3
//hardware: Arduino, ESP8266, Czujnik temperatury i wilgotności DHT11, Grove - LM358 czujnik natężenia światła otoczenia,
//Grove - czujnik opadów / wody, Grove - czujnik czystości powietrza MP503 v1.3, Arduino-Dem - moduł wyświetlacza LCD 2'' 128x64px - SPI,
//Czujnik dźwięku LM393.
//title: Stacja pogodowa.
//END OF COMMENTS

                                                          
                                                          //DO KOMUNIKACJI Z ESP.
#include <SoftwareSerial.h>
SoftwareSerial espSerial (2, 3); //RX, TX                 //Wirtualny port szeregowy do przesyłu do ESP.
String ans;                                               //Zmienna, w której przechowywana jest odpowiedź z ESP.
char buffor[10];                                          //Bufor na dane do wysłania.

#include <Arduino.h>                                      //Deklaracja wyświetlacza.
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
U8G2_ST7565_NHD_C12864_1_4W_SW_SPI u8g2(U8G2_R0, 11, 8, 13, 12, U8X8_PIN_NONE);
                                                          
                                                          //DO SENSORA DŹWIĘKU.
const int sampleWindow = 50;                              //Częstotliwość próbkowania dźwięku w mS (50 mS = 20Hz).
unsigned int sample;                                      //Próba dźwięku.
#define SENSOR_PIN A3                                     //Deklaracja sensora dźwięku.
int db;
String vDb;                                               //Zmienna przechowująca wartość dźwięku.

                                                          //DO TERMOMETRA I WILGOTNOŚCIOMIERZA.
char vvDb[8];
#include <dht11.h>                                            
#define DHT11_pin A4
dht11 tempSensor;                                         //Deklaracja termometra na pinie 6.


                                                           
#define WATER_SENSOR A2                                   //Deklaracja wykrywacza opadów.
                                                          //DO MIERNIKA JAKOŚCI POWIETRZA.
#include"AirQuality.h"
#include"Arduino.h"
AirQuality airqualitysensor;
int current_quality = -1;
                                                          //DANE DO SYNCHRONIZACJI I PRZESYŁU DANYCH.
int timer = 0;
int timeSend = 60;
String destination;

char temp[8], hum[8], light[8], fall[8], quality[8];      //Tablice char do przesyłania wartości do ESP.

void setup() {

  Serial.begin(9600);                                     //Uruchomienie portów szeregowych.
  espSerial.begin(9600);
  Serial.println("Station: start!");

  u8g2.begin();                                           //Inicjalizacja wyświetlacza.
  u8g2.setContrast(500);
  pinMode(WATER_SENSOR, INPUT);
  airqualitysensor.init(A1);

  cli();                                                  //Wyłączenie przerwań.

                                                          //Zerowanie rejestrów licznika.
  TCCR1A = 0;                                             //Timer Counter Control 2 Register A
  TCCR1B = 0;                                             //Timer Counter Control 2 Register B
  TCNT1  = 0;                                             //Timer Counter 1

                                                          //Ustawienie rejestru porównawczego.
  OCR1A = 15624;                                          //=16MHZ/(1*1024)-1=15624

  TCCR1B |= (1 << WGM12);                                 //Włączanie trybu CTC.
  TCCR1B |= (1 << CS12) | (1 << CS10);                    //Ustawienie bitów CS10 i CS12 dla podziału
                                                          //1024 preskalera.
  TIMSK1 |= (1 << OCIE1A);                                //Odblokowanie przerwań od porównania licznika1.
  sei();                                                  //Włączenie przerwań.

}
void loop() {
                                                          //Mechanizm sensora dźwięku.
  unsigned long startMillis = millis();                   //Początek przedziału próbkowania.
  float peakToPeak = 0;                                   //Poziom różnicy wartości max i min.

  unsigned int signalMax = 0;                             //Wartość maksymalna.
  unsigned int signalMin = 1024;                          //Wartość minimalna.

                                                          //Zbieranie danych przez 50 mS.
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(SENSOR_PIN);                      //Odczyt z mikrofonu.
    if (sample < 1024)                                    //Określanie max i min.
    {
      if (sample > signalMax)
      {
        signalMax = sample;                               //Zapis max.
      }
      else if (sample < signalMin)
      {
        signalMin = sample;                               //Zapis min.
      }
    }
  }

  peakToPeak = signalMax - signalMin;                     //Różnica wartości szczytowych.
  db = map(peakToPeak, 20, 900, 49.5, 90);                //Przekalibrowanie na decybele.
  //End of sound sensor

  tempSensor.read(DHT11_pin);                             //Odczyt temperatury i wilgotności.

                                                          //Konwersja odczytanych wartości int -> char.
  itoa(tempSensor.temperature, temp, 10);
  itoa(tempSensor.humidity, hum, 10);
  itoa(analogRead(A0), light, 10);
  itoa(digitalRead(WATER_SENSOR), fall, 10);
  itoa(analogRead(A1), quality, 10);
  itoa(db, vvDb, 10);

  u8g2.setFont(u8g2_font_6x13B_tf);                       //Ustawienie czcionki wyświetlacza.


  u8g2.setFontDirection(0);                               //Ustawienie kierunku wyświetlania.

  u8g2.firstPage();                                       //Wyświetlanie wartości pomiarów na ekranie.
  do {
    u8g2.drawStr( 0, 9, "Temp: ");
    u8g2.drawStr( 100, 9, temp);
    u8g2.drawStr( 0, 20, "Wilg.: ");
    u8g2.drawStr( 100, 20, hum);
    u8g2.drawStr( 0, 31, "Opady: ");
    u8g2.drawStr( 100, 31, fall);
    u8g2.drawStr( 0, 42, "Czys. pow.: ");
    u8g2.drawStr( 100, 42, quality);
    u8g2.drawStr( 0, 53, "Nat. sw.: ");
    u8g2.drawStr( 100, 53, light);
    u8g2.drawStr( 0, 64, "Dzwiek: ");
    u8g2.drawStr( 100, 64, vvDb);
  } while ( u8g2.nextPage() );

  while (Serial.available()) {                            //Oczyt poleceń z portu szeregowego Arduino i przesłanie do ESP
    ans = Serial.readString();                            //Wykorzystywane w czasie testowania i budowania układu.
    Serial.print("Main serial: ");
    Serial.println(ans);
    ans.toCharArray(buffor, 10);
    espSerial.write(buffor);
  }

  while (espSerial.available()) {                         //Odczyt danych przesyłanych z ESP.
    ans = espSerial.readString();
    Serial.print("Secound serial: ");
    Serial.println(ans);
    if (ans == "Buffor ready!")                           //Przesłanie wartości pomiarów do ESP po otrzymaniu polecenia "Buffor ready!".
    {
      Serial.println("Transfer open!");
      delay(1000);
      Serial.println("Sending destination");
      destination.toCharArray(buffor, 7);
      Serial.println(buffor);
      espSerial.write(buffor);
      delay(1000);
      Serial.println("Sending temp");
      espSerial.write(temp);
      delay(1000);
      Serial.println("Sending hum");
      espSerial.write(hum);
      delay(1000);
      Serial.println("Sending fall");
      espSerial.write(fall);
      delay(1000);
      Serial.println("Sending quality");
      espSerial.write(quality);
      delay(1000);
      Serial.println("Sending light");
      espSerial.write(light);
      delay(1000);
      Serial.println("Sending vvDb");
      espSerial.write(vvDb);
      delay(1000);
    }
  }

}
ISR(TIMER1_COMPA_vect) {                                  //Funkcja wykonująca się w przerwaniu licznika.

  if(timer == 3600){                                   	  //Przesłanie wartości pomiarów do bazy danych co godzinę.
    Serial.println("Trying to send data(toBase)!");
    espSerial.write("Data");
    destination = "toBase";
    timer = 0;
    timeSend = 60;
    }

  if (timer == timeSend) {                                //Przesłanie wartości na stronę internetową co 2 minuty.
    Serial.println("Trying to send data!");
    espSerial.write("Data");
    destination = "toWebs";
    timeSend+=60;
  }
  timer++;
}
ISR(TIMER1_OVF_vect)                                      //Mechanizm miernika jakości powietrza (dostarczany w bibliotece producenta).
{
  if (airqualitysensor.counter == 122)
  {
    airqualitysensor.last_vol = airqualitysensor.first_vol;
    airqualitysensor.first_vol = analogRead(A1);
    airqualitysensor.counter = 0;
    airqualitysensor.timer_index = 1;
    PORTB = PORTB ^ 0x20;
  }
  else
  {
    airqualitysensor.counter++;
  }
}
