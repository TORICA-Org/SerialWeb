# SerialWeb
キャプティブポータルをデバッグモニターとして用いるためのライブラリです．

## ライブラリの導入
1. 上部の`Download ZIP`でZIPファイルをダウンロードしてください．
2. ArduinoIDEの `Sketch` -> `Include Library` -> `Add .ZIP Library...` で導入できます．もしくは，ArduinoIDEのライブラリが保存されたディレクトリ（Windowsでは通常`C:\Users\{UserName}\Documents\Arduino\libraries\`です．）に直接展開してください．

## 必要なライブラリ
以下のライブラリをライブラリマネージャからインストールしてください．
> [!NOTE]
> 名前の似たライブラリがたくさん存在します．十分注意してください．

- ESP32シリーズのマイコンの場合
  - ESPAsyncWebServer.h
  - AsyncTCP.h
- ESP8266の場合
  - ESPAsyncWebServer.h
  - ESPAsyncTCP.h
- Raspberry Pi Pico WなどのRP系マイコンの場合
  - ESPAsyncWebServer.h
  - RPAsyncTCP.h

## インスタンス
インスタンス化は不要です．グローバルインスタンスを提供しています．

## 関数
- `Serial.begin(ssid, password)`/`Serial.begin(ipAddress)`
  - 前者ではアクセスポイントモード，後者ではクライアントモードでWiFiが開始されます．

```cpp
constexpr char SSID[] = "SerialWeb";
constexpr char PASSWORD[] = "12345678";
void setup() {
  Serial.begin(SSID, PASSWORD);
}
```
```cpp
void setup() {
  // WiFiアクセスポイントへの接続後
  IPAddress localIP = WiFi.localIP();
  Serial.begin(localIP);
}
```

- `Serial.send(label, value)`
  - Webページのダッシュボードにデータを表示します．`label`がすでに存在する場合，その`value`を更新します．

```cpp
char label[] = "NOW_TIME";
char value[32];
sprintf(value, "%ld", millis());
SerialWeb.send(label, value);
```

- `Serial.print()`など
  - Webページのログに表示します．
  - `Print.h`を継承しています．`print()`や`println()`，`printf()`が使用可能です．

```cpp
SerialWeb.print("Hello, ");
SerialWeb.println("world."); // -> `Hello, world.`

SerialWeb.printf("Time: %ld", millis());
```

- `Serial.readString`など
  - 実装が不完全です．使用することはおすすめしません．