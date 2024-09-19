// ######################################################################
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//
//    TITLE : SFP_RECEIVER.CPP
//    ABOUT : SFP Receive tasks
// ######################################################################
#include "hardware/pio.h"
#include "sfp_receiver.h"
#include "sfp_receiver.pio.h"
#include "../board/pin.h"

// 8b10b K28.5 Code
#define DEF_K28_5_RDN           (0b0011111010)  // 0x0FA (250)
#define DEF_K28_5_RDP           (0b1100000101)  // 0x305 (773)

// 10b -> 8b decode
// 0x0xx is valid data
// 0x100 is Undefined
// 0x200 is K28.5
static const uint16_t __not_in_flash("dec_tbl") dec_tbl[1024] = {
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 15
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 31
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 47
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 63
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 79
    0x100, 0x100, 0x100, 0x100, 0x100, 0x057, 0x0D7, 0x100, 0x100, 0x037, 0x0B7, 0x017, 0x077, 0x097, 0x0F7, 0x100, // 95
    0x100, 0x100, 0x100, 0x100, 0x100, 0x048, 0x0C8, 0x100, 0x100, 0x028, 0x0A8, 0x008, 0x068, 0x088, 0x0E8, 0x100, // 111
    0x100, 0x0E7, 0x087, 0x067, 0x007, 0x047, 0x0C7, 0x100, 0x100, 0x027, 0x0A7, 0x100, 0x100, 0x100, 0x100, 0x100, // 127
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 143
    0x100, 0x100, 0x100, 0x100, 0x100, 0x05B, 0x0DB, 0x100, 0x100, 0x03B, 0x0BB, 0x01B, 0x07B, 0x09B, 0x0FB, 0x100, // 159
    0x100, 0x100, 0x100, 0x100, 0x100, 0x044, 0x0C4, 0x100, 0x100, 0x024, 0x0A4, 0x004, 0x064, 0x084, 0x0E4, 0x100, // 175
    0x100, 0x0F4, 0x094, 0x074, 0x014, 0x054, 0x0D4, 0x0F4, 0x100, 0x034, 0x0B4, 0x014, 0x074, 0x094, 0x100, 0x100, // 191
    0x100, 0x100, 0x100, 0x100, 0x100, 0x058, 0x0D8, 0x100, 0x100, 0x038, 0x0B8, 0x018, 0x078, 0x098, 0x0F8, 0x100, // 207
    0x100, 0x0EC, 0x08C, 0x06C, 0x00C, 0x04C, 0x0CC, 0x100, 0x100, 0x02C, 0x0AC, 0x00C, 0x06C, 0x08C, 0x0EC, 0x100, // 223
    0x100, 0x0FC, 0x09C, 0x07C, 0x01C, 0x05C, 0x0DC, 0x100, 0x100, 0x03C, 0x0BC, 0x01C, 0x07C, 0x09C, 0x0FC, 0x100, // 239
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x200, 0x100, 0x100, 0x100, 0x100, 0x100, // 255
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 271
    0x100, 0x100, 0x100, 0x100, 0x100, 0x05D, 0x0DD, 0x100, 0x100, 0x03D, 0x0BD, 0x01D, 0x07D, 0x09D, 0x0FD, 0x100, // 287
    0x100, 0x100, 0x100, 0x100, 0x100, 0x042, 0x0C2, 0x100, 0x100, 0x022, 0x0A2, 0x002, 0x062, 0x082, 0x0E2, 0x100, // 303
    0x100, 0x0F2, 0x092, 0x072, 0x012, 0x052, 0x0D2, 0x0F2, 0x100, 0x032, 0x0B2, 0x012, 0x072, 0x092, 0x100, 0x100, // 319
    0x100, 0x100, 0x100, 0x100, 0x100, 0x05F, 0x0DF, 0x100, 0x100, 0x03F, 0x0BF, 0x01F, 0x07F, 0x09F, 0x0FF, 0x100, // 335
    0x100, 0x0EA, 0x08A, 0x06A, 0x00A, 0x04A, 0x0CA, 0x100, 0x100, 0x02A, 0x0AA, 0x00A, 0x06A, 0x08A, 0x0EA, 0x100, // 351
    0x100, 0x0FA, 0x09A, 0x07A, 0x01A, 0x05A, 0x0DA, 0x100, 0x100, 0x03A, 0x0BA, 0x01A, 0x07A, 0x09A, 0x0FA, 0x100, // 367
    0x100, 0x0EF, 0x08F, 0x06F, 0x00F, 0x04F, 0x0CF, 0x100, 0x100, 0x02F, 0x0AF, 0x100, 0x100, 0x100, 0x100, 0x100, // 383
    0x100, 0x100, 0x100, 0x100, 0x100, 0x040, 0x0C0, 0x100, 0x100, 0x020, 0x0A0, 0x000, 0x060, 0x080, 0x0E0, 0x100, // 399
    0x100, 0x0E6, 0x086, 0x066, 0x006, 0x046, 0x0C6, 0x100, 0x100, 0x026, 0x0A6, 0x006, 0x066, 0x086, 0x0E6, 0x100, // 415
    0x100, 0x0F6, 0x096, 0x076, 0x016, 0x056, 0x0D6, 0x100, 0x100, 0x036, 0x0B6, 0x016, 0x076, 0x096, 0x0F6, 0x100, // 431
    0x100, 0x0F0, 0x090, 0x070, 0x010, 0x050, 0x0D0, 0x100, 0x100, 0x030, 0x0B0, 0x100, 0x100, 0x100, 0x100, 0x100, // 447
    0x100, 0x100, 0x08E, 0x06E, 0x00E, 0x04E, 0x0CE, 0x100, 0x0EE, 0x02E, 0x0AE, 0x00E, 0x06E, 0x08E, 0x0EE, 0x100, // 463
    0x100, 0x0E1, 0x081, 0x061, 0x001, 0x041, 0x0C1, 0x100, 0x100, 0x021, 0x0A1, 0x100, 0x100, 0x100, 0x100, 0x100, // 479
    0x100, 0x0FE, 0x09E, 0x07E, 0x01E, 0x05E, 0x0DE, 0x100, 0x100, 0x03E, 0x0BE, 0x100, 0x100, 0x100, 0x100, 0x100, // 495
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 511
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 527
    0x100, 0x100, 0x100, 0x100, 0x100, 0x05E, 0x0DE, 0x100, 0x100, 0x03E, 0x0BE, 0x01E, 0x07E, 0x09E, 0x0FE, 0x100, // 543
    0x100, 0x100, 0x100, 0x100, 0x100, 0x041, 0x0C1, 0x100, 0x100, 0x021, 0x0A1, 0x001, 0x061, 0x081, 0x0E1, 0x100, // 559
    0x100, 0x0F1, 0x091, 0x071, 0x011, 0x051, 0x0D1, 0x0F1, 0x100, 0x031, 0x0B1, 0x011, 0x071, 0x091, 0x100, 0x100, // 575
    0x100, 0x100, 0x100, 0x100, 0x100, 0x050, 0x0D0, 0x100, 0x100, 0x030, 0x0B0, 0x010, 0x070, 0x090, 0x0F0, 0x100, // 591
    0x100, 0x0E9, 0x089, 0x069, 0x009, 0x049, 0x0C9, 0x100, 0x100, 0x029, 0x0A9, 0x009, 0x069, 0x089, 0x0E9, 0x100, // 607
    0x100, 0x0F9, 0x099, 0x079, 0x019, 0x059, 0x0D9, 0x100, 0x100, 0x039, 0x0B9, 0x019, 0x079, 0x099, 0x0F9, 0x100, // 623
    0x100, 0x0E0, 0x080, 0x060, 0x000, 0x040, 0x0C0, 0x100, 0x100, 0x020, 0x0A0, 0x100, 0x100, 0x100, 0x100, 0x100, // 639
    0x100, 0x100, 0x100, 0x100, 0x100, 0x04F, 0x0CF, 0x100, 0x100, 0x02F, 0x0AF, 0x00F, 0x06F, 0x08F, 0x0EF, 0x100, // 655
    0x100, 0x0E5, 0x085, 0x065, 0x005, 0x045, 0x0C5, 0x100, 0x100, 0x025, 0x0A5, 0x005, 0x065, 0x085, 0x0E5, 0x100, // 671
    0x100, 0x0F5, 0x095, 0x075, 0x015, 0x055, 0x0D5, 0x100, 0x100, 0x035, 0x0B5, 0x015, 0x075, 0x095, 0x0F5, 0x100, // 687
    0x100, 0x0FF, 0x09F, 0x07F, 0x01F, 0x05F, 0x0DF, 0x100, 0x100, 0x03F, 0x0BF, 0x100, 0x100, 0x100, 0x100, 0x100, // 703
    0x100, 0x100, 0x08D, 0x06D, 0x00D, 0x04D, 0x0CD, 0x100, 0x0ED, 0x02D, 0x0AD, 0x00D, 0x06D, 0x08D, 0x0ED, 0x100, // 719
    0x100, 0x0E2, 0x082, 0x062, 0x002, 0x042, 0x0C2, 0x100, 0x100, 0x022, 0x0A2, 0x100, 0x100, 0x100, 0x100, 0x100, // 735
    0x100, 0x0FD, 0x09D, 0x07D, 0x01D, 0x05D, 0x0DD, 0x100, 0x100, 0x03D, 0x0BD, 0x100, 0x100, 0x100, 0x100, 0x100, // 751
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 767
    0x100, 0x100, 0x100, 0x100, 0x100, 0x200, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 783
    0x100, 0x0E3, 0x083, 0x063, 0x003, 0x043, 0x0C3, 0x100, 0x100, 0x023, 0x0A3, 0x003, 0x063, 0x083, 0x0E3, 0x100, // 799
    0x100, 0x0F3, 0x093, 0x073, 0x013, 0x053, 0x0D3, 0x100, 0x100, 0x033, 0x0B3, 0x013, 0x073, 0x093, 0x0F3, 0x100, // 815
    0x100, 0x0F8, 0x098, 0x078, 0x018, 0x058, 0x0D8, 0x100, 0x100, 0x038, 0x0B8, 0x100, 0x100, 0x100, 0x100, 0x100, // 831
    0x100, 0x100, 0x08B, 0x06B, 0x00B, 0x04B, 0x0CB, 0x100, 0x0EB, 0x02B, 0x0AB, 0x00B, 0x06B, 0x08B, 0x0EB, 0x100, // 847
    0x100, 0x0E4, 0x084, 0x064, 0x004, 0x044, 0x0C4, 0x100, 0x100, 0x024, 0x0A4, 0x100, 0x100, 0x100, 0x100, 0x100, // 863
    0x100, 0x0FB, 0x09B, 0x07B, 0x01B, 0x05B, 0x0DB, 0x100, 0x100, 0x03B, 0x0BB, 0x100, 0x100, 0x100, 0x100, 0x100, // 879
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 895
    0x100, 0x100, 0x100, 0x100, 0x100, 0x047, 0x0C7, 0x100, 0x100, 0x027, 0x0A7, 0x007, 0x067, 0x087, 0x0E7, 0x100, // 911
    0x100, 0x0E8, 0x088, 0x068, 0x008, 0x048, 0x0C8, 0x100, 0x100, 0x028, 0x0A8, 0x100, 0x100, 0x100, 0x100, 0x100, // 927
    0x100, 0x0F7, 0x097, 0x077, 0x017, 0x057, 0x0D7, 0x100, 0x100, 0x037, 0x0B7, 0x100, 0x100, 0x100, 0x100, 0x100, // 943
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 959
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 975
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 991
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 1007
    0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, // 1023
};

