// ######################################################################
//    HTLAB.NET RP2040 Universal SFP Transceiver Firmware
//
//    TITLE : SFP.H
//    ABOUT : SFP Transmit tasks
// ######################################################################
#ifndef SFP_H
#define SFP_H

typedef struct lock_state {
    bool myself;
    bool yourself;
} lock_state_t;

void sfp_transmitter_initialize();
void sfp_transmitter_main(lock_state_t ls);

void sfp_receiver_initialize();
void sfp_receiver_main(lock_state_t* ls);

#endif // SFP_H
