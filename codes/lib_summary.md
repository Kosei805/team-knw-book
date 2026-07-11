# `codes/lib` まとめ

このファイルは、キー1個のキーボードで使うローカルモジュールの確認用メモです。本文では動作の理由を順に説明し、このファイルでは各ファイルの責務を横に並べています。

## ファイルごとの役割

| ファイル | 主な責務 | 呼び出す側 | 変更するときの注意 |
| --- | --- | --- | --- |
| `keyboard.py` | HIDキーボードの選択、キー状態の管理、レポート送信 | `onekey_with_debounce.py`、`onekey_without_debounce.py` | レポートディスクリプタの形式と8バイト配置を一致させる |
| `keycode.py` | キー名とHIDキーコードの対応付け | `keyboard.py`を使う各プログラム | 数値は文字コードではなくHIDのキーコード |
| `debouncer.py` | 入力のチャタリング除去と押下イベント生成 | `onekey_with_debounce.py` | `update()`を繰り返し呼び、`fell`を必要な場所で一度だけ読む |

## `keyboard.py`

- `Keyboard(usb_hid.devices)`で、`usage_page`が`0x01`、`usage`が`0x06`のキーボード用HIDデバイスを選ぶ
- `_modifiers`は修飾キーをビットで保持する
- `_keys`は通常キーを最大6個保持する
- `_sync()`は修飾キー、予約領域、通常キー6個から8バイトのレポートを作る
- `press()`は状態を追加してレポートを送り、`release()`は指定キーを取り除いて送る
- `release_all()`は空のレポートを送り、`send()`は押下と全解放を連続して行う
- 8バイト形式は現在選択しているHIDデバイスの形式に依存する

## `keycode.py`

- `Keycode.A`の`0x04`はAという文字ではなく、Aキーを表すHIDキーコード
- `SHIFT`などの修飾キーは`keyboard.py`の`_MODIFIER_BITS`と対応する
- 大文字・小文字・記号の最終的な解釈は、キーコードを受け取るOSやキーボード配列が担当する
- 新しいキーを追加するときは、HIDの定義とOSでの挙動を別々に確認する

## `debouncer.py`

- `last_raw_state`は直近の入力値、`current_stable_state`は確定値
- `last_changed_time`から20ミリ秒以上変化が続いたときに、状態を確定する
- HighからLowへの確定時だけ`_fell`を立てる
- `fell`は読み出すとフラグを消すワンショットイベント
- `update()`をメインループから呼ばないと、入力の観測も状態確定も進まない

## 呼び出し経路

```text
GP0の電圧
  -> Debouncer.update()（デバウンスありの場合）
  -> switch.fell または前回値との比較
  -> Keyboard.send(Keycode.A)
  -> press()
  -> _add_key()
  -> _sync()
  -> send_report()
  -> release_all()
  -> 空のレポート
```

## 変更時の確認

1. GP0を入力にし、プルアップを有効にしたか。
2. スイッチを押したときに`pin.value`が`False`になるか。
3. デバウンスあり版では、ループごとに`switch.update()`を呼んでいるか。
4. `fell`を同じイベントについて複数回読んでいないか。
5. `send()`が空の解放レポートまで送っているか。
6. HIDデバイスのレポート形式と`_sync()`の配列配置が一致しているか。
