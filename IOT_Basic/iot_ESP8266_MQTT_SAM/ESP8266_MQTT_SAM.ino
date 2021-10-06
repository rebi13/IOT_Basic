#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Q9_0296";
const char* password = "13131313";
const char* mqttServer = "3.92.133.55";
const int mqttPort = 1883;;
const char* mqttUser = "WJS";
const char* mqttPassword = "js123456";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
  Serial.println("Connecting to MQTT...");
      if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
        Serial.println("connected");
      } 
      else 
      {
        Serial.print("failed with state "); Serial.println(client.state());
        delay(2000); 
      }
  } // end of while
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");

} // end of setup

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
      Serial.println("-----------------------");
}
void loop() 
{
  client.loop();
}
