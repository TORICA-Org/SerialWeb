#include <SerialWeb.h>

constexpr char SSID[] = "SerialWeb";
constexpr char PASSWORD[] = "12345678";

void setup() {
  Serial.begin(115200);
  SerialWeb.begin(SSID, PASSWORD);
}

void loop() {
  char label[] = "NOW_TIME";
  char value[32];
  sprintf(value, "%ld", millis());
  SerialWeb.send(label, value);

  // Serial.printf("now buffer has %d data.\n", SerialWeb.available());
  if (SerialWeb.available() > 0) {
    String msg = SerialWeb.readString();
    Serial.printf("readString: %s\n", msg);
  }
  delay(100);
}
