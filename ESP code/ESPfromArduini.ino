//PROGRAM DZIAŁAJĄCY NA MODULE ESP8266-01
//autor: Tomasz Lipowiec
//date: 18.06.2021r.
//code: Mikrokomputery Projekt
//chip: ESP8266
//hardware: Arduino, ESP8266, Czujnik temperatury i wilgotności DHT11, Grove - LM358 czujnik natężenia światła otoczenia,
//Grove - czujnik opadów / wody, Grove - czujnik czystości powietrza MP503 v1.3, Arduino-Dem - moduł wyświetlacza LCD 2'' 128x64px - SPI,
//Czujnik dźwięku LM393.
//title: Stacja pogodowa.
//END OF COMMENTS
#include <ESP8266WiFi.h>
                                                      //Informacje potrzebne do połączenia z siecią.
const char* ssid = "";
const char* password = "";
const char* host = "";

String command;                                       //Zmienne służące do odbierania, przetwarzania
String buffor[7];                                     //i przesyłania danych.
char chbuffor[10];
long int i = 0;

void setup() {
  delay(100);
  Serial.begin(9600);                                 //Uruchomienie portu szeregowego.

  WiFi.mode(WIFI_STA);                                //Zmiana trybu na połączeniowy.
  WiFi.begin(ssid, password);                         //Łączenie ze zdefiniowanym access pointem.
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);                                       //Wyświetlanie kropek w czasie nawiązywania połączenia.
    Serial.print(".");                                  
  }

}
void loop() {
  while (Serial.available())                          //Oczekiwanie na dane z portu szeregowego (z Arduino).
  {
    command = Serial.readString();                      
    if (command == "Data") {                          //W momencie otrzymania polecenia "Data"
      Serial.write("Buffor ready!");                  //ESP odsyła polecenie "Buffor ready!" co znaczy, że moduł
      while (i < 7)                                   //jest gotowy aby przyjąć dane.
      {
        if (Serial.available() > 0) {
          command = Serial.readString();              //Dane wszystykich pomiarów zapisywane są w tablicy buffor.
          buffor[i] = command;
          i++;
        }
      }
      Serial.write("Buffor filled!");                 //Komunikat o wypełnieniu buforu.
      i=0;
      while (i < 7)
      {
        Serial.write("Buffor: ");                       
        if(i==0){
          buffor[i].toCharArray(chbuffor, 7);
          }
        else {
          buffor[i].toCharArray(chbuffor, 4);         //Wyświetlenie otrzymanych danych.
          }
        Serial.write(chbuffor);
        delay(10);
        i++;
      }
      if(buffor[0]=="toBase"){                        //W zależności od pierwszego elementu tablicy jakim jest
          sendDataToPHP("/toBase.php?");              //informacja o tym gdzie mają być wysłane dane, uklad decyduje
          sendDataToPHP("/toWeb.php?");               //czy przesyłać je do bazy i do strony internetowej (co godzinę)
        }                                             //czy tylko do strony internetowej.
      else if(buffor[0]=="toWebs"){
          sendDataToPHP("/toWeb.php?");
        }
      i = 0;
    }
  }
}
void sendDataToPHP(String destUrl) {                  //Metoda przesyłająca dane do podanego jako argument skryptu PHP
  Serial.println("Try send to PHP");                  //na serwerze.

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");              //Komunikat wyświetlany kiedy połączenie z serwerem nie może
    sendDataToPHP(destUrl);                           //zostać zrealizowane. Następuje po nim ponowna próba.
  }

  String url = destUrl;                               //Budowanie łańcucha znaków, w którym zawarte zostają pomiary
                                                      //,które zostaną przesłane do skryptu PHP metodą GET.
  url += "temp=";
  url += buffor[1];
  url += "&hum=";
  url += buffor[2];
  url += "&fall=";
  url += buffor[3];
  url += "&quality=";
  url += buffor[4];
  url += "&light=";
  url += buffor[5];
  url += "&vvDb=";
  url += buffor[6];

                                                      //Przesłanie danych metodą GET.
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

}