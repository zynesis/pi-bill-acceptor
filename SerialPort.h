#ifndef _PRINTER_SERIAL
#define _PRINTER_SERIAL

//MSVC (Visual studio 2005) is not C99 compliant
#ifdef _WIN32
#include "stdint.h"
#else
#include <stdint.h>
#endif

#include <string>
#include <iostream>
#include <errno.h>

using namespace std;

class SerialPort{
private:
	std::string serial_port;
	int32_t portFileDescriptor;

public:
typedef enum _SerialHandshake{
	HANDSHAKE_NONE,
	HANDSHAKE_XON_XOFF,
	HANDSHAKE_RTS_CTS,
}SerialHandshake;

typedef enum _SerialParity{
	SERIAL_PARITY_NONE=0,
	SERIAL_PARITY_ODD,
	SERIAL_PARITY_EVEN,
}SerialParity;

typedef enum _SerialStopBits{
	STOP_BIT_NONE=0,
	STOP_BIT_ONE=1,
	STOP_BIT_ONE_AND_A_HALF,
	STOP_BIT_TWO
}SerialStopBits;

SerialPort();

int32_t serialOpen(const std::string &portName, uint32_t baudRate, SerialHandshake handshake, SerialParity parity, SerialStopBits stopBits);
int32_t serialClose();

//input flush is called before the write is executed
int32_t serialWrite(const unsigned char *data, size_t dataLength);
int32_t serialRead(char *data, int32_t maxDataLength);

};

#endif
