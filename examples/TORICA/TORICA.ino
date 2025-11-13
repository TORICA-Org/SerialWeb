#include <TORICA_SerialWeb.h>

char *SSID = "TORICA";
char *PASSWORD = "12345678";

void setup() {
  SerialWeb.begin(SSID, PASSWORD);
}

void loop() {
  char now[32];
  sprintf(now, "%ld", millis());
  SerialWeb.send("NOW_TIME", now);
  delay(1000);
}
