# DUALSHOCK2

DUALSHOCK2(SCPH10010) との通信用ライブラリ

## 回路
- DAT(SPIのMISO)ピンは 1kΩ で 3.3V にプルアップする(マイコンのプルアップだと通信が安定しない)  

| ピン番号 | ピン名 | 備考           |
| -------- | ------ | -------------- |
| 1        | DAT    | SPIのMISO      |
| 2        | CMAD   | SPIのMOSI      |
| 3        | ~7V    | 振動モータ電源 |
| 4        | GND    | GND            |
| 5        | 3.3V   | IC電源         |
| 6        | SEL    | SPIのSS        |
| 7        | SCK    | SPIのCLK       |
| 8        | NC     | なし           |
| 9        | ACK    | アクノリッジ   |

## CubeMX
```yaml
Pinout
- SPIn
- SSピンとして任意のピンを GPIO_output に変更
SPIn
- Mode
    - Mode              = Full-Duplex Master
    - Hardware Signal   = Disable

- Parameter setting
    - Basic parameter
        - Frame Format  = Motorola
        - Data Size     = 8bit
        - First bit     = LSB first
    
    - Clock parameter
        - Prescaler         = その下の baud rate が 500KBit/s になるように設定 (1000KBit/sでもできたがたまにうまくいかないことがある)
        - Clock Polality    = High
        - clock Phase       = 2Edge
```

## サンプルプログラム

### コンストラクタ
```C++
DUALSHOCK2 dualshock2(hspi2, SPI_SS_GPIO_Port, SPI_SS_Pin, 0xF);
```

### 初期設定
```C++
dualshock2.init();
dualshock2.reset_stick();
```

### 更新と取得
```C++
std::array<uint8_t, 21> receive_data;
std::array<int16_t, 4> stick_data;
dualshock2.update(0x20);
receive_data = dualshock2.get_data_exex();
stick_data = dualshock2.get_stick_data();
```
注：3秒以上 update() を実行しないと DUALSHOCK2 の内部設定が初期化されてしまう。その際は再び init() を実行すること。

## 参考資料
- [プレイステーション 2 専用 ローリングスイッチ技術資料](http://magicpuppet.org/rolling_switch_technical_document.pdf)
- [デュアルショック(SCPH-1200)の解析 - 寺川 愛印](http://applause.elfmimi.jp/dualshock.txt)
- [デュアルショック2(SCPH-10010)の新機能使用方法 - NT](https://applause.elfmimi.jp/dualshock/millar/NT/dualshock_2.txt)
