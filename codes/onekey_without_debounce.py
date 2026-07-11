import board
import digitalio
import usb_hid

# from adafruit_hid.keyboard import Keyboard
from keyboard import Keyboard

# from adafruit_hid.keycode import Keycode
from keycode import Keycode

# 1. USBキーボードとして初期化
kbd = Keyboard(usb_hid.devices)

# 2. ピンの設定 (GP0とGNDの間にスイッチを繋ぐ想定)
pin = digitalio.DigitalInOut(board.GP0)
pin.direction = digitalio.Direction.INPUT
pin.pull = digitalio.Pull.UP  # 何も押していない時はHigh

# 3. チャタリング対策なし。前回の状態をそのまま覚えておくだけ
last_state = pin.value

print("1キーボード 待機中...")

while True:
    current_state = pin.value

    # スイッチが押された瞬間（HighからLowに落ちた瞬間）を検知
    if last_state and not current_state:
        print("Aキーを送信！")
        kbd.send(Keycode.A)

    last_state = current_state
