// 即時実行関数で囲み、グローバルを汚染しないようにしつつ、
// window.WebSocket だけを上書きする。
(function() {

    let mockIndexCounter = 0;
    let pushIntervalId = null; // 定期的なプッシュを制御するためのID

    const ReadyState = {
        CONNECTING: 0,
        OPEN: 1,
        CLOSING: 2,
        CLOSED: 3
    };

    class MockWebSocket {
        
        url = '';
        readyState = ReadyState.CONNECTING;
        
        onopen = null;
        onmessage = null;
        onclose = null;
        onerror = null;

        static CONNECTING = ReadyState.CONNECTING;
        static OPEN = ReadyState.OPEN;
        static CLOSING = ReadyState.CLOSING;
        static CLOSED = ReadyState.CLOSED;

        constructor(url) {
            this.url = url;
            console.log(`[MockWebSocket] 接続試行中... -> ${url}`);

            // 接続成功をエミュレート
            setTimeout(() => {
                this.readyState = ReadyState.OPEN;
                if (this.onopen) {
                    this.onopen(new Event('open'));
                }
                console.log("[MockWebSocket] 接続成功 (OPEN)");
                
                // ★★★ 接続成功後にプッシュ処理を開始 ★★★
                this.startPeriodicPush();

            }, 500); 
        }

        // --- 定期的なデータプッシュ処理 ---
        startPeriodicPush() {
            // 既にインターバルが設定されていたらクリア
            if (pushIntervalId) {
                clearInterval(pushIntervalId);
            }
            
            // 200ミリ秒ごとにデータをプッシュするインターバルを設定
            pushIntervalId = setInterval(() => {
                if (this.readyState !== ReadyState.OPEN) {
                    // 接続が閉じたらインターバルを停止
                    clearInterval(pushIntervalId);
                    return;
                }
                
                // 0から4を循環させる
                const currentIndex = mockIndexCounter % 5; 
                mockIndexCounter++; 
                
                const pushMsg = `(PUSH) Data update at ${new Date().toLocaleTimeString('ja-JP')} #${currentIndex}`;
                
                const customData = {
                    index: currentIndex, // 循環するインデックス
                    label: `data${currentIndex + 1}`,
                    value: pushMsg
                };

                const event = new MessageEvent('message', {
                    data: JSON.stringify(customData) 
                });

                if (this.onmessage) {
                    this.onmessage(event);
                }
                console.log(`[MockWebSocket] サーバープッシュ (Index ${currentIndex})`);

            }, 10); // 10ミリ秒ごと
        }
        
        // --- send メソッド (純粋にログ出力のみ) ---
        send(data) {
            if (this.readyState !== ReadyState.OPEN) {
                console.error("[MockWebSocket] エラー: WebSocketが開いていません。");
                return;
            }
            // クライアントからの送信はログに記録するのみ
            console.log(`[MockWebSocket] クライアントから送信: ${data}`);
        }

        // --- close メソッド ---
        close(code = 1000, reason = "Normal closure") {
            if (this.readyState === ReadyState.CLOSED || this.readyState === ReadyState.CLOSING) {
                return;
            }

            this.readyState = ReadyState.CLOSING;
            
            // 定期プッシュを停止
            if (pushIntervalId) {
                clearInterval(pushIntervalId);
                pushIntervalId = null;
            }
            
            // 切断処理をエミュレート
            setTimeout(() => {
                this.readyState = ReadyState.CLOSED;
                if (this.onclose) {
                    this.onclose(new CloseEvent('close', { code: code, reason: reason }));
                }
                console.log(`[MockWebSocket] 接続切断 (CLOSED)`);
            }, 200); 
        }
    }

    // グローバルスコープの本物の WebSocket を、作成したモックで上書きする
    window.WebSocket = MockWebSocket;

    console.log("WebSocketエミュレーションが有効になりました。");

})();