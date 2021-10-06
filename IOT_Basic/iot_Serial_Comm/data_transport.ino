#include <SoftwareSerial.h>

SoftwareSerial Softserial(12,13, false, 256);
// rx = 12, tx = 13 으로 지정

void setup()
{
   Serial.begin(115200);
   Softserial.begin(9600);
}
void loop()
{
  while(Softserial.available()){ // Softserial에 글자가 입력되면
    Serial.write(Softserial.read()); // Serial에 출력(Softserial에서 읽은 것을)
  }
  while(Serial.available()){ // Serial에 글자가 입력되면
    Softserial.write(Serial.read()); // Softserial에 출력(Serial에서 읽은 것을)
  } 
}
