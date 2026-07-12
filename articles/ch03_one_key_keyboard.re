= キー1個のキーボードを作る

スイッチを押したという事実は、そのままではパソコンのキー入力になりません。スイッチの動きを電気の変化に変え、その変化をプログラムで読み取り、パソコンが理解できるデータへ順番に変換する必要があります。

この章では、最初から完成版のプログラムを読むことはしません。各段階で「いま手元にあるデータでは、次の処理に足りないものは何か」を考え、その不足を埋めるコードを追加します。最後に、それぞれの段階で作ったコードをつなぎ、実行ファイルとして保存します。

作るものは、Raspberry Pi Pico H、ブレッドボード、タクトスイッチ1個で構成したキーボードです。ここでいうキーボードは、パソコンへキー入力を送る装置を指します。Raspberry Pi Pico Hは、プログラムを書き込んで回路を動かす小さなコンピューター基板です。ブレッドボードは、部品やワイヤーを穴へ差し込んで、はんだ付けなしで回路を組むための板です。タクトスイッチは、指で押している間だけ内部の電気の通り道がつながる部品です。Pico Hとスイッチをワイヤーでつなぎ、押すたびにパソコンへAキーを1回送ります。

== まず回路を作る

=== 回路の目的を決める

今回の回路で知りたいのは、スイッチが押されているかどうかだけです。回路とは、電気の通り道をつないだものです。電圧は、電気を流そうとする力の大きさです。Pico Hには、電圧を読み取る端子がいくつもあります。端子は、基板から出ている金属部分で、ワイヤーをつなぐ場所です。GPIO（General Purpose Input/Output）は、プログラムから電圧を読み取ったり、電圧を出力したりできる端子です。そのうち今回は、GPIO 0を表す@<tt>{GP0}を使います。

もう一つ使う@<tt>{GND}は、回路の電圧を測る基準になる端子です。ここでは0V側として使います。GP0とGNDの間をスイッチでつなぐと、スイッチを押したときだけGP0の電圧がGND側へ変わります。

スイッチを押していないときのGP0はHigh、押したときのGP0はLowにします。Highは端子の電圧が高い状態、Lowは低い状態を表す呼び方です。押していないときのHighは、Pico Hの内部プルアップで作ります。プルアップは、スイッチを離して入力先がどこにもつながっていないときでも、入力をHigh側へ保つ設定です。この回路にすると、外付け抵抗を使わずに、スイッチを押したときだけGP0をGNDへ引き下げられます。

回路とプログラム上の値は、次のように対応します。

//table[pin-state][回路の状態とプログラムの値]{
スイッチ	GP0の電気的な状態	CircuitPythonで読む値
離している	High	True
押している	Low	False
//}

押下をHighで表す回路も作れますが、今回はLowへの変化を押下として扱います。以降のコードで@<tt>{False}や@<tt>{not pin.value}が出てきたら、この表へ戻って確認してください。

=== 部品を置く

//image[one_switch][タクトスイッチ1個を使った回路][scale=0.8]{
//}

ジャンパーワイヤーは、ブレッドボードの穴同士や、Pico Hと部品をつなぐ短い電線です。ブレッドボードの中央には、縦方向の溝があります。溝の左右にある5個ずつの穴は、列番号ごとに内部でつながっています。溝をまたぐと、左側と右側の穴はつながりません。この違いを使って、タクトスイッチの端子を左右の回路へ分けます。

まず、Pico Hをブレッドボード中央の溝をまたぐように、横向きに置きます。USBは、Pico Hとパソコンをつないで電源やデータをやり取りするための規格です。USBコネクタが左を向く向きにしてください。左右のピン列が溝の左右に分かれ、Pico Hを押し込んだときに一方のピン列だけが同じ接続列へ入らないことを確認します。

次に、タクトスイッチを中央の溝にまたがせます。4本の端子を、左側の@<tt>{e23}、@<tt>{e24}、右側の@<tt>{f23}、@<tt>{f24}へ入れてください。スイッチの端子が23行と24行に並び、溝の左右に2本ずつ分かれる向きです。タクトスイッチは、同じ側にある2本の端子が内部でつながっています。したがって、@<tt>{e23}と@<tt>{e24}を別々の信号線だと思わないでください。今回使うのは、スイッチの左右から1本ずつです。

4端子のタクトスイッチは、向かい合う2端子がそれぞれ内部でつながっています。片側の2端子を別々の信号線だと思ってはいけません。GP0へつなぐ端子とGNDへつなぐ端子を、スイッチの反対側の組から1本ずつ選びます。

Pico Hのシルク印刷（基板表面に印刷された端子名）で@<tt>{GP0}と@<tt>{GND}の位置を確認します。図の向き（USBコネクタが左）では、Pico H側の@<tt>{a1}が@<tt>{GP0}、@<tt>{a3}が@<tt>{GND}です。基板の向きを変えた場合は穴番号をそのまま使わず、必ず端子名を見てください。赤いジャンパーワイヤーは、Pico H側の@<tt>{a1}とスイッチ右側の@<tt>{h23}をつなぎます。@<tt>{h23}は、タクトスイッチの右側にある@<tt>{f23}と同じ接続列です。青いジャンパーワイヤーは、Pico H側の@<tt>{a3}とスイッチ左側の@<tt>{c24}をつなぎます。@<tt>{c24}は、タクトスイッチの左側にある@<tt>{e24}と同じ接続列です。

