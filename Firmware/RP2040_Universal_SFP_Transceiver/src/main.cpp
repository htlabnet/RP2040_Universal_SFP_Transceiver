//
// ######################################################################
//
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//      https://htlab.net/electronics/teslacoil/
//
//    Copyright (C) 2024
//      Hideto Kikuchi / PJ (@pcjpnet) - http://pc-jp.net/
//      Tsukuba Science Inc. - http://www.tsukuba-kagaku.co.jp/
//      kingyo (@twi_kingyo) - https://github.com/kingyoPiyo
//
// ######################################################################
//



//
// ######################### Compatible Boards ##########################
//
//    - HTLAB.NET RP2040 Universal SFP Transceiver Module Ver1.0
//    - HTLAB.NET RP2040 Universal SFP Transceiver Mini Module Ver1.0
//
// ######################################################################
//



//
// ########################## Pin Assignments ###########################
//
//  GPIO [ 0] - GPIO 0
//  GPIO [ 1] - GPIO 1
//  GPIO [ 2] - GPIO 2
//  GPIO [ 3] - GPIO 3
//  GPIO [ 4] - GPIO 4
//  GPIO [ 5] - GPIO 5
//  GPIO [ 6] - GPIO 6 [ Mini Module : * NONE * ]
//  GPIO [ 7] - GPIO 7 [ Mini Module : * NONE * ]
//  GPIO [ 8] - GPIO 8 [ Mini Module : * NONE * ]
//  GPIO [ 9] - GPIO 9 [ Mini Module : * NONE * ]
//  GPIO [10] - LED TX UP (Red)
//  GPIO [11] - LED TX DOWN (Green)
//  GPIO [12] - SFP TX Fault
//  GPIO [13] - SFP TX Disable
//  GPIO [14] - SFP SDA I2C1 (MOD-DEF 2) / IO Exp [ Mini Module : * NONE * ]
//  GPIO [15] - SFP SCL I2C1 (MOD-DEF 1) / IO Exp [ Mini Module : * NONE * ]
//  GPIO [16] - SFP TD (RP2040 OUTPUT -> SFP INPUT)
//  GPIO [17] - SFP RD (RP2040 INPUT <- SFP OUTPUT)
//  GPIO [18] - SFP Detect (MOD-DEF 0)
//  GPIO [19] - SFP Rate Select
//  GPIO [20] - SFP LOS
//  GPIO [21] - LED RX UP (Red)
//  GPIO [22] - LED RX DOWN (Green)
//  GPIO [23] - IO Exp INT [ Mini Module : * NONE * ]
//  GPIO [24] - GPIO 24 [ Mini Module : * NONE * ]
//  GPIO [25] - GPIO 25 [ Mini Module : * NONE * ]
//  GPIO [26] - ADC [0]
//  GPIO [27] - ADC [1]
//  GPIO [28] - ADC [2]
//  GPIO [29] - ADC [3]
//
// ######################################################################
//



//
// ####################### IO Exp Pin Assignments #######################
//
//  GPIO [0_0] - SW [1]
//  GPIO [0_1] - SW [2]
//  GPIO [0_2] - SW [3]
//  GPIO [0_3] - SW [4]
//  GPIO [0_4] - SW [5]
//  GPIO [0_5] - SW [6]
//  GPIO [0_6] - SW [7]
//  GPIO [0_7] - SW [8]
//  GPIO [1_0] - Reserved
//  GPIO [1_1] - Reserved
//  GPIO [1_2] - Reserved
//  GPIO [1_3] - Reserved
//  GPIO [1_4] - SubType[0]
//  GPIO [1_5] - SubType[1]
//  GPIO [1_6] - BoardType[0]
//  GPIO [1_7] - BoardType[1]
//
// ######################################################################
//



//
// ###################### IO Exp System Identifier ######################
//
//   BoardType: P1_[7:6]
//     0b 00 [Universal]
//     0b 01 [Master]
//     0b 10 [Slave]
//     0b 11 [Reserved]
//
//   SubType: P1_[5:4]
//     0b 00 [Universal - Main Board]
//     0b 01 [Reserved]
//     0b 10 [Reserved]
//     0b 11 [Reserved]
//
// ######################################################################
//



//
// ############################## Includes ##############################
//
#include "pico/stdlib.h"
#include "version.h"
#include "board/pin.h"
#include "board/board.h"
#include "sfp/sfp_transmitter.h"
#include "sfp/sfp_receiver.h"
//
// ######################################################################
//



//
// ############################# Functions ##############################
//
#ifdef ARDUINO
void setup() {
#else
int main() {
#endif
//
// ####################### Main(Setup) Function #########################
//
#ifndef ARDUINO
    stdio_init_all(); // Pico SDK ONLY
#endif
    board_initialize();

    // LEDチェック
    gpio_put(BOARD_PIN_LED_TXU, 1); gpio_put(BOARD_PIN_LED_TXD, 1);
    gpio_put(BOARD_PIN_LED_RXU, 1); gpio_put(BOARD_PIN_LED_RXD, 1);
    sleep_ms(1000);
    gpio_put(BOARD_PIN_LED_TXU, 0); gpio_put(BOARD_PIN_LED_TXD, 0);
    gpio_put(BOARD_PIN_LED_RXU, 0); gpio_put(BOARD_PIN_LED_RXD, 0);
    sleep_ms(1000);


    sfp_transmitter_initialize();  // 送信処理初期化
    sfp_receiver_initialize();  // 受信処理初期化


//
// ######################################################################
//
#ifdef ARDUINO
}
void loop() {
#endif
//
// ######################## Main(Loop) Function #########################
//
    static bool locked1 = false;   // 自機のロック状態
    static uint32_t lp_cnt = 0;

    while (true) {  // Loop Period = 0.5us

        // 送信処理
        sfp_transmitter_main(locked1);

        // 受信処理
        sfp_receiver_main(&locked1);

        // 自機アンロック中はRXU点滅、ロック時はRXU点灯
        if (!locked1) {
            gpio_put(BOARD_PIN_LED_RXU, lp_cnt & 1 << 19);
        } else {
            gpio_put(BOARD_PIN_LED_RXU, 1);
        }

        // TX_FLT検出でTXU点滅、TX_FLT非検出でTXU点灯
        if (gpio_get(BOARD_PIN_SFP_TX_FLT)) {
            gpio_put(BOARD_PIN_LED_TXU, lp_cnt & 1 << 19);
        } else {
            gpio_put(BOARD_PIN_LED_TXU, 1);
        }

        lp_cnt++;

    }
//
// ######################################################################
//
}

// END
