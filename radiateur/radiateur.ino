#include <Arduino.h>
#include <ArduinoJson.h>
//#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <DHT.h>
#include <SPI.h>
#include "FS.h"
#include "espUser.h"
#include "espUser.cpp"
#include "connexion.h"
#include "mode_radiateur.h"
#include"request.h"


#define ssid "TP-Link_8A62"
#define password "98901814"
#define pinLed 0
#define DHTPIN 2                      // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11              // DHT 11 sensor 
DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure client;
ESP8266WebServer server(80);


//redemarrage toutes les 72 heures
const unsigned long TIME_INTERVAL = 259200000; //72heure
unsigned long previousMillis = millis();

const unsigned long TIME_INTERVAL_MESSAGE = 3000; //5 SECONDES
unsigned long previousMillisMessage = millis();

float indice = 0;           //indice de passage dans la fonction TemperaturAutoOnly()
int sliderValue = 0;
bool ledValue = false;
boolean relaySwicht = false;
int RelayValue = 0;
const String relayON = "ON";
const String relayOFF = "OFF";
String nomEsp = "esp_njhomes3";
const String errorMessage = "Vous n'avez pas de compte veuillez-vous inscrire";
String espWebMessage = "";
int indexAgrs = 0;
EspUser esp( dht.readHumidity(), dht.readTemperature(), 19, "31690849-3cae-42d3-982c-b5141b84d835", "", "off", "", "", 1, nomEsp + ".local" , "", true);
EspUser espTempo;
void ArgsPage();
void handleTemperatureHumidity();
void getAddUser();
EspUser sendMessage();


void setup() {

  //ip fixe
  //      IPAddress ip(192, 168, 1, 67);
  //    IPAddress dns(192, 168, 1, 254);
  //    IPAddress gateway(192, 168, 1, 254);
  //    IPAddress subnet(255, 255, 255, 0);
  //    WiFi.config(ip);
  // initialisation de la communication série, Spi et dht

  Serial.begin(115200);
  SPIFFS.begin();
  dht.begin();
  pinMode(pinLed, OUTPUT);

  connexion();

  esp.setIpAdresse(WiFi.localIP());                       //set ip local pour le transmettre à la base de données
  client.setInsecure();                                   //the magic line, use with caution
  /**
     vereification de l'exitantce du compte utilisateur
  */
  espTempo = GetOne( client, esp);
  if (espTempo.getEmail_user() != "") {
    Serial.println("Ce module est bien associé à un utilisateur");
    esp = espTempo;
  }
  else {
    espWebMessage =  "Ce module n'est pas associé";
  }



  //ApiServer.begin();
  Serial.print(F("IP address: "));
  Serial.println(esp.getIpAdresse());
  if (!MDNS.begin(nomEsp)) {
    Serial.println(F("Error setting up MDNS responder!"));
    while (1) {
      delay(1000);
    }
  }

  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/iot-widget.min.js", SPIFFS, "/iot-widget.min.js");
  server.serveStatic("/style.css", SPIFFS, "/style.css");
  server.serveStatic("/hot.png", SPIFFS, "/hot.png");
  server.serveStatic("/gauge.js", SPIFFS, "/gauge.js");
  server.on("/temperatureHumidity", HTTP_GET, handleTemperatureHumidity);
  server.on("/add_user", getAddUser);
  server.on("/args", ArgsPage);
  server.begin(); Serial.println("Server OK");
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

}
void loop() {
  MDNS.update();
  espReconnect();
  if (esp.get_name() != "" || esp.get_name() != NULL) {
    esp = sendMessage();
  }
  // redammare le module au bout de 72heures
  rebootTimeOut(TIME_INTERVAL, previousMillis);

  server.handleClient();
}

/**
   Retrieve temperature humidity realtime data
*/
void handleTemperatureHumidity() {
  const size_t capacity = 1024;
  DynamicJsonDocument doc(capacity);
  doc["espWebMessage"] = espWebMessage;
  doc["userTemperature"] = esp.getTemperature_user();
  doc["espName"] = esp.get_name();
  doc["espStatus"] = esp.get_status();
  doc["humidity"] = dht.readHumidity();
  doc["temp"] = dht.readTemperature();
  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);

}

void ArgsPage() {
  Serial.println( F("void ArgsPage()"));
  if (server.arg("slider") != "") {
    sliderValue = server.arg("slider").toInt();
    Serial.print(F("Valeur du Slider: ")); Serial.println(sliderValue);
    esp.setTemperature_user(float(sliderValue));
  }
  if (server.arg("led") != "") {
    ledValue = server.arg("led").toInt();
    Serial.print(F("Valeur de la led: ")); 
    Serial.println(ledValue);
    digitalWrite(pinLed, ledValue);
    if (ledValue) {
      relaySwicht = true;
      esp.set_status("ON");
    } else {
      relaySwicht = false;
      esp.set_status("OFF");
    }
  }
  if (server.arg("led") != "" || server.arg("slider") != "") {
    esp =  sendMessage();
  }
  server.send(200, "text/plain", "ok");
}
void getAddUser() {

  EspUser espTemporaire;                      //viriable temporaire pour tester le resultat de la fonction add de request.cpp

  /**
     reprise des arguments que renvoie la page association
  */
  if (server.arg("email") != "") {
    indexAgrs ++ ;
    esp.setEmail_user(server.arg("email"));
    Serial.println(esp.getEmail_user());
  }
  if (server.arg("espName") != "") {
    indexAgrs ++ ;
    esp.set_name(server.arg("espName"));
    Serial.println(esp.get_name());
  }
  /**
     fin de de la reprise et début du test
  */
  espTemporaire = Add( client, esp);
  if (espTemporaire.getEmail_user() != "") {
    Serial.println(F("Ce module est bien associé"));
    esp = espTemporaire;
    Serial.println("Add"+ esp.get_name());
    Serial.println(esp.getEmail_user());
    espWebMessage = "";
  }
  else {
    espWebMessage = "Ce module n'a pas pu être associer<br>- Sois l'email renseigner n’est pas associer a un compte ou <br>- Le nom du module est trop long";
    Serial.println(espWebMessage);
  }

  server.send(200, "text/plain", "ok");
}
EspUser sendMessage() {
  //toutes les 5 secondes enoie des donnée
  //Serial.println("GetStatus(client, esp)");
  //Serial.println(millis() );
  //Serial.println(previousMillisMessage);
  //Serial.println(TIME_INTERVAL_MESSAGE);
  if (esp.get_name() != "") {
    if ((millis() - previousMillisMessage) >= TIME_INTERVAL_MESSAGE) {

      Serial.println(F("GetStatus(client, esp >>>)"));

      esp.setTemperature_esp(dht.readTemperature());
      esp.setHumidity_esp(dht.readHumidity());
      esp =  GetStatus(client, esp);
      if (esp.get_status() == "ON") {
        ledValue = true;
        //  digitalWrite(pinLed, ledValue);
        relaySwicht = true;
      } else {
        indice = 0;
        ledValue = false;
        relaySwicht = false;
        digitalWrite(pinLed, ledValue);
      }
      if (relaySwicht) {
        if (indice > 10000) {
          indice = 2;
        }
        Serial.println("TemperaturAutoOnly");
        RelayValue = TemperaturAutoOnly(esp.getTemperature_user(), dht.readTemperature(), pinLed, RelayValue ,indice);
        indice += 1;
        esp.set_status(relayON);
      } else {
        esp.set_status(relayOFF);
      }

      previousMillisMessage = millis();
    }
  }
  return esp;
}
