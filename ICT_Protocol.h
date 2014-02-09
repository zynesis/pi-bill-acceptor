#ifndef ICT_PROTOCOL_INCLUDED
#define ICT_PROTOCOL_INCLUDED
#include <iostream>
#include <map>
class Protocol{

public:

//    const ErrorList::value_type error[]={ std::make_pair(0x20, "Motor failure"),
//                                    std::make_pair(0x21, "Checksum error"),
//                                    std::make_pair(0x22, "Bill jam"),
//                                    std::make_pair(0x23, "Bill remove"),
//                                    std::make_pair(0x24, "Stacker open"),
//                                    std::make_pair(0x25, "Sensor problem"),
//                                    std::make_pair(0x27, "Bill fish"),
//                                    std::make_pair(0x28, "Stacker problem"),
//                                    std::make_pair(0x29, "Bill reject"),
//                                    std::make_pair(0x2A, "Invalid command"),
//                                    std::make_pair(0x2E, "Reserved"),
//                                    std::make_pair(0x2F, "Error status is exclusion")};


    enum MessageCodes{
        POWERUP=0x80,
        COMM_FAILURE=0x26,
        PWRUP_COMMFAIL_RESPONSE=0x02,
        ESCROW=0x81,
        BILL_ACCEPT_FINISH=0x10,
        BILL_ACCEPT_FAILURE=0x11,
        BILL_REQUEST_ACCEPT=0x02,
        BILL_REQUEST_REJECT=0x0f,
        ENABLE_ACCEPTOR_REQ=0x3e,
        DISABLE_ACCEPTOR_REQ=0x5e,
        STATUS_REQ=0x0C,
        STATUS_RESP_ENABLED=0x3e,
        STATUS_RESP_INHIBIT=0x5e
    };
};
#endif
