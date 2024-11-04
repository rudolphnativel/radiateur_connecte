

/**
 * allume ou eteind le relais en fonction de la temperature demander
 * si il n'y a pas de temperature demander une temperature de 19 degres est appliqué
 */
int TemperaturAutoOnly(float temperatureAuto, float temperatureBed, int ledPin,int value , float indice);

/**
 * fonction qui joue le role de switch
 */
 void tick(int ledPin);

 
/**
   reboot apres 72h heure d'utilisation
*/
void rebootTimeOut(const unsigned long TIME_INTERVAL, unsigned long previousMillis);
