#include <SerialWeb.h>

constexpr char SSID[] = "SerialWeb";
constexpr char PASSWORD[] = "12345678";

void setup() {
  SerialWeb.begin(SSID, PASSWORD);
}

void loop() {
  char label[] = "NOW_TIME";
  char value[32];
  sprintf(value, "%ld", millis());
  SerialWeb.send(label, value);
  delay(1000);
}
