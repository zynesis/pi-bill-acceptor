//#define _DEBUG

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "config.h"
#include "Logger.h"
#include "ICT_Protocol.h"
#include "ConfigReader.h"

using namespace std;

void die(){
    Logger::instance().logFatal("Application exit forced.");
    std::exit(-1);
}


int main()
{
    Logger::instance().init("ICT_Bill_Acceptor_Driver", Logger::LEVEL_LOG_DEBUG);

    Logger::instance().logInfo("Started.");
    if(ConfigReader::instance().readConfig("ICT_Bill_Acceptor_Driver.conf")!=ConfigReader::INIT_RETURN_OK){
        Logger::instance().logError("Cannot load configuration.");
        die();
    }

    ICT_Protocol Protocol;

    if(!Protocol.init(ConfigReader::instance().configValues.portName)){
        die();
    }

    Protocol.run();
    return 0;
}
