# 第2章「環境構築」執筆仕様書（Re:VIEW形式特化・修正版）

## 1. 執筆ルール
*   出力ファイル形式は、Re:VIEW形式（拡張子「.re」）であること。Markdown（.md）で書いてはならない。
*   コラムやポエム的要素は「一切不要」。最速で動かす手順のみを簡潔に書くこと。
*   以下の「Re:VIEW基本文法」を厳密に遵守して執筆すること。

## 2. Re:VIEW（.re）の基本文法ルール（AI遵守）
1. 見出し:
   = 第2章：環境構築
   == 2.1：中見出し
2. 箇条書き: アスタリスクの前に必ず半角スペースを1つ入れること。
    * パーツ1
3. 画像の挿入（※画像ファイルは後から articles/images/ フォルダに指定ID名で配置するプレースホルダー方式）:
   //image[画像ファイル名_拡張子なし][図のキャプション]
4. コードブロック:
   //list[リストID][キャプション]{
   （コードの中身）
   //}
5. インライン等幅（ファイル名等）:
   @<tt>{code.py}

## 3. 各セクションの構成と執筆指示

### 2.1：必要なパーツを揃えよう
*   内容: 本書で使用する電子部品の一覧（BOM）と購入先リンク。
*   配置する画像:
    //image[pico_parts][今回使用する電子部品一覧]
*   部品リスト（※アスタリスクの前の半角スペースを厳守すること）:
     * Raspberry Pi Pico H（ピンヘッダ実装済み）×1基
       * 購入先例：秋月電子通商（https://akizukidenshi.com/catalog/g/g117865/）
     * ブレッドボード（ハーフサイズ、1本2レールタイプ）×2枚
       * 購入先例：秋月電子通商（https://akizukidenshi.com/catalog/g/g115995/）
     * タクトスイッチ（1マス空き 4本足タイプ）×4個
       * 購入先例：秋月電子通商（https://akizukidenshi.com/catalog/g/g116480/）
     * スイッチング・ダイオード（1N4148、DO-35パッケージ、アキシャル・リード線タイプ）×4本（予備含め50本パックがおすすめ）
       * 購入先例：秋月電子通商 50本パック（https://akizukidenshi.com/catalog/g/g115982/）
     * ジャンパーワイヤ（オス-オス）×最低8本（本書内では以下の120本混合パックを使用）
       * 購入先リンク：Amazon（https://amzn.asia/d/0i3oEqtV）

### 2.2：Pico HにCircuitPythonをインストールする
*   内容: BOOTSELボタンを押しながら接続し、.uf2ファイルを書き込む手順。
*   配置する画像:
    //image[bootsel_button][Pico H基板上の白いBOOTSELボタンの場所]
    //image[drive_rpi_rp2][PC上に「RPI-RP2」ドライブがマウントされた画面]
    //image[drive_circuitpy][PC上に「CIRCUITPY」ドライブがマウントされた画面]
*   記述内容: 
    *   データ通信非対応の「充電専用USBケーブル」を使用するとドライブが認識されないトラブルへの警告。
    *   .uf2ファイルは書き込み後に自動消滅し、ドライブ名が CIRCUITPY に変わっていれば正常に完了しているという解説。

### 2.3：必要なライブラリを手動コピーする
*   内容: CIRCUITPY/lib フォルダに必要なライブラリを配置する手順。
*   ルール: 必ずファームウェアのバージョンと一致する「10.x用」のライブラリバンドルを使用すること。
*   配置するファイル: `adafruit_hid` (フォルダ)、`adafruit_debouncer.mpy`、`adafruit_ticks.mpy`
*   配置する画像:
    //image[lib_folder_structure][CIRCUITPY/lib フォルダの中身のFinder画面]

### 2.4：開発エディタ「Thonny」を準備する
*   内容: Thonnyをダウンロードし、Pico Hとシリアル接続（設定）する手順。
*   設定手順:
    1. Thonny右下のインタープリタ選択から「CircuitPython (generic)」を選択する。
    2. 下部のShellエリアに「Adafruit CircuitPython...」と表示されれば接続成功。
*   配置する画像:
    //image[thonny_interpreter][Thonnyの画面右下のインタープリタ選択メニュー]
