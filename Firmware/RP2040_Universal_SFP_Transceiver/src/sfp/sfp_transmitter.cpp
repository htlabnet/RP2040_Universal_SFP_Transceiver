// ######################################################################
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//
//    TITLE : SFP_TRANSMITTER.CPP
//    ABOUT : SFP Transmit tasks
// ######################################################################
#include "hardware/pio.h"
#include "sfp_transmitter.h"
#include "sfp_transmitter.pio.h"
#include "../board/pin.h"
#include "sfp_8b10b_table.h"

static uint16_t tx_buf;
static uint8_t mosi_data;
static uint32_t tx_0;
static uint32_t k28_5_last_cnt = 0;
static bool rd = false;
static const PIO pio = pio0;
static const uint sm0 = 0;


void sfp_transmitter_initialize() {
    uint offset = pio_add_program(pio, &tx10b_program);
    tx10b_program_init(pio, sm0, offset, BOARD_PIN_SFP_TD, 20000000);

    // FIFOクリア
    pio_sm_clear_fifos(pio, sm0);

    // 送信FIFO 8wordを埋める
    for (int i = 0; i < 8; i++) { 
        tx_buf = tbl_8b10b_k28_5(&rd);
        tx_10b_blocking(pio, sm0, tx_buf << 22);
    }

    pio_sm_set_enabled(pio, sm0, true); // sm0起動

    // Slave側強制ロックのため K28.5 送出
    for (int i = 0; i < 1000; i++) { 
        tx_buf = tbl_8b10b_k28_5(&rd);
        tx_10b_blocking(pio, sm0, tx_buf << 22);
    }
}


void sfp_transmitter_main(bool locked1) {
    // 送信FIFOがFullなら処理しない
    if (pio_sm_is_tx_fifo_full(pio, sm0)) return;

    tx_0 = gpio_get(BOARD_PIN_GPIO0);

    // TX_LED_Green:TX[0]
    gpio_put(BOARD_PIN_LED_TXD, tx_0);
    
    if (k28_5_last_cnt == 0) {
        // 送信データ生成(暫定)
        mosi_data = tx_0 ? 0b11100011 : 0b11100000;
        // 8b10b変換
        tx_buf = tbl_8b10b_enc(mosi_data, &rd);

        // アンロック時は再ロック要求発行
        if (!locked1) {
            k28_5_last_cnt = 10;
        }
    } else if (k28_5_last_cnt > 5) {
        // K28.5送出
        tx_buf = tbl_8b10b_k28_5(&rd);
        k28_5_last_cnt--;
    } else {
        // 自機のロックハズレ状態を対向に通知
        mosi_data = tx_0 ? 0b10100010 : 0b10100001;
        tx_buf = tbl_8b10b_enc(mosi_data, &rd);
        k28_5_last_cnt--;
    }

    tx_10b_nblocking(pio, sm0, tx_buf << 22);
}


