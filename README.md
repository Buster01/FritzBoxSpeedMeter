# FritzBoxSpeedMeter
FritzBox per TR-064 auslesen und per MQTT versenden

Mit desem Arduino Scatch und einem ESP8266 kann man die DSL Leitungsgeschwindigkeit und die aktuell benutzte Bandbreite alle 15 Sekunden auslesen und per MQTT übertragen werden. Zusätzlich können noch Dallas DS18B20 Temperatursensoren und ein DHT22 angeschlossen werden. Die Sensorwerte werden alle 60 Sekunden ebenfalls per MQTT übertragen.
