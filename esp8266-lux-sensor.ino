#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <DFRobot_B_LUX_V30B.h>
#include <ESP8266WebServer.h>
#include "index.h"

DFRobot_B_LUX_V30B    myLux(13);//The sensor chip is set to 13 pins, SCL and SDA adopt default configuration
// Update these with values suitable for your network.
const char* ssid = "CHANGEME"; //put your wifi ssid here
const char* password = "CHANGEME"; //put your wifi password here.
const char* HostName = "CHANGEME"; //set your device hostname here.
const char* mqtt_server = "CHANGEME"; //put your MQTT broker domain / IP address here.
const char* mqtt_clientId = "CHANGEME"; //put your MQTT Client name here.
const char* mqtt_userName = "CHANGEME"; //put your MQTT Username here.
const char* mqtt_passWord = "CHANGEME"; //put your MQTT Password here.

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsgSent = 0;

const int numReadings = 10;

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
int currentLuxLevel = 0;

//ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
void handleRoot();
void handleNotFound();

void setupWifi() {
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HostName);
  Serial.printf("Hostname: %s\n", WiFi.hostname());
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{

} //end callback

void reconnect() {
  // Loop until we're reconnected
  while (client.connected() == false)
  {
    Serial.print("Attempting MQTT connection...");
    client.connect(mqtt_clientId, mqtt_userName, mqtt_passWord);
    if (client.connected() == true)
    {
      Serial.println("connected");
      client.subscribe("tele/lux-sensor/LWT");
      client.publish("tele/lux-sensor/LWT", "online");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} //end reconnect()

void setup(void) {
  Serial.begin(115200);
  setupWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myLux.begin();
  server.on("/", handleRoot);
  server.on("/luxread", luxData);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void handleRoot() {
  String s = webpage;
  server.send(200, "text/html", s);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void luxData(){
 String lux_value = String(currentLuxLevel);
 server.send(200, "text/plain", lux_value);
}

void mqttMessage() {
   long now = millis();
  lastMsgSent = now;
  static char mqttMsg[15];

  total = total - readings[readIndex];
  readings[readIndex] = myLux.lightStrengthLux();
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = total / numReadings;
  dtostrf(average, 6, 2, mqttMsg);
  currentLuxLevel = average;
  Serial.println(mqttMsg);
  delay(1000);
  
  if (client.connected() == false) {
    reconnect();
  }
  client.loop();
  client.publish("cmnd/lux-sensor/Value", mqttMsg);
}

void loop(void) {
  server.handleClient();
  mqttMessage();
}