配線を穴番号で書くと、次の2本です。

 * 赤ワイヤー: @<tt>{a1} ↔ @<tt>{h23}
 * 青ワイヤー: @<tt>{a3} ↔ @<tt>{c24}

今回の回路では電源レール（電源を配るためのブレッドボード上の列）を使いません。GP0とGNDを直接スイッチへ接続するため、電源レールの途中にある切れ目を気にせずに済みます。GP0のワイヤーとGNDのワイヤーを同じ接続列へ入れないこと、5VやVSYS（電源用の端子）をスイッチへ入れないことを確認してください。

電源を入れる前に、テスター（電圧や電気の通り道を調べる測定器）で導通を確認します。導通とは、2点の間に電気が通る状態です。スイッチを離しているときはGP0とGNDが導通せず、押しているときだけ導通すれば、今回必要な回路になっています。

== GP0の電圧を読む

回路ができても、Pico HのGP0はまだプログラムから読む設定になっていません。入力は外部から来た電圧をプログラムで読む使い方、出力はプログラムから電圧を決める使い方です。今回は外部のスイッチが作った電圧を読むので、GP0を入力にします。

また、スイッチを離したときにGP0をHighへ戻す基準が必要です。入力ピンをどこにも接続しないと、周囲のノイズによって値が変わる浮いた入力になります。ノイズとは、本来の操作ではない短い電気的な変化です。入力がこの影響を受けて値を決められない状態を「浮いた入力」と呼びます。そこで内部プルアップを有効にします。

この段階で必要なのは、GP0の電圧をプログラムから読むことです。ここではCircuitPythonという、Pico Hで動くPythonの一種を使います。次のコードを入力確認用のプログラムにします。

//emlist[][python]{
pin = digitalio.DigitalInOut(board.GP0)
pin.direction = digitalio.Direction.INPUT
pin.pull = digitalio.Pull.UP
//}

1行目で、読む対象をGP0に固定します。2行目で、そのピンを入力にします。3行目で、スイッチを離しているときの基準をHighにします。

このコードを実行すると、@<tt>{pin.value}にはGP0の現在の電圧が入ります。スイッチを押している間は@<tt>{pin.value}が@<tt>{False}、離している間は@<tt>{True}です。ここで得られるデータは「押された瞬間」ではなく「現在の電圧」です。次の処理では、まずこのデータが回路と一致しているかを確認します。

=== GPIOの値を表示する

すでに説明した初期化に、表示処理だけを追加します。@<tt>{time.sleep()}は表示を読みやすくするために使います。キーボード入力を作る段階では、この待ち時間は使いません。

//emlist[][python]{
while True:
    print("押している" if not pin.value else "押していない")
    time.sleep(0.2)
//}

押していないときに「押していない」、押したときに「押している」と表示されれば、回路からプログラムへの最初の変換は成功です。押していないのに@<tt>{False}になるなら、GP0とGNDの短絡、タクトスイッチの向き、GP0の取り違えを確認します。値が不規則に変わるなら、プルアップ設定か接続列を確認します。

== 押した瞬間を見つける

次のコードのように、@<tt>{pin.value}が@<tt>{False}なら処理する、と書くと、スイッチを押している間ずっと処理されます。

//emlist[][python]{
while True:
    if not pin.value:
        print("Aキーを送信")
//}

このコードが扱っているのは、状態です。状態とは、その瞬間の「押している」「離している」を表す値です。必要なのは、HighからLowへ変わった瞬間というイベントです。イベントとは、状態が変わった瞬間に一度だけ起きる出来事です。現在値だけからは、いま押されたのか、すでに1秒前から押されているのかを区別できません。

イベントを作るには、現在値だけでなく、直前に読んだ値を保存します。2つの値を並べると、変化の方向を判定できます。

//table[state-transition][前回値と現在値の組み合わせ]{
前回値	現在値	意味
True	True	離したまま
True	False	押した瞬間
False	False	押したまま
False	True	離した瞬間
//}

今回の押下イベントは、@<tt>{True}から@<tt>{False}への変化です。したがって条件は@<tt>{last_state and not current_state}になります。

=== 前回値と現在値から押下を見つける

まず、現在値を保存する場所を作ります。起動直後のGP0の状態を前回値として使うため、ループの前に読みます。

//emlist[][python]{
last_state = pin.value
//}

次に、ループの1周ごとに現在値を読みます。前回がHighで現在がLowのときだけ、押下イベントとして扱います。

//emlist[][python]{
while True:
    current_state = pin.value

    if last_state and not current_state:
        print("Aキーを送信")

    last_state = current_state
//}

