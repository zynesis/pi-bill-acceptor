#include <cstdlib>
#include <string.h>
#include "ICT_Protocol.h"
#include "config.h"

ICT_Protocol::ICT_Protocol():commLineState(ICT_Protocol::COMMUNICATION_STATE_UNKNOWN),
                            deviceInhibitState(ICT_Protocol::DEVICE_STATE_UNKNOWN),
                            commErrorCounter(0)
{

}

void ICT_Protocol::die(){
    Logger::instance().logFatal("Application exit forced.");
    port.serialClose();
    std::exit(-1);
}

bool ICT_Protocol::portReadByte(unsigned char &data){
    int result;
    char readData[1024];
    memset(readData, 0x00, sizeof(readData));
    if((result=port.serialRead(readData, 1))<0){
        stringstream ss;
        ss<<result;
        Logger::instance().logError("Cannot read serial port "+ss.str());
        ss.str(string());
        die();
    }

    if(result==0){
        commErrorCounter++;

        if(commErrorCounter>COMMUNICATION_DEAD_COUNTER){
            commErrorCounter=0;
            //TODO inform the application.
            commLineState=COMMUNICATION_STATE_BROKEN;
        }

        return false;
    }

    commLineState=COMMUNICATION_STATE_ALIVE;

    data=(unsigned char)readData[0];
#ifdef _DEBUG
    stringstream ss;
    ss<<static_cast<unsigned>(data);
    Logger::instance().logDebug("Received "+ss.str());
    ss.str(std::string());
#endif

    return true;
}

bool ICT_Protocol::sendMessage(ICT_Protocol::MessageCodesController messageCode){
    unsigned char response[1]={(unsigned char)messageCode};
    if(port.serialWrite(response, 1)!=E_OK){
        Logger::instance().logFatal("Cannot write to the serial port.");
        die();
    }
    return true;
}

void ICT_Protocol::parseInput(unsigned char data){

    stringstream ss;
    unsigned char buf;
    string errorString;

    switch(data){
        case (unsigned char)ICT_Protocol::MSG_DEVICE_POWERUP:
        case 0x8f:
            Logger::instance().logInfo("Power up message received");
            sendMessage(ICT_Protocol::MSG_CTRL_PWRUP_COMMFAIL_RESPONSE);
        break;

        case (unsigned char)ICT_Protocol::MSG_DEVICE_COMM_FAILURE:
            Logger::instance().logInfo("Communication failure message received");
            //TODO report this
        break;

        case (unsigned char)ICT_Protocol::MSG_DEVICE_ESCROW:
            if(portReadByte(buf)){
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
                    ss<<ConfigReader::instance().configValues.bill1_value;
                    Logger::instance().logInfo("First bill type inserted - "+ss.str());
                    ss.str(std::string());
                break;

                case 0x41:
                    ss<<ConfigReader::instance().configValues.bill2_value;
                    Logger::instance().logInfo("Second bill type inserted - "+ss.str());
                    ss.str(std::string());
                break;

                case 0x42:
                    ss<<ConfigReader::instance().configValues.bill3_value;
                    Logger::instance().logInfo("Third bill type inserted - "+ss.str());
                    ss.str(std::string());
                break;

                case 0x43:
                    ss<<ConfigReader::instance().configValues.bill4_value;
                    Logger::instance().logInfo("Fourth bill type inserted - "+ss.str());
                    ss.str(std::string());
                break;

                case 0x44:
                    ss<<ConfigReader::instance().configValues.bill5_value;
                    Logger::instance().logInfo("Fifth bill type inserted - "+ss.str());
                    ss.str(std::string());
                break;

                default:
                    Logger::instance().logError("Unknown bill type inserted");
                break;
            }

            if(buf>=0x40 && buf<0x44){
                Logger::instance().logInfo("Accepting the bill");
                sendMessage(ICT_Protocol::MSG_CTRL_BILL_REQUEST_ACCEPT);

                if(portReadByte(buf)){
                    parseInput(buf);
                }else{
                    Logger::instance().logWarning("No response on accept command");
                }

            }else{
                Logger::instance().logInfo("Rejecting the bill");
                sendMessage(ICT_Protocol::MSG_CTRL_BILL_REQUEST_REJECT);
            }


        break;

        case (unsigned char)ICT_Protocol::MSG_DEVICE_STATUS_RESP_INHIBIT:
            Logger::instance().logDebug("Status reasponse - device is in inhibit state");
            deviceInhibitState=DEVICE_STATE_DISABLED;
        break;

        case (unsigned char)ICT_Protocol::MSG_DEVICE_STATUS_RESP_ENABLED:
            Logger::instance().logDebug("Status reasponse - device is ready to accept the bills");
            deviceInhibitState=DEVICE_STATE_ENABLED;
        break;

        case (unsigned char)ICT_Protocol::MSG_DEVICE_BILL_ACCEPT_FINISH:
            Logger::instance().logDebug("Bill accept finish.");
        break;

        case (unsigned char)ICT_Protocol::MSG_DEVICE_BILL_ACCEPT_FAILURE:
            Logger::instance().logDebug("Bill accept failure");
        break;

        case 0x20:
            errorString="Motor failure";
        break;

        case 0x21:
            errorString="Checksum error";
        break;

        case 0x22:
            errorString="Bill jam";
        break;

        case 0x23:
            errorString="Bill remove";
        break;

        case 0x24:
            errorString="Stacker open";
        break;

        case 0x25:
            errorString="Sensor problem";
        break;

        case 0x27:
            errorString="Bill fish";
        break;

        case 0x28:
            errorString="Stacker problem";
        break;

        case 0x29:
            errorString="Bill reject";
        break;

        case 0x2A:
            errorString="Invalid command";
        break;

        case 0x2E:
            errorString="Reserved / Not defined";
        break;

        case 0x2F:
            errorString="Error status is exclusion";
        break;

        default:
            ss<<static_cast<unsigned>(data);
            string err=ss.str();
            ss.str(std::string());
            Logger::instance().logInfo("Unknown message: "+err);
        break;
    }
}

