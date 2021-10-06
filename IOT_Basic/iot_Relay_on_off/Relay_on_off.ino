#include <SoftwareSerial.h>

SoftwareSerial Softserial(12,13, false, 256);
// rx = 12, tx = 13 으로 지정

int in = 0;
int RLY = 15;

void setup()
{
   Serial.begin(9600);
   Softserial.begin(9600);
   pinMode(15,OUTPUT);
}

void loop()
{
    in = Softserial.read();
    Softserial.write(in);
    if(in == '1'){
    digitalWrite(RLY, HIGH);  
    }
    else if(in == '2'){
    digitalWrite(RLY, LOW);  
    }
}
