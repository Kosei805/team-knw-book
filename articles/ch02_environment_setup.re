= 環境構築

== 必要なパーツを揃えよう

キーボードを自作すると聞くと、何をどれだけ揃えればいいのか分からず、身構えてしまうかもしれません。
実際に必要な部品は多くなく、電子工作の経験がなくても入手できるものばかりです。
まずは部品と購入先の一覧です。

//image[required-parts][今回使用する電子部品一覧]

#@# TODO: 各パーツについて、この商品・型番を選んだ論理的な理由（他の選択肢との違いなど）を書く
 * Raspberry Pi Pico H × 1個
 ** 購入先： Switch Science （https://www.switch-science.com/products/8170）
 * ブレッドボード × 2枚
 ** 購入先： Amazon （https://amzn.asia/d/01QLKXpi）
 * タクトスイッチ × 4個
 ** 購入先：秋月電子通商（https://akizukidenshi.com/catalog/g/g103647/）
 * スイッチングダイオード（1N4148）× 4本（予備含め50本パックがおすすめ）
 ** 購入先：秋月電子通商（https://akizukidenshi.com/catalog/g/g100941/）
 * ジャンパーワイヤー（オス-オス）× 最低8本（本書内では以下の120本混合パックを使用）
 ** 購入先： Amazon （https://amzn.asia/d/0gWP8NfT）

ダイオードとジャンパーワイヤーは、必要な本数ぴったりより多めのパックがおすすめです。
はんだ付けの失敗や紛失で、作業中に本数が減ることがあるためです。

作者がブレッドボードとジャンパーワイヤーを Amazon で購入したのは、翌日配達を優先したためです。
急いでいない場合は、他の購入先も検討してみてください。

== Pico H に CircuitPython をインストールする

部品を揃えても、 Pico H はまだただのマイコン基板です。
動かすには、ファームウェアの書き込みが要ります。
本書で使うのは CircuitPython というファームウェアです。
CircuitPython を採用した理由は単純で、後の章で Pico H を PC にキーボードとして認識させるために使う adafruit_hid ライブラリが CircuitPython 向けに提供されているからです。
ここからは、 Pico H に CircuitPython 10.x 系を書き込む手順です。

Pico H の基板上には、白い BOOTSEL ボタンがあります。
このボタンを押しながら USB ケーブルで PC に接続すると、「RPI-RP2」という名前のドライブとして表示されます。
ファームウェアを書き込むための特別なモード、いわゆるブートローダーモードに入った状態です。

circuitpython.org のダウンロードページ（https://circuitpython.org/downloads）を開き、ボード一覧から「Raspberry Pi Pico / Pico H」を選びます。
言語を選ぶ画面が出るので、任意の言語（未選択のままでも ENGLISH (US) になります）を選びます。
STABLE（安定版）の 10.x 系のバージョンにある「DOWNLOAD .UF2 NOW」ボタンをクリックすると、 .uf2 ファイルがダウンロードされます。

 * BOOTSEL ボタンを押しながら、 USB ケーブルで Pico H を PC に接続する
 * 「RPI-RP2」ドライブが表示されることを確認する
 * ダウンロードした 10.x 系の .uf2 ファイルを「RPI-RP2」ドライブにコピーする

//image[drive-rpi-rp2][PC 上に「RPI-RP2」ドライブがマウントされた画面]

.uf2 ファイルをコピーすると、 Pico H は自動的に再起動します。
再起動後は「RPI-RP2」ドライブが消え、代わりに「CIRCUITPY」というドライブに変わります。
ドライブ名が「CIRCUITPY」になっていれば、書き込みは完了です。

//image[drive-circuitpy][PC 上に「CIRCUITPY」ドライブがマウントされた画面]

USB ケーブルの中には、充電専用でデータ通信に対応していないものがあります。
このケーブルだと、 BOOTSEL ボタンを押しながら接続しても「RPI-RP2」ドライブが表示されません。
ドライブが表示されないときは、まずケーブルをデータ通信対応のものに替えてみてください。

== 必要なライブラリをコピーする

CircuitPython を書き込んだだけでは、 Pico H はまだキーボードとして動く準備ができていません。
adafruit_hid のような、ハードウェア固有の機能を扱うライブラリは標準では含まれておらず、自分で追加する必要があります。
ここでは、キーボードを作るのに必要なライブラリを CIRCUITPY ドライブにコピーする手順を説明します。

追加するライブラリは次の3つです。

 * adafruit_hid （ディレクトリ）：Pico H を USB 経由でキーボードとして PC に認識させ、キー入力を送るためのライブラリ
 * adafruit_debouncer.mpy：タクトスイッチのチャタリング（一瞬のオン・オフの揺れ）を吸収するためのライブラリ
 * adafruit_ticks.mpy：adafruit_debouncer が内部で使う、時間を計るためのライブラリ

ライブラリは、 circuitpython.org のライブラリバンドル配布ページ（https://circuitpython.org/libraries）からまとめて入手します。
バンドルは CircuitPython のバージョンごとに分かれているため、前節でインストールした 10.x 系に対応するバンドルをダウンロードしてください。
バージョンが一致していないと、ライブラリが正しく読み込めないことがあります。
ダウンロードする zip ファイルの名前は「adafruit-circuitpython-bundle-10.x-mpy-<日付>.zip」のように、末尾の日付部分がリリースのたびに変わります。
展開すると、同じ名前のディレクトリの中に lib ディレクトリがあり、その中に数百個のライブラリが並んでいます。
この中から、必要な3つだけを探して使います。

 * circuitpython.org のライブラリバンドル配布ページを開く
 * 10.x 系に対応したバンドルの zip ファイルをダウンロードする
 * ダウンロードした zip ファイルを展開する
 * 展開したディレクトリの中の lib ディレクトリから、 adafruit_hid ディレクトリ、 adafruit_debouncer.mpy 、 adafruit_ticks.mpy を探す
 * CIRCUITPY ドライブの直下に lib ディレクトリがなければ作成する
 * 探した3つのファイル（ディレクトリ）を CIRCUITPY ドライブの lib ディレクトリにコピーする

コピーが終わると、 lib ディレクトリの中身は次のようになります。

//list[lib_folder_structure][CIRCUITPY/lib ディレクトリの中身]{
lib
├── adafruit_debouncer.mpy
├── adafruit_hid
│   ├── __init__.mpy
│   ├── consumer_control_code.mpy
│   ├── consumer_control.mpy
│   ├── keyboard_layout_base.mpy
│   ├── keyboard_layout_us.mpy
│   ├── keyboard.mpy
│   ├── keycode.mpy
│   └── mouse.mpy
└── adafruit_ticks.mpy
//}

このような構成になっていれば、ライブラリのコピーは完了です。

== 開発エディタ「Thonny」を準備する

ライブラリまでそろえても、コードを書いて Pico H に送る道具がまだありません。
本書では、そのためのエディタに Thonny を使います。
Thonny は Python 専用のエディタで、 CircuitPython を書き込んだボードをシリアル接続で認識する機能を標準で備えています。

 * Thonny の公式サイト（https://thonny.org/）からインストーラーをダウンロードする
 * インストーラーの手順に従ってインストールする
 * USB ケーブルで Pico H と PC を接続する
 * Thonny を起動し、右下のインタープリタ選択メニューから「CircuitPython (generic)」を選ぶ

//image[thonny-interpreter][Thonny の画面右下のインタープリタ選択メニュー]

インタープリタを選ぶと、画面下部の Shell エリアに「Adafruit CircuitPython ...」という表示が出ます。
この表示が出ていれば、 Thonny と Pico H の接続は完了です。
