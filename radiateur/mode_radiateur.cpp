#include <DHT.h>
#include <DHT_U.h>
#include <pt.h>   // include protothread library
#include "mode_radiateur.h"
/**
   INTERVAL DE TEMPERATURE 18 ET 19 DEGRES
*/

#include <DHT.h>
#include <DHT_U.h>


/**
   INTERVAL DE TEMPERATURE 18 ET 19 DEGRES la nuit pas d'horloge interne probleme
*/
void TemperaturAutoNuit(int temperatureAuto, float temperatureBed, int ledPin ) {

}


/**
   INTERVAL DE TEMPERATURE 18 ET 19 DEGRES la nuit pas d'horloge interne probleme
*/
int TemperaturAutoOnly(float temperatureAuto, float temperatureBed, int ledPin, int value, float indice ) {

  //temperature mis de force si on ne la renseigne pas.
  if (temperatureAuto < 1) {
    temperatureAuto = 19;
  }
  if(indice < 1 ){
    value = HIGH;
    
  }
 
  if (temperatureBed > temperatureAuto) {
    value = LOW;
  } else if (temperatureBed < (temperatureAuto - 0.8)) {
    value = HIGH;
  }
  digitalWrite(ledPin, value); // led pin 3
  return value;
}

/**
   fonction qui joue le role de switch
*/
void tick(int ledPin)
{
  digitalWrite(ledPin, !digitalRead(ledPin));     // set pin to the opposite state
}


/**
   reboot apres 72h heure d'utilisation
*/
void rebootTimeOut(const unsigned long TIME_INTERVAL, unsigned long previousMillis) {
  if (millis() - previousMillis >= TIME_INTERVAL) {
     Serial.println("esp restart");
    return ESP.restart();
  }
}
