## ESP8266 Lux sensor to MQTT

This is an Arduino sketch for an ESP8266 with a [DFRobot Lux Sensor](https://shop.pimoroni.com/products/ambient-light-sensor-0-200klx) that reports a value per minute to an MQTT broker. I'm using this to feed Lux data into [Openhab](https://www.openhab.org/) to drive some rules for turning lights on and off.

Libraries needed
[PubSubClient](https://www.arduino.cc/reference/en/libraries/pubsubclient/)
[ESP8266WiFi and ESP8266WebServer](https://github.com/esp8266/Arduino/)
[DFRobot_B_LUX_V30B](https://wiki.dfrobot.com/Ambient_Light_Sensor_0_200klx_SKU_SEN0390)

