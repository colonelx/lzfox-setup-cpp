#ifdef _WIN32
#include "lzfox_windows.h"

SerialConn::SerialConn(string port) {}
SerialConn::~SerialConn() {}
void SerialConn::send(string msg){}
void SerialConn::send_wait(string msg){}

string read_data() {}
void SerialConn::open_conn() {}
bool SerialConn::check_conn() {}
void SerialConn::close_conn() {}

#endif