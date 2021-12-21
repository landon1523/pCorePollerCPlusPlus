#include <string>

class PS_Object {

public:
    // Constructor
    PS_Object(std::string ip_address, std::string debug);

    bool checkHostAlive() { return true; }
    void initSnmp();

protected:

    std::string getCache();
    bool getCanCache();
    std::string getCacheTime();
    void fork();
    std::string* handleResults();
    std::string* parseSnmp();
    void rrdCounter();

    std::string ping();
    std::string pollSnmpValue();
    std::string* runLocal();
    std::string* runRemote();

    void setCache();
    void setCacheTime();
    std::string validateAddress();

    // snmp
    // remoteTerminal
    // address
    // logger
    // debug

private:
    const std::string max_32_bit_unsigned			    =	"4294967295";
    const std::string max_64_bit_unsigned	            =	"18446744073709551615‬";
    const std::string max_32_bit_signed					=	"2147483648‬";
    const std::string max_64_bit_signed					=	"9223372036854775808‬";
    const std::string data_path							=	"data/";
    const std::string interface_cache_file				=	"interfaces";
    const std::string log_path							=	"logs/";
    const std::string rrd_path							=	"rrd/";
    const std::string default_community					=	"public";
    const std::string default_version					=	"2c";
    const int default_debug						        =	0;
    const std::string default_output					=	"nagios";
    const int max_cmd_deadline					        =	30;
    const int host_alive_tries					        =	3;
    const int cache_valid						        =	3600;
    const std::string key_cache_time					=	"cachetime";
    const std::string key_ping						    =	"ping";
    const std::string key_packet_loss					=	"packetloss";
    const std::string key_input_voltage					=	"inputvoltage";
    const std::string key_input_current					=	"inputcurrent";
    const std::string key_input_wattage					=	"inputwattage";
    const std::string key_input_frequency				=	"inputfrequency";
    const std::string key_output_voltage				=	"outputvoltage";
    const std::string key_output_current				=	"outputcurrent";
    const std::string key_output_wattage				=	"outputwattage";
    const std::string key_output_frequency				=	"outputfrequency";
    const std::string key_inverter_status				=	"inverterstatus";
    const std::string key_temperature					=	"temp";
    const std::string key_ps_battery_current_support	=	"batterycurrentsupport";
    const std::string key_ps_battery_voltage_support	=	"batteryvoltagesupport";
    const std::string key_ps_float_current_support		=	"batteryfloatsupport";
    const std::string key_ps_temp_sensors				=	"batterytemperaturesupport";
    const std::string key_ps_battery_storage			=	"batterystrings";
    const std::string key_ps_batteries					=	"batteries";
    const std::string key_ps_battery_voltage			=	"batteryvoltage";
    const std::string key_ps_battery_string_voltage		=	"batterystringvoltage";
    const std::string key_ps_string_charge_current		=	"chargerate";
    const std::string key_ps_string_discharge_current	=	"dischargerate";
    const std::string key_ps_string_float				=	"floatcurrent";
    const std::string uom_voltage						=	"v";
    const std::string uom_amperage						=	"a";
    const std::string uom_wattage						=	"w";
    const std::string uom_celcius						=	"c";
    const std::string uom_fahrenheit					=	"f";
    const std::string uom_frequency						=	"hz";
    const std::string uom_seconds						=	"s";
    const std::string uom_bytes							=	"b";
    const std::string uom_bits							=	"b";
    const std::string uom_milli							=	"m";
    const std::string uom_kilo							=	"k";
    const std::string uom_mega							=	"m";
    const std::string uom_giga							=	"g";
    const std::string uom_per_second					=	"ps";
    const std::string error_snmp_not_init				=	"snmp not initialized\n";
    const std::string error_check_not_supported			=	"this check is not supported by this device\n";
    const std::string error_snmp_failed 				=	"check values not available\n";
    const std::string error_no_batteries				=	"no batteries detected!\n";


};