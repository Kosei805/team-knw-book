import usb_hid


class Keyboard:
    _MODIFIER_BITS = {
        0xE0: 0x01,  # Left Control
        0xE1: 0x02,  # Left Shift
        0xE2: 0x04,  # Left Alt
        0xE3: 0x08,  # Left GUI
        0xE4: 0x10,  # Right Control
        0xE5: 0x20,  # Right Shift
        0xE6: 0x40,  # Right Alt
        0xE7: 0x80,  # Right GUI
    }

    def __init__(self, devices):
        self._device = None
        for device in devices:
            if getattr(device, "usage_page", None) == 0x01 and getattr(device, "usage", None) == 0x06:
                self._device = device
                break

        if self._device is None:
            raise ValueError("USB HID keyboard device was not found")

        self._modifiers = 0
        self._keys = []
        self._report = bytearray(8)

    def _sync(self):
        self._report[0] = self._modifiers
        self._report[1] = 0
        for index in range(6):
            self._report[2 + index] = self._keys[index] if index < len(self._keys) else 0
        self._device.send_report(self._report)

    def _add_key(self, keycode):
        if keycode in self._MODIFIER_BITS:
            self._modifiers |= self._MODIFIER_BITS[keycode]
            return

        if keycode not in self._keys:
            if len(self._keys) >= 6:
                raise ValueError("Keyboard report can hold at most 6 simultaneous keys")
            self._keys.append(keycode)

    def _remove_key(self, keycode):
        if keycode in self._MODIFIER_BITS:
            self._modifiers &= ~self._MODIFIER_BITS[keycode]
            return

        if keycode in self._keys:
            self._keys.remove(keycode)

    def send(self, *keycodes):
        self.press(*keycodes)
        self.release_all()

    def press(self, *keycodes):
        for keycode in keycodes:
            self._add_key(keycode)
        self._sync()

    def release(self, *keycodes):
        for keycode in keycodes:
            self._remove_key(keycode)
        self._sync()

    def release_all(self):
        self._modifiers = 0
        self._keys.clear()
        self._sync()
