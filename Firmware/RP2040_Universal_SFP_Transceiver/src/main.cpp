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
//  GPIO [ 6] - * NONE *
//  GPIO [ 7] - * NONE *
//  GPIO [ 8] - * NONE *
//  GPIO [ 9] - * NONE *
//  GPIO [10] - LED TX UP (Red)
//  GPIO [11] - LED TX DOWN (Green)
//  GPIO [12] - SFP TX Fault
//  GPIO [13] - SFP TX Disable
//  GPIO [14] - SFP SDA I2C1 (MOD-DEF 2)
//  GPIO [15] - SFP SCL I2C1 (MOD-DEF 1)
//  GPIO [16] - SFP TD (RP2040 OUTPUT -> SFP INPUT)
//  GPIO [17] - SFP RD (RP2040 INPUT <- SFP OUTPUT)
//  GPIO [18] - SFP Detect (MOD-DEF 0)
//  GPIO [19] - SFP Rate Select
//  GPIO [20] - SFP LOS
//  GPIO [21] - LED RX UP (Red)
//  GPIO [22] - LED RX DOWN (Green)
//  GPIO [23] - * NONE *
//  GPIO [24] - * NONE *
//  GPIO [25] - * NONE *
//  GPIO [26] - ADC [0]
//  GPIO [27] - ADC [1]
//  GPIO [28] - ADC [2]
//  GPIO [29] - ADC [3]
//
// ######################################################################
//



//
// ############################## Includes ##############################
//
#include "pico/stdlib.h"
//
// ######################################################################
//



//
// ########################## Global Variables ##########################
//
    const int LED_PIN = 22;
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

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

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
    while (true) {

        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);

    }
//
// ######################################################################
//
}

// END
