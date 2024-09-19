// ######################################################################
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//
//    TITLE : BOARD.CPP
//    ABOUT : Board initialization
// ######################################################################
#include "pico/stdlib.h"
#include "board.h"
#include "pin.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"


void board_initialize() {
    // Core voltage
    //vreg_set_voltage(VREG_VOLTAGE_1_10); // default
    //vreg_set_voltage(VREG_VOLTAGE_1_15);
    vreg_set_voltage(VREG_VOLTAGE_1_20);
    //vreg_set_voltage(VREG_VOLTAGE_1_25);
    sleep_ms(10);

    // クロック周波数を320MHzから変更する場合は rx.pio の修正が必要
    set_sys_clock_khz(320000, true);

    // SFP0
    gpio_init(BOARD_PIN_SFP_RD);
    gpio_set_dir(BOARD_PIN_SFP_RD, GPIO_IN);
    gpio_init(BOARD_PIN_SFP_TD);
    gpio_set_dir(BOARD_PIN_SFP_TD, GPIO_OUT);
    i2c_init(i2c1, 100000);
    gpio_set_function(BOARD_PIN_SFP_SDA, GPIO_FUNC_I2C);
    gpio_set_function(BOARD_PIN_SFP_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(BOARD_PIN_SFP_SDA);
    gpio_pull_up(BOARD_PIN_SFP_SCL);
    gpio_init(BOARD_PIN_SFP_TX_FLT);
    gpio_set_dir(BOARD_PIN_SFP_TX_FLT, GPIO_IN);
    gpio_pull_up(BOARD_PIN_SFP_TX_FLT);
    gpio_init(BOARD_PIN_SFP_TX_DIS);
    gpio_set_dir(BOARD_PIN_SFP_TX_DIS, GPIO_OUT);
    gpio_put(BOARD_PIN_SFP_TX_DIS, 0);
    gpio_init(BOARD_PIN_SFP_LOS);
    gpio_set_dir(BOARD_PIN_SFP_LOS, GPIO_IN);
    gpio_pull_up(BOARD_PIN_SFP_LOS);
    gpio_init(BOARD_PIN_SFP_RATE);
    gpio_set_dir(BOARD_PIN_SFP_RATE, GPIO_OUT);
    gpio_put(BOARD_PIN_SFP_RATE, 1);
    gpio_init(BOARD_PIN_SFP_DETECT);
    gpio_set_dir(BOARD_PIN_SFP_DETECT, GPIO_IN);

    // LEDs
    gpio_init(BOARD_PIN_LED_TXU);
    gpio_set_dir(BOARD_PIN_LED_TXU, GPIO_OUT);
    gpio_init(BOARD_PIN_LED_TXD);
    gpio_set_dir(BOARD_PIN_LED_TXD, GPIO_OUT);
    gpio_init(BOARD_PIN_LED_RXU);
    gpio_set_dir(BOARD_PIN_LED_RXU, GPIO_OUT);
    gpio_init(BOARD_PIN_LED_RXD);
    gpio_set_dir(BOARD_PIN_LED_RXD, GPIO_OUT);

    // GPIO
    gpio_init(BOARD_PIN_GPIO0);
    gpio_set_dir(BOARD_PIN_GPIO0, GPIO_IN);
    gpio_pull_down(BOARD_PIN_GPIO0);
    gpio_init(BOARD_PIN_GPIO1);
    gpio_set_dir(BOARD_PIN_GPIO1, GPIO_OUT);
    //gpio_pull_down(BOARD_PIN_GPIO1);
    gpio_init(BOARD_PIN_GPIO2);
    gpio_set_dir(BOARD_PIN_GPIO2, GPIO_OUT);
    //gpio_pull_down(BOARD_PIN_GPIO2);
    gpio_init(BOARD_PIN_GPIO3);
    gpio_set_dir(BOARD_PIN_GPIO3, GPIO_OUT);
    //gpio_pull_down(BOARD_PIN_GPIO3);
    gpio_init(BOARD_PIN_GPIO4);
    gpio_set_dir(BOARD_PIN_GPIO4, GPIO_IN);
    gpio_pull_down(BOARD_PIN_GPIO4);
    gpio_init(BOARD_PIN_GPIO5);
    gpio_set_dir(BOARD_PIN_GPIO5, GPIO_IN);
    gpio_pull_down(BOARD_PIN_GPIO5);
    gpio_init(BOARD_PIN_GPIO25);
    gpio_set_dir(BOARD_PIN_GPIO25, GPIO_IN);
    gpio_pull_down(BOARD_PIN_GPIO25);
    gpio_init(BOARD_PIN_GPIO26);
    gpio_set_dir(BOARD_PIN_GPIO26, GPIO_IN);
    gpio_pull_down(BOARD_PIN_GPIO26);
    gpio_init(BOARD_PIN_GPIO27);
    gpio_set_dir(BOARD_PIN_GPIO27, GPIO_IN);
    gpio_pull_down(BOARD_PIN_GPIO27);
    gpio_init(BOARD_PIN_GPIO28);
    gpio_set_dir(BOARD_PIN_GPIO28, GPIO_IN);
    gpio_pull_down(BOARD_PIN_GPIO28);
}

