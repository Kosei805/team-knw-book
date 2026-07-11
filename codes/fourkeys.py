import board
import keypad
import usb_hid
from keyboard import Keyboard
from keycode import Keycode

# 1. USBキーボードとして初期化
kbd = Keyboard(usb_hid.devices)

# 2. 物理配線（Pico H c1〜c20、h1〜h20）に合わせた「行」と「列」のピン指定
row_pins = (board.GP0, board.GP1)
col_pins = (board.GP2, board.GP3)

# 3. マトリクス制御オブジェクトの生成
# ダイオードの向きが Col(アノード) ➔ Row(カソード) なので columns_to_anodes=True
matrix = keypad.KeyMatrix(row_pins, col_pins, columns_to_anodes=True)

# 4. キーマップの定義 (2行 x 2列 = 4キー)
# 順番は スイッチ1 (A), スイッチ2 (B), スイッチ3 (C), スイッチ4 (D)
KEYMAP = (
    Keycode.A, Keycode.B,
    Keycode.C, Keycode.D,
)

print("2x2 マトリクスキーボード 待機中...")

while True:
    # 押された・離されたという「イベント」を取得
    event = matrix.events.get()

    if event:
        # 発生したイベントの番号から、対応するキーコードを取得
        keycode = KEYMAP[event.key_number]

        if event.pressed:
            print(f"押しました: {keycode}")
            kbd.press(keycode)

        if event.released:
            print(f"離しました: {keycode}")
            kbd.release(keycode)
