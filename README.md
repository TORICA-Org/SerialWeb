# SerialWeb
キャプティブポータルをデバッグモニターとして用いるためのライブラリです．

## ライブラリの導入
1. 上部の`Download ZIP`でZIPファイルをダウンロードしてください．
2. ArduinoIDEの `Sketch` -> `Include Library` -> `Add .ZIP Library...` で導入できます．もしくは，ArduinoIDEのライブラリが保存されたディレクトリ（Windowsでは通常`C:\Users\{UserName}\Documents\Arduino\libraries\`です．）に直接展開してください．

## 必要なライブラリ
以下のライブラリをライブラリマネージャからインストールしてください．
> [!NOTE]
> 名前の似たライブラリがたくさん存在します．作者の名前なども使って照合してください．

### 非同期通信機能(WebSocketなど)を提供するライブラリ
- name: `ESP Async WebServer`
- author: `ESP32Async`

> [!NOTE]
> このライブラリはほとんどのマイコンで共通です．

### TCP通信管理機能を提供するライブラリ
- ESP32シリーズのマイコンの場合
  - name: `Async TCP`
  - author: `ESP32Async`
- ESP8266の場合
  - name: `ESP Async TCP`
  - author: `ESP32Async`
- Raspberry Pi Pico WなどのRP系マイコンの場合
  - name: `RPAsyncTCP`
  - author: `Hristo Gochkov,Khoi Hoang,Ayush Sharma`
  
> [!NOTE]
> これらのライブラリはマイコンごとに異なります．

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