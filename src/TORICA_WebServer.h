//===クラスの定義======================================

#ifndef TORICA_WEBSERVER_H
#define TORICA_WEBSERVER_H

#include <Arduino.h>

#ifdef ARDUINO_ARCH_RP2040 //RP2040及びRP2350のチェックに対応
#include <FreeRTOS.h>
#include <task.h>
#endif

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

struct data {
  char label[64];
  char content[32];
};

template <size_t DATA_SIZE>
class TORICA_WebServer {
  public:
    TORICA_WebServer(const char *_constant_massage_p);
    ~TORICA_WebServer();
    void makeLabel(const char *_label_p);
    void updateContent(const char *_label_p, const char *_content_p);
    void begin(const char *_ssid, const char *_password, const int _priority);

  private:
    WebServer _server;
    IPAddress _apIP;
    IPAddress _netMsk;
    DNSServer _dns;
    const byte _dnsPort = 53;

    int _num_of_data = DATA_SIZE;
    int _label_counter = 0;
    data _data_array[DATA_SIZE];
    const char *_constant_message = NULL;

    static const uint32_t STACK_SIZE = 8192;
    StaticTask_t xTaskBuffer;
    StackType_t xStack[STACK_SIZE];

    void handleRoot();
    void sendConstantMessage();
    void sendConfig();
    void sendData();

    static void loop(void *_param);
};

//===メンバ関数の定義======================================

#include "TORICA_WebServer_Index.h"

template <size_t DATA_SIZE>
TORICA_WebServer<DATA_SIZE>::TORICA_WebServer (const char *_constant_massage_p)
  : _server(80), _apIP(198, 168, 4, 1), _netMsk(255, 255, 255, 0) {
  _constant_message = _constant_massage_p;
}

template <size_t DATA_SIZE>
TORICA_WebServer<DATA_SIZE>::~TORICA_WebServer () {}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::makeLabel (const char *_label_p) {
  strlcpy(_data_array[_label_counter].label, _label_p, 64);
  _label_counter++;
}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::updateContent (const char *_label_p, const char *_content_p) {
  for (int i = 0; i < _num_of_data; i++) {
    if (strcmp(_data_array[i].label, _label_p) == 0) {
      strlcpy(_data_array[i].content, _content_p, 32);
      break;
    }
  }
}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::handleRoot () {
  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");
  _server.send(200, "text/html", html);
}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::sendConstantMessage () {
  _server.send(200, "text/plain", _constant_message);
}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::sendConfig () {
  int buff_length = 0;
  buff_length += strlen(R"({"num_of_data":"")"); // 1文目の基本構造
  buff_length += 10; // int型の最大桁数

  buff_length += strlen(R"(,"label_":"")"); // ラベルの基本構造
  for (int i = 0; i < _num_of_data; i++) {
    buff_length += 10; // int型の最大桁数
    buff_length += strlen(_data_array[i].label);
  }
  buff_length += 2; // '}'+'\0'

  char *config = (char *)malloc(buff_length);
  if (config == NULL) {
    _server.send(200, "application/json", R"({"num_of_data":"0"})");
    return;
  }

  char *writing_ptr = config;
  int written_length = 0;
  written_length = snprintf(writing_ptr, buff_length, R"({"num_of_data":"%d")", _num_of_data);
  writing_ptr += written_length;
  buff_length -= written_length;

  for (int i = 0; i < _num_of_data; i++) {
    written_length = snprintf(writing_ptr, buff_length, R"(,"label_%d":"%s")", i, _data_array[i].label);
    writing_ptr += written_length;
    buff_length -= written_length;
  }

  snprintf(writing_ptr, buff_length, "}");

  _server.send(200, "application/json", config);
  free(config);
}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::sendData () {
  int index = 0;
  if (_server.method() == HTTP_POST) {
    String index_str = _server.arg("index");
    index = index_str.toInt();
  }

  int buff_length = 0;
  buff_length += strlen(R"({"index":"","content":""})");
  buff_length += 10; // int型最大桁数
  buff_length += strlen(_data_array[index].content);
  buff_length++; // '\0'

  char *buff = (char *)malloc(buff_length);
  if (buff == NULL) {
    int error_length = 0;
    error_length += strlen(R"({"index":"","content":"failed malloc"})");
    error_length += 10;
    error_length++;

    char *error = (char *)malloc(error_length);
    snprintf(error, error_length, R"({"index":"%d","content":"failed malloc"})", index);
    _server.send(200, "application/json", error);
    free(error);
    return;
  }
  snprintf(buff, buff_length, R"({"index":"%d","content":"%s"})", index, _data_array[index].content);
#ifdef ESP_PLATFORM
  _server.sendHeader("Connection", "keep-alive"); 
  _server.sendHeader("Keep-Alive", "timeout=5, max=100"); 
#endif
  _server.send(200, "application/json", buff);
  free(buff);
}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::loop (void *_param) {
  // 型情報がxTaskCreateStaticでthisを渡した際に失われる．（void *_param）
  // TORICA_WebServer*型にキャストする．
  TORICA_WebServer* self = static_cast<TORICA_WebServer *>(_param);
  while (true) {
    self -> _dns.processNextRequest();
    self -> _server.handleClient();
    vTaskDelay(10/portTICK_RATE_MS);
  }
}

template <size_t DATA_SIZE>
void TORICA_WebServer<DATA_SIZE>::begin (const char *_ssid, const char *_password, const int _priority) {
  WiFi.disconnect(true);//これがないとwebServerを再起動できない
  delay(1000);//この遅延は必須（これがないとwebServerが起動しない）
  WiFi.softAPConfig(_apIP, _apIP, _netMsk);
  WiFi.softAP(_ssid, _password);
  delay(500);  // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  _dns.setErrorReplyCode(DNSReplyCode::NoError);
  _dns.start(_dnsPort, "*", _apIP);

  _server.on("/", [this]() {this -> handleRoot();} );
  _server.on("/generate_204", [this]() {this -> handleRoot();} );
  _server.on("/fwlink", [this]() {this -> handleRoot();} );
  _server.onNotFound([this]() {this -> handleRoot();} );
  _server.on("/constantmessage", [this]() {this -> sendConstantMessage();} );
  _server.on("/config", [this]() {this -> sendConfig();} );
  _server.on("/data", [this]() {this -> sendData();} );
  _server.begin();

  //マルチスレッドでタスクを実行
  xTaskCreateStatic(
    loop, //タスク関数へのポインタ
    "WebServerLoop", //タスク名
    STACK_SIZE, //スタックサイズ
    this, //インスタンス自身のポインタ
    (UBaseType_t)_priority, //タスクの優先順位
    xStack, //タスクのスタックとして使用する配列
    &xTaskBuffer //タスクのデータ構造体へのポインタ
  );
}

#endif // TORICA_WEBSERVER_H