最後の@<tt>{last_state = current_state}が重要です。これがないと、前回値は起動時の@<tt>{True}のままです。押している間の各周回で@<tt>{True}と@<tt>{False}を比較し続けるため、Aを何度も送ることになります。前回値を更新したことで、押した直後の次の周回では@<tt>{False}と@<tt>{False}になり、イベントは1回で止まります。

ここまでで、GP0の状態から押下イベントを作れました。ただし、機械式スイッチは押した瞬間に一度だけ値が変わるとは限りません。

== 不安定な押下を1回にまとめる

まず、いったん実機でキー入力を確認します。後の「完成版を保存する」節にあるプログラムと、同じ節で説明する3つの補助ファイルをPico Hへコピーして実行してください。CircuitPythonのドライブが@<tt>{CIRCUITPY}として表示されたら、完成版を@<tt>{code.py}という名前でその直下へ置き、@<tt>{debouncer.py}、@<tt>{keyboard.py}、@<tt>{keycode.py}も同じ場所へ置きます。テキストエディターを開き、スイッチを1回だけ押します。Aが1文字入力されれば、回路の状態が押下イベントになり、キーボードとしてパソコンに認識されています。

次に、スイッチを10回押してみます。押した回数と、エディターに入力されたAの数が合わないことがあります。1回の操作なのに2文字以上入力されるなら、ここで初めて「接点の揺れ」を疑います。

タクトスイッチの中には、電気をつないだり切ったりする金属部分があります。この金属部分を接点と呼びます。接点は押した瞬間にぴたりと止まらず、短い時間だけ接触と離脱を繰り返します。1回押したのに、この揺れが複数回の入力として現れる現象をチャタリングと呼びます。チャタリングというのは、スイッチを押した回数と、プログラムが押されたと判断する回数が合わなくなる現象です。GP0からは、次のような値が見えることがあります。

//emlist[]{
True, False, True, False, True, False, False, False, ...
//}

前節のコードは、@<tt>{True}から@<tt>{False}へ変わるたびにイベントを作ります。そのため、1回の物理操作から複数の押下イベントが生まれます。前回値を保存する処理は「押している間の繰り返し」を防ぎますが、「接点の揺れ」、つまりチャタリングは防ぎません。

必要なのは、GP0から読んだ直後の値をすぐに押下イベントへ変換しないことです。この読み取った直後の値を生値と呼びます。一方、一定時間変わらないことを確認して採用した値を確定値と呼びます。次の3つのデータを持ちます。

 * @<tt>{last_raw_state}: 直前に読んだ生の値
 * @<tt>{current_stable_state}: 値が落ち着いたあとに確定した値
 * @<tt>{last_changed_time}: 生の値が最後に変化した時刻

生値が変わったら、確定を保留します。その後、20ミリ秒以上同じ値が続いたら確定値を更新します。ミリ秒は1秒の1000分の1です。確定値がHighからLowへ変わった場合にだけ、押下イベントを作ります。このように、入力値が落ち着くまで判定を待つ処理をデバウンスと呼びます。デバウンスは、チャタリングを1回の押下として扱うための処理です。

=== 20ミリ秒を時間軸で追う

デバウンスの動きを、スイッチを押した直後から追ってみます。時刻は説明のための例です。実際のループは、Pico Hが処理できる速さで何度も回ります。

//table[debounce-timeline][押下時の値とデバウンス処理]{
時刻	GP0の生値	デバウンス処理
0ミリ秒	True	まだ離している
1ミリ秒	False	変化を見つけ、タイマーを開始する
5ミリ秒	True	値が戻ったので、タイマーをやり直す
8ミリ秒	False	もう一度変化したので、タイマーをやり直す
28ミリ秒	False	20ミリ秒変わらなかったため、押下を確定する
//}

1ミリ秒から8ミリ秒までの変化は、スイッチの接点が落ち着くまでの揺れです。8ミリ秒のFalseを最後の変化とすると、28ミリ秒になって初めて20ミリ秒が経過します。途中のFalseを押下として送らないので、1回の操作が複数の入力になりません。

この時間を短くしすぎると、まだ揺れている途中で押下を確定する可能性があります。長くしすぎると、押してから入力されるまでの遅れが大きくなります。20ミリ秒は今回のタクトスイッチを試すための設定で、すべてのスイッチにとって唯一の正解ではありません。まず20ミリ秒で動かし、入力が二重になるときや反応が遅いときに値を調整します。

=== 値が変わってからの時間を測る

デバウンス時間をミリ秒で指定できるようにし、内部では秒に変換します。CircuitPythonの@<tt>{time.monotonic()}で、経過時間を測ります。

//emlist[][python]{
debounce_duration = debounce_ms / 1000.0
last_changed_time = time.monotonic()
//}

ここで必要なのは、時計の現在時刻を表示することではありません。生値が変わってから何秒経ったかです。そのため、変化したときに@<tt>{last_changed_time}を更新し、現在時刻との差を計算します。

=== 生値の変化を保留する

生値が前回値と異なるとき、すぐに確定せず、タイマーをリセットします。

