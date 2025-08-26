#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

ESP8266WebServer server(80);
Servo lockServo;
int servoPin = D1; // Connect servo signal pin

void setup() {
  Serial.begin(115200);
  lockServo.attach(servoPin);
  lockServo.write(0); // Lock position

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi Connected!");

  server.on("/", []() {
    server.send(200, "text/html",
      "<h1>ESP8266 Door Lock</h1>"
      "<p><a href=\"/unlock\">Unlock Door</a></p>"
      "<p><a href=\"/lock\">Lock Door</a></p>");
  });

  server.on("/unlock", []() { lockServo.write(90); server.send(200, "text/html", "<h1>Door Unlocked</h1>"); });
  server.on("/lock", []() { lockServo.write(0); server.send(200, "text/html", "<h1>Door Locked</h1>"); });

  server.begin();
}

void loop() {
  server.handleClient();
}
