#if defined(__linux__)

#include <vector>
#include <string>


using std::vector;
using std::string;

struct PortInfo {
  string port;
  string description;
  string hardware_id;

};
class Serial {
    public:
        Serial();
        static vector<PortInfo> list_ports();
};


#endif // defined(__linux__)