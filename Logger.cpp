#include "Logger.h"
#include <syslog.h>
#include <iostream>
static std::string _loggerName="";

void Logger::init(const std::string &loggerName,  Logger::LogLevel logPriority){

    switch(logPriority){
        case LEVEL_LOG_DEBUG:
            setlogmask(LOG_UPTO(LOG_DEBUG));
        break;

        case LEVEL_LOG_FATAL:
            setlogmask(LOG_UPTO(LOG_CRIT));
        break;

        case LEVEL_LOG_INFO:
            setlogmask(LOG_UPTO(LOG_INFO));
        break;

        case LEVEL_LOG_WARN:
            setlogmask(LOG_UPTO(LOG_WARNING));
        break;

        case LEVEL_LOG_ERROR:
            setlogmask(LOG_UPTO(LOG_ERR));
        break;

        default:
            setlogmask(LOG_UPTO(LOG_INFO));
        break;
    }

    _loggerName=loggerName;
    openlog(loggerName.c_str(), LOG_PID, LOG_USER);
}

Logger::~Logger(){
    closelog();
}

void Logger::logDebug(const std::string &message) const{
#ifdef _DEBUG
    cout<<message<<endl;
#else
    syslog(LOG_DEBUG, "DEBUG: %s", message.c_str());
#endif

}

void Logger::logInfo(const std::string &message) const{
#ifdef _DEBUG
    cout<<message<<endl;
#endif
    syslog(LOG_INFO, "INFO: %s", message.c_str());

}

void Logger::logWarning(const std::string &message) const{
#ifdef _DEBUG
    cerr<<message<<endl;
#endif
    syslog(LOG_WARNING, "WARNING: %s", message.c_str());
}

void Logger::logError(const std::string &message) const{
#ifdef _DEBUG
    cerr<<message<<endl;
#endif
    syslog(LOG_ERR, "ERROR: %s", message.c_str());
}

void Logger::logFatal(const std::string &message) const{
#ifdef _DEBUG
    cerr<<message<<endl;
#endif
    syslog(LOG_CRIT, "FATAL: %s", message.c_str());
}
