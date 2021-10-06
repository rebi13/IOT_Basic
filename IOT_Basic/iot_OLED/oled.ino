#include <OLED32.h>

int val = 0; // AD 변환값을저장할변수

//display(SDA, SCL);
OLED display(4,5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(A0); // 아날로그0번핀(VR) 값을입력
  Serial.println(val);
  delay(50);
  char buf[20];
  sprintf(buf,"%4d",val); // 여기서 %d가 아니라 %4d를 잡는 이유는 1000
  display.print(buf, 3, 2);
}
