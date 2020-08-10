#ifdef _WIN32
#ifndef LZFOX_WINDOWS_H_
#define LZFOX_WINDOWS_H_

#include <string>

using namespace std;

class SerialConn {
    public:
      SerialConn(string);
      void send(string);
      ~SerialConn();
    //  static const char CMD_SET_TIME[];
    private:
      void open_conn();
      void close_conn();
      bool check_conn();
      int serial_port;
      string port;
};
#endif
#endif