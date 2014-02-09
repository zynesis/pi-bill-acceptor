//#define _DEBUG

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include "config.h"
#include "Logger.h"
#include "SerialPort.h"
#include "ICT_Protocol.h"
#include <string.h>
#include <errno.h>

using namespace std;

void die(SerialPort &port){
    Logger::instance().logFatal("Application exit forced.");
    port.serialClose();
    std::exit(-1);
}

bool portReadByte(SerialPort &port, unsigned char &data){
    int result;
    char readData[1024];
    memset(readData, 0x00, sizeof(readData));
    if((result=port.serialRead(readData, 1))<0){
        stringstream ss;
        ss<<result;
        Logger::instance().logError("Cannot read serial port "+ss.str());
        ss.str(string());
        die(port);
    }

    if(result==0){
        return false;
    }

    data=(unsigned char)readData[0];
#ifdef _DEBUG
    stringstream ss;
    ss << std::hex << static_cast<unsigned>(data);
    Logger::instance().logDebug("Received 0x" + ss.str());
    ss.str(std::string());
#endif

    return true;
}
bool sendMessage(SerialPort &port, Protocol::MessageCodes messageCode){
    unsigned char response[1]={(unsigned char)messageCode};
    if(port.serialWrite(response, 1)!=E_OK){
        die(port);
    }
    return true;
}

void parseInput(unsigned char data, SerialPort &port){

    stringstream ss;
    unsigned char buf;

    switch(data){
        case (unsigned char)Protocol::POWERUP:
            if(portReadByte(port, buf)){
                if(buf!=0x8f){
                    ss<<static_cast<unsigned>(buf);
                    Logger::instance().logInfo("Power up message error. Received data: "+ss.str());
                    ss.str(string());
                }
            }else{
                Logger::instance().logInfo("Power up message not complete");
            }

            Logger::instance().logDebug("Power up message received");
            sendMessage(port, Protocol::PWRUP_COMMFAIL_RESPONSE);
        break;

        case (unsigned char)Protocol::COMM_FAILURE:
            Logger::instance().logDebug("Communication failure message received");
            sendMessage(port, Protocol::PWRUP_COMMFAIL_RESPONSE);
        break;

        case (unsigned char)Protocol::ESCROW:
            if(portReadByte(port, buf)){
                if(buf<0x40  || buf>0x44){
                    ss<<static_cast<unsigned>(buf);
                    Logger::instance().logInfo("Escrow message message error. Received data: "+ss.str());
                    ss.str(string());
                    break;
                }
            }else{
                Logger::instance().logInfo("Escrow message not complete");
                break;
            }

            switch(buf){
                case 0x40:
                    Logger::instance().logInfo("First bill type");
                break;

                case 0x41:
                    Logger::instance().logInfo("Second bill type");
                break;

                case 0x42:
                    Logger::instance().logInfo("Third bill type");
                break;

                case 0x43:
                    Logger::instance().logInfo("Fourth bill type");
                break;

                case 0x44:
                    Logger::instance().logInfo("Fifth bill type");
                break;

                default:
                    Logger::instance().logError("Unknown bill type");
                break;
            }

            if(buf>=0x40 && buf<0x44){
                Logger::instance().logInfo("Accepting the bill");
                sendMessage(port, Protocol::BILL_REQUEST_ACCEPT);
            }else{
                Logger::instance().logInfo("Rejecting the bill");
                sendMessage(port, Protocol::BILL_REQUEST_REJECT);
            }


        break;

        case (unsigned char)Protocol::STATUS_RESP_INHIBIT:
            Logger::instance().logDebug("Status reasponse - device is in inhibit state");
        break;

        case (unsigned char)Protocol::STATUS_RESP_ENABLED:
            Logger::instance().logDebug("Status reasponse - device is ready to accept the bills");
        break;

        case (unsigned char)Protocol::BILL_ACCEPT_FINISH:
            Logger::instance().logDebug("Bill accept finish.");
        break;

        case (unsigned char)Protocol::BILL_ACCEPT_FAILURE:
            Logger::instance().logDebug("Bill accept failure");
        break;

//        case 0x20:
//        break;
//
//        case 0x21:
//        break;
//
//        case 0x22:
//        break;
//
//        case 0x23:
//        break;
//
//        case 0x24:
//        break;
//
//        case 0x25:
//        break;
//
//        case 0x26:
//        break;
//
//        case 0x27:
//        break;
//
//        case 0x28:
//        break;
//
//        case 0x29:
//        break;
//
//        case 0x2A:
//        break;
//
//        case 0x2E:
//        break;
//
//        case 0x2F:
//        break;

        default:
            ss<<static_cast<unsigned>(data);
            Logger::instance().logInfo("Unknown message: "+ss.str());
        break;
    }
}

bool sendEnableAcceptor(SerialPort &port){
    Logger::instance().logDebug("Sending enable acceptor");
    unsigned char foo[1]={(unsigned char)Protocol::ENABLE_ACCEPTOR_REQ};
    if(port.serialWrite(foo, 1)!=E_OK){
        die(port);
    }
    return true;
}

bool sendDisableAcceptor(SerialPort &port){
    Logger::instance().logDebug("Sending disable acceptor");
    unsigned char foo[1]={(unsigned char)Protocol::DISABLE_ACCEPTOR_REQ};
    if(port.serialWrite(foo, 1)!=E_OK){
        die(port);
    }
    return true;
}

int main()
{
    Logger::instance().init("ICT_Bill_Acceptor_Driver", Logger::LEVEL_LOG_DEBUG);

    Logger::instance().logInfo("Started.");
    string portName="/dev/ttyUSB0";

    SerialPort port;
    int result=port.serialOpen(portName, 9600, SerialPort::HANDSHAKE_NONE, SerialPort::SERIAL_PARITY_EVEN, SerialPort::STOP_BIT_ONE);
    if(result!=E_OK){
        Logger::instance().logFatal("Cannot open serial port "+portName);
        die(port);
    }

    sendMessage(port, Protocol::PWRUP_COMMFAIL_RESPONSE);
//sendMessage(port,  (Protocol::MessageCodes)0x30); //reset command
    Logger::instance().logDebug("Sent ACK");

    usleep(1000000);

    sendEnableAcceptor(port);
//    Logger::instance().logDebug("Sent enable");

    while(1){

        unsigned char data;
        if(portReadByte(port, data)){
            parseInput(data, port);
        }


        usleep(20000);
        unsigned char foo[1]={(unsigned char)Protocol::STATUS_REQ};
        port.serialWrite(foo, 1);
        Logger::instance().logDebug("Sent status req.");
    }
    return 0;
}
