#include <TORICA_WebServer.h>

void setup() {
  Serial.begin(115200);
  WebMonitor.begin("torica", "12345678");
}

void loop() {
  char now[32];
  sprintf(now, "%ld", millis());
  WebMonitor.send("NOW_TIME", now);
  WebMonitor.cleanupClients(2);
  delay(1000);
}
