#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

#define RELAY 15

String responseHTML = ""
    "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body><center>"
    "<p>Captive Sample Server App</p>"
    "<button id='button1' style='width:160px;height:60px'><font size='20'>On</font></button>"
    "<button id='button2' style='width:160px;height:60px'><font size='20'>Off</font></button>"
    "<script>var xhttp=new XMLHttpRequest();"
    "  button1.onclick=function(e) {"
    "    xhttp.open('GET', 'http://192.168.1.1/turnOn', false);"
    "    xhttp.send(''); };"
    "  button2.onclick=function(e) {"
    "    xhttp.open('GET', 'http://192.168.1.1/turnOff', false);"
    "    xhttp.send(''); };"
    "</script><p>This is a captive portal example</p></center></body></html>";
    
void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("JSRELAY");
    pinMode(RELAY, OUTPUT);

    // if started with "*" for domain name, it will reply with provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP);

    webServer.on("/turnOn", turnOn);
    webServer.on("/turnOff", turnOff);

    // replay to all requests with same HTML
    webServer.onNotFound([]() {
        webServer.send(200, "text/html", responseHTML);
    });
    webServer.begin();
    Serial.println("Captive Portal Started");
}

void loop() {
    dnsServer.processNextRequest();
    webServer.handleClient();
}

void turnOn(){
    Serial.println("Trun On the relay");
    digitalWrite(RELAY, HIGH);
    webServer.send(200, "text/plain", "OK"); // 안쓰면 esp8266이 죽을거임
    // 클로즈를 안해주면 
}

void turnOff(){
    Serial.println("Trun Off the relay");
    digitalWrite(RELAY, LOW);
    webServer.send(200, "text/plain", "OK");
}