bool ICT_Protocol::sendEnableAcceptor(){
    Logger::instance().logDebug("Sending enable acceptor");
    unsigned char foo[1]={(unsigned char)ICT_Protocol::MSG_CTRL_ENABLE_ACCEPTOR_REQ};
    if(port.serialWrite(foo, 1)!=E_OK){
        die();
    }
    return true;
}

bool ICT_Protocol::sendDisableAcceptor(){
    Logger::instance().logDebug("Sending disable acceptor");
    unsigned char foo[1]={(unsigned char)ICT_Protocol::MSG_CTRL_DISABLE_ACCEPTOR_REQ};
    if(port.serialWrite(foo, 1)!=E_OK){
        die();
    }
    return true;
}

bool ICT_Protocol::init(string serialPortName){
    int result=port.serialOpen(serialPortName, 9600, SerialPort::HANDSHAKE_NONE, SerialPort::SERIAL_PARITY_EVEN, SerialPort::STOP_BIT_ONE);
    if(result!=E_OK){
        Logger::instance().logFatal("Cannot open serial port "+serialPortName);
        return false;
    }else{
        Logger::instance().logInfo("Port "+serialPortName+" opened");
    }

    commLineState=COMMUNICATION_STATE_UNKNOWN;
    deviceInhibitState=DEVICE_STATE_UNKNOWN;
    return true;
}

void ICT_Protocol::run(){

    while(1){

        unsigned char data;
        if(portReadByte(data)){
            parseInput(data);
        }

        if(lastDeviceInhibitState!=deviceInhibitState){
            //TODO report to the application
            lastDeviceInhibitState=deviceInhibitState;
        }

        if(lastCommLineState!=commLineState){
            //TODO report to the application
            lastCommLineState=commLineState;
            if(commLineState==COMMUNICATION_STATE_ALIVE){
                Logger::instance().logInfo("Communication to the device is alive");
            }else if(commLineState==COMMUNICATION_STATE_BROKEN){
                Logger::instance().logWarning("No communication to the device");
            }
        }

        usleep(10000);
        sendMessage(ICT_Protocol::MSG_CTRL_STATUS_REQ);

    }

}
