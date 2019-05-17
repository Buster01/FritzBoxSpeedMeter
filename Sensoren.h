

String get_ds1820_temp(int index) {
  char msgBuffer[20];
  float ds_temp = 0.00;
  String temp = "";
    
  DS18B20.requestTemperatures();
  ds_temp = DS18B20.getTempCByIndex(index);
  temp=String(ds_temp);
  //temp= dtostrf( ds_temp, 10, 2, msgBuffer);
  temp.trim();
  return temp;
  }

String get_dht_feuchte(){
  String feuchte = "";
  feuchte = String(dht.getHumidity());
  feuchte.trim();
  return feuchte;
}
