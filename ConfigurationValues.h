#ifndef _CONFIGURATION_VALUES_CLASS
#define _CONFIGURATION_VALUES_CLASS

using namespace std;
class ConfigurationValues{
public:
	string portName;
	int32_t bill1_value;
	int32_t bill2_value;
	int32_t bill3_value;
	int32_t bill4_value;
	int32_t bill5_value;

	ConfigurationValues():portName(""), bill1_value(-1){

	}
};

#endif
