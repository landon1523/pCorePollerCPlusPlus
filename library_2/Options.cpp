#include <string>

class Options {

public:
    Options();

    std::string get();

    std::string* parse();

    std::string printHelp();

private:

    bool checkReqOpts();
    std::string findVar();


    const std::string program_name = "Device Poller";

    const std::string* opts;
    /*array(
    'h::' => 'help',
    'H:' => 'host:',
    'f:' => 'function:',
    'o:' => 'output:',
    'O:' => 'object:',
    'C:' => 'community',
    'v:' => 'version',
    'w:' => 'warn:',
    'c:' => 'crit:',
    'i:' => 'interface:',
    'r:' => 'vrf:',
    'b:' => 'bgp-peer:',
    's:' => 'upstream-start:',
    'e:' => 'upstream-end:',
    'n:' => 'node-name:',
    'm:' => 'mode:',
    'd:' => 'display:',
    'u:' => 'uom:',
    'p:' => 'parentAddress');*/

    const std::string* required_opts;
    /*array(
    'host',
    'function',
    'output');*/



};