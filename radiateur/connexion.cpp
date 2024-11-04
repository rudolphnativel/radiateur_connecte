#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager  


void connexion() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  // WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  //reset settings - wipe credentials for testing
  //wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "njhomesPwd"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
}

void connexionWithTimeOut() {

  //ip fixe
  IPAddress ip(192, 168, 1, 67);
  IPAddress dns(192, 168, 1, 254);
  IPAddress gateway(192, 168, 1, 254);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, dns, gateway, subnet);


  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setConfigPortalTimeout(180);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "njhomesPwd")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
}

void disconnect() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect();
}
void espReconnect() {
  //variable pour le decompte avant redémarrage
  int indexReconnect = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (indexReconnect >= 20) {
      Serial.println("ESp restart");
      ESP.restart();
    }
    indexReconnect++;
    WiFi.reconnect();
  }

}
