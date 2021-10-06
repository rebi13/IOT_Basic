//로타리 누르면 불들어오는건 2번에 연결되어있음 쓰고싶으면 이걸 2번으로 핀모드 설정
//attachInterupt (*,calling(),change()) 올라갈때도 하고 내려갈때도 해야됨
// A에 변화가 일어나든지 B에 변화가 일어나든지
//왼쪽 스위치 2개를 켜야 로타리엔코더를 쓸 수있다.

//핀A 핀B는 12 13에 연결되어있음
//volatile로 라스트인코더 인코더밸류 선언 = 0
#include <Wire.h>
#include <OLED32.h>
const int pulseA = 12;
const int pulseB = 13;
const int pushSW = 2;
int a = 0;
int b = 0;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

OLED display(4, 5);

void Rotary() 
{
    int MSB = digitalRead(pulseA); //MSB = most significant bit
    int LSB = digitalRead(pulseB); //LSB = least significant bit
    int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
    int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
    if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
    if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
    lastEncoded = encoded; //store this value for next time;=
    if (encoderValue > 255) 
    {
        encoderValue = 255;
    } 
    else if (encoderValue < 0 ) 
    {
        encoderValue = 0;
    }
}

void buttonClicked() 
{
    Serial.println("pushed");
}

void setup() 
{
    Serial.begin(115200);
    pinMode(pushSW, INPUT_PULLUP);
    pinMode(pulseA, INPUT_PULLUP);
    pinMode(pulseB, INPUT_PULLUP);
    attachInterrupt(pushSW, buttonClicked, FALLING);
    attachInterrupt(pulseA, Rotary, CHANGE);
    attachInterrupt(pulseB, Rotary, CHANGE);
}

void loop() 
{
    if(pushSW == FALLING){
    Serial.println("Clicked!");
    delay(500);
    }
    else if(pulseA == CHANGE){
      a = digitalRead(pulseA);
      Serial.println(a);
      delay(500);
    }
    else if(pulseB == CHANGE){
      b = digitalRead(pulseB);
      Serial.println(b);
      delay(500);
    }
    Serial.println(encoderValue);
}
