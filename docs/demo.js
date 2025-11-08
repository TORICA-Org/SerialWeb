// 即時実行関数で囲み、グローバルを汚染しないようにしつつ、
// window.WebSocket だけを上書きする。
(function() {

    // (1) 本物のWebSocketの定数をコピーしておく
    const ReadyState = {
        CONNECTING: 0,
        OPEN: 1,
        CLOSING: 2,
        CLOSED: 3
    };

    // (2) WebSocketの「ふり」をするモッククラスを定義
    class MockWebSocket {
        
        // --- 本物のAPIと同じプロパティを定義 ---
        url = '';
        readyState = ReadyState.CONNECTING;
        
        // イベントハンドラ
        onopen = null;
        onmessage = null;
        onclose = null;
        onerror = null;

        // --- 定数 (static) ---
        static CONNECTING = ReadyState.CONNECTING;
        static OPEN = ReadyState.OPEN;
        static CLOSING = ReadyState.CLOSING;
        static CLOSED = ReadyState.CLOSED;

        constructor(url) {
            this.url = url;
            console.log(`[MockWebSocket] 接続試行中... -> ${url}`);

            // (3) 非同期処理をシミュレート
            // 0.5秒後に「接続成功」をエミュレート
            setTimeout(() => {
                this.readyState = ReadyState.OPEN;
                // onopenイベントハンドラが設定されていれば実行
                if (this.onopen) {
                    // 本物と同じようにEventオブジェクトを渡す
                    this.onopen(new Event('open'));
                }
                console.log("[MockWebSocket] 接続成功 (OPEN)");
            }, 500); 
        }

        // --- 本物のAPIと同じメソッドを定義 ---

        send(data) {
            if (this.readyState !== ReadyState.OPEN) {
                console.error("[MockWebSocket] エラー: WebSocketが開いていません。");
                return;
            }

            console.log(`[MockWebSocket] クライアントから受信: ${data}`);

            // (4) サーバーからの応答をエミュレート（エコーサーバー）
            // 1秒後にオウム返しする
            setTimeout(() => {
                // 本物と同じようにMessageEventオブジェクトを作成
                const echoMsg = `(エコー) ${data}`;
                const customData = {
                    index: 0,
                    label: "dummy",
                    value: echoMsg
                };

                // 正しい書き方: カスタムデータを 'data' プロパティに格納
                const event = new MessageEvent('message', {
                    data: JSON.stringify(customData) 
                    // 他のオプション（origin, lastEventId, source, ports）も必要に応じて追加可能
                });

                // onmessageハンドラが設定されていれば実行
                if (this.onmessage) {
                    this.onmessage(event);
                }
                console.log(`[MockWebSocket] サーバーから送信: ${echoMsg}`);

            }, 1000);
        }

        close(code = 1000, reason = "Normal closure") {
            if (this.readyState === ReadyState.CLOSED || this.readyState === ReadyState.CLOSING) {
                return;
            }

            this.readyState = ReadyState.CLOSING;
            
            // (5) 切断処理をエミュレート
            setTimeout(() => {
                this.readyState = ReadyState.CLOSED;
                if (this.onclose) {
                    this.onclose(new CloseEvent('close', { code: code, reason: reason }));
                }
                console.log(`[MockWebSocket] 接続切断 (CLOSED)`);
            }, 200); // すぐに切断
        }
    }

    // (6) ★最重要★
    // グローバルスコープの本物の WebSocket を、作成したモックで上書きする
    window.WebSocket = MockWebSocket;

    console.log("WebSocketエミュレーションが有効になりました。");

})();