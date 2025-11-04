#include "TORICA_WebMonitor.h"

namespace TORICA_WebMonitor_Namespace {

  AsyncWebServer TORICA_WebMonitor_Class::server(80);
  AsyncWebSocket TORICA_WebMonitor_Class::ws("/ws");
  DNSServer TORICA_WebMonitor_Class::dns;

  void TORICA_WebMonitor_Class::handleRoot (AsyncWebServerRequest *request) {
    request->send(200, "text/html", html);
  }

  void TORICA_WebMonitor_Class::handleWsEvent (AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
      ws.textAll("new client connected");
      Serial.println("ws connect");
      client->setCloseClientOnQueueFull(false);
      client->ping();

    } else if (type == WS_EVT_DISCONNECT) {
      ws.textAll("client disconnected");
      Serial.println("ws disconnect");

    } else if (type == WS_EVT_ERROR) {
      Serial.println("ws error");

    } else if (type == WS_EVT_PONG) {
      Serial.println("ws pong");

    } else if (type == WS_EVT_DATA) {
      AwsFrameInfo *info = (AwsFrameInfo *)arg;
      Serial.printf("index: %" PRIu64 ", len: %" PRIu64 ", final: %" PRIu8 ", opcode: %" PRIu8 "\n", info->index, info->len, info->final, info->opcode);
      String msg = "";
      if (info->final && info->index == 0 && info->len == len) {
        if (info->opcode == WS_TEXT) {
          data[len] = 0;
          Serial.printf("ws text: %s\n", (char *)data);
        }
      }
    }
  }

  void TORICA_WebMonitor_Class::begin(const char *ssid, const char *password) {

  WiFi.disconnect(true);//これがないとwebServerを再起動できない
  delay(1000);//この遅延は必須（これがないとwebServerが起動しない）
  WiFi.softAPConfig(AP_IP, AP_IP, NET_MSK);
  WiFi.softAP(ssid, password);
  delay(500);  // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  ws.onEvent(handleWsEvent);
  Serial.println("WebSocket server started.");

  // // shows how to prevent a third WS client to connect
  // server.addHandler(&ws).addMiddleware([](AsyncWebServerRequest *request, ArMiddlewareNext next) {
  //   // ws.count() is the current count of WS clients: this one is trying to upgrade its HTTP connection
  //   if (ws.count() > 1) {
  //     // if we have 2 clients or more, prevent the next one to connect
  //     request->send(503, "text/plain", "Server is busy");
  //   } else {
  //     // process next middleware and at the end the handler
  //     next();
  //   }
  // });

  
    dns.setErrorReplyCode(DNSReplyCode::NoError);
    dns.start(DNS_PORT, "*", AP_IP);
    Serial.println("DNS server started.");

    server.on("/", HTTP_GET, handleRoot);
    server.on("/generate_204", HTTP_GET, handleRoot);
    server.on("/fwlink", HTTP_GET, handleRoot);
    server.onNotFound(handleRoot);

    server.addHandler(&ws);

    server.begin();
    Serial.println("HTTP server started.");
  }

  void TORICA_WebMonitor_Class::updateContent (const char *_label_p, const char *_content_p) {
    String message = String("{\"label\":\"") + String(_label_p) + String("\",\"content\":\"") + String(_content_p) + String("\"}");
    ws.textAll(message);
  }

  void TORICA_WebMonitor_Class::cleanupClients (uint16_t maxClients) {
    ws.cleanupClients(maxClients);
  }


  TORICA_WebMonitor_Class WebMonitor;

} // namespace TORICA_WebMonitor_Namespace