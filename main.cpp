#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <stdexcept>
#include <regex>
#include <cstring>
#include <fstream>
#include <unordered_map>

// SNMP Test

bool invalidChar (char c)
{
    return !(c>=0 && c < 128);
}
void stripUnicode(std::string & str)
{
    str.erase(remove_if(str.begin(),str.end(), invalidChar), str.end());
}

class SnmpResponse {

public:

    SnmpResponse() = default;

    SnmpResponse(std::string oid_str, std::string data_type, std::string value, std::string raw_response) {
        this->oid_str = oid_str;
        this->data_type = data_type;
        this->value = value;
        this->raw_response = raw_response;


    }

    std::string getOidString() {
        return this->oid_str;
    }

    std::string getDataType () {
        return this->data_type;
    }

    std::string getValue() {
        return this->value;
    }

    std::string getRawResponse() {
        return this->raw_response;
    }

    void setOidString(std::string oid_str) {
        this->oid_str = oid_str;
    }

    void setDataType(std::string data_type) {
        this->data_type = data_type;
    }

    void setValue(std::string value) {
        this->value = value;
    }

private:
    std::string oid_str, data_type, value, raw_response;

};

class Snmp {

public:

    // Default constructor
    Snmp() = default;

    // Prototyping constructor
    // Input Param 1: Host
    // Input Param 2: Community
    // Input Param 3: Version
    Snmp(std::string host_str, std::string community_str = "", std::string version_str = "") {
        this->host = host_str;
        this->community = community_str;
        this->version = version_str;
    }

    /* LEFT OFF HERE */
    auto walk(std::string oid_str, std::string community_str, std::string version_str) {
        std::string f_community = this->verifyCommunity(community_str);
        std::string f_version = this->verifyVersion(version_str);
        return this->doSnmp(this->SNMP_WALK_CMD, f_community, f_version, this->host, oid_str)[0];
    }

    auto get(std::string oid_str, std::string community_str, std::string version_str) {
        std::string f_community = this->verifyCommunity(community_str);
        std::string f_version = this->verifyVersion(version_str);
        return this->doSnmp(this->SNMP_GET_CMD, f_community, f_version, this->host, oid_str)[0];
    }

    SnmpResponse getBatteryVoltageSupport() {
        return this->walk(this->snmp_ps_battery_voltage_support, this->getCommunity(), this->getVersion());
    }

    std::vector<SnmpResponse> getBatteryVoltages() {
        std::string battery_support = getBatteryVoltageSupport().getValue();
        // std::cout << std::stoi(battery_support) << std::endl;
        std::vector<SnmpResponse> voltage_vect;

        switch (std::stoi(battery_support)) {
            case 1:
                std::cout << "getBatteryVoltage case 1 not setup yet" << std::endl;
                std::exit(0);
                break;
            case 2:
                // Return only the battery string voltage
                voltage_vect.push_back(this->walk(this->snmp_ps_total_string_voltage, this->getCommunity(), this->getVersion()));
            case 3:
                // Get the battery string voltage then merge with individual voltages
                SnmpResponse battery_str_voltage = this->walk(this->snmp_ps_total_string_voltage, this->getCommunity(), this->getVersion());
                SnmpResponse battery_voltages = this->walk(this->snmp_ps_battery_voltage, this->getCommunity(), this->getVersion());

                voltage_vect.push_back(battery_str_voltage);
                voltage_vect.push_back(battery_voltages);
        }
        return voltage_vect;
    }

    // Host, community, and version getters and setters
    std::string getHost() { return this->host; }
    std::string getCommunity() { return this->community; }
    std::string getVersion() { return this->version; }
    void setHost(std::string host_str) { this->host = host_str; }
    void setCommunity(std::string community_str) { this->community = community_str; }
    void setVersion(std::string version_str) { this->version = version_str; }

    // SNMP commands
    const std::string SNMP_WALK_CMD = "snmpwalk";
    const std::string SNMP_GET_CMD = "snmpget";
    const std::string SNMP_SET_CMD = "snmpset";

    // SNMP options
    const std::string SNMP_GET_OPTS = "-OEtn";

    // Array of SNMP errors
    std::string SNMP_ERRORS[4] = {"No Such Object",
                                      "No such Instance",
                                      "Error in packet",
                                      "Timeout: No Response from"
    };

    // Regex response string
    const std::string RESPONSE_REGEX =  "/((\\.\\d+)+)\\s+=\\s+([\\w\\d-]*):*\\s*('(?!'))?(.+?)(?(4)\4|(\r\n|\r|\n|$))/ms";

