#include <SerialWeb.h>

constexpr char SSID[] = "Xiaomi 11T Pro";
constexpr char PASSWORD[] = "87654321";

void setup() {
  Serial.begin(115200);

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
  char label[] = "NOW_TIME";
  char value[32];
  sprintf(value, "%ld", millis());
  SerialWeb.send(label, value);
  delay(1000);
}
