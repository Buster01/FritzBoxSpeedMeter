const char* ssid = "SSID Name";  // SSID deines WLAN Netzwerkes
const char* wlan_passwd = "WLAN Passwort"; // Passwort deines WLAN Netzwerkes

const char* mqtt_server = "MQTT-SERVER"; // MQTT Server
const char* host_name = "FritzBoxSpeedMeter";
bool sending = false;

const char* fbIP = "192.168.0.1";
const int fbPORT = 49000;
const char* fbuser = "Fritzbox User";
const char* fbpass = "Fritzbox Passwort";
int sensor_count = 0;
float factor1 = 1024;
float factor2 = 1048576;

#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <tr064.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#define DS18B20_PIN D4
#define TEMPERATURE_PRECISION 12
OneWire oneWire(DS18B20_PIN);          
DallasTemperature DS18B20(&oneWire);

#include <DHTesp.h>
#define DHTPIN D3
DHTesp dht;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"de.pool.ntp.org", 3600, 60000);
TR064 TR064connection(fbPORT, fbIP, fbuser, fbpass);

#include "sensoren.h"
#include "WLAN.h"
#include "FritzBox.h"
#include "MQTT.h"
#include "OTA.h"


void setup() {
  Serial.begin(115200); // Serielle Schnittstelle öffnen
  Serial.println("Starte ESP8266 Keller-Arbeitszimmer!");

  // WLAN Verbindung
  if (!wlan_connect()) {
      Serial.println ("WLAN Verbindung fehlgeschlagen!");
      Serial.println ("Reset!");
      delay(2000);
      ESP.restart();
    } else {
      Serial.println();
      Serial.print("Verbindung! RSSI: "); Serial.print(WiFi.RSSI()); Serial.println(" dBi");
      Serial.print("IP-Adresse: "); Serial.println(WiFi.localIP());
      timeClient.begin();
      timeClient.forceUpdate();
      
      OTAsetup(); // OTA

      Serial.print("Initialisierung beendet (UTC): "); Serial.println(String(timeClient.getFormattedTime()));
      Serial.println("TR064 Verbindung zur Fritz!Box");
      TR064connection.init();
      int numDev = getDeviceNumber();
      Serial.print("Router has "); Serial.print (numDev); Serial.println(" (connected) devices.");
   }
   
  dht.setup(DHTPIN, DHTesp::DHT22);
  delay(2000);
  Serial.print("Humidity: "); Serial.print(get_dht_feuchte()); Serial.println("%");

  DS18B20.begin(); // Dallas 18b20
  delay(1000);

  // dallas Sensor Init
  Serial.print("Anzahl Dallas Temperatur Sensoren: "); Serial.println(DS18B20.getDeviceCount());
  sensor_count=DS18B20.getDeviceCount();
  DS18B20.setResolution(TEMPERATURE_PRECISION);
  Serial.print ("Setzte DS1B20 Temperatur Praezision auf ");
  Serial.print (TEMPERATURE_PRECISION);
  Serial.println ("Bit");  
}

void loop() {
  // WLAN prüfen
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println ("WLAN Verbindung fehlgeschlagen!");
    Serial.println ("Reset!");
    delay(2000);
    ESP.restart();
  }
  // OTA Handle
  ArduinoOTA.handle();
  
  //MQTT prüfen
  if (mqtt_client.connected()) {
    mqtt_client.loop();
  }
  //Fritzbox Daten senden
  if ((timeClient.getSeconds() == 14) || (timeClient.getSeconds() == 28) || (timeClient.getSeconds() == 44)) {sending=false;}
  if ((timeClient.getSeconds() == 13) || (timeClient.getSeconds() == 27) || (timeClient.getSeconds() == 43)) {
    if (sending == false) {
      if (mqtt_connect()) {
          mqtt_FritzBoxDaten_senden();
          Serial.print("Daten gesendet (UTC): "); Serial.println(String(timeClient.getFormattedTime()));
          sending = true;
      }
    }
  }

  // Sensor Meßwerte senden
  if ((timeClient.getSeconds() == 58) && (sending == true)) { sending = false; }
  if ((timeClient.getSeconds() ==57) && (WiFi.status() == WL_CONNECTED)) {
    if (sending == false) {
      if (mqtt_connect()) {    
        mqtt_sensordaten_senden();
        mqtt_FritzBoxDaten_senden();
        Serial.print("Daten gesendet (UTC): "); Serial.println(String(timeClient.getFormattedTime()));
        sending=true;
        timeClient.update();
      }
    }
  }
}
