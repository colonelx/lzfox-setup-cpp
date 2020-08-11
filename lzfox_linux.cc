#if defined(__linux__)

#include <string>
#include <iostream>
#include <cstring>
#include "lzfox_linux.h"
#include <unistd.h>
#include "exception.h"
#include <thread> 

using namespace std;
SerialConn::SerialConn(string port) {
  if(port.empty()) {
    throw GeneralException("No port was selected!");
  }
  this->port = port;
  this->open_conn();
  if (!check_conn()) {
    throw GeneralException("Invalid connection!");
  }
}

bool SerialConn::check_conn() {
  char cmd[] = {'P','I','N','G','\n'};
  bool valid_response = false;
  int i = 0;
  while (!valid_response && i < 10) {
    i++;
    write(this->serial_port, cmd, sizeof(cmd));
    char read_buf [9];
    memset(&read_buf, '\0', sizeof(read_buf));
    int num_bytes = read(this->serial_port, &read_buf, sizeof(read_buf));
    string response(read_buf);
    if (response.compare("\r\nPONG\r\n>") == 0){
      valid_response = true;
    }
  };
  return valid_response;
}

void SerialConn::open_conn() {
  if(this->port.empty()) {
    throw GeneralException("No port was selected!");
  }
  if(this->serial_port > 0) {
    this->close_conn();
  }
  this->serial_port = open(("/dev/" + this->port).c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  
  struct termios tty;
  memset(&tty, 0, sizeof tty);

  if(tcgetattr(this->serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  tty.c_cc[VTIME] = 100;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  cfsetispeed(&tty, B38400);
  cfsetospeed(&tty, B38400);

  if (tcsetattr(this->serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
  }
}

void SerialConn::close_conn() {
  close(this->serial_port);
}

SerialConn::~SerialConn() {
  close_conn();
}

string SerialConn::send(string msg) {
  
  int len = msg.length();
  char cmd[len+1];
  strcpy(cmd, msg.c_str());
  write(this->serial_port, cmd, sizeof(cmd));
  return this->read_data();
}

string SerialConn::read_data() {
  char read_buf [256];
  memset(&read_buf, '\0', sizeof(read_buf));
  int num_bytes = read(this->serial_port, &read_buf, sizeof(read_buf));
  if (num_bytes < 0) {
    printf("Error reading: %s\n", strerror(errno));
  }
  string response(read_buf);
  return response;
}

string SerialConn::send_wait(string msg) {
  int len = msg.length();
  char cmd[len+1];
  strcpy(cmd, msg.c_str());
  write(this->serial_port, cmd, sizeof(cmd));
  bool loop = true;
  string result;
  result = this->read_data();
  cout << result;
  while(loop) {
    result = this->read_data();
    cout << result;
    size_t found = result.rfind("OK"); 
    if (found != std::string::npos) {
      loop = false;
    }
    sleep(1);
  }
  return result;
}

string SerialConn::send_long(string msg) {
  int len = msg.length();
  char cmd[len+1];
  strcpy(cmd, msg.c_str());
  write(this->serial_port, cmd, sizeof(cmd));
  string response;
  bool loop = true;
  char read_buf [255];
  while(loop) {
    memset(&read_buf, '\0', sizeof(read_buf));
    int num_bytes = read(this->serial_port, &read_buf, sizeof(read_buf));
    if (num_bytes < 0) {
      printf("Error reading: %s\n", strerror(errno));
    }

    string res(read_buf);
    response += res;
    cout << res;
    size_t found = res.rfind("$");
    if (found != std::string::npos) {
      loop = false;
    }
  }
  return response;
}
#endif