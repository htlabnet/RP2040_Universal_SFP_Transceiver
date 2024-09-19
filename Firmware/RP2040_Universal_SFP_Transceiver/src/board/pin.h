// ######################################################################
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//
//    TITLE : PIN.H
//    ABOUT : Pin definitions on the board
// ######################################################################
#ifndef PIN_H
#define PIN_H

  // SFP
  #define BOARD_PIN_SFP_RD        (17)
  #define BOARD_PIN_SFP_TD        (16)
  #define BOARD_PIN_SFP_SCL       (15)
  #define BOARD_PIN_SFP_SDA       (14)
  #define BOARD_PIN_SFP_TX_FLT    (12)
  #define BOARD_PIN_SFP_TX_DIS    (13)
  #define BOARD_PIN_SFP_LOS       (20)
  #define BOARD_PIN_SFP_RATE      (19)
  #define BOARD_PIN_SFP_DETECT    (18)

  // LEDs
  #define BOARD_PIN_LED_TXU       (10)
  #define BOARD_PIN_LED_TXD       (11)
  #define BOARD_PIN_LED_RXU       (21)
  #define BOARD_PIN_LED_RXD       (22)

  // GPIO
  #define BOARD_PIN_GPIO0         (0)
  #define BOARD_PIN_GPIO1         (1)
  #define BOARD_PIN_GPIO2         (2)
  #define BOARD_PIN_GPIO3         (3)
  #define BOARD_PIN_GPIO4         (4)
  #define BOARD_PIN_GPIO5         (5)

  #define BOARD_PIN_GPIO25        (25)
  #define BOARD_PIN_GPIO26        (26)
  #define BOARD_PIN_GPIO27        (27)
  #define BOARD_PIN_GPIO28        (28)

#endif // PIN_H