//emlist[][python]{
current_raw_state = self.pin.value
current_time = time.monotonic()

if current_raw_state != self.last_raw_state:
    self.last_changed_time = current_time
    self.last_raw_state = current_raw_state
//}

チャタリングで値が変わるたびにタイマーがリセットされます。High、Low、Highと短時間に戻った場合、Lowは確定値になりません。最後の変化から20ミリ秒が経過するまで、押下イベントは作られません。

=== 安定した値だけを確定する

最後の変化から十分な時間が経ったら、生値と確定値を比較します。異なっていれば、新しい状態として確定します。

//emlist[][python]{
time_since_last_change = current_time - self.last_changed_time

if time_since_last_change >= self.debounce_duration:
    if current_raw_state != self.current_stable_state:
        if self.current_stable_state and not current_raw_state:
            self._fell = True
        self.current_stable_state = current_raw_state
//}

@<tt>{self.current_stable_state}が@<tt>{True}で、@<tt>{current_raw_state}が@<tt>{False}なら、確定した押下です。この条件でだけ@<tt>{_fell}を立てます。LowからHighへ戻ったときは、状態を更新しますが、押下イベントは作りません。

=== 押下イベントを1回だけ読む

@<tt>{_fell}をループから直接読むと、フラグを消す責任が呼び出し側へ漏れます。そこで、押下イベントを読む処理を1か所にまとめます。

//emlist[][python]{
@property
def fell(self):
    if self._fell:
        self._fell = False
        return True
    return False
//}

@<tt>{fell}を読むと、TrueだったフラグがFalseへ戻ります。同じイベントを次のループで再処理しないためです。イベントをデバッグ表示するときも、@<tt>{print(switch.fell)}と条件判定の両方で読まないようにします。イベントは読んだ場所で消費されます。

=== ここまでを@<tt>{Debouncer}にまとめる

ここまでの処理を、入力ピンを受け取り、@<tt>{update()}で観測し、@<tt>{fell}で押下を返すオブジェクトにまとめたものが@<tt>{codes/lib/debouncer.py}です。ファイル全体は次の構成です。

//list[debouncer][デバウンス処理][python]{
import time

class Debouncer:
    def __init__(self, pin, debounce_ms=20):
        self.pin = pin
        self.debounce_duration = debounce_ms / 1000.0
        self.current_stable_state = self.pin.value
        self.last_raw_state = self.current_stable_state
        self.last_changed_time = time.monotonic()
        self._fell = False

    @property
    def fell(self):
        if self._fell:
            self._fell = False
            return True
        return False

    def update(self):
        current_raw_state = self.pin.value
        current_time = time.monotonic()

        if current_raw_state != self.last_raw_state:
            self.last_changed_time = current_time
            self.last_raw_state = current_raw_state

        time_since_last_change = current_time - self.last_changed_time
        if time_since_last_change >= self.debounce_duration:
            if current_raw_state != self.current_stable_state:
                if self.current_stable_state and not current_raw_state:
                    self._fell = True
                self.current_stable_state = current_raw_state
//}

呼び出し側が必要とするデータは、生値ではなく@<tt>{fell}です。したがって、メインループでは毎周回@<tt>{update()}を呼び、押下イベントがあるときだけ@<tt>{fell}を読みます。

== Aキーを表す番号にする

ここまでで、プログラムは「押された」というイベントを持てるようになりました。しかし、イベントの値は真偽値であり、USBキーボードが必要とするAキーの情報ではありません。

文字列の@<tt>{"A"}を送ればよいように見えますが、キーボードは文字列を送る装置ではありません。Shiftは、他のキーと一緒に押して大文字などを入力するキーです。Caps Lockは、大文字入力の状態を切り替えるキーです。キーボード配列は、キーの位置と入力される文字の対応関係です。これらの状態によって、同じキーでも結果が変わるからです。キーボードが報告するのは、どのキーが押されたかです。

HIDは、キーボードやマウスのような入力機器とOS（パソコンの基本ソフト）が通信するための規格です。HIDでは、文字そのものではなく、どのキーを押したかをキーコードという番号で知らせます。@<tt>{codes/lib/keycode.py}で、Aキーを@<tt>{0x04}というキーコードに対応付けます。

//emlist[][python]{
class Keycode:
    A = 0x04
//}

ここで@<tt>{0x04}は文字コードではありません。Aキーという物理的なキーの種類を表す番号です。OSはこの番号とShiftやCaps Lockの状態から、画面へ入力する文字を決めます。

この変換で、押下イベントの出力は@<tt>{Keycode.A}になりました。次に必要なのは、このキーコードを「押された状態」としてUSBへ渡すことです。

== USBへキー入力を知らせる

Aキーを1回入力するには、Aの番号を一度送れば終わり、ではありません。キーコードを含むデータを、キーボードの現在の状態としてOSへ知らせる必要があります。このデータをHIDレポートと呼びます。レポートには、どのキーが押されているかを入れます。押された状態だけを送ると、OSはキーがまだ押されていると考え、同じ文字を自動的に繰り返すキーリピートが起きます。

 1. Aが押されている
 2. 何も押されていない

最初のレポートだけで止めると、OSはAを押したままだと判断します。キーリピートが始まり、次の入力へ状態が残ります。直前に何も押されていない状態からAを送る場合でも、必要なのはこの「Aを含むレポート」と「空のレポート」の組み合わせです。

キーコードを受け取ったら、まず現在押しているキーの集合へ追加します。ShiftやControlのように、他のキーと組み合わせて意味を変えるキーを修飾キーと呼びます。修飾キーは別のビット列で持ちます。ビット列は、0と1を並べて複数の状態をまとめて表すデータです。Aのような通常キーは最大6個の欄へ入れます。その状態から8バイトのレポートを作り、HIDデバイスへ送ります。HIDデバイスは、ここではUSB経由でパソコンにつながるキーボードとして動くPico Hを指します。

この設計で必要になるデータは次の3つです。

 * @<tt>{_modifiers}: ShiftやControlのビット列
 * @<tt>{_keys}: 押している通常キーのキーコード
 * @<tt>{_report}: USBへ渡す8バイトの配列

8バイトという長さは、すべてのHIDデバイスに共通するものではありません。今回のHIDキーボードデバイスが、修飾キー1バイト、予約領域（今回は使わず空けておく欄）1バイト、通常キー6バイトのレポートを使うためです。

=== HIDキーボードを選ぶ

@<tt>{usb_hid.devices}の中には、キーボード以外のHIDデバイスが含まれる場合があります。最初に、キーボードを表すデバイスだけを選びます。

//emlist[][python]{
for device in devices:
    if getattr(device, "usage_page", None) == 0x01 and \\
       getattr(device, "usage", None) == 0x06:
        self._device = device
        break

if self._device is None:
    raise ValueError("USB HID keyboard device was not found")
//}

@<tt>{usage_page == 0x01}と@<tt>{usage == 0x06}が、今回選ぶキーボードの識別条件です。見つからない状態で処理を続けると、後の送信で原因が分かりにくくなります。そのため、初期化時点でエラーにします。

=== 押しているキーを状態へ追加する

修飾キーは、キーコードを通常キーの配列へ入れず、対応するビットを立てます。Aのような通常キーは、まだ押していなければ@<tt>{_keys}へ追加します。

//emlist[][python]{
_MODIFIER_BITS = {
    0xE0: 0x01,
    0xE1: 0x02,
    0xE2: 0x04,
    0xE3: 0x08,
    0xE4: 0x10,
    0xE5: 0x20,
    0xE6: 0x40,
    0xE7: 0x80,
}

def _add_key(self, keycode):
    if keycode in self._MODIFIER_BITS:
        self._modifiers |= self._MODIFIER_BITS[keycode]
        return

    if keycode not in self._keys:
        if len(self._keys) >= 6:
            raise ValueError("Keyboard report can hold at most 6 simultaneous keys")
        self._keys.append(keycode)
//}

@<tt>{Keycode.A}の@<tt>{0x04}は@<tt>{_MODIFIER_BITS}に含まれないため、通常キーとして@<tt>{_keys}へ入ります。@<tt>{Keycode.SHIFT}の@<tt>{0xE1}は辞書に含まれるため、修飾キーのビットだけが変わります。

=== 状態を8バイトへ写す

内部状態を、デバイスが期待するレポートへ変換します。

//emlist[][python]{
def _sync(self):
    self._report[0] = self._modifiers
    self._report[1] = 0
    for index in range(6):
        self._report[2 + index] = \\
            self._keys[index] if index < len(self._keys) else 0
    self._device.send_report(self._report)
//}

Aだけを押しているときのレポートは、@<tt>{[0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00]}です。先頭は修飾キー、2番目は予約領域、その後ろに通常キーを並べます。空いている場所は0で埋めます。

@<tt>{send_report()}は、配列をUSB HIDデバイスへ渡します。ここでも文字を作っていません。OSがこのレポートを前回のレポートと比較し、Aが押された状態になったと解釈します。

=== 押下と解放を一つの操作にする

レポートを2回送る処理を呼び出し側へ毎回書くと、解放漏れが起きます。そこで、押下状態を送る@<tt>{press()}と、すべて空にした状態を送る@<tt>{release_all()}を、1回の入力としてまとめます。

//emlist[][python]{
def press(self, *keycodes):
    for keycode in keycodes:
        self._add_key(keycode)
    self._sync()

def release_all(self):
    self._modifiers = 0
    self._keys.clear()
    self._sync()

def send(self, *keycodes):
    self.press(*keycodes)
    self.release_all()
//}

@<tt>{send(Keycode.A)}は、Aを含むレポートを送ったあと、すべて0のレポートを送ります。これでOSから見た状態は「Aが押された」「Aが離された」になります。長押しや複数キーの保持を作る場合は、@<tt>{send()}ではなく@<tt>{press()}と@<tt>{release()}を組み合わせます。

=== ここまでを@<tt>{Keyboard}にまとめる

