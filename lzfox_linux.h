#ifndef LZFOX_LINUX_H_
#define LZFOX_LINUX_H_

#include <string>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

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
      int serial_port;
      string port;
      
      

};
#endif