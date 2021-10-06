#include <ESP8266WiFi.h> // 헤더 인클루드

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.begin("IoT518", "iot123456");
  //WiFi.begin("KPU_WiFi858");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("."); // 붙을때까지 시간이좀 걸림 그러다가 붙을거
  }
  
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
}
