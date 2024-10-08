
# RP2040 Universal SFP Transceiver


## 対応ボード

 - [HTLAB.NET - RP2040 Universal SFP Transceiver Module Ver 1.0](https://htlab.net/products/rp2040-universal-sfp-transceiver-module-1/)
 - [HTLAB.NET - RP2040 Universal SFP Transceiver Mini Module Ver 1.0](https://htlab.net/products/rp2040-universal-sfp-transceiver-mini-module-1/)


## Windows上でのビルド方法：Raspberry Pi Pico SDK v1.5.1 (Pico setup for Windows)

スタートメニューより「Pico - Developer Command Prompt」を開く。

対象のパス（CMakeLists.txtのあるディレクトリ）に移動

（例：cd D:\Git\RP2040_Universal_SFP_Transceiver\Firmware\RP2040_Universal_SFP_Transceiver\）

```bash
mkdir build
cd build
cmake .. -G Ninja --fresh
ninja
```

buildフォルダに.uf2ファイルが生成される。


## Windows上でのビルド方法：Arduino IDE + Raspberry Pi Pico Arduino core

必須： [Raspberry Pi Pico Arduino core](https://github.com/earlephilhower/arduino-pico)

Firmwareフォルダ内の「pio_h_generator_windows.bat」を実行してPIO用のファイルを生成する。

（Pico SDKでは自動で生成されるが、Arduinoだと自動で生成されないため手動で対応する必要がある）

Arduino IDEで「RP2040_Universal_SFP_Transceiver.ino」を開く。

ボード種別を「Generic RP2040」にする。

ポートはデフォルトの場合、USBドライブ経由の書き込みになるので、特に指定しなくてもOK。

Boot Stage 2 を「W25Q16JVxQ QSPI /4」にする。

CPU Speed は「133MHz」のままにする（コード中でクロックアップしているため）

Upload Method は Default (UF2)の場合、「RPI-RP2」が見える状態にして書き込みを行う。


