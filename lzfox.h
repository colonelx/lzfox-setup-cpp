#ifndef LZFOX_H_
#define LZFOX_H_
#include <string>
#include <glibmm/ustring.h>
#ifdef __linux__
#include "lzfox_linux.h"
#elif _WIN32
#include "lzfox_windows.h"
#endif
using namespace std;

struct PortInfo {
  string port;
  string description;
  string hardware_id;
};
class LZFox{
    public:
      LZFox(Glib::ustring);
      bool connect(Glib::ustring);
      string setTime();
      string setControl(string);
      string setMode(string);
      string setWriter();
      string writeInfo(string);
      string getTime();
      string getMode();
      string getID();
      string getVersion();
      string getVoltage();
      string getBackup();
      string resetBackup();
    private:
      bool make_call();
      string send(string);
      string send_wait(string);
      string send_long(string);
      SerialConn conn;

};
#endif