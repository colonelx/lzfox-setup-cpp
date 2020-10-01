#ifdef _WIN32
#ifndef LZFOX_WINDOWS_H_
#define LZFOX_WINDOWS_H_

#include <string>
#include <errno.h> // Error integer and strerror() function
#include <windows.h>

using namespace std;

class SerialConn {
    public:
      SerialConn(string);
      string send(string);
      string send_wait(string);
      string send_long(string);
      ~SerialConn();
    //  static const char CMD_SET_TIME[];
    private:
      void open_conn();
      void close_conn();
      bool check_conn();
      string read_data();
      HANDLE serial_port;
      string port;
};
#endif
#endif