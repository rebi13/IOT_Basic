
#include <DHT.h>
#include <OLED32.h>
#define DHTTYPE DHT22 // dht의 타입
// DHT22 is attached to GPIO14 on this Kit
#define DHTPIN 14 // 핀 설정

DHT dht(DHTPIN, DHTTYPE, 11); // dht 오브젝트를 만듬 인풋풀업 안넣어줘도 dht안에서 알아서 함
//char dht_buffer[10];
float humidity; // 16비트 레졸루션이 오니까 int가 아니라 float로 변수를 잡아줌
float temp_f;
unsigned long lastDHTReadMillis = 0; // will store last temp was read 언제 읽었는지 기억 할 변수임
//2초 따져줘야되서 씀

#define interval 2000

OLED display(4,5);
char buf1[10];
char buf2[10];
void setup(){
    Serial.begin(115200);
    delay(500);
    Serial.println("starting");
    dht.begin();
    display.begin();
}

void loop()
{
    gettemperature();
    Serial.print("Temperature : "); Serial.print(temp_f);
    Serial.print(", Humidity : "); Serial.println(humidity);
    delay(1000); // 그냥 함수로 읽어주는게 다임
    sprintf(buf1,"%.2f",temp_f);
    display.print(buf1,2,2);
    sprintf(buf2,"%.2f",humidity);
    display.print(buf2,3,2);
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
