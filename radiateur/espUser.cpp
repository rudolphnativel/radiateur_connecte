
#include <ESP8266WiFi.h>

class EspUser{
  private :
    int id;
    float temperature_esp     = 0 ;
    float  humidity_esp       = 0 ;
    float temperature_user    = 0 ;
    String serial_number      = "";
    String _name              = "";
    String _status            = "";
    String email_user         = "";
    String ipAdresse          = "";
    String categoryLabel      = "";
    int idCategory            = 0 ;
    String dns                = "";
    boolean power             = true;

  public :
    EspUser() {
    }

    EspUser( float  humidity_esp , float temperature_esp, float temperature_user, String serial_number, String _name, String _status, String email_user,String categoryLabel, int idCategory,String dns ,String ipAdresse, boolean power) {
      this->dns               = dns;
      this->ipAdresse         = ipAdresse;
      this->humidity_esp      = humidity_esp;
      this->temperature_esp   = temperature_esp;
      this->temperature_user  = temperature_user;
      this->serial_number     = serial_number;
      this->_name             = _name;
      this->_status           = _status;
      this->email_user        = email_user;
      this->categoryLabel     = categoryLabel;
      this->idCategory        = idCategory;
      this->power             = power;      
    }
    
    //get and set dns  local
    void setDns(String dns) {
      this->dns   = dns;
    }
    String getDns() {
      return this->dns;
    }
        
    //get and set  category label
    void setCategoryLabel(float categoryLabel) {
      this->categoryLabel  = categoryLabel;
    }
    String getCategoryLabel() {
      return this->categoryLabel;
    }
    
    //get and set  id category
    void setIdCategory(float idCategory) {
      this->idCategory   = idCategory;
    }
    int getIdCategory() {
      return this->idCategory;
    }
    
    //get and set  humidity_esp
    void setHumidity_esp(float temperature) {
      this->humidity_esp   = temperature;
    }
    float getHumidity_esp() {
      return this->humidity_esp;
    }
    
    //get and set  temperature_esp
    void setTemperature_esp(float temperature) {
      this->temperature_esp   = temperature;
    }
    float getTemperature_esp() {
      return this->temperature_esp;
    }

    //get and set temperature_user
    void setTemperature_user(float temperature) {
      this->temperature_user   = temperature;
    }
    float getTemperature_user() {
      return this->temperature_user;
    }

    //get and set serial_number
    void setSerial_number(String SNumber) {
      this->serial_number       = SNumber;
    }
    String getSerial_number() {
      return this->serial_number;
    }

    //get and set _name
    void set_name(String _nameU) {
      this->_name               = _nameU;
    }
    String get_name() {
      return this->_name;
    }

    //get abd set -status
    void set_status(String _statusU) {
      this->_status             = _statusU;
    }
    String get_status() {
      return this->_status;
    }

    //get and set email_user
    void setEmail_user(String email) {
      this->email_user          = email;
    }
    String getEmail_user() {
      return this->email_user;
    }

    //get and set ipAdresse
    void setIpAdresse(IPAddress  ipAdr) {
      String s = "";
      for (int i = 0; i < 4; i++) {
        s += i  ? "." + String(ipAdr[i]) : String(ipAdr[i]);
      }
      this->ipAdresse          = s;
    }
    String getIpAdresse() {
      return this->ipAdresse;
    }
      //get and set power
    void setPower(boolean indice) {
      this->power          = indice;
    }
    boolean getPower() {
      return this->power;
    }
};
