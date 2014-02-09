#include <algorithm>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/time.h>
#include <sstream>

#define SI_SUPPORT_IOSTREAMS

using namespace std;

#include "SimpleIni.h"
#include "ConfigReader.h"
#include "config.h"
#include "Logger.h"

ConfigReader::ConfigReader(void)
{

}

ConfigReader::~ConfigReader(void)
{

}

ConfigReader::InitReturnValues ConfigReader::readConfig(const string &fileName){

	//ICT_Bill_Accepptor_Driver.conf - Sections
	const string sectionPort("Port");
	const string sectionBills("Bills");


	//ICT_Bill_Accepptor_Driver.conf, Port section
	const string keyPort("Name");

	//ICT_Bill_Accepptor_Driver.conf, Bill section
	const string keyBill_1("1");
	const string keyBill_2("2");
	const string keyBill_3("3");
	const string keyBill_4("4");
	const string keyBill_5("5");
	const string keyBill_6("6");


	CSimpleIniA ini;
	ini.SetUnicode(false);
	ini.SetMultiKey(false);
	ini.SetMultiLine(false);

    string err_str;

	// load the file
	std::ifstream instream;
	instream.open(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if(!instream.good()){
		err_str="Cannot open configuration file \""+fileName+"\"";
		Logger::instance().logFatal(err_str);
		return INIT_RETURN_FILE_OPEN_FAILED;
	}
	if (ini.LoadData(instream) < 0){
		err_str="Cannot load INI file data from config file";
		Logger::instance().logFatal(err_str);
		return INIT_RETURN_FILE_OPEN_FAILED;
	}
	instream.close();


	const char *valuePort=ini.GetValue(sectionPort.c_str(), keyPort.c_str(), NULL);
	const char *valueBill_1=ini.GetValue(sectionBills.c_str(), keyBill_1.c_str(), NULL);
	const char *valueBill_2=ini.GetValue(sectionBills.c_str(), keyBill_2.c_str(), NULL);
	const char *valueBill_3=ini.GetValue(sectionBills.c_str(), keyBill_3.c_str(), NULL);
	const char *valueBill_4=ini.GetValue(sectionBills.c_str(), keyBill_4.c_str(), NULL);
	const char *valueBill_5=ini.GetValue(sectionBills.c_str(), keyBill_5.c_str(), NULL);
	const char *valueBill_6=ini.GetValue(sectionBills.c_str(), keyBill_6.c_str(), NULL);

	if(valuePort==NULL){
		err_str=string("Cannot load \"") + keyPort + "\" value from \"" + sectionPort + "\" section in config file";
		Logger::instance().logError(err_str);
		return INIT_RETURN_VALUE_LOAD_FAILED;
	}

    configValues.portName=string(valuePort);

    stringstream converter;
    string buf;

    int billCounter=0;
	if(valueBill_1==NULL){
		err_str=string("Cannot load \"") + keyBill_1 + "\" value from \"" + sectionBills + "\" section in config file";
        Logger::instance().logWarning(err_str);
	}else{
	    buf=string(valueBill_1);
	    if(buf.length()>0){
            Logger::instance().logInfo("Bill 1 value = "+string(valueBill_1));
            billCounter++;
            converter<<string(valueBill_1);
            converter>>configValues.bill1_value;
            if(converter.fail() || configValues.bill1_value<1){
                Logger::instance().logInfo("Bill 1 value not valid!");
                return INIT_RETURN_VALUE_LOAD_FAILED;
            }
            converter.clear();
            converter.str(std::string());
	    }else{
            Logger::instance().logInfo("Bill 1 value not defined");
            configValues.bill1_value=-1;
	    }
	}

	if(valueBill_2==NULL){
		err_str=string("Cannot load \"") + keyBill_2 + "\" value from \"" + sectionBills + "\" section in config file";
        Logger::instance().logWarning(err_str);
	}else{
	    buf=string(valueBill_2);
	    if(buf.length()>0){
            Logger::instance().logInfo("Bill 2 value = "+string(valueBill_2));
            billCounter++;
            converter<<string(valueBill_2);
            converter>>configValues.bill2_value;
            if(converter.fail() || configValues.bill2_value<1){
                Logger::instance().logInfo("Bill 2 value not valid!");
                return INIT_RETURN_VALUE_LOAD_FAILED;
            }
            converter.clear();
            converter.str(std::string());
	    }else{
            Logger::instance().logInfo("Bill 2 value not defined");
            configValues.bill2_value=-1;
	    }
	}

	if(valueBill_3==NULL){
		err_str=string("Cannot load \"") + keyBill_3 + "\" value from \"" + sectionBills + "\" section in config file";
        Logger::instance().logWarning(err_str);
	}else{
	    buf=string(valueBill_3);
	    if(buf.length()>0){
            Logger::instance().logInfo("Bill 3 value = "+string(valueBill_3));
            billCounter++;
            converter<<string(valueBill_3);
            converter>>configValues.bill3_value;
            if(converter.fail() || configValues.bill3_value<1){
                Logger::instance().logInfo("Bill 3 value not valid!");
                return INIT_RETURN_VALUE_LOAD_FAILED;
            }
            converter.clear();
            converter.str(std::string());
	    }else{
            Logger::instance().logInfo("Bill 3 value not defined");
            configValues.bill3_value=-1;
	    }
	}

	if(valueBill_4==NULL){
		err_str=string("Cannot load \"") + keyBill_4 + "\" value from \"" + sectionBills + "\" section in config file";
        Logger::instance().logWarning(err_str);
	}else{
	    buf=string(valueBill_4);
	    if(buf.length()>0){
            Logger::instance().logInfo("Bill 4 value = "+string(valueBill_4));
            billCounter++;
            converter<<string(valueBill_4);
            converter>>configValues.bill4_value;
            if(converter.fail() || configValues.bill4_value<1){
                Logger::instance().logInfo("Bill 4 value not valid!");
                return INIT_RETURN_VALUE_LOAD_FAILED;
            }
            converter.clear();
            converter.str(std::string());
	    }else{
            Logger::instance().logInfo("Bill 4 value not defined");
            configValues.bill4_value=-1;
	    }
	}

	if(valueBill_5==NULL){
		err_str=string("Cannot load \"") + keyBill_5 + "\" value from \"" + sectionBills + "\" section in config file";
        Logger::instance().logWarning(err_str);
	}else{
	    buf=string(valueBill_5);
	    if(buf.length()>0){
            Logger::instance().logInfo("Bill 5 value = "+string(valueBill_5));
            billCounter++;
            converter<<string(valueBill_5);
            converter>>configValues.bill5_value;
            if(converter.fail() || configValues.bill5_value<1){
                Logger::instance().logInfo("Bill 5 value not valid!");
                return INIT_RETURN_VALUE_LOAD_FAILED;
            }
            converter.clear();
            converter.str(std::string());
	    }else{
            Logger::instance().logInfo("Bill 5 value not defined");
            configValues.bill5_value=-1;
	    }
	}

	if(valueBill_6==NULL){
		err_str=string("Cannot load \"") + keyBill_6 + "\" value from \"" + sectionBills + "\" section in config file";
        Logger::instance().logWarning(err_str);
	}else{
	    buf=string(valueBill_6);
	    if(buf.length()>0){
            Logger::instance().logInfo("Bill 6 value = "+string(valueBill_6));
            billCounter++;
            converter<<string(valueBill_6);
            converter>>configValues.bill6_value;
            if(converter.fail() || configValues.bill6_value<1){
                Logger::instance().logInfo("Bill 6 value not valid!");
                return INIT_RETURN_VALUE_LOAD_FAILED;
            }
            converter.clear();
            converter.str(std::string());
	    }else{
            Logger::instance().logInfo("Bill 6 value not defined");
            configValues.bill6_value=-1;
	    }
	}

    if(billCounter==0){
        Logger::instance().logError("No bill values defined in the configuration.");
        return INIT_RETURN_VALUE_LOAD_FAILED;
    }

	return INIT_RETURN_OK;
}

