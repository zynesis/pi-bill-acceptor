#ifndef _CONF_READER_H_
#define _CONF_READER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdint.h>
#include "ConfigurationValues.h"

using namespace std;

class ConfigReader{

public:

	typedef enum _InitReturnValues{
		INIT_RETURN_OK=0,
		INIT_RETURN_FILE_OPEN_FAILED=-1,
		INIT_RETURN_VALUE_LOAD_FAILED=-2
	}InitReturnValues;

    static ConfigReader& instance()
    {
        // The only instance
        // Guaranteed to be lazy initialized
        // Guaranteed that it will be destroyed correctly
        static ConfigReader _instance;
        return _instance;
    }


    virtual ~ConfigReader(void);

	/**
	   \fn InitReturnValues readConfig(const string &fileName)
	   \param[in] fileName Name of the configuration file to be loaded
	   \return If successful, returns INIT_RETURN_OK. If the file couldn't be opened, returns
	      INIT_RETURN_FILE_OPEN_FAILED. If some mandatory value couldn't be loaded from file,
	      returns INIT_RETURN_VALUE_LOAD_FAILED.
	   \brief Reads data from configuration file and stores the data
	 */
	InitReturnValues readConfig(const string &fileName);
    ConfigurationValues configValues;

private:
    ConfigReader(void);
    // Stop the compiler generating methods of copy the object
    ConfigReader(ConfigReader const& copy);            // Not Implemented
    ConfigReader& operator=(ConfigReader const& copy); // Not Implemented
};

#endif
