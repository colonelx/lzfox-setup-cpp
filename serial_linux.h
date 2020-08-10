#if defined(__linux__)

#include <vector>
#include <string>
#include "lzfox.h"

using std::vector;
using std::string;

class Serial {
  public:
    Serial();
    static vector<PortInfo> list_ports();
};


#endif // defined(__linux__)