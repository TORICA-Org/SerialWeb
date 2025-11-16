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
      SWClass () {
        instance = this;
        tx_buffer[0] = '\0';
        rx_buffer1[0] = '\0';
        rx_buffer2[0] = '\0';
        writing = rx_buffer1;
        reading = rx_buffer2;
      }
      ~SWClass(){}

      void begin (
        const char *ssid,
        const char *password,
        const IPAddress AP_IP = IPAddress(198, 168, 4, 1),
        const IPAddress NET_MSK = IPAddress(255, 255, 255, 0),
        const byte DNS_PORT = 53
      );
      void begin(const IPAddress STA_IP, const byte DNS_PORT = 53);
      void send(const char *label, const char *value);
      int available();
      String readString();

      // Setter
      void setMaxClients (uint16_t _maxClients) {
        maxClients = _maxClients;
      }

    private:
      static SWClass *instance;
      static DNSServer dns;
      static AsyncWebServer server;
      static AsyncWebSocket ws;
      
      void handleRoot(AsyncWebServerRequest *request);
      void handleWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
        AwsEventType type, void *arg, uint8_t *data, size_t len);


      char *labels[99] = {nullptr};

      uint16_t maxClients = DEFAULT_MAX_WS_CLIENTS;
      static constexpr int BUFFER_SIZE = 256;
      char tx_buffer[BUFFER_SIZE];
      char rx_buffer1[BUFFER_SIZE];
      char rx_buffer2[BUFFER_SIZE];
      volatile char *writing;
      volatile char *reading;

      // Override `write()` in `Print.h`
      virtual void flush(){}
      virtual size_t write(uint8_t c);
  };

  extern SWClass SerialWeb;

} // namespace SWNamespace

using namespace SWNamespace;