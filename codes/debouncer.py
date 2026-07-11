import time

class Debouncer:
    def __init__(self, pin, debounce_ms=20):
        self.pin = pin
        self.debounce_duration = debounce_ms / 1000.0  # 判定保留にする時間 (秒)
        
        # 内部の状態管理変数
        self.current_stable_state = self.pin.value       # 確定している現在の状態
        self.last_raw_state = self.current_stable_state  # 前回のサンプリング状態
        self.last_changed_time = time.monotonic()        # タイマーリセット用時刻
        
        # イベント検知フラグ
        self._fell = False

    @property
    def fell(self):
        """スイッチが押された瞬間（High から Low に変化した時）に True を返す"""
        if self._fell:
            self._fell = False  # 1回読み取ったらフラグをクリア
            return True
        return False

    def update(self):
        """メインループで毎回呼び出され、内部でチャタリングを排除するコア処理"""
        current_raw_state = self.pin.value
        current_time = time.monotonic()  # マイコン内部の秒数を取得

        # 1. 状態に変化（ノイズ含む）があったか？
        if current_raw_state != self.last_raw_state:
            # 変化した瞬間、タイマーをリセットして「判定保留」にする
            self.last_changed_time = current_time
            self.last_raw_state = current_raw_state

        # 2. 設定した時間（デバウンス時間）ずーーっと安定していたかをチェック
        time_since_last_change = current_time - self.last_changed_time
        
        if time_since_last_change >= self.debounce_duration:
            # 3. 十分な時間が経過したら、ようやく「状態を確定」する
            if current_raw_state != self.current_stable_state:
                # 以前が High(True) で、新しい安定状態が Low(False) の場合 = 押された瞬間
                if self.current_stable_state and not current_raw_state:
                    self._fell = True
                
                self.current_stable_state = current_raw_state