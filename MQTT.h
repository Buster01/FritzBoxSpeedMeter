WiFiClient wlanClient;
PubSubClient mqtt_client(wlanClient);


boolean mqtt_connect(void) {
  if (!wlan_status()) {
    return false;
  } else {
    int counter = 0;
  
    mqtt_client.setServer(mqtt_server, 1883);
    //mqtt_client.setCallback(callback);
    
    mqtt_client.connect("Keller1");
    Serial.print("MQTT Connection: ");
    while (!mqtt_client.connected()) 
    {
      delay(500);
      Serial.print(".");
      counter++;
      if (counter > 15 ) {
        Serial.println("Verbindung konnte nicht aufgebaut werden!");
        ESP.restart();
        delay(2000);
        return false;
      }
    }
    Serial.println ("OK");
    return true;
  }
}

void mqtt_sensordaten_senden() {
  String feuchte=get_dht_feuchte();
  String rssi = String(WiFi.RSSI());
  String topic = "";
  
  mqtt_connect();
  // Temp Sensoren
  
  for (int i=0; i < sensor_count; i++){
    String temp=get_ds1820_temp(i);
    String mqtt_topic="/openHAB/Keller/Temperatur" + String(i+1);
    mqtt_client.publish(mqtt_topic.c_str(), temp.c_str());
    Serial.print("Temperatur "); Serial.print(String(i+1)); Serial.print(" :"); Serial.println(temp.c_str());
    delay(10);
  }
    
  mqtt_client.publish("/openHAB/Keller/Feuchte", feuchte.c_str());
  Serial.print("Feuchte: "); Serial.print(feuchte.c_str() ); Serial.println(" %");
 
  Serial.println("RSSI: "+ rssi);
  mqtt_client.publish("/openHAB/Keller/RSSI", rssi.c_str());
}

void mqtt_FritzBoxDaten_senden() {
  Serial.println("Senden FritzBox Daten!");
  refreshNonce(); // Dummy request - Workaround for lib fail
  String fb_MaxUpstreamBitRate = String(getUpstreamMaxBitRate());
  String fb_MaxDownstreamBitRate = String(getDSLDownstreamMaxBitRate());
  String fb_UploadBitRate = String(getNewByteUploadRate());
  String fb_DownloadBitRate = String(getNewByteDownloadRate());
  
  mqtt_connect();
  mqtt_client.publish("/openHAB/Fritzbox/UpstreamMaxBitRate", fb_MaxUpstreamBitRate.c_str());
  mqtt_client.publish("/openHAB/Fritzbox/DownstreamMaxBitRate", fb_MaxDownstreamBitRate.c_str());
  mqtt_client.publish("/openHAB/Fritzbox/UploadBitRate", fb_UploadBitRate.c_str());
  mqtt_client.publish("/openHAB/Fritzbox/DownloadBitRate", fb_DownloadBitRate.c_str());
}