    const std::string snmp_ps_device_address			=	".1.3.6.1.4.1.5591.1.4.2.1.1";
    const std::string snmp_ps_protocol_version			=	".1.3.6.1.4.1.5591.1.4.2.1.2";
    const std::string snmp_ps_software_version			=	".1.3.6.1.4.1.5591.1.4.2.1.3";
    const std::string snmp_ps_device_id					=	".1.3.6.1.4.1.5591.1.4.2.1.4";
    const std::string snmp_ps_batteries					=	".1.3.6.1.4.1.5591.1.4.2.1.5";
    const std::string snmp_ps_battery_storage			=	".1.3.6.1.4.1.5591.1.4.2.1.6";
    const std::string snmp_ps_temp_sensors				=	".1.3.6.1.4.1.5591.1.4.2.1.7";
    const std::string snmp_ps_outputs					=	".1.3.6.1.4.1.5591.1.4.2.1.8";
    const std::string snmp_ps_battery_current_support	=	".1.3.6.1.4.1.5591.1.4.2.1.9";
    const std::string snmp_ps_float_current_support		=	".1.3.6.1.4.1.5591.1.4.2.1.10";
    const std::string snmp_ps_output_voltage_support	=	".1.3.6.1.4.1.5591.1.4.2.1.11";
    const std::string snmp_ps_input_voltage_support		=	".1.3.6.1.4.1.5591.1.4.2.1.12";
    const std::string snmp_ps_power_supply_test			=	".1.3.6.1.4.1.5591.1.4.2.1.13";
    const std::string snmp_ps_major_alarm_support		=	".1.3.6.1.4.1.5591.1.4.2.1.14";
    const std::string snmp_ps_minor_alarm_support		=	".1.3.6.1.4.1.5591.1.4.2.1.15";
    const std::string snmp_ps_tamper_support			=	".1.3.6.1.4.1.5591.1.4.2.1.16";
    const std::string snmp_ps_battery_voltage_support	=	".1.3.6.1.4.1.5591.1.4.2.1.17";
    const std::string snmp_ps_output_power_support		=	".1.3.6.1.4.1.5591.1.4.2.1.18";
    const std::string snmp_ps_output_frequency_support	=	".1.3.6.1.4.1.5591.1.4.2.1.19";
    const std::string snmp_ps_input_current_support		=	".1.3.6.1.4.1.5591.1.4.2.1.20";
    const std::string snmp_ps_input_power_support		=	".1.3.6.1.4.1.5591.1.4.2.1.21";
    const std::string snmp_ps_output_voltage			=	".1.3.6.1.4.1.5591.1.4.2.1.22";
    const std::string snmp_ps_input_voltage				=	".1.3.6.1.4.1.5591.1.4.2.1.23";
    const std::string snmp_ps_inverter_status			=	".1.3.6.1.4.1.5591.1.4.2.1.24";
    const std::string snmp_ps_major_alarm				=	".1.3.6.1.4.1.5591.1.4.2.1.25";
    const std::string snmp_ps_minor_alarm				=	".1.3.6.1.4.1.5591.1.4.2.1.26";
    const std::string snmp_ps_total_string_voltage		=	".1.3.6.1.4.1.5591.1.4.2.1.28";
    const std::string snmp_ps_equipment_control			=	".1.3.6.1.4.1.5591.1.4.2.1.29";
    const std::string snmp_ps_power_out					=	".1.3.6.1.4.1.5591.1.4.2.1.30";
    const std::string snmp_ps_frequency_out				=	".1.3.6.1.4.1.5591.1.4.2.1.31";
    const std::string snmp_ps_rms_current_in			=	".1.3.6.1.4.1.5591.1.4.2.1.32";
    const std::string snmp_ps_power_in					=	".1.3.6.1.4.1.5591.1.4.2.1.33";
    const std::string snmp_ps_intput_voltage_presence	=	".1.3.6.1.4.1.5591.1.4.2.1.34";
    const std::string snmp_ps_frequncy_in				=	".1.3.6.1.4.1.5591.1.4.2.1.35";
    const std::string snmp_ps_string_device_address		=	".1.3.6.1.4.1.5591.1.4.3.1.1";
    const std::string snmp_ps_string					=	".1.3.6.1.4.1.5591.1.4.3.1.2";
    const std::string snmp_ps_string_charge_current		=	".1.3.6.1.4.1.5591.1.4.3.1.3";
    const std::string snmp_ps_string_discharge_current	=	".1.3.6.1.4.1.5591.1.4.3.1.4";
    const std::string snmp_ps_string_float				=	".1.3.6.1.4.1.5591.1.4.3.1.5";
    const std::string snmp_ps_battery_device_address	=	".1.3.6.1.4.1.5591.1.4.4.1.1";
    const std::string snmp_ps_battery_string			=	".1.3.6.1.4.1.5591.1.4.4.1.2";
    const std::string snmp_ps_battery					=	".1.3.6.1.4.1.5591.1.4.4.1.3";
    const std::string snmp_ps_battery_voltage			=	".1.3.6.1.4.1.5591.1.4.4.1.4";
    const std::string snmp_ps_output_device_address		=	".1.3.6.1.4.1.5591.1.4.5.1.1";
    const std::string snmp_ps_output					=	".1.3.6.1.4.1.5591.1.4.5.1.2";
    const std::string snmp_ps_output_current			=	".1.3.6.1.4.1.5591.1.4.5.1.3";
    const std::string snmp_ps_temp_device_address		=	".1.3.6.1.4.1.5591.1.4.6.1.1";
    const std::string snmp_ps_temperature_sensor		=	".1.3.6.1.4.1.5591.1.4.6.1.2";
    const std::string snmp_ps_temperature				=	".1.3.6.1.4.1.5591.1.4.6.1.3";

private:

