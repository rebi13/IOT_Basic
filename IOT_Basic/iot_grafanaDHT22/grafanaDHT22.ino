#include <Wire.h>
#include <OLED32.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define DHTTYPE DHT22
#define DHTPIN 14
#define interval 2000
char ssid[] = "IoT518";
char password[] = "iot123456";
OLED display(4, 5);
DHT dht(DHTPIN, DHTTYPE, 11);

char humi[10];
char temp[10];
float humidity = 0 ;
float temp_f = 0 ;
unsigned long lastDHTReadMillis = 0; // will store last temp was read

HTTPClient http;
void setup() {
    delay(500);
    display.begin();
    display.print("DHT22");
    dht.begin();
    display.on();
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to "); Serial.println(ssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
    http.begin("http://192.168.0.13:8086/write?db=sensorDB"); // iot518 연결하고나서의 컴퓨터의 ip 주소
}
void loop() {
    gettemperature();
    sprintf(temp,"%2.2f",temp_f);
    sprintf(humi,"%2.2f",humidity);
    display.print("temp : ", 1, 0);
    display.print(temp, 1, 9);
    display.print("humi : ", 2, 0);
    display.print(humi, 2, 9);
    http.addHeader("Content-Type", "text/plain");
    int httpCode1 = http.POST("dht,host=server01,region=us-west value="+String(temp_f));
    Serial.println(httpCode1);
    http.end();
    int httpCode2 = http.POST("dht,host=server02,region=us-west value="+String(humidity));
    Serial.println(httpCode2);
    http.end();
    String payload = http.getString();
    Serial.println(payload);
    
    delay(10000);
}
void gettemperature() {
unsigned long currentMillis = millis();
    if(currentMillis - lastDHTReadMillis >= interval) {
    lastDHTReadMillis = currentMillis;
    humidity = dht.readHumidity(); // Read humidity (percent)
    temp_f = dht.readTemperature(); // Read temperature as Fahrenheit
// Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temp_f)) {
        return;
        }
    }
}
