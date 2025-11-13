#include <SerialWeb.h>

char *SSID = "Xiaomi 11T Pro";
char *PASSWORD = "87654321";

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  IPAddress localIP = WiFi.localIP();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(localIP);

  SerialWeb.begin(localIP);
}

void loop() {
  char now[32];
  sprintf(now, "%ld", millis());
  SerialWeb.send("NOW_TIME", now);
  delay(1000);
}
