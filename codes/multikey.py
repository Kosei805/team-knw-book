import board
import keypad
import usb_hid
from keyboard import Keyboard
from keycode import Keycode

# 1. USBキーボードとして初期化
kbd = Keyboard(usb_hid.devices)

# 2. マトリクスの行(Row)と列(Column)のピンを指定
# 例: 行にGP0, GP1 / 列にGP2, GP3, GP4 を使用する2行3列(計6キー)
row_pins = (board.GP0, board.GP1)
col_pins = (board.GP2, board.GP3, board.GP4)

# 3. マトリクス制御オブジェクトの生成
# 内部で高速スキャンとチャタリング除去を自動で行ってくれる
matrix = keypad.KeyMatrix(row_pins, col_pins, columns_to_anodes=True)

# 4. キーマップの定義 (順番は Row0-Col0, Row0-Col1... となる)
KEYMAP = (Keycode.A, Keycode.B, Keycode.C, Keycode.D, Keycode.E, Keycode.F)

print("マトリクスキーボード 待機中...")

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
