#include <string>
#include "PS_Object.cpp"

class PS_Modem : public PS_Object {

public:
    bool checkHostAlive() { return true; }
    float** checkSNR();
    float** checkUSSNR();
    float** checkMicroRefl();
    std::string** checkErrors();
    std::string* statusCodes();

protected:
    std::string pollType();


private:
    // Declare and initialize const OID strings
    const std::string snmp_ds_chan_id			= "1.3.6.1.2.1.10.127.1.1.1.1.1";
    const std::string snmp_ds_frequency         = "1.3.6.1.2.1.10.127.1.1.1.1.2";
    const std::string snmp_ds_width             = "1.3.6.1.2.1.10.127.1.1.1.1.3";
    const std::string snmp_ds_mod               = "1.3.6.1.2.1.10.127.1.1.1.1.4";
    const std::string snmp_ds_power             = "1.3.6.1.2.1.10.127.1.1.1.1.6";
    const std::string snmp_us_chan_id           = "1.3.6.1.2.1.10.127.1.1.2.1.1";
    const std::string snmp_us_frequency         = "1.3.6.1.2.1.10.127.1.1.2.1.2";
    const std::string snmp_us_width             = "1.3.6.1.2.1.10.127.1.1.2.1.3";
    const std::string snmp_us_mod               = "1.3.6.1.2.1.10.127.1.1.2.1.4";
    const std::string snmp_us_snr               = "1.3.6.1.2.1.10.127.1.3.3.1.13";
    const std::string snmp_cm_index             = "1.3.6.1.2.1.10.127.1.3.7.1.2";
    const std::string snmp_unerroreds           = "1.3.6.1.2.1.10.127.1.1.4.1.2";
    const std::string snmp_correcteds           = "1.3.6.1.2.1.10.127.1.1.4.1.3";
    const std::string snmp_uncorrecteds         = "1.3.6.1.2.1.10.127.1.1.4.1.4";
    const std::string snmp_snr                  = "1.3.6.1.2.1.10.127.1.1.4.1.5";
    const std::string snmp_microreflections     = "1.3.6.1.2.1.10.127.1.1.4.1.6";
    const std::string snmp_equalization         = "1.3.6.1.2.1.10.127.1.1.4.1.7";
    const std::string snmp_ext_unerroreds       = "1.3.6.1.2.1.10.127.1.1.4.1.8";
    const std::string snmp_ext_correcteds       = "1.3.6.1.2.1.10.127.1.1.4.1.9";
    const std::string snmp_ext_uncorrecteds     =  "1.3.6.1.2.1.10.127.1.1.4.1.10";

    // Declare and initialize const keys
    const std::string key_ds_chan_id            = "downstreamchannelid";
    const std::string key_ds_frequency          = "downstreamfrequency";
    const std::string key_ds_width              = "downstreamwidth";
    const std::string key_ds_mod                = "downstreammodulation";
    const std::string key_ds_power              = "downstreampower";
    const std::string key_us_chan_id            = "upstreamchannelid";
    const std::string key_us_frequency          = "upstreamfrequency";
    const std::string key_us_width              = "upstreamwidth";
    const std::string key_us_mod                = "upstreammodulation";
    const std::string key_us_snr                = "primaryupstreamsnr";
    const std::string key_cm_index              = "cmtscmindex";
    const std::string key_unerroreds            = "receiveunerroreds";
    const std::string key_correcteds            = "receivecorrecteds";
    const std::string key_uncorrecteds          = "receiveuncorrecteds";
    const std::string key_snr                   = "receivesnr";
    const std::string key_microreflections      = "receivemicroreflections";
    const std::string key_equalization          = "receiveequalization";
    const std::string key_ext_unerroreds        = "receiveextunerroreds";
    const std::string key_ext_correcteds        = "receiveextcorrectables";
    const std::string key_ext_uncorrecteds      = "receiveextuncorrectables";



};