#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <stdexcept>
#include <regex>
#include <cstring>


// SNMP Test

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
    std::string walk(std::string oid_str, std::string community_str, std::string version_str) {
        std::string f_community = this->verifyCommunity(community_str);
        std::string f_version = this->verifyVersion(version_str);
        return this->doSnmp(this->SNMP_WALK_CMD, f_community, f_version, this->host, oid_str);
    }

    std::string get(std::string oid_str, std::string community_str, std::string version_str) {
        std::string f_community = this->verifyCommunity(community_str);
        std::string f_version = this->verifyVersion(version_str);
        return this->doSnmp(this->SNMP_GET_CMD, f_community, f_version, this->host, oid_str);
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

    std::string doSnmp(std::string method, std::string community, std::string version, std::string host, std::string oid){

        std::string command_str = method + " " + this->SNMP_GET_OPTS + " -c " +
                community + " -v " + version + " " + host + " " + oid + " 2>&1";

        std::cout << command_str << std::endl << std::endl;

        char command_arr[command_str.length() + 1];
        strcpy(command_arr, command_str.c_str());

        std::string response = exec(command_arr);
        // std::string response = "'batteryStringVoltage'=42.6V;;36:45 'batteryVoltage1'=14.22V;;12:15.25 'batteryVoltage2'=14.16V;;12:15.25 'batteryVoltage3'=14.25V;;12:15.25";

        if (errorsPresent(response)) { /* do something about it*/ }
        // std::cout << response << std::endl << std::endl;

        std::istringstream iss(response);
        std::vector<std::string> res;
        std::string temp;
        char delim = ' ';
        while (getline(iss, temp, delim)) {
            if (!(temp.empty())) {
                res.push_back(temp);

                int start_value_pos = temp.find_first_of('=') + 1;
                std::string key = temp.substr(1, start_value_pos-3);

                std::string value_raw = temp.substr(start_value_pos);

                // declaring character array
                char value_char_arr[value_raw.length() + 1];
                strcpy(value_char_arr, value_raw.c_str());

                std::vector<std::string> values;
                std::string temp_str;

                /*for (const auto &i : value_char_arr) {
                    std::cout << i ;
                }*/

                int i = 0;
                int array_len = sizeof(value_char_arr) / sizeof(value_char_arr[0]);
                while(i < array_len) {

                    if (value_char_arr[i] == ';' || value_char_arr[i] == ':') {
                        values.push_back(temp_str);
                        temp_str.clear();
                    } else {

                        temp_str += value_char_arr[i];
                        if (i == array_len - 1) {
                            values.push_back(temp_str);
                        }

                    }
                    i++;
                }

                for (const auto &x : values) {
                    std::cout << key << " - " << x << std::endl;
                }

            }
        }



        return "";
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

    std::string addr = argv[1];
    std::string community = argv[2];
    std::string version = argv[3];

    Snmp snmp = *new Snmp(addr, community, version);

    std::cout << snmp.getHost() << " - " << snmp.getCommunity() << " - " << snmp.getVersion() << std::endl;
    std::cout << snmp.get(".1.3.6.1.4.1.5591.1.4.4.1.4", snmp.getCommunity(), snmp.getVersion());


    return 0;
}
