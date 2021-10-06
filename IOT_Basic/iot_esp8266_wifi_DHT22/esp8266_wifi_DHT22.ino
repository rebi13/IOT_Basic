#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>

#define DHTTYPE DHT22 // dht의 타입
// DHT22 is attached to GPIO14 on this Kit
#define DHTPIN 14 // 핀 설정
DHT dht(DHTPIN, DHTTYPE, 11); // dht 오브젝트를 만듬 인풋풀업 안넣어줘도 dht안에서 알아서 함
float humidity; // 16비트 레졸루션이 오니까 int가 아니라 float로 변수를 잡아줌
float temp_f;
unsigned long lastDHTReadMillis = 0; // will store last temp was read 언제 읽었는지 기억 할 변수임 2초 따져줘야되서 씀
#define interval 2000

int RLY = 15;


//const char* ssid = "IoT518";
//const char* password = "iot123456";
ESP8266WebServer server(80); // 웹서버 운영할거라서 웹서버 오브젝트를 하나 만드러줌

void setup(void){
    Serial.begin(115200);
    WiFi.mode(WIFI_STA); // esp8266이 좋은게 스스로 액세스포인트가 될 수 있음. 스테이션으로 쓸지 AP로 쓸지 정함. 스테이션이 디폴트임
//    WiFi.begin(ssid, password);
    WiFi.begin("KPU_WiFi858");
    Serial.println("");
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to "); Serial.println("KPU_WiFi858");
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
    
    if (MDNS.begin("DESKTOP-G5U0NGF")) {
    Serial.println("MDNS responder started");
    }
    
    server.on("/", handleRoot); // uri가 / 치고 엔터하면 handRoot로 옴 uri와 콜백을 연결

    server.on("/mypage",thermo);
    server.on("/turnon",turnon);
    server.on("/turnoff",turnoff);
    
    // This is an example of inline function for a handler
    server.on("/inline", [](){ // 이렇게 줘도 가능하다.
    server.send(200, "text/plain", "Hello from the inline function\n");
    }); 
    
    server.onNotFound(handleNotFound); // 누군가 등록하지 않은 uri로 접근했을 경우 handlenotfound 함수를 실행해라
    server.begin();
    Serial.println("HTTP server started");
    dht.begin();

    pinMode(15,OUTPUT);
}

void loop(void){
  server.handleClient();
  gettemperature();
  Serial.print("Temperature : "); Serial.print(temp_f);
  Serial.print(", Humidity : "); Serial.println(humidity);
  delay(1000); // 그냥 함수로 읽어주는게 다임
}
  
void handleRoot() {
    String message = (server.method() == HTTP_GET)?"GET":"POST"; //get이라하면 get
    message += " " + server.uri() + "\n";
    for (uint8_t i=0; i < server.args(); i++){ // 서버의 아규먼트
    message += " " + server.argName(i) + " : " + server.arg(i) + "\n";
    }
    message += "\nHello from ESP8266! with Won Jong Seok\n";
    server.send(200, "text/plain", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void gettemperature() {
    unsigned long currentMillis = millis(); // millis 현재 시간을 줘 부팅한 시점부터의 밀리세컨드
    
    if(currentMillis - lastDHTReadMillis >= interval) { // 2초가 지났느냐를 보는거
        lastDHTReadMillis = currentMillis; // 지났으면 그 시간을 last에 넣어줌 지금시간으로 리셋
      
        humidity = dht.readHumidity(); // Read humidity (percent) //      
        temp_f = dht.readTemperature(); // Read temperature as Fahrenheit
        // Check if any reads failed and exit early (to try again).
        if (isnan(humidity) || isnan(temp_f)) { // not a number
        return;
        }
    }
}

//void mypage(){
//  String message = "<html><head><meta charset=\"utf-8\">"
//                   "<meta http-equiv='refresh' content='5'/>"
//                   "<title>온습도계</title></head>"
//                   "<body>"
//                   "<head>my page<head>"
//                   "<br>"
//                   "'buf2'"
//                   "</body></html>";
//     server.send(200,"text/html", );
//     sprintf(buf1,"%.2f",temp_f);
//     sprintf(buf2,"%.2f",humidity);
//}
void thermo(){
  char mBuf[500];
  char tmplt[] = "<html><head><meta charset=\"utf-8\">"
                 "<meta http-equiv='refresh' content='5'/>"
                 "<title>온습도계</title></head>"
                 "<body>"
                 "<center><p>"
                 "<head>온도 : <h1>%.2f"
                 "<p><head>습도 : <h1>%.2f"
                 "</center>"
                 "</body></html>";

        sprintf(mBuf, tmplt, temp_f, humidity); // 여기서tmplt이 로터리 함수 짰을때 "%4d" 같은거임 tmplt에서 찾아서 넣을 수도 있는거
        Serial.println("serving");
        server.send(200,"text/html", mBuf);
}

void turnon(){
  Serial.println("TurnOn");
  server.send(200, "text/plain","");
  digitalWrite(RLY,HIGH);
}

void turnoff(){
  Serial.println("TurnOff");
  server.send(200, "text/plain","");
  digitalWrite(RLY,LOW);
}
