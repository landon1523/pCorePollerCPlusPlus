#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <memory>
#include <stdexcept>
#include <regex>
#include <cstring>
#include <cmath>
#include <chrono>

char *strToChars(std::string value) {
    char *c_arr = new char[value.length() + 1];
    strcpy(c_arr, value.c_str());
    return c_arr;
}

template <
        class result_t   = std::chrono::milliseconds,
        class clock_t    = std::chrono::steady_clock,
        class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

class SnmpResponse {

public:

    SnmpResponse() = default;

    SnmpResponse(std::string oid_str, std::string data_type, std::string value, std::string raw_response) {
        this->oid_str = std::move(oid_str);
        this->data_type = std::move(data_type);
        this->value = std::move(value);
        this->raw_response = std::move(raw_response);

        char *value_char_arr = strToChars(this->value);
        for (unsigned int i = 0; i < strlen(value_char_arr); i++) {
            if (!(isalpha(value_char_arr[i]) || isalnum(value_char_arr[i]))) {
                this->value.erase(std::remove(this->value.begin(), this->value.end(), value_char_arr[i]),
                                  this->value.end());
            }
        }
    }

    std::string getOidString() {
        return this->oid_str;
    }

    std::string getDataType() {
        return this->data_type;
    }

    std::string getValue() {

        this->value.erase(std::remove(this->value.begin(), this->value.end(), '\n'), this->value.end());
        return this->value;
    }

    std::string getValueInVolts() {
        float t_a_value = std::stof(this->value);
        std::string output_str = (this->value.find('V') == std::string::npos ? std::to_string(
                floor(((t_a_value * 100) + 0.5) / 100) / 100) : this->value);
        output_str.erase(output_str.find_last_not_of('0') + 1, std::string::npos);
        output_str.erase(output_str.find_last_not_of('.') + 1, std::string::npos);

        if (output_str.find('V') == std::string::npos) {
            output_str += 'V';
        }

        return output_str;
    }

    std::string getValueInCelsius() {
        return this->value + 'C';
    }

    std::string getValueInAmps() {
        float t_a_value = std::stof(this->value);
        std::string output_str = (this->value.find('A') == std::string::npos ? std::to_string(
                floor(((t_a_value * 100) + 0.5) / 100) / 100) : this->value);
        output_str.erase(output_str.find_last_not_of('0') + 1, std::string::npos);
        output_str.erase(output_str.find_last_not_of('.') + 1, std::string::npos);

        if (output_str.find('A') == std::string::npos) {
            output_str += 'A';
        }

        return output_str;
    }

    std::string getRawResponse() {
        return this->raw_response;
    }

    void setOidString(std::string oid_str) {
        this->oid_str = std::move(oid_str);
    }

    void setDataType(std::string data_type) {
        this->data_type = std::move(data_type);
    }

