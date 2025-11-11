#include <SerialWeb.h>

void setup() {
  Serial.begin(115200);
  SerialWeb.begin("SerialWeb", "12345678");
}

void loop() {
  char now[32];
  sprintf(now, "%ld", millis());
  SerialWeb.send("NOW_TIME", now);
  delay(1000);
}