// ----------------------------------------
// 8b10b Decoder
//  - in_data   : 10bit data input
//  - out_data  : 8bit data output
// return
//  - 0 : valid data
//  - 1 : Undefined
//  - 2 : K28.5
// ----------------------------------------
static inline __attribute__((always_inline)) uint8_t tbl_8b10b_dec (uint32_t in_data, uint8_t* out_data)
{
    uint16_t tmp = dec_tbl[in_data];

    *out_data = tmp & 0x00FF;
    return (tmp >> 8);
}

// RX init
static const PIO pio = pio1;
static const uint sm0 = 0;
static uint offset = 0;
static uint32_t read10b = 0;
static uint8_t data_8b = 0;
static uint8_t lock_cnt = 0;


void sfp_receiver_initialize() {
    uint offset = pio_add_program(pio, &rx10b_program);
    rx10b_program_init(pio, sm0, offset, BOARD_PIN_SFP_RD);
}


void sfp_receiver_main(bool* locked1) {
    // 受信FIFOが空なら処理しない
    if (pio_sm_is_rx_fifo_empty(pio, sm0)) return;

    // 受光信号低下時は強制アンロック
    if (gpio_get(BOARD_PIN_SFP_LOS)) *locked1 = false;

    read10b = rx10b_program_nbget10b(pio, sm0);                     // [31:10]:zero, [9:0]:data
    if (*locked1) {
        switch (tbl_8b10b_dec(read10b, &data_8b)) {
            case 0:                                                 // Valid data
                gpio_put(BOARD_PIN_GPIO1, (data_8b & 0b00000010));      // RawData1
                gpio_put(BOARD_PIN_LED_RXD, (data_8b & 0b00000010));    // RawData1
                break;
            case 1: *locked1 = false; lock_cnt = 0; break;           // Undefined
            case 2: break;                                          // K28.5
            default: break;
        }
    } else {
        gpio_put(BOARD_PIN_GPIO1, 0);
        gpio_put(BOARD_PIN_LED_RXD, 0);
        if (((read10b & 0x3FF) == DEF_K28_5_RDN) || ((read10b & 0x3FF) == DEF_K28_5_RDP)) {
            if (++lock_cnt == 5) { *locked1 = true; }                // K28.5シンボルを5回連続検出でロック状態に遷移
        } else {
            pio_sm_restart(pio, sm0); lock_cnt = 0;                 // Retry
        }
    }
}