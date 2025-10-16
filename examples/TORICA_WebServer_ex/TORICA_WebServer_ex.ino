//TORICA_WebServer

//WiFiを利用したスマートフォンやPCでデバッグ用モニターです．

//「ログインが必要です」などと表示される画面（キャプティブポータル）で
//任意の文字列（改行コードを含む）をモニターできます．

//2025.04.07時点で以下のマイコンでのコンパイルを確認済みです．
//ESP32C3
//Raspberry Pi Pico W
//Raspberry Pi Pico 2W

#include <TORICA_WebServer.h>

const char *ssid = "TORICA WebServer";
const char *password = "toricadensou";

const char *constant_massage = R"(

固定の文字列の先頭ポインタを渡します．

Rで始まるこの記法を生文字列リテラルといいます．

改行や"ダブルクオーテーション"などはそのまま反映され，エスケープシーケンスは無視されます．

\0 \t \n

JSONを記述するのに役立ちます．

)";

const int data_num = 5;
const int priority = 2; // タスクの優先順位：WebServerがうまく動作しないときには調整してください．(range: 0 ~ 7)
TORICA_WebServer<data_num> webserver(constant_massage);

void setup() {
  Serial.begin(115200);
  Serial.println("TORICA WebServer");

  webserver.makeLabel("data1");
  webserver.makeLabel("data2");
  webserver.makeLabel("data3");
  webserver.makeLabel("data4");
  webserver.makeLabel("data5");
  webserver.begin(ssid, password, priority);

  #ifdef ARDUINO_ARCH_RP2040 //RP2040及びRP2350のチェックに対応
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  #endif
}

void loop() {
  uint32_t now = millis();

  char data_buff[32];
  sprintf(data_buff, "%lu", now);
  webserver.updateContent("data1", data_buff);
  Serial.println(data_buff);

  sprintf(data_buff, "%lu", now + 10000);
  webserver.updateContent("data2", data_buff);
  Serial.println(data_buff);

  sprintf(data_buff, "%lu", now + 20000);
  webserver.updateContent("data3", data_buff);
  Serial.println(data_buff);

  sprintf(data_buff, "%lu", now + 30000);
  webserver.updateContent("data4", data_buff);
  Serial.println(data_buff);

  sprintf(data_buff, "%lu", now + 40000);
  webserver.updateContent("data5", data_buff);
  Serial.println(data_buff);
  
  delay(100);
}
