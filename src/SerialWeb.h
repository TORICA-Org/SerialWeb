//=============================================================
// TORICA WebServer Library
// Created by 00kenno
//=============================================================

#pragma once

#include <DNSServer.h>
#include <Arduino.h>
#if defined(ESP32) || defined(LIBRETINY)
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
#include <RPAsyncTCP.h>
#include <WiFi.h>
#endif

#include <ESPAsyncWebServer.h>

#include "assets/incbin.h"

namespace WMNamespace {
  
  class WMClass {
    public:
      void begin(const char *ssid, const char *password);
      void send(const char *label, const char *value);
      void cleanupClients(uint16_t maxClients = DEFAULT_MAX_WS_CLIENTS);

    private:
      static void handleRoot(AsyncWebServerRequest *request);
      static void handleWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

      static DNSServer dns;
      static AsyncWebServer server;
      static AsyncWebSocket ws;

      const byte DNS_PORT = 53;
      const IPAddress AP_IP = IPAddress(198, 168, 4, 1);
      const IPAddress NET_MSK = IPAddress(255, 255, 255, 0);

      char *labels[100] = {nullptr};
  };

  extern WMClass WebMonitor;

} // namespace WMNamespace

using namespace WMNamespace;