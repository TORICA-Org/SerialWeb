//=============================================================
// TORICA WebServer Library
// Created by 00kenno
//=============================================================

#pragma once

#include <Arduino.h>
#include <Print.h>
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
  
  class SWClass : public Print {
    public:
      SWClass();
      ~SWClass();

      void begin (
        const char *ssid,
        const char *password,
        const IPAddress AP_IP = IPAddress(192, 168, 4, 1),
        const IPAddress NET_MSK = IPAddress(255, 255, 255, 0),
        const byte DNS_PORT = 53
      );
      void begin(const IPAddress STA_IP, const byte DNS_PORT = 53);
      void send(const char *label, const char *value);
      bool available();
      String readString();

      // Config Setter
      void setMaxClients (uint16_t _maxClients);

      // Status Getter
      

    private:
      static SWClass *instance;
      static DNSServer dns;
      static AsyncWebServer server;
      static AsyncWebSocket ws;
      
      void handleRoot(AsyncWebServerRequest *request);
      void handleWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
        AwsEventType type, void *arg, uint8_t *data, size_t len);
      void receiveHttp(AsyncWebServerRequest *request);
      void handleString(const char *str, size_t len);

      char *labels[99] = {nullptr};

      uint16_t maxClients = DEFAULT_MAX_WS_CLIENTS;
      static constexpr int BUFFER_SIZE = 256;
      char rx_buffer1[BUFFER_SIZE];
      char rx_buffer2[BUFFER_SIZE];
      volatile char *writing;
      volatile char *reading;
      volatile bool isReading;
      volatile bool newData;
      
      // Override `write()` in `Print.h`
      size_t write(uint8_t c) override;
      size_t write(const uint8_t *buffer, size_t size) override;
      void flush() override;
  };

  extern SWClass SerialWeb;

} // namespace SWNamespace

using namespace SWNamespace;