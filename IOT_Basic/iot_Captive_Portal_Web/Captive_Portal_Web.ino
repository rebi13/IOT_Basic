#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
const int RLY = 15; //GPIO15
const byte DNS_PORT = 53;
int i = 0;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML = ""
"<!DOCTYPE html><html><head><title>Captive_LAB</title></head><body><center>"
"<p>Relay Button</p>"
"<button id='button1' style='width:160px;height:60px'><font size='20'>On/Off</font></button>"
"<script>var xhttp=new XMLHttpRequest();"
" button1.onclick=function(e) {"
" xhttp.open('GET', 'http://192.168.1.1/button', false);"
" xhttp.send(''); }"
"</script><p>This is a captive portal example</p></center></body></html>";

void setup() {
    pinMode(RLY, OUTPUT);
    Serial.begin(115200);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("KPU_WiFi858");
    // if started with "*" for domain name, it will reply with provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP);
    webServer.on("/button", button);
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
     if (i == 0) {
        digitalWrite(RLY, HIGH);
     } 
    else {
        digitalWrite(RLY, LOW);
     }
    if(i>1)i=0;

}
void button(){
    Serial.println("button pressed");
    i++;
    webServer.send(200, "text/plain", "OK");
}
