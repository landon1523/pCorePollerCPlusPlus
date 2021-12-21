#include<string>

class Conversion {

public:
    Conversion();
    std::string data();
    std::string time(std::string t) { return t; }

    float temperature();
    std::string getType();

private:
    const int byte_conversion		        =	1024;
    const int bit_conversion		        =	1000;
    const std::string uom_factor;
    const std::string uom_bytes				=	"b";
    const std::string uom_bits				=	"b";
    const std::string uom_kilo				=	"k";
    const std::string uom_mega				=	"m";
    const std::string uom_giga				=	"g";
    const std::string uom_tera				=	"t";
    const std::string uom_per_second		=	"ps";
    const std::string uom_type;
    const std::string uom_base;
    const std::string uom_temp_display		=	"f";


};