ここまでのHIDデバイス選択、キー状態の保持、レポート生成、押下と解放をまとめたものが@<tt>{codes/lib/keyboard.py}です。@<tt>{keycode.py}はキーの番号を名前へ変換し、@<tt>{debouncer.py}は物理入力を押下イベントへ変換します。3つのファイルは、次のデータ変換を分担しています。

//table[code-responsibility][ファイルごとの役割]{
ファイル	入力	出力
debouncer.py	GP0の生値	押下イベント fell
keycode.py	キー名	HIDキーコード
keyboard.py	HIDキーコード	USB HIDレポート
//}

各ファイルを独立させたことで、デバウンス時間、キーの割り当て、HIDレポートの組み立てを別々に変更できます。呼び出し側は、入力イベントを得たらキーコードを選び、@<tt>{send()}へ渡すだけになります。

== すべての変換をつなぐ

=== 完成前の呼び出し側を作る

必要なデータ変換はそろいました。メインプログラムでは、GP0を入力にし、そのピンを@<tt>{Debouncer}へ渡し、押下イベントを@<tt>{Keyboard.send()}へつなぎます。

//emlist[][python]{
kbd = Keyboard(usb_hid.devices)

pin = digitalio.DigitalInOut(board.GP0)
pin.direction = digitalio.Direction.INPUT
pin.pull = digitalio.Pull.UP

switch = Debouncer(pin)

while True:
    switch.update()
    if switch.fell:
        kbd.send(Keycode.A)
//}

この短いループで、データは次のように変わります。

//emlist[]{
GP0のHigh/Low
  -> Debouncer.update()で生値を観測
  -> 20ミリ秒安定した押下イベント
  -> Keycode.A = 0x04
  -> Keyboard.send()
  -> Aを含むレポート
  -> 空のレポート
//}

@<tt>{switch.update()}を呼ばないと、デバウンサーの内部データが更新されません。@<tt>{switch.fell}は押下イベントを読む処理であり、押している間のLowそのものではありません。@<tt>{kbd.send()}はキーコードをUSBレポートへ変換し、解放まで行います。

=== 完成版を保存する

ここまでの処理を、次のファイルへ保存します。

@<tt>{codes/onekey_with_debounce.py}

コードのimportは完成版に一度だけ掲載します。上の断片で省略したimportも含め、実際にPico Hへコピーするファイルは次の内容です。

//list[onekey_with_debounce][デバウンスありの1キーキーボード][python]{
import board
import digitalio
import usb_hid

from debouncer import Debouncer
from keyboard import Keyboard
from keycode import Keycode

kbd = Keyboard(usb_hid.devices)

pin = digitalio.DigitalInOut(board.GP0)
pin.direction = digitalio.Direction.INPUT
pin.pull = digitalio.Pull.UP

switch = Debouncer(pin)

print("1キーボード 待機中...")

while True:
    switch.update()
    if switch.fell:
        print("Aキーを送信！")
        kbd.send(Keycode.A)
//}

@<tt>{debouncer.py}、@<tt>{keyboard.py}、@<tt>{keycode.py}は、実行時に読み込める場所へ置きます。リポジトリ内では@<tt>{codes/lib}に整理していますが、CircuitPythonへコピーするときは、完成版の@<tt>{code.py}と同じ場所（通常は@<tt>{CIRCUITPY}の直下）へ3ファイルを置いてください。フォルダー名をそのままコピーして@<tt>{lib}の下へ入れると、現在のimport文では見つけられません。

この章で導入したコードは、次の順番で必要になります。

 1. @<tt>{debouncer.py}: GPIOの生値を押下イベントにする
 2. @<tt>{keycode.py}: Aキーという意味を@<tt>{0x04}にする
 3. @<tt>{keyboard.py}: キーコードをHIDレポートへする
 4. @<tt>{onekey_with_debounce.py}: GP0からAキーの送信までをつなぐ

=== デバウンスなし版との違い

@<tt>{codes/onekey_without_debounce.py}は、デバウンス処理を追加する前の比較用コードです。入力データを前回値と現在値で比較し、HighからLowへの変化をそのまま押下イベントにします。

//list[onekey_without_debounce][デバウンスなしの比較用コード][python]{
import board
import digitalio
import usb_hid

from keyboard import Keyboard
from keycode import Keycode

kbd = Keyboard(usb_hid.devices)

pin = digitalio.DigitalInOut(board.GP0)
pin.direction = digitalio.Direction.INPUT
pin.pull = digitalio.Pull.UP

last_state = pin.value

print("1キーボード 待機中...")

while True:
    current_state = pin.value
    if last_state and not current_state:
        print("Aキーを送信！")
        kbd.send(Keycode.A)
    last_state = current_state
//}

この版でも、押している間の毎周回送信は起きません。前回値を更新しているからです。一方、接点が@<tt>{True, False, True, False}と揺れれば、Lowへの変化ごとに送信します。まずこの版でエッジ検出を確認し、複数入力が見えたら@<tt>{onekey_with_debounce.py}へ進みます。

== 回路とコードを一緒に検証する

