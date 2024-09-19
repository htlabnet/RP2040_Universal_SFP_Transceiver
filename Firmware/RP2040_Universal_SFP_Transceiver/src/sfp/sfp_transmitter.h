// ######################################################################
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//
//    TITLE : SFP_TRANSMITTER.CPP
//    ABOUT : SFP Transmit tasks
// ######################################################################
#ifndef SFP_TRANSMITTER_H
#define SFP_TRANSMITTER_H

void sfp_transmitter_initialize();
void sfp_transmitter_main(bool locked1);

#endif // SFP_TRANSMITTER_H
