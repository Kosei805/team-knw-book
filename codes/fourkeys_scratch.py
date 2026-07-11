import board
import digitalio
import usb_hid

from keyboard import Keyboard
from keycode import Keycode

# 1. USBキーボードとして初期化
kbd = Keyboard(usb_hid.devices)

# 2. 物理配線に合わせた行(Row)・列(Column)のピン
ROW_PINS = (board.GP0, board.GP1)
COL_PINS = (board.GP2, board.GP3)

# 3. 行は普段「入力(ハイインピーダンス)」にしておき、
#    スキャンする瞬間だけ「出力Low」に切り替える
rows = []
for pin in ROW_PINS:
    row = digitalio.DigitalInOut(pin)
    row.direction = digitalio.Direction.INPUT
    rows.append(row)

# 列は常にプルアップ入力。押されていなければHigh、ダイオード経由で引っ張られるとLow
cols = []
for pin in COL_PINS:
    col = digitalio.DigitalInOut(pin)
    col.direction = digitalio.Direction.INPUT
    col.pull = digitalio.Pull.UP
    cols.append(col)

# 4. どの(行, 列)がどのキーに対応するかを、ここで直接見える形で定義する
#    keypad.KeyMatrixのkey_numberのような自動採番には頼らない
KEYMAP = (
    (Keycode.A, Keycode.B),  # Row0: Col0, Col1
    (Keycode.C, Keycode.D),  # Row1: Col0, Col1
)

# 5. 直前の押下状態を自分で保持しておく（pressed/releasedイベントを自作するため）
pressed_state = [[False for _ in cols] for _ in rows]

print("自作マトリクススキャン 待機中...")

while True:
    for row_index, row in enumerate(rows):
        # スキャンする行だけ出力Lowに切り替える
        row.direction = digitalio.Direction.OUTPUT
        row.value = False

        for col_index, col in enumerate(cols):
            # プルアップなので、ダイオード経由でLowに引っ張られていれば押されている
            is_pressed = not col.value
            was_pressed = pressed_state[row_index][col_index]

            if is_pressed and not was_pressed:
                keycode = KEYMAP[row_index][col_index]
                print(f"押しました: {keycode}")
                kbd.press(keycode)
            elif not is_pressed and was_pressed:
                keycode = KEYMAP[row_index][col_index]
                print(f"離しました: {keycode}")
                kbd.release(keycode)

            pressed_state[row_index][col_index] = is_pressed

        # 次の行に移る前に、この行をハイインピーダンスに戻す
        row.direction = digitalio.Direction.INPUT
