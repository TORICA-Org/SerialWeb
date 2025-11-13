#include "SerialWeb.h"

namespace SWNamespace {

#ifdef TORICA
  INCBIN(html, "../docs/TORICA_index.html"); // HTMLファイルをバイナリとして埋め込む
#else
  INCBIN(html, "../docs/index.html"); // HTMLファイルをバイナリとして埋め込む
#endif

  AsyncWebServer SWClass::server(80); // HTTPサーバーインスタンスの定義
  AsyncWebSocket SWClass::ws("/ws"); // WebSocketエンドポイントの設定
  DNSServer SWClass::dns; // DNSサーバーインスタンスの定義
  uint16_t SWClass::maxClients = DEFAULT_MAX_WS_CLIENTS;

  void SWClass::handleRoot (AsyncWebServerRequest *request) {
    request->send(200, "text/html", html); // HTMLコンテンツを返す
  }

  void SWClass::handleWsEvent (AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { // WebSocketイベントハンドラ
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

  void SWClass::begin (const char *ssid, const char *password, 
    const IPAddress AP_IP, const IPAddress NET_MSK, const byte DNS_PORT) {
    
    WiFi.disconnect(true); // これがないとwebServerを再起動できない
    delay(1000); // この遅延は必須（これがないとwebServerが起動しない）
    WiFi.softAPConfig(AP_IP, AP_IP, NET_MSK); // IPアドレスの設定
    WiFi.softAP(ssid, password); // APモードでWiFi開始
    delay(500);  // IPアドレス取得のための遅延
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    
    begin(AP_IP, DNS_PORT);
  }

  void SWClass::begin (const IPAddress AP_IP, const byte DNS_PORT) {
    ws.onEvent(handleWsEvent); // WebSocketイベントハンドラの登録
    Serial.println("WebSocket server started.");

    // WebSocketの同時接続数を制限するミドルウェアの追加
    server.addHandler(&ws).addMiddleware([](AsyncWebServerRequest *request, ArMiddlewareNext next) {
      if (ws.count() > maxClients) { // ws.count()は現在のWSクライアントの数
        request->send(503, "text/plain", "Server is busy");
      } else {
        next(); // 次のミドルウェアと最後にハンドラを処理
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

  void SWClass::send (const char *label, const char *value) {
    int dataIndex = -1;
    for (int i = 0; i < sizeof(labels)/sizeof(labels[0]); i++) {
      if (labels[i] == nullptr) { // 未登録のラベルを見つけた場合
        labels[i] = strdup(label); // ラベルをコピーして登録
        dataIndex = i; // データインデックスを設定
        Serial.printf("Registered new label: %s at index %d\n", label, i);
        break; // ループを抜ける
      }
      else if (strcmp(labels[i], label) == 0) { // 既存のラベルを見つけた場合
        dataIndex = i; // 既存のラベルのインデックスを設定
        Serial.printf("Found existing label: %s at index %d\n", label, i);
        break; // ループを抜ける
      }
    }
    if (dataIndex == -1) {
      Serial.println("Error: No available slot for new label.");
      return; // ラベル登録に失敗した場合は終了
    }
    String message = String("{\"index\":\"") + String(dataIndex) + String("\",\"label\":\"") + String(label) + String("\",\"content\":\"") + String(value) + String("\"}"); // JSON形式のメッセージ作成
    ws.textAll(message); // すべてのクライアントにメッセージ送信
    ws.cleanupClients(maxClients); // クライアントのクリーンアップ
  }

  SWClass SerialWeb; // グローバルインスタンスの作成

} // namespace SWNamespace