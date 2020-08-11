#include <string>
#include <iostream>
#include <regex>
#include "lzfox.h"

#include <gtkmm/statusbar.h>
#include <ctime>

using namespace std;

LZFox::LZFox(Glib::ustring port): conn(port) {}


string LZFox::send(string message) {
  string result = this->conn.send(message);
  std::regex newlines_re("\r\n");
  auto ret = std::regex_replace(result, newlines_re, "");
  return ret;
}

string LZFox::send_wait(string message) {
  string result = this->conn.send_wait(message);
  std::regex newlines_re("\r\n");
  auto ret = std::regex_replace(result, newlines_re, "");
  return ret;
}

string LZFox::send_long(string message) {
  string result = this->conn.send_long(message);
  // std::regex newlines_re("\r\n");
  // auto ret = std::regex_replace(result, newlines_re, "");
  return result;
}

string LZFox::setTime() {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];
  time (&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer,sizeof(buffer),"%Y%m%d%H%M%S",timeinfo);
  std::string strTime(buffer);
  return this->send("SET TIME " + strTime + "\r\n");
}

string LZFox::setMode(string mode) {
  return this->send("SET MODE " + mode + "\r\n");
} 

string LZFox::setControl(string control_id) {
  return this->send("SET CTRL " + control_id + "\r\n");
}

string LZFox::getID() {
  return this->send("GET CTRL\r\n");
}
string LZFox::getTime() {
  return this->send("GET TIME\r\n");
}
string LZFox::getMode() {
  return this->send("GET MODE\r\n");
}
string LZFox::getVersion() {
  return this->send("GET VERSION\r\n");
}
string LZFox::getVoltage() {
  return this->send("GET VOLTAGE\r\n");
}

string LZFox::resetBackup() {
  return this->send("SET RESET_BACKUP\r\n");
}

string LZFox::setWriter() {
  return this->send("SET WRITER\r\n");
}

string LZFox::writeInfo(string info) {
  return this->send_wait("WRITE INFO " + info + "\r\n");
}

string LZFox::getBackup() {
  return this->send_long("GET BACKUP\r\n");
}