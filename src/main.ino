#include <M5Atom.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <SPIFFS.h>

DNSServer dnsServer;
AsyncWebServer server(80);

void setup() {
  M5.begin(true, false, true);

  M5.dis.drawpix(0, 0x00ff00);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounting error");
    return;
  }

  WiFi.softAP("Free Wifi");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  dnsServer.start(53, "*", IP);

  server.onNotFound([](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/nggyu.mp3", "audio/mpeg");
      });

  server.begin();
}

uint8_t FSM = 0;

void loop() {
  dnsServer.processNextRequest();

  if (M5.Btn.wasPressed()) {
    switch (FSM) {
      case 0:
        M5.dis.drawpix(0, 0xfff000);  // YELLOW
        break;
      case 1:
        M5.dis.drawpix(0, 0xff0000);  // RED
        break;
      case 2:
        M5.dis.drawpix(0, 0x0000f0);  // BLUE
        break;
      case 3:
        M5.dis.drawpix(0, 0x00ff00);  // GREEN
        break;
      default:
        break;
    }
    FSM++;
    if (FSM >= 4) {
      FSM = 0;
    }
  }

  delay(50);
  M5.update();
}
