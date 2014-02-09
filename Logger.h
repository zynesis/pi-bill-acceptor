#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <stdint.h>
#include "config.h"

using namespace std;

class Logger{

public:

enum LogLevel{
    LEVEL_LOG_DEBUG,
    LEVEL_LOG_INFO,
    LEVEL_LOG_WARN,
    LEVEL_LOG_ERROR,
    LEVEL_LOG_FATAL
};
static Logger& instance()
{
    // The only instance
    // Guaranteed to be lazy initialized
    // Guaranteed that it will be destroyed correctly
    static Logger _instance;
    return _instance;
}

~Logger(void);
void init(const std::string &loggerName,  Logger::LogLevel logPriority);
void logDebug(const std::string &message) const;
void logInfo(const std::string &message) const;
void logWarning(const std::string &message) const;
void logError(const std::string &message) const;
void logFatal(const std::string &message) const;

private:
    // Private Constructor
    Logger(){}
    // Stop the compiler generating methods of copy the object
    Logger(Logger const& copy);            // Not Implemented
    Logger& operator=(Logger const& copy); // Not Implemented


};

#endif