=== GPIOだけを検証する

最初に、値表示用のコードで、離しているときが@<tt>{True}、押しているときが@<tt>{False}になることを確認します。ここで失敗した場合は、@<tt>{Keyboard}やUSBを調べません。回路、GP0、GND、プルアップの層に問題があります。

=== USBレポートを検証する

端末に送信メッセージが1回出ているのに、テキストエディターへAが入力されない場合は、入力処理ではなくHID側を調べます。@<tt>{Keyboard}がキーボード用デバイスを選べているか、USB HID設定が有効かを確認します。

逆に、端末へ送信メッセージが出ない場合は、USBを調べる前にGP0の値、@<tt>{update()}、@<tt>{fell}の順で確認します。エラーの層を分けると、配線とプログラムを同時に疑わずに済みます。

デバウンスあり版へ差し替えたら、もう一度10回押します。今度は押した回数とAの入力数が一致するはずです。ここで一致しない場合は、デバウンス処理の呼び出しやスイッチの接触を確認します。

=== どの層で止まっているかを切り分ける

キー入力が出ないとき、いきなり完成版の全行を眺めると、配線とプログラムの両方を同時に疑うことになります。データが変換される順番に沿って、通過したところまでを確認します。

//table[troubleshooting][症状から調べる場所]{
見えている症状	次に確認するもの
CIRCUITPYが表示されない	USBケーブル、Pico Hへの電源、CircuitPythonの書き込み
押していないのにFalseになる	GP0とGNDの接続列、スイッチの向き、プルアップ設定
押すとFalseになるがAが出ない	コードがcode.pyとして保存されているか、update()とfellの呼び出し
「Aキーを送信！」は出るが入力されない	USB HIDデバイスの選択、Pico HとPCのUSB接続
1回の操作でAが複数出る	デバウンスあり版か、Debouncer.update()の呼び出し
//}

最初の行で止まったら、まだPythonの処理を調べる段階ではありません。Pico Hがドライブとして見えること、ファイルを保存できることを先に確認します。GP0の値が期待どおりなら、回路からプログラムへの変換は通過しています。送信メッセージまで出ているなら、押下イベントの生成も通過しています。最後に残るのがUSB HIDの層です。

確認用の表示を追加するときは、イベントを二度読まないようにします。@<tt>{fell}は読むとフラグを消すため、次のような書き方では、表示したあとに条件がFalseになります。

//emlist[][python]{
print(switch.fell)
if switch.fell:
    kbd.send(Keycode.A)
//}

イベントを変数へ一度だけ取り出せば、表示と判定の両方に使えます。

//emlist[][python]{
switch.update()
pressed = switch.fell
print("押下イベント", pressed)
if pressed:
    kbd.send(Keycode.A)
//}

ただし、この確認用コードを完成版へ残す必要はありません。原因を確認したら表示を外し、@<tt>{switch.update()}のあとに@<tt>{switch.fell}を1回だけ読む形へ戻します。こうしておくと、入力を読む処理と送る処理の境界がはっきりします。

== まとめ

この章で作ったデータの流れは、次の1本です。

//emlist[]{
スイッチの導通
  -> GP0のHigh/Low
  -> Debouncer.fellの押下イベント
  -> Keycode.Aの0x04
  -> Keyboard._keysの状態
  -> 8バイトのHIDレポート
  -> Aキーの押下と解放
//}

最初の問題は、回路の状態をプログラムから読めないことでした。入力設定とプルアップで@<tt>{True}と@<tt>{False}へ変えました。次の問題は、現在値だけでは押下の瞬間を表せないことでした。前回値と現在値を比較してイベントへ変えました。その次は、接点の揺れが複数イベントになることでした。生値と確定値、経過時間を使って@<tt>{Debouncer}へまとめました。

押下イベントは、そのままではAキーではありません。@<tt>{Keycode.A}でHIDキーコードへ変え、@<tt>{Keyboard}で押下レポートと解放レポートへ変えました。最後に、これらを@<tt>{codes/onekey_with_debounce.py}へつなぎました。

@<tt>{codes/lib_summary.md}には、@<tt>{keyboard.py}、@<tt>{keycode.py}、@<tt>{debouncer.py}の責務と変更時の確認事項をまとめています。本文で各コードが必要になった理由を追ったあと、実装時の早見表として使ってください。

===[column] ライブラリに任せると、どこまで短くできるか

この章では、入力ピンの値を読み、押下イベントを作り、キーコードをHIDレポートへ変換する処理を自分で実装しました。実際のキーボードを作るときは、毎回この処理を書く必要はありません。CircuitPythonには、同じ仕事をすでに済ませてくれるライブラリがあります。

実装をライブラリへ置き換えると、プログラムは短くなります。その代わり、内部で何が起きているかをコードから直接追いにくくなります。この章で仕組みを一度作ったのは、便利なライブラリを使う前に、どの処理を任せるのかを知るためです。

==== コメントアウトしていたライブラリを使う

サンプルコードのコメントアウトを外すと、HIDキーボード部分は次の3行で置き換えられます。

