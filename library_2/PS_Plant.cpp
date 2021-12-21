#include <string>
#include "PS_Object.cpp"

class PowerSupply_Plant : public PS_Object {

public:
    // Return array of status codes
    std::string * statusCodes();

    float checkInputPower() { return this->getInputVoltage(); }
    float * checkOutputPower() {
        // return array_merge(this->getOutputVoltage(), this->getOutputCurrent());
        return nullptr;
    }
    std::string * checkPsStatus() {
        // return array_merge(this->getStatus(), this->getInputVoltage());
        return nullptr;
    }
    float checkBatteryTemp();
    // public function checkBatteryCharging()
    // public function checkBatteryVoltage()

protected:

    bool checkHostAlive() { return true; }
    std::string getBatteryCurrentSupport();
    std::string getBatteryVoltageSupport();
    std::string getBatteryFloatSupport();
    std::string getTemperatureSensors();
    std::string getBatteries();
    std::string getBatteryStrings();
    float getBatteryVoltage();
    std::string getBatteryStringVoltage();
    float getStringChargeCurrent();
    float getStringDischargeCurrent();
    float getStringFloat();
    float getInputVoltage();
    int getInputCurrent() { return -1; };
    float getOutputVoltage();
    float getOutputCurrent();
    int getPowerSource() { return -1; }
    std::string getStatus();
    float getTemperature();


private:
    // Declare and initialize const OID strings
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
    
};
