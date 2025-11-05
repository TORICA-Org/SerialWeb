#include "TORICA_WebServer.h"

namespace WMNamespace { // Namespaceの開始

  INCBIN(html, "assets/index.html"); // HTMLファイルをバイナリとして埋め込む

  AsyncWebServer WMClass::server(80); // HTTPサーバーインスタンスの定義
  AsyncWebSocket WMClass::ws("/ws"); // WebSocketエンドポイントの設定
  DNSServer WMClass::dns; // DNSサーバーインスタンスの定義

  void WMClass::handleRoot (AsyncWebServerRequest *request) {
    request->send(200, "text/html", html); // HTMLコンテンツを返す
  }

  void WMClass::handleWsEvent (AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { // WebSocketイベントハンドラ
    if (type == WS_EVT_CONNECT) { // クライアント接続時
      ws.textAll("new client connected"); // 全クライアントにメッセージ送信
      Serial.println("ws connect");
      client->setCloseClientOnQueueFull(false); // キューが満杯でもクライアントを閉じない
      client->ping(); // 接続確認のpingを送信

    } else if (type == WS_EVT_DISCONNECT) { // クライアント切断時
      ws.textAll("client disconnected"); // 全クライアントにメッセージ送信
      Serial.println("ws disconnect");

    } else if (type == WS_EVT_ERROR) { // エラー発生時
      Serial.println("ws error");

    } else if (type == WS_EVT_PONG) { // pong受信時
      Serial.println("ws pong");

    } else if (type == WS_EVT_DATA) { // データ受信時
      AwsFrameInfo *info = (AwsFrameInfo *)arg; // フレーム情報の取得
      Serial.printf("index: %" PRIu64 ", len: %" PRIu64 ", final: %" PRIu8 ", opcode: %" PRIu8 "\n", info->index, info->len, info->final, info->opcode); // フレーム情報の表示
      String msg = "";
      if (info->final && info->index == 0 && info->len == len) { // 完全なメッセージが1フレームで届いた場合
        if (info->opcode == WS_TEXT) { // テキストメッセージの場合
          data[len] = 0; // 文字列終端
          Serial.printf("ws text: %s\n", (char *)data); // 受信メッセージの表示
        }
      }
    }
  }

  void WMClass::begin(const char *ssid, const char *password) {
    Serial.println("Starting TORICA WebServer...");
    WiFi.disconnect(true); // これがないとwebServerを再起動できない
    delay(1000); // この遅延は必須（これがないとwebServerが起動しない）
    WiFi.softAPConfig(AP_IP, AP_IP, NET_MSK); // IPアドレスの設定
    WiFi.softAP(ssid, password); // APモードでWiFi開始
    delay(500);  // IPアドレス取得のための遅延
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    ws.onEvent(handleWsEvent); // WebSocketイベントハンドラの登録
    Serial.println("WebSocket server started.");

    // WebSocketの同時接続数を制限するミドルウェアの追加
    server.addHandler(&ws).addMiddleware([](AsyncWebServerRequest *request, ArMiddlewareNext next) {
      // ws.count()は現在のWSクライアントの数
      if (ws.count() > 5) {
        // 2つ以上のクライアントがいる場合、次のクライアントの接続を防止
        request->send(503, "text/plain", "Server is busy");
      } else {
        // 次のミドルウェアと最後にハンドラを処理
        next();
      }
    });
  
    dns.setErrorReplyCode(DNSReplyCode::NoError); // DNSエラーコードの設定
    dns.start(DNS_PORT, "*", AP_IP); // DNSサーバーの開始
    Serial.println("DNS server started.");

    server.on("/", HTTP_GET, handleRoot); // ルートハンドラの登録
    server.on("/generate_204", HTTP_GET, handleRoot); // Android Captive Portal対応
    server.on("/fwlink", HTTP_GET, handleRoot); // Windows Captive Portal対応
    server.onNotFound(handleRoot); // その他のリクエストはすべてルートへ誘導

    server.addHandler(&ws); // WebSocketハンドラの追加

    server.begin(); // HTTPサーバーの開始
    Serial.println("HTTP server started.");
  }

  void WMClass::send (const char *label, const char *value) {
    String message = String("{\"label\":\"") + String(label) + String("\",\"content\":\"") + String(value) + String("\"}"); // JSON形式のメッセージ作成
    ws.textAll(message); // すべてのクライアントにメッセージ送信
  }

  void WMClass::cleanupClients (uint16_t maxClients) {
    ws.cleanupClients(maxClients); // クライアントのクリーンアップ
  }

  WMClass WebMonitor; // グローバルインスタンスの作成

} // namespace WMNamespace