    // Return true if error strings are found in the response, else return false
    bool errorsPresent(std::string response){
        /*for (int i = 0; i < this->SNMP_ERRORS->length(); i++)
        {
            if (response.find(this->SNMP_ERRORS[i])) {
                std::cout << "Response: " << response << std::endl;
                return true;
            }
        }*/
        return false;
    }

    struct snmp_response {

    };

    std::vector<SnmpResponse> doSnmp(std::string method, std::string community, std::string version, std::string host, std::string oid){
        std::vector<SnmpResponse> snmp_response;

        std::string command_str = method + " " + this->SNMP_GET_OPTS + " -c " +
                community + " -v " + version + " " + host + " " + oid + " 2>&1";

        std::cout << std::endl << "SNMP String: " << command_str << std::endl;

        char command_arr[command_str.length() + 1];
        strcpy(command_arr, command_str.c_str());

        std::string response = exec(command_arr);
        // std::string response = "'batteryStringVoltage'=42.6V;;36:45 'batteryVoltage1'=14.22V;;12:15.25 'batteryVoltage2'=14.16V;;12:15.25 'batteryVoltage3'=14.25V;;12:15.25";

        if (errorsPresent(response)) { /* do something about it*/ }
        // std::cout << response <<  std::endl << std::endl;

        // Parse result

        std::vector<std::string> lines;

        char char_resp[response.length() + 1];
        strcpy(char_resp, response.c_str());

        int equal_signs = 0;
        for (const char& c : char_resp) {
            if (c == '=') {
                equal_signs++;
            }
        }

        if (equal_signs > 1) {
            std::stringstream data(response);
            std::string line;
            while (std::getline(data, line, '\n')) {
                lines.push_back(line); // Note: You may get a couple of blank lines
            }
        } else {
            lines.push_back(response);
        }

        for (std::string line : lines) {
            std::string delimiter = " = ";
            size_t pos = 0;
            std::string response_vals = line.substr(line.find(delimiter), std::string::npos);
            response_vals = response_vals.substr(3, std::string::npos);

            char * response_char_arr = new char [response_vals.length()+1];
            strcpy (response_char_arr, response_vals.c_str());
            std::string data_type, value;
            for (char response_val : response_vals) {
                if (isalpha(response_val)) {
                    data_type += response_val;
                } else {
                    value = response_vals.substr(response_vals.find(data_type) + data_type.length()+2, std::string::npos);
                }
            }
            SnmpResponse single = SnmpResponse(oid, data_type, value, line);
            snmp_response.push_back(single);
        }



        // std::cout << "Response Data Type: " << data_type << std::endl;
        // std::cout << "Response Value: " << value << std::endl;


        return snmp_response;

    }

    std::string exec(char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe)  throw std::runtime_error("popen() failed!");
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    std::string verifyCommunity(std::string community_str) {
        return (community_str.empty() ? this->community : community_str);
    }

    std::string verifyVersion(std::string version_str) {
        return (version_str.empty() ? this->version : version_str);
    }



    // Declaring strings for the host, community, and version
    std::string host, community, version;
    // Vector containing errors if generated
    std::vector<std::string> errors;



};

/*
 *
 * 'batteryStringVoltage'=42V;;36:45 'batteryVoltage1'=13.98V;;12:15.25 'batteryVoltage2'=14.01V;;12:15.25 'batteryVoltage3'=14V;;12:15.25
 *
 * */


int main(int argc, char* argv[]) {
    std::string libPath = "/usr/share/poller/";


    /*std::string addr = argv[1];
    std::string community = argv[2];
    std::string version = argv[3];

    std::string oid_str = argv[4];
    std::string walk_or_get = argv[5];*/

    std::string addr = "0090ea05ad04.cm.vyvebroadband.net";
    std::string community = "eeth2ohRie6o";
    std::string version = "2c";

    // Battery voltage support - .1.3.6.1.4.1.5591.1.4.2.1.17
    std::string walk_or_get = "walk";

    Snmp snmp = *new Snmp(addr, community, version);

    // std::cout << snmp.getHost() << " - " << snmp.getCommunity() << " - " << snmp.getVersion() << std::endl;

    if (walk_or_get == "get") {
        // std::cout << snmp.get(oid_str, snmp.getCommunity(), snmp.getVersion());
    } else if (walk_or_get == "walk") {
        std::cout << "------------------" << std::endl;
        std::cout << "getBatteryVoltage():" << std::endl;
        std::vector<SnmpResponse> battery_voltages = snmp.getBatteryVoltages();
        for (SnmpResponse b_voltage : battery_voltages) {
            std::cout << "Raw Response: " << b_voltage.getRawResponse() << std::endl;
            std::cout << "OID: " << b_voltage.getOidString() << std::endl;
            std::cout << "DataType: " << b_voltage.getDataType() << std::endl;
            std::cout << "Value: " << b_voltage.getValue() << std::endl;
            std::cout << "------------------" << std::endl;
        }
    }


    return 0;
}
