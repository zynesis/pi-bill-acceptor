CC=g++
CFLAGS=-c -O2 -Wall
LDFLAGS=
SOURCES=Logger.cpp SerialPort.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ICT_Bill_Acceptor_Driver
all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


