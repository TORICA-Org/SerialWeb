#include <SerialWeb.h>

constexpr char SSID[] = "SerialWeb";
constexpr char PASSWORD[] = "12345678";

// インスタンス化は不要です．グローバルインスタンスを提供しています．

void setup() {
  Serial.begin(115200);
  SerialWeb.begin(SSID, PASSWORD); // Serialなどと同様に初期化します．
}

void loop() {
  char label[] = "NOW_TIME";
  char value[32];

  // send関数で送信したデータは，ダッシュボードに表示されます．
  sprintf(value, "%ld", millis());
  SerialWeb.send(label, value);

  // print関数やprintln関数はSerialなどと同様に使用できます．
  SerialWeb.print("Hello, ");
  SerialWeb.println("world.");

  // 読み取りは，readString関数のみの実装です．
  // available関数はbool型です．
  if (SerialWeb.available()) {
    // メモリの上書き問題が解決できていません．
    // 半角英数字13文字までなら正常動作します．
    String msg = SerialWeb.readString();
    Serial.printf("readString: %s\n", msg);
  }

  delay(100);
}
