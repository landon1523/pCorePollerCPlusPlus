
class SNMP_Response {

public:
    SNMP_Response();

    int getIndex() { return this->index; }
    int getValue() { return this->value; }
    int getType() { return this->type; }
private:
    int index, value, type;

};