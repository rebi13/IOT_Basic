
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

char ssid[] = "IoT518";
char password[] = "iot123456";
const char* mqttServer = "3.92.133.55"; // AWS에서 부여받은 개인의 ip 주소
const int mqttPort = 1883; // Port 할당
const char* mqttUser = "yhur";
const char* mqttPassword = "hi";

// MMA8452Q I2C address is 0x1C(28)
#define Addr 0x1C

char Accelstr[10];
int RLY = 15;
int Pin_Buzzer = 2;
int Accelration = 0;

HTTPClient http;
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(115200);
  pinMode(RLY,OUTPUT);
  pinMode(Pin_Buzzer, OUTPUT);    
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x2A);
  // StandBy mode
  Wire.write((byte)0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
 
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x2A);
  // Active mode
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
 
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x0E);
  // Set range to +/- 2g
  Wire.write((byte)0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("");
  
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
    
  http.begin("http://3.92.133.55:8086/write?db=sensorDB");
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
  unsigned int data[7];
  digitalWrite(Pin_Buzzer, LOW);
  // Request 7 bytes of data
  Wire.requestFrom(Addr, 7);
 
  // Read 7 bytes of data
  // staus, xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
  if(Wire.available() == 7) 
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
    data[6] = Wire.read();
  }
 
  // Convert the data to 12-bits
  int xAccla = ((data[1] * 256) + data[2]) / 16;
  if (xAccla > 2047)
  {
    xAccla -= 4096;
  }
 
  int yAccla = ((data[3] * 256) + data[4]) / 16;
  if (yAccla > 2047)
  {
    yAccla -= 4096;
  }
 
  int zAccla = ((data[5] * 256) + data[6]) / 16;
  if (zAccla > 2047)
  {
    zAccla -= 4096;
  }
  delay(1000);


  Wire.requestFrom(Addr, 7);
  if(Wire.available() == 7) 
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
    data[6] = Wire.read();
  }
    // Convert the data to 12-bits
  int xAcclb = ((data[1] * 256) + data[2]) / 16;
  if (xAcclb > 2047)
  {
    xAcclb -= 4096;
  }
 
  int yAcclb = ((data[3] * 256) + data[4]) / 16;
  if (yAcclb > 2047)
  {
    yAcclb -= 4096;
  }
 
  int zAcclb = ((data[5] * 256) + data[6]) / 16;
  if (zAcclb > 2047)
  {
    zAcclb -= 4096;
  }
  int xAccl = xAccla - xAcclb;
  int yAccl = yAccla - yAcclb;
  int zAccl = zAccla - zAcclb;
 
  // Output data to serial monitor
  Serial.print("Acceleration in X-Axis : ");
  Serial.println(xAccl);
  Serial.print("Acceleration in Y-Axis : ");
  Serial.println(yAccl);
  Serial.print("Acceleration in Z-Axis : ");
  Serial.println(zAccl);
  delay(500);


  Accelration = xAccl + yAccl + zAccl;
  sprintf(Accelstr, "Accelration = %4d", Accelration); 
  client.publish("esp/rebi/evt/Accelration", Accelstr);
  delay(1000);
  client.loop();
  int httpCode = http.POST("dht,host=server01,region=us-west value="+String(Accelration));
  delay(200);
  //int httpCode = http.POST("Earthquake,host=server01,region=us-west value="+String(-Accelration));
  Serial.println(httpCode);
  http.end();
  String payload = http.getString();
  Serial.println(payload);

  xAccl = abs(xAccl);
  yAccl = abs(yAccl);
  zAccl = abs(zAccl);
  
  if( xAccl>= 600 || yAccl>= 600 || zAccl>= 600)
  {
    digitalWrite(RLY, HIGH);
    digitalWrite(Pin_Buzzer, HIGH);

  }
  else{
    digitalWrite(RLY, LOW);
    digitalWrite(Pin_Buzzer, LOW);

  }
  delay(100);
}


//void Emergency_state_(){
//  tone(Pin_Buzzer,1000,500); // 부저핀에 1000Hz로 100ms 동안 부저 울리도록 한다.
//}
