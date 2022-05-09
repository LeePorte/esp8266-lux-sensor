#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <DFRobot_B_LUX_V30B.h>

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

void setup_wifi() {
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
      //once connected to MQTT broker, subscribe command if any
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

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myLux.begin();
}

void loop() {
  long now = millis();
  //send message every minuite
  delay(60000);
  lastMsgSent = now;
  static char mqttMsg[15];

  float lux = myLux.lightStrengthLux();
  dtostrf(lux, 6, 2, mqttMsg);

  Serial.println(mqttMsg);
  if (client.connected() == false) {
    reconnect();
  }
  client.loop();
  //publish sensor data to MQTT broker
  client.publish("cmnd/lux-sensor/Value", mqttMsg);

}

