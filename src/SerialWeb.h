//=============================================================
// TORICA WebServer Library
// Created by 00kenno
//=============================================================

#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
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

#include "incbin.h"

namespace SWNamespace {
  
  class SWClass {
    public:
      void begin(
        const char *ssid,
        const char *password,
        const IPAddress AP_IP = IPAddress(198, 168, 4, 1),
        const IPAddress NET_MSK = IPAddress(255, 255, 255, 0),
        const byte DNS_PORT = 53
      );
      void begin(const IPAddress STA_IP, const byte DNS_PORT = 53);
      // void print(char *string);
      // void println(char *string);
      void send(const char *label, const char *value);

      // Setter
      void setMaxClients(uint16_t _maxClients) {
        maxClients = _maxClients;
      }

    private:
      static void handleRoot(AsyncWebServerRequest *request);
      static void handleWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

      static DNSServer dns;
      static AsyncWebServer server;
      static AsyncWebSocket ws;

      char *labels[99] = {nullptr};
      static uint16_t maxClients;
  };

  extern SWClass SerialWeb;

} // namespace SWNamespace

using namespace SWNamespace;