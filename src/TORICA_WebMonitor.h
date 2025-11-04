//=============================================================
// TORICA WebMonitor Library
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

#include "TORICA_WebMonitor_Index.h"

namespace TORICA_WebMonitor_Namespace {
  
  class TORICA_WebMonitor_Class {
    public:
      void begin(const char *_ssid, const char *_password);
      void updateContent(const char *_label_p, const char *_content_p);
      void cleanupClients(uint16_t maxClients = DEFAULT_MAX_WS_CLIENTS);

    private:
      static void handleRoot(AsyncWebServerRequest *request);
      static void handleWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
      //void sendConstantMessage(AsyncWebServerRequest *request);
      //void sendConfig(AsyncWebServerRequest *request);
      //void sendData(AsyncWebServerRequest *request);

      static DNSServer dns;
      static AsyncWebServer server;
      static AsyncWebSocket ws;

      const byte DNS_PORT = 53;
      const IPAddress AP_IP = IPAddress(198, 168, 4, 1);
      const IPAddress NET_MSK = IPAddress(255, 255, 255, 0);
  };

  extern TORICA_WebMonitor_Class WebMonitor;

} // namespace TORICA_WebMonitor_Namespace

using namespace TORICA_WebMonitor_Namespace;