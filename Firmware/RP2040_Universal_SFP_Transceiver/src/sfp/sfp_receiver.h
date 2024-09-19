// ######################################################################
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//
//    TITLE : SFP_RECEIVER.CPP
//    ABOUT : SFP Receive tasks
// ######################################################################
#ifndef SFP_RECEIVER_H
#define SFP_RECEIVER_H

void sfp_receiver_initialize();
void sfp_receiver_main(bool* locked1);

#endif // SFP_RECEIVER_H
