#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1); // IP주소 지정해준 것
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML = ""
    "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body><center>"
    "<p>Captive Sample Server App</p>"
    "<button id='button1' style='width:160px;height:60px'><font size='20'>Lamp</font></button>"
    "<script>var xhttp=new XMLHttpRequest();"
    "  button1.onclick=function(e) {"
    "    xhttp.open('GET', 'http://192.168.1.1/button', false);"
    "    xhttp.send(''); }"
    "</script><p>This is a captive portal example</p></center></body></html>";
    
void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("WJS");

    // if started with "*" for domain name, it will reply with provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP); // 뭐든 apIP에 연결시켜라

    webServer.on("/button", button); // 버튼 uri가 눌려졌을때 button()을 실행해라

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

void button(){
    Serial.println("button pressed");
    webServer.send(200, "text/plain", "OK");
}
