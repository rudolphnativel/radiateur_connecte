

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "espUser.h"
#include "espUser.cpp"

/**
  constant for api adresse
*/
const char* API_HOST        = "https://api.njhomes.fr";
const char* GET_SATUS       = "/api/article/get_status";
const char* GET_ONE         = "/api/article/get_one";
const char* GET_ALL         = "/article/get_all";
const char* ADD             = "/api/article/add";
const char* GET_STATUS      = "/api/article/get_status";

/**
   désérialise la reponce de get_one et hydrate l'objet esp
*/
EspUser deserializerGetOneAndAdd(String input, EspUser esp);
String serialiserAdd(EspUser esp);



void GetAll(  WiFiClientSecure client, EspUser esp) {
  //variable and constant
  HTTPClient https;
  String payload;
  String request = API_HOST;

  Serial.println("https begin");


  request += GET_ALL;//request -->https://api.njhomes.fr/article/get_all
  https.begin(client, request);


  if (https.GET() == HTTP_CODE_OK) {
    payload = https.getString();
    Serial.println(payload);//print response
  }
  else
  {
    Serial.println(https.GET());//response code example--> 404 or 400 or 500...
    // print html page for debug
    while (client.available())
    { String line = client.readStringUntil('\r'); Serial.print(line);
    }
    Serial.println("certificate doesn't match");
  }

  https.end();
}
/**
   une fois a chaque allumage
*/
EspUser GetOne( WiFiClientSecure client, EspUser esp) {
  //varaiable and constant
  String request = API_HOST;
  EspUser espTempo;
  HTTPClient https;
  String output = serialiserAdd( esp);                    //serialiser les données du module

  request += GET_ONE;
  Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  https.begin(client, request); //HTTP
  https.addHeader("Content-Type", "application/json");

  Serial.print(F("[HTTP] POST...\n"));
  // start connection and send HTTP header and body
  int httpCode = https.POST(output);
  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_ACCEPTED) {
      const String& payload = https.getString();
      Serial.println(F("received payload:\n<<"));
      Serial.println(payload);
      Serial.println(F(">>"));
      esp  = deserializerGetOneAndAdd(payload, esp);      //deserialiser les données reçu en hitratant un objet esp
      https.end();
      return esp;
    }
    // print html page for debug
    while (client.available())
    { String line = client.readStringUntil('\r'); Serial.print(line);
    }
  } else {

    Serial.printf("[HTTP] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
  }

  https.end();
  Serial.println(F("Ce boitier n'est associére à aucun compte"));
  return espTempo;
}

EspUser Add(WiFiClientSecure client, EspUser esp) {
  //varaiable and constant
  String request = API_HOST;
  EspUser espTempo;
  HTTPClient https;
  String json = serialiserAdd( esp);

  Serial.println(json);
  request += ADD;
  Serial.print(F("[HTTP] begin...\n"));
  // configure traged server and url
  https.begin(client, request); //HTTP
  https.addHeader("Content-Type", "application/json");



  Serial.print(F("[HTTP] POST...\n"));
  // start connection and send HTTP header and body
  int httpCode = https.POST(json);

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_CREATED) {
      const String& payload = https.getString();
      Serial.println(F("received payload:\n<<"));
      Serial.println(payload);
      Serial.println(F(">>"));
      esp  = deserializerGetOneAndAdd(payload, esp);
      https.end();

      Serial.println(F("Ce boitier est associére à aucun compte"));
      return esp;
    }
    // print html page for debug
    while (client.available())
    { String line = client.readStringUntil('\r'); Serial.print(line);
    }
  } else {

    Serial.printf("[HTTP] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
  }

  https.end();
  Serial.println(F("Ce boitier n'est associére à aucun compte"));
  return espTempo;
}


EspUser GetStatus(WiFiClientSecure client, EspUser esp) {
  //varaiable and constant
  String request = API_HOST;
  EspUser espTempo;
  HTTPClient https;
  const String json = serialiserAdd( esp);

  Serial.println(json);
  request += GET_STATUS;
  Serial.print(F("[HTTP] begin...\n"));
  // configure traged server and url
  https.begin(client, request); //HTTP
  https.addHeader("Content-Type", "application/json");



  Serial.print(F("[HTTP] POST...\n"));
  // start connection and send HTTP header and body
  int httpCode = https.POST(json);

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_ACCEPTED || httpCode == HTTP_CODE_PARTIAL_CONTENT) {
      const String& payload = https.getString();
      //Serial.println("received payload:\n<<");
      //Serial.println(payload);
      Serial.println(F(">>"));
      esp  = deserializerGetOneAndAdd(payload, esp);
      https.end();
      if (httpCode == HTTP_CODE_PARTIAL_CONTENT) {
        Serial.println(F("Ce boitier n'est associére à aucun utilisateur"));
      } else {
        Serial.println(F("Ce boitier est associére à un utilisateur"));
      }
      return esp;
    }
    // print html page for debug
    while (client.available())
    { String line = client.readStringUntil('\r'); Serial.print(line);
    }
  } else {

    Serial.printf("[HTTP] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
  }

  https.end();
  Serial.println(F("Ce boitier n'est associére à aucun compte"));
  return esp;
}




EspUser deserializerGetOneAndAdd(String input, EspUser esp) {
  // String input;

  StaticJsonDocument<768> doc;
  deserializeJson(doc, input);

  esp.set_name(doc["name"]); // "CHAMBRE"
  esp.set_status(doc["status"]); // "éteint"
  esp.setDns(doc["dns"]); // "esp_njhomes"
  esp.setTemperature_user(doc["temperature_user"]); // "esp_njhomes"

  esp.setCategoryLabel(doc["category"]["label"]); // "Chauffage analogique"
  esp.setIdCategory(doc["category"]["id"]); // "Chauffage analogique"

  JsonObject user = doc["user"];
  esp.setEmail_user(user["email"]); // "toto.tata@gmail.com"
  return esp;
}

String serialiserAdd(EspUser esp) {
  //variable
  String output = "";
  StaticJsonDocument<384> doc;

  doc["serial_number"] = esp.getSerial_number();
  doc["temperature_user"] = esp.getTemperature_user();
  doc["temperature_esp"] = esp.getTemperature_esp();
  doc["humidity"] = esp.getHumidity_esp();
  doc["name"] = esp.get_name();
  doc["status"] = esp.get_status();
  doc["dns"] = esp.getDns();
  doc["ip_adress"] = esp.getIpAdresse();

  JsonObject category = doc.createNestedObject("category");
  category["id"] = 1;
  category["label"] = "Chauffage analogique";
  doc["user"]["email"] = esp.getEmail_user();

  serializeJson(doc, output);
  return output;
}
