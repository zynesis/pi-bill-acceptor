#ifdef _WIN32
#include "stdint.h"
#include <windows.h>
#include <windowsx.h>
#include "PComm.h"
#else
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#include <iostream>
#include <sstream>
#include <errno.h>
#include <cstring>

#include "Logger.h"
#include "config.h"
#include "SerialPort.h"

#ifdef _WIN32

#else
//POSIX specifies serial timeout in tenth of second
//#define PORT_READ_TIMEOUT (10)
#define PORT_READ_TIMEOUT (10)
#endif

SerialPort::SerialPort(void):serial_port(""),portFileDescriptor(-1){

}

int32_t SerialPort::serialOpen(const std::string &portName, uint32_t baudRate, SerialPort::SerialHandshake handshake,
							   SerialPort::SerialParity parity, SerialPort::SerialStopBits stopBits){
	int32_t result=0;

    serial_port.assign(portName.begin(), portName.end());

	//remove compiler warnings
	result=static_cast<uint32_t>(baudRate);
	result=static_cast<uint32_t>(handshake);
	result=static_cast<uint32_t>(parity);
	result=static_cast<uint32_t>(stopBits);

#ifdef _WIN32

#else

    struct termios newOptions;

    portFileDescriptor = open(serial_port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if(portFileDescriptor<0){
        return portFileDescriptor;
    }

    fcntl(portFileDescriptor, F_SETFL, 0);


    result=tcgetattr(portFileDescriptor, &newOptions);
    if(result<0){
        Logger::instance().logInfo("serial port parameters read call failed");
        close(portFileDescriptor);
        return result;
    }


    if(parity==SERIAL_PARITY_NONE){
    	newOptions.c_cflag &= ~PARENB;
    }else if(parity==SERIAL_PARITY_EVEN){
    	newOptions.c_cflag |= PARENB;
    }else{
    	newOptions.c_cflag |= PARODD;
    }

    newOptions.c_iflag &= ~ICRNL;
    newOptions.c_cflag &= ~CSTOPB;
    newOptions.c_cflag &= ~CSIZE;
    newOptions.c_cflag |= CS8;

    if(handshake==HANDSHAKE_NONE){
        //No HW or SW flow control
        newOptions.c_iflag &= ~(IXON | IXOFF | IXANY | CRTSCTS);
    }else if(handshake==HANDSHAKE_RTS_CTS){

        //Disable SW flow control
        newOptions.c_iflag &= ~(IXON | IXOFF | IXANY);

        //Enable HW flow control
        //newOptions.c_cflag |= CNEW_RTSCTS;
        newOptions.c_cflag |= CRTSCTS;

    }else{
    	newOptions.c_cflag &= ~CRTSCTS;
        //Enable SW flow control
        newOptions.c_iflag |= (IXON | IXOFF);
    }


    //raw input
    newOptions.c_cflag |= (CLOCAL | CREAD);
    newOptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    //raw output
    newOptions.c_oflag &= ~OPOST;

    newOptions.c_cc[VMIN] = 1;
    newOptions.c_cc[VTIME] = PORT_READ_TIMEOUT;
    //newOptions.c_cc[VTIME] = 0;

    speed_t portSpeed;
    switch (baudRate){
        case 115200:
            portSpeed=B115200;
        break;

        case 57600:
            portSpeed=B57600;
        break;

        case 38400:
            portSpeed=B38400;
        break;

        case 19200:
            portSpeed=B19200;
        break;

        case 9600:
            portSpeed=B9600;
        break;

        case 4800:
            portSpeed=B4800;
        break;

        case 2400:
            portSpeed=B2400;
        break;

        case 1200:
            portSpeed=B1200;
        break;

        default:
            //sanity check
        	Logger::instance().logError("unsupported serial port baudrate defined");
            close(portFileDescriptor);
            return EINVAL;
        break;
    }

    result=cfsetispeed(&newOptions, portSpeed);
    if(result<0){
    	Logger::instance().logError("set serial port input speed failed");
        close(portFileDescriptor);
        return result;
    }
    result=cfsetospeed(&newOptions, portSpeed);
    if(result<0){
    	Logger::instance().logError("set serial port output speed failed");
        close(portFileDescriptor);
        return result;
    }

    result=tcsetattr(portFileDescriptor, TCSANOW, &newOptions);
    if(result<0){
    	Logger::instance().logError("write serial port parameters failed");
        close(portFileDescriptor);
        return result;
    }

    result=tcflush(portFileDescriptor, TCIFLUSH);
    if(result<0){
    	Logger::instance().logError("serial port input flush failed");
        return result;
    }

#endif

	return result;
}

int32_t SerialPort::serialClose(){
	int32_t result=0;

#ifdef _WIN32

#else
//suppress warning
//result=portName.length();

result=close(portFileDescriptor);
#endif

    if(result!=E_OK){
        stringstream errStrstr;
        errStrstr<<result;
        Logger::instance().logError("Serial port close failed. Error number: "+errStrstr.str());
        errStrstr.clear();
        errStrstr.str(std::string());
    }
	return result;
}

int32_t SerialPort::serialRead(char *data, int32_t maxDataLength){
	int32_t result=0;

	char buffer[0x200];

	if(maxDataLength>static_cast<int32_t>(sizeof(buffer))){
		return EINVAL;
	}

	memset(buffer, 0x00, sizeof(buffer));

#ifdef _WIN32


#else
	struct termios newOptions;

	fcntl(portFileDescriptor, F_SETFL, 0);

	result=tcgetattr(portFileDescriptor, &newOptions);
	if(result<0){
		Logger::instance().logError("serialRead(), read serial port parameters call failed");
		close(portFileDescriptor);
		return result;
	}

    if(maxDataLength>1){
        //newOptions.c_cc[VMIN] = maxDataLength;
        newOptions.c_cc[VMIN] = 1;
    }else{
        newOptions.c_cc[VMIN] = 0;
    }
	newOptions.c_cc[VTIME] = PORT_READ_TIMEOUT;

    //newOptions.c_cc[VTIME] = 0;

	result=tcsetattr(portFileDescriptor, TCSANOW, &newOptions);
	if(result<0){
		Logger::instance().logError("serialRead(), write serial port parameters failed");
	        close(portFileDescriptor);
	        return result;
	}

    fcntl(portFileDescriptor, F_SETFL, 0);
    result=read(portFileDescriptor, buffer, maxDataLength);
    if(result<0){
    	Logger::instance().logError("serial port read failed");
    }
#endif

	if(result>0 && result<=maxDataLength){
		memcpy(data, buffer, static_cast<size_t>(result));
	}else if(result>maxDataLength){
		memcpy(data, buffer, static_cast<size_t>(maxDataLength));
	}


	if(result>0){
		//result=E_OK;
	}else if(result==0){
	    //hack
        //result=EHOSTUNREACH;
	}

	return result;
}
int32_t SerialPort::serialWrite(const unsigned char *data, size_t dataLength){
		int32_t result=0;

#ifdef _WIN32

#else
//    result=tcflush(portFileDescriptor, TCIFLUSH);
//    if(result<0){
//    	Logger::instance().logError("serial port input flush failed");
//        return result;
//    }

    result=write(portFileDescriptor, data, dataLength);
    if(result!=static_cast<int32_t>(dataLength)){
    	Logger::instance().logFatal("serial port write failed");
        return result;
    }else{
        result=E_OK;
    }
#endif
	return result;
}