    void setValue(std::string value) {
        this->value = std::move(value);
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
    explicit Snmp(std::string host_str, std::string community_str = "", std::string version_str = "") {
        this->host = std::move(host_str);
        this->community = std::move(community_str);
        this->version = std::move(version_str);
    }

    struct SnmpType {
        SnmpResponse one_response;
        std::vector<SnmpResponse> response_vect;
    };

    /* LEFT OFF HERE */
    SnmpType walk(std::string oid_str, std::string community_str, std::string version_str) {

        struct SnmpType snmp_return;
        snmp_return.response_vect = this->doSnmp(this->SNMP_WALK_CMD,
                                                 this->verifyCommunity(std::move(community_str)),
                                                 this->verifyVersion(std::move(version_str)),
                                                 this->host,
                                                 std::move(oid_str));
        if (snmp_return.response_vect.size() > 1) {
            snmp_return.one_response = snmp_return.response_vect.at(0);
        }

        return snmp_return;
    }

    /*auto get(std::string oid_str, std::string community_str, std::string version_str) {
        std::string f_community = this->verifyCommunity(community_str);
        std::string f_version = this->verifyVersion(version_str);
        return this->doSnmp(this->SNMP_GET_CMD, f_community, f_version, this->host, oid_str)[0];
    }*/

    SnmpType getInputPower() {
        return this->walk(this->snmp_ps_input_voltage, this->getCommunity(), this->getVersion());
    }

    SnmpType getBatteryTemp() {
        return this->walk(this->snmp_ps_temperature, this->getCommunity(), this->getVersion());
    }

    std::vector<SnmpResponse> getStatus() {
        std::vector<SnmpResponse> status_vect;
        status_vect.push_back(
                this->walk(this->snmp_ps_inverter_status, this->getCommunity(), this->getVersion()).response_vect[0]);
        std::vector<SnmpResponse> input_voltage = this->getInputPower().response_vect;

        for (SnmpResponse &voltage: input_voltage) {
            //////////
            // current.setValue(current.getValueInVolts());
            status_vect.push_back(voltage);
        }
        return status_vect;
    }

    std::vector<SnmpResponse> getOutputPower() {
        std::vector<SnmpResponse> output_voltage_vect;
        output_voltage_vect.push_back(
                this->walk(this->snmp_ps_output_voltage, this->getCommunity(), this->getVersion()).response_vect[0]);
        std::vector<SnmpResponse> output_current = this->walk(this->snmp_ps_output_current, this->getCommunity(),
                                                              this->getVersion()).response_vect;

        for (SnmpResponse &current: output_current) {
            //////////
            // current.setValue(current.getValueInVolts());
            output_voltage_vect.push_back(current);
        }
        return output_voltage_vect;
    }

    SnmpType getBatteryStrings() {
        return this->walk(this->snmp_ps_battery_storage, this->getCommunity(), this->getVersion());
    }

    std::vector<SnmpResponse> getBatteryCharging() {
        return getBatteryStrings().response_vect;

    }

    SnmpType getBatteryVoltageSupport() {
        return this->walk(this->snmp_ps_battery_voltage_support, this->getCommunity(), this->getVersion());
    }

    std::vector<SnmpResponse> getBatteryVoltages() {
        std::string battery_support = getBatteryVoltageSupport().response_vect[0].getValue();
        // std::cout << std::stoi(battery_support) << std::endl;
        std::vector<SnmpResponse> voltage_vect;

        switch (std::stoi(battery_support)) {
            case 1:
                std::cout << "getBatteryVoltage case 1 not setup yet" << std::endl;
                std::exit(0);
                break;
            case 2:
                // Return only the battery string voltage
                voltage_vect.push_back(this->walk(this->snmp_ps_total_string_voltage, this->getCommunity(),
                                                  this->getVersion()).response_vect[0]);
            case 3:
                // Get the battery string voltage then merge with individual voltages
                SnmpResponse battery_str_voltage = this->walk(this->snmp_ps_total_string_voltage, this->getCommunity(),
                                                              this->getVersion()).response_vect[0];
                std::vector<SnmpResponse> battery_voltages = this->walk(this->snmp_ps_battery_voltage,
                                                                        this->getCommunity(),
                                                                        this->getVersion()).response_vect;

                battery_str_voltage.setValue(battery_str_voltage.getValueInVolts());
                voltage_vect.push_back(battery_str_voltage);
                for (SnmpResponse &voltage: battery_voltages) {
                    //////////
                    voltage.setValue(voltage.getValueInVolts());
                    voltage_vect.push_back(voltage);
                }

        }
        return voltage_vect;
    }

    // Host, community, and version getters and setters
    std::string getHost() { return this->host; }

    std::string getCommunity() { return this->community; }

    std::string getVersion() { return this->version; }

    void setHost(std::string host_str) { this->host = std::move(host_str); }

    void setCommunity(std::string community_str) { this->community = std::move(community_str); }

    void setVersion(std::string version_str) { this->version = std::move(version_str); }

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
    const std::string RESPONSE_REGEX = "/((\\.\\d+)+)\\s+=\\s+([\\w\\d-]*):*\\s*('(?!'))?(.+?)(?(4)\4|(\r\n|\r|\n|$))/ms";

    const std::string snmp_ps_device_address = ".1.3.6.1.4.1.5591.1.4.2.1.1";
    const std::string snmp_ps_protocol_version = ".1.3.6.1.4.1.5591.1.4.2.1.2";
    const std::string snmp_ps_software_version = ".1.3.6.1.4.1.5591.1.4.2.1.3";
    const std::string snmp_ps_device_id = ".1.3.6.1.4.1.5591.1.4.2.1.4";
    const std::string snmp_ps_batteries = ".1.3.6.1.4.1.5591.1.4.2.1.5";
    const std::string snmp_ps_battery_storage = ".1.3.6.1.4.1.5591.1.4.2.1.6";
    const std::string snmp_ps_temp_sensors = ".1.3.6.1.4.1.5591.1.4.2.1.7";
    const std::string snmp_ps_outputs = ".1.3.6.1.4.1.5591.1.4.2.1.8";
    const std::string snmp_ps_battery_current_support = ".1.3.6.1.4.1.5591.1.4.2.1.9";
    const std::string snmp_ps_float_current_support = ".1.3.6.1.4.1.5591.1.4.2.1.10";
    const std::string snmp_ps_output_voltage_support = ".1.3.6.1.4.1.5591.1.4.2.1.11";
    const std::string snmp_ps_input_voltage_support = ".1.3.6.1.4.1.5591.1.4.2.1.12";
    const std::string snmp_ps_power_supply_test = ".1.3.6.1.4.1.5591.1.4.2.1.13";
    const std::string snmp_ps_major_alarm_support = ".1.3.6.1.4.1.5591.1.4.2.1.14";
    const std::string snmp_ps_minor_alarm_support = ".1.3.6.1.4.1.5591.1.4.2.1.15";
    const std::string snmp_ps_tamper_support = ".1.3.6.1.4.1.5591.1.4.2.1.16";
    const std::string snmp_ps_battery_voltage_support = ".1.3.6.1.4.1.5591.1.4.2.1.17";
    const std::string snmp_ps_output_power_support = ".1.3.6.1.4.1.5591.1.4.2.1.18";
    const std::string snmp_ps_output_frequency_support = ".1.3.6.1.4.1.5591.1.4.2.1.19";
    const std::string snmp_ps_input_current_support = ".1.3.6.1.4.1.5591.1.4.2.1.20";
    const std::string snmp_ps_input_power_support = ".1.3.6.1.4.1.5591.1.4.2.1.21";
    const std::string snmp_ps_output_voltage = ".1.3.6.1.4.1.5591.1.4.2.1.22";
    const std::string snmp_ps_input_voltage = ".1.3.6.1.4.1.5591.1.4.2.1.23";
    const std::string snmp_ps_inverter_status = ".1.3.6.1.4.1.5591.1.4.2.1.24";
    const std::string snmp_ps_major_alarm = ".1.3.6.1.4.1.5591.1.4.2.1.25";
    const std::string snmp_ps_minor_alarm = ".1.3.6.1.4.1.5591.1.4.2.1.26";
    const std::string snmp_ps_total_string_voltage = ".1.3.6.1.4.1.5591.1.4.2.1.28";
    const std::string snmp_ps_equipment_control = ".1.3.6.1.4.1.5591.1.4.2.1.29";
    const std::string snmp_ps_power_out = ".1.3.6.1.4.1.5591.1.4.2.1.30";
    const std::string snmp_ps_frequency_out = ".1.3.6.1.4.1.5591.1.4.2.1.31";
    const std::string snmp_ps_rms_current_in = ".1.3.6.1.4.1.5591.1.4.2.1.32";
    const std::string snmp_ps_power_in = ".1.3.6.1.4.1.5591.1.4.2.1.33";
    const std::string snmp_ps_intput_voltage_presence = ".1.3.6.1.4.1.5591.1.4.2.1.34";
    const std::string snmp_ps_frequncy_in = ".1.3.6.1.4.1.5591.1.4.2.1.35";
    const std::string snmp_ps_string_device_address = ".1.3.6.1.4.1.5591.1.4.3.1.1";
    const std::string snmp_ps_string = ".1.3.6.1.4.1.5591.1.4.3.1.2";
    const std::string snmp_ps_string_charge_current = ".1.3.6.1.4.1.5591.1.4.3.1.3";
    const std::string snmp_ps_string_discharge_current = ".1.3.6.1.4.1.5591.1.4.3.1.4";
    const std::string snmp_ps_string_float = ".1.3.6.1.4.1.5591.1.4.3.1.5";
    const std::string snmp_ps_battery_device_address = ".1.3.6.1.4.1.5591.1.4.4.1.1";
    const std::string snmp_ps_battery_string = ".1.3.6.1.4.1.5591.1.4.4.1.2";
    const std::string snmp_ps_battery = ".1.3.6.1.4.1.5591.1.4.4.1.3";
    const std::string snmp_ps_battery_voltage = ".1.3.6.1.4.1.5591.1.4.4.1.4";
    const std::string snmp_ps_output_device_address = ".1.3.6.1.4.1.5591.1.4.5.1.1";
    const std::string snmp_ps_output = ".1.3.6.1.4.1.5591.1.4.5.1.2";
    const std::string snmp_ps_output_current = ".1.3.6.1.4.1.5591.1.4.5.1.3";
    const std::string snmp_ps_temp_device_address = ".1.3.6.1.4.1.5591.1.4.6.1.1";
    const std::string snmp_ps_temperature_sensor = ".1.3.6.1.4.1.5591.1.4.6.1.2";
    const std::string snmp_ps_temperature = ".1.3.6.1.4.1.5591.1.4.6.1.3";

private:

    // Return true if error strings are found in the response, else return false
    bool errorsPresent(const std::string &response) {
        /*for (int i = 0; i < this->SNMP_ERRORS->length(); i++)
        {
            if (response.find(this->SNMP_ERRORS[i])) {
                std::cout << "Response: " << response << std::endl;
                return true;
            }
        }*/
        return false;
    }

    std::vector<SnmpResponse> doSnmp(const std::string& method,
                                     const std::string community,
                                     const std::string version,
                                     const std::string host,
                                     const std::string oid) {
        std::vector<SnmpResponse> snmp_response;
// echo "elapsed time: $end" . PHP_EOL;
        // $end = (round(microtime(true)*1000) - round($start*1000));
        std::string command_str;
        command_str += method;
        command_str += " ";
        command_str += this->SNMP_GET_OPTS;
        command_str += " -c ";
        command_str += community;
        command_str += " -v ";
        command_str += version;
        command_str += " ";
        command_str += host;
        command_str += " ";
        command_str += oid;
        command_str += " 2>&1";

        char *command_arr = strToChars(command_str);
        std::string response = exec(command_arr);


        if (errorsPresent(response)) { /* do something about it*/ }

        std::vector<std::string> lines;

        const char *resp_arr = strToChars(response);

        unsigned int equal_signs = 0;
        for (unsigned int j = 0; j < strlen(resp_arr); j++) {
            if (resp_arr[j] == '=') equal_signs++;
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

        for (unsigned int i = 0; i < lines.size(); i++) {
            std::string line = lines[i];
            /*std::cout << "LINE #" << i << ": [ " << line << " ]" << std::endl;*/

            std::string delimiter = " = ";
            unsigned int pos = 0;
            std::string response_vals = line.substr(line.find(delimiter), std::string::npos);
            response_vals = response_vals.substr(3, std::string::npos);

            const char *response_char_arr = strToChars(response_vals);
            std::string data_type, value;

            for (unsigned int x = 0; x < response_vals.length(); x++) {
                char response_val = response_vals[x];
                /*std::cout << "RESPONSE VAL #" << x << ": [ " << response_val << " ]" << std::endl;*/

                if (isalpha(response_val)) {
                    data_type += response_val;
                } else {
                    value = response_vals.substr(response_vals.find(data_type) + data_type.length() + 2,
                                                 std::string::npos);
                }
            }
            SnmpResponse single = SnmpResponse(oid, data_type, value, line);
            /*std::cout << "PUSHING BACK RESPONSE OBJECT #" << i << ": [ " << oid << " - " << data_type <<
            " - " << value << " - " << line << " ]" << std::endl;*/
            snmp_response.push_back(single);
        }



        // std::cout << "Response Data Type: " << data_type << std::endl;
        // std::cout << "Response Value: " << value << std::endl;


        return snmp_response;

    }

    std::string exec(char *cmd) {
        auto exec_time = std::chrono::steady_clock::now();
        std::string result;

        /*std::array<char, 512> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) throw std::runtime_error("popen() failed!");
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }*/

        char buf[128];
        FILE *fp;
        if ((fp = popen(cmd, "r")) == NULL) {
            printf("Error opening pipe!\n");
        }

        while (fgets(buf, 128, fp) != NULL) {
            // Do whatever you want here...
            result += buf;
        }

        /*if(pclose(fp))  {
            printf("Command not found or exited with error status\n");
        }
        */
        /*fp = popen_noshell("ls", (const char * const *)argv, "r", &pclose_arg, 0);
        while (fgets(buf, sizeof(buf)-1, fp)) {
            printf("Got line: %s", buf);
        }
        status = pclose_noshell(&pclose_arg);*/

        std::cout << "Total Elapsed Exec Time(ms)=" << since(exec_time).count() << std::endl;
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

std::string formatValueOutput(SnmpResponse one_response, std::string value_unit_type) {
    std::string value;
    if (value_unit_type == "current")
        value = one_response.getValueInAmps();
    else if (value_unit_type == "voltage")
        value = one_response.getValueInVolts();
    else if (value_unit_type == "temp")
        value = one_response.getValueInCelsius();
    else
        value = one_response.getValue();
    return value;
}

// Params: (vector of response objects, table header key string, "current"/"voltage"/"temp", limit param string)
std::string constructSingleVectorOutput(std::vector<SnmpResponse> response_vect,
                                        std::string table_header_key,
                                        std::string value_unit_type,
                                        std::string limit_str) {
    std::string value = formatValueOutput(response_vect[0], value_unit_type);
    return ("'" + table_header_key + "'=" + value + limit_str);
}

// Params: (vector of response objects, parent header key string, "current"/"voltage"/"temp", child header key string, "current"/"voltage"/"temp", limit param string)
// -w 58:,14 -c 57:,15
std::string constructMultiVectorOutput(std::vector<SnmpResponse> response_vect,
                                       std::string parent_header_key,
                                       std::string parent_value_unit_type,

                                       std::string child_header_key,
                                       std::string child_value_unit_type,

                                       std::string first_limits_str,
                                       std::string second_limits_str) {

    auto constr_multi = std::chrono::steady_clock::now();

    std::string to_powercore =
            "'" + parent_header_key + "'=" + formatValueOutput(response_vect[0], parent_value_unit_type) +
            first_limits_str;

    for (int v_index = 1; v_index < response_vect.size(); v_index++) {
        std::string v_index_str = (response_vect.size() - 1 != 1 && !response_vect.empty() ? std::to_string(v_index)
                                                                                           : "");
        to_powercore += " '" + (child_header_key + v_index_str + "'=" +
                                formatValueOutput(response_vect[v_index], child_value_unit_type) + second_limits_str);
    }
    std::cout << "Total Elapsed Multi(ms)=" << since(constr_multi).count() << std::endl;
    return to_powercore;

    // return ("'" + parent_header_key + "'=" + value + first_limits_str);
}



int main(int argc, char *argv[]) {
    // auto start_prog = std::chrono::steady_clock::now();

    std::string libPath = "/usr/share/poller/";

    // RUN: g++ main.cpp -o test && ./test ADDRESS COMMUNITY VERSION FUNCTION
    /*
     ./test 0090ea05ad04.cm.vyvebroadband.net eeth2ohRie6o 2c checkBatteryVoltage
     ./test 0090ea05ad04.cm.vyvebroadband.net eeth2ohRie6o 2c checkInputPower
     ./test 0090ea05ad04.cm.vyvebroadband.net eeth2ohRie6o 2c checkOutputPower
     ./test 0090ea05ad04.cm.vyvebroadband.net eeth2ohRie6o 2c checkPsStatus
     ./test 0090ea05ad04.cm.vyvebroadband.net eeth2ohRie6o 2c checkBatteryTemp
     ./test 0090ea05ad04.cm.vyvebroadband.net eeth2ohRie6o 2c checkBatteryCharging
    */

    std::string addr = argv[1];
    std::string community = argv[2];
    std::string version = argv[3];

    std::string function = argv[4];
    // std::string walk_or_get = argv[5];*/

    // std::string addr = "0090ea05ad04.cm.vyvebroadband.net";
    // std::string community = "eeth2ohRie6o";
    // std::string version = "2c";

    // Battery voltage support - .1.3.6.1.4.1.5591.1.4.2.1.17
    std::string walk_or_get = "walk";
    // std::string function = "checkBatteryVoltage";
    // std::string function = "checkInputPower";
    std::vector<std::string> function_vect = {
            "checkBatteryVoltage",
            "checkInputPower",
            "checkOutputPower",
            "checkPsStatus",
            "checkBatteryTemp",
            "checkBatteryCharging"
    };

    Snmp snmp = *new Snmp(addr, community, version);

    // std::cout << snmp.getHost() << " - " << snmp.getCommunity() << " - " << snmp.getVersion() << std::endl;

    if (walk_or_get == "walk") {

        // 'batteryStringVoltage'=41.6V;;36:45 'batteryVoltage1'=13.9V;;12:15.25 'batteryVoltage2'=13.9V;;12:15.25 'batteryVoltage3'=13.9V;;12:15.25
        if (function == "checkBatteryVoltage") {

            auto start_check = std::chrono::steady_clock::now();
            std::cout << constructMultiVectorOutput(snmp.getBatteryVoltages(),
                                                                                  "batteryStringVoltage",
                                                                                  "voltage",
                                                                                  "batteryVoltage",
                                                                                  "voltage",
                                                                                  ";;36:45",
                                                                                  ";;12:15.25") << std::endl;

            std::cout << "Total Check Elapsed(ms)=" << since(start_check).count() << std::endl;


        }
        else if (function == "checkInputPower") {
            std::cout << constructSingleVectorOutput(snmp.getInputPower().response_vect,
                                                                               "inputVoltage",
                                                                               "voltage",
                                                                               ";;110:130") << std::endl;
        }
        else if (function == "checkOutputPower") {
            std::cout << constructMultiVectorOutput(snmp.getOutputPower(),
                                                                               "outputVoltage",
                                                                               "voltage",
                                                                               "outputCurrent",
                                                                               "current",
                                                                               ";58:;57:",
                                                                               ";14;15") << std::endl;
        }
        else if (function == "checkPsStatus") {
            std::cout << constructMultiVectorOutput(snmp.getStatus(),
                                                                            "inverterStatus",
                                                                            "",
                                                                            "inputVoltage",
                                                                            "voltage",
                                                                            ";1;1",
                                                                            ";;") << std::endl;
        }
        else if (function == "checkBatteryTemp") {
            std::cout << constructSingleVectorOutput(snmp.getBatteryTemp().response_vect,
                                                                                "temp",
                                                                                "temp",
                                                                                ";;") << std::endl;

        }
        else if (function == "checkBatteryCharging") {
            /*std::cout << "checkBatteryCharging --- " << constructSingleVectorOutput(snmp.getBatteryCharging().response_vect,
                                                                                "floatCurrent",
                                                                                "current",
                                                                                ";;")  << std::endl;*/
        }

    }

    // std::cout << "Total Elapsed(ms)=" << since(start_prog).count() << std::endl;
    return 0;
}