//emlist[][python]{
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode
from adafruit_debouncer import Debouncer
//}

@<tt>{adafruit_hid.keyboard.Keyboard}は、キーボード用HIDデバイスの選択、キーの押下と解放、レポートの送信を担当します。@<tt>{adafruit_hid.keycode.Keycode}には、AやShiftなどの標準的なキーコードが定義されています。つまり、今回作った@<tt>{Keyboard}、@<tt>{Keycode}、@<tt>{_sync()}の役割をまとめて任せられます。

@<tt>{adafruit_debouncer.Debouncer}は、今回の@<tt>{Debouncer}と同じように、入力の変化を監視して@<tt>{fell}や@<tt>{rose}として返します。使う側のループもほぼ同じです。

//emlist[][python]{
switch = Debouncer(pin)

while True:
    switch.update()
    if switch.fell:
        kbd.send(Keycode.A)
//}

この形なら、本文の完成版から自作の3ファイルを外し、Adafruitのライブラリを@<tt>{CIRCUITPY/lib}へ置くだけで済みます。@<tt>{adafruit_debouncer}は内部で別の依存ライブラリも使うため、配布されているライブラリ一式から依存関係を含めてコピーします。ファイルが足りないと、importの行でエラーになります。

ライブラリ版の利点は、実装量が減ることだけではありません。HIDレポートの形式や、修飾キーをビットへ詰める処理を自分で保守せずに済みます。一方で、イベントが二重に出たときにデバウンスの内部状態を調べるには、ライブラリのドキュメントやソースコードを読む必要があります。まず自作版で値の流れを確認し、動くものを作る段階でライブラリ版へ移る、という順番が扱いやすいでしょう。

==== ほかの選択肢と使い分ける

同じ「キーを読む」処理でも、選ぶ道具によって担当範囲が違います。

 * @<tt>{adafruit_hid} + adafruit_debouncer}: HID送信とデバウンスを任せます。今回のようにCircuitPythonで部品を組み合わせる構成向きです。
 * @<tt>{keypad}: 単独キーやキーマトリクスの走査とイベント化を任せます。キー数を増やすときに候補になります。
 * KMK Firmware: キーマップ、レイヤー、マトリクス、HIDを設定中心でまとめます。キーボード全体を早く形にしたいときに向きます。
 * QMK: マトリクス、デバウンス、キーマップ、HIDをC/C++系のファームウェアで扱います。成熟した環境で本格的に作りたいときの選択肢です。
 * Arduino @<tt>{Keyboard}: 対応ボードからUSBキーボード入力を送ります。Arduino環境とネイティブUSB対応ボードが前提です。

CircuitPython標準の@<tt>{keypad}は、外部ライブラリを追加しなくても、個別のキーや行列になったキーを走査できます。キーが押された、離されたというイベントをキューから取り出す設計なので、第4章のキーマトリクスへ進むときの候補になります。ただし、@<tt>{keypad}はUSBキーボードのレポートまでは送らないため、HID送信には@<tt>{adafruit_hid}などを組み合わせます。

KMK Firmwareは、CircuitPythonの上にキーボード向けの設定層を載せる選択肢です。キーの割り当てやレイヤー、マトリクスなどを設定ファイル中心に組めるため、複数キーのキーボードを早く形にしたいときに向いています。その反面、入力イベントがどのモジュールを通ってHIDレポートになるかを学ぶ目的では、今回の自作版より見通しが悪くなります。

QMKは、キーボード用ファームウェアとして長く使われている別系統の選択肢です。C/C++を使い、マトリクス走査、デバウンス、キーマップ、レイヤーなどをまとめて扱えます。対応するキーボードや情報が多い一方、CircuitPythonのファイルをコピーして試す手軽さはありません。動作速度や細かな制御を優先し、専用のビルド環境を用意できる段階で候補になります。

Arduinoの@<tt>{Keyboard}ライブラリは、USBデバイスとして振る舞えるArduinoボードで、@<tt>{Keyboard.write()}や@<tt>{Keyboard.press()}を使ってキー入力を送ります。短いコードで済みますが、対応するボードとArduino環境が前提です。Pico HへCircuitPythonで書き込んでいる今回の構成に、そのまま移植できるライブラリではありません。

選び方に迷ったら、目的で決めます。仕組みを理解したいならこの章の自作版、CircuitPythonで少ないコードにしたいなら@<tt>{adafruit_hid}と@<tt>{adafruit_debouncer}、行列入力を標準機能で扱いたいなら@<tt>{keypad}、キーボード全体を設定中心で作りたいならKMK、本格的なファームウェアを使うならQMKです。道具を変えても、スイッチの電気的な状態がイベントになり、キーコードを経てHIDレポートになるという流れは変わりません。

===[/column]

次章ではキーを増やします。1キーではGP0を1本使うだけで済みましたが、キーごとにGPIOを割り当てると、キー数に比例してピンが必要になります。次章では、今回の「電圧を読み、状態を確定し、キーコードへ変える」という流れを保ったまま、行と列を使う回路へ進みます。
