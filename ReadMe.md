[schema](ESP-WROOM-02_BreakOut)

## About repo. ##
  
  
This repository converts "ESP-WROOM-02" module to 2.54 mm pitch.  
I am assuming to use on a solderless breadboard.

---
## Board's ##

#### V4 ####

Memo : V4はサンプル画像と作者の実物でいくつかの変更点が含まれていますが、回路な変更はありません。<br>
　　　　Sillk等にいくつかの軽微な修正が含まれています。また、それに伴いBuildDataが更新されています。

<table>
    <tr>
        <td>
            <img src="/doc/HsESP02-V4_Top.png" alt="Top View(image)" width="320px">
        </td>
        <td>
            <img src="/doc/HsESP02-V4_Bottom.png" alt="Bottom View(image)" width="320px">
        </td>
    </tr>
</table>

<!--
[schema]
-->

---

<details>
<summary>Old Version...</summary>

#### V3 ####

<table>
    <tr>
        <td>
            <img src="/doc/HsESP02-V3_Top.png" alt="Top View(image)" width="320px">
        </td>
        <td>
            <img src="/doc/HsESP02-V3_Bottom.png" alt="Bottom View(image)" width="320px">
        </td>
    </tr>
</table>

<!--
[schema]
-->

---

#### V2 ####

<table>
    <tr>
        <td>
            <img src="/doc/esp-wroom-02_BO_top.png" alt="Top View(image)" width="320px">
        </td>
        <td>
            <img src="/doc/esp-wroom-02_BO_bottom.png" alt="Bottom View(image)" width="320px">
        </td>
    </tr>
</table>

<!--
[schema]
-->

</details>

---
  
  
## This Repo... Empty??? ##
  
~~I am currently manufacturing the board.~~
~~All files are added as soon as normal operation can be confirmed.~~

~~(I am waiting for luggage from Elecrow.)~~

## 疲れたので日本語で追記します。 ##

Elecrowが中国の１０月の国のお休み的なやつ（国慶節）から帰ってきてくれたので
、頼んでいた基板が（かなり遅れて）到着し、現品を確認できました。

難しい部分を極力省いて、比較的小型で最小限の機能のブレークアウト基板を開発することが目的でしたので、
回路的に動作しない等のエラーはありませんでした。
そもそもAutoRoutingも使ってるので、失敗する要素が少ないんですが、やっぱり不安でして。

今回の目標であった、
* Reset、Boot0 および ユーザスイッチ1個を実装する。
* 全てハード的にプルアップする(マイコンでプルアップダウンをしない)。
* ブレッドボードに直接刺せる幅で、なおかつ左右に1列ずつ余るようにする。
* 組み込んだりする際の為に、SWや書き込み端子部分を切り離せるようにする。

今回の製作で目標はとりあえず達成できたと思われるので、
設計データ等をバックアップも兼ねてアップロードして置きます。

利用は自由ですし、改変も問題ないと思います。むしろ、自分も他人様のライブラリを引っ張ってきて作っている身です。

もし、Elecrow等にガーバーぶん投げるついでに、BreakOut基板も投げたいけど作るのだるいなーって感じだった時は、一応、このデータで作れているので一緒に出すと現時点では5枚600~700円程度で作れますので割と安価だと思います。

~~もっとよくできたのが転がってそうな気がしますが…~~

ガーバーデータはKiCadフォルダ内のG_Silks_v*.zipです。基本的に数字は製作した順ですので、複数ある場合は最新を見てもらえると良いかと。
いらなくなったら最新残して消します。

一方、プロジェクトファイル自体はそのまま更新しています。内部的にはzipを弾いてGit管理しているので…。

あ、あと、世間がWiFiのセキュリティで騒いでいますね。クラックとかなんとか…
FWのアップデートがあるようなので、組み立て後にアップデートして見るのも良いかと。

その際だけはWindowsマシンがあると楽かもしれません…(実体験)

##  ファイルについて ##

* KiCad
> Kicadフォルダ内にはKiCadにて設計した基板のデータ等が入っています。

* Arduino
> Arduinoフォルダ内には基板組み立て後にLED,およびWiFi,と適当なピン(デフォルトではbeep_pinとして5ピンを指定しています)をテストするためのスケッチです。ほとんどがESP8226のHttpServerのスケッチ例そのままですが、beep_pinの名の通り、圧電スピーカを接続し、状況を音で知らせたり、MACアドレス制限(を、していて不便だったので、制限)をしている場合のためにMACアドレスを表示するようになっています。SSIDとPassWordをそれぞれ自分のWiFiものに書き換えて制作したボードに書き込み、テストしてみてください。(APとして動作するように書き換えてもいいかもしれませんね。自分はネットに接続したまま使用したいのでこのような仕様となっています。)

<details>
<summary>スケッチの使い方...</summary>

>> OTA(Over The Air 無線LAN経由によるUpdate )を使用してみたい場合
>> 1. Boot0スイッチを使用して起動しシリアル接続にてHsESP02_BasicOTA.inoを書き込んでください。
>> 1. その後、スケッチを書き込みする際にはBasicOTAに追記する形で作成し、ArduinoIDE上のシリアルポートにてネットワークポートに表示されるOTA用ポートを指定して * Boot0スイッチ等でブートモードに入ったりはせずに * Uploadをしてください。
>> 1. (今後はHsESP02_BasicOTA.inoやその元になっているBasicOTA.ino等にアップデートがない限り、１を実行する必要はありません。)

>> OTAを使用しない場合
>> 1. HsESP02_ESP-WROOM-02_Test.inoを開き、シリアルポートを選択後、書き込んで動作を確認します。

</details>

##### 念のため #####
何かあっても責任はとりません。
ご自身の判断のもと行ってください。
