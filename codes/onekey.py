import board
import digitalio
import usb_hid
# from adafruit_debouncer import Debouncer
from debouncer import Debouncer
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

# 3. チャタリング対策（デバウンサー）を被せる
switch = Debouncer(pin)

print("1キーボード 待機中...")

while True:
    switch.update()  # 常に状態を監視

    # スイッチが押された瞬間（HighからLowに落ちた = fell を検知）
    if switch.fell:
        print("Aキーを送信！")
        kbd.send(Keycode.A)
