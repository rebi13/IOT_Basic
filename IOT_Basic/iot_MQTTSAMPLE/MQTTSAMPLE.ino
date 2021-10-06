#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN 14
#define interval 2000
DHT dht(DHTPIN, DHTTYPE, 11); 
float humidity;
float temp_f;
unsigned long lastDHTReadMillis = 0; 
char str1[15]; // 온도 값을 담을 배열
char str2[15]; // 습도 값을 담을 배열
char str3[15]; // 빛 값을 담을 배열

const char* ssid = "KPU_WiFi858"; // 개인 WIFI 설정
const char* password = ""; // 개인 WIFI 비밀번호 설정
const char* mqttServer = "3.92.133.55"; // AWS에서 부여받은 개인의 ip 주소
const int mqttPort = 1883; // Port 할당
const char* mqttUser = "yhur";
const char* mqttPassword = "hi";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
    Serial.begin(115200);
    dht.begin();
    delay(500);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    
    while (!client.connected()) 
    {
        Serial.println("Connecting to MQTT...");
        if (client.connect("ESP8266Client", mqttUser, mqttPassword )) 
        {
            Serial.println("connected");
        } 
        else 
        {
            Serial.print("failed with state "); Serial.println(client.state());
            delay(2000);
        }
    }
}
void callback(char* topic, byte* payload, unsigned int length) 
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)payload[i]);
    }
    Serial.println("-----------------------");
}

void loop() 
{   
    int light = analogRead(A0);
    gettemperature();
    Serial.print("Temperature : "); Serial.println(temp_f);
    Serial.print("Humidity : "); Serial.println(humidity);
    Serial.print("Light : "); Serial.println(light);
    delay(1000);
    sprintf(str1, "temp = %2.2f", temp_f); 
    sprintf(str2, "humi = %2.2f", humidity);
    sprintf(str3, "light = %04d", light);
    client.publish("esp/rebi/evt/temperature", str1);
    delay(1000);
    client.publish("esp/rebi/evt/humidity", str2);
    delay(1000);
    client.publish("esp/rebi/evt/light", str3);
    client.loop();
    delay(2000);
}

void gettemperature() 
{
    unsigned long currentMillis = millis();
    if(currentMillis - lastDHTReadMillis >= interval) 
    {
        lastDHTReadMillis = currentMillis;
        humidity = dht.readHumidity(); // Read humidity (percent)
        temp_f = dht.readTemperature(); // Read temperature as Fahrenheit
        // Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temp_f)) 
        {
            return;
        }
    }
}
