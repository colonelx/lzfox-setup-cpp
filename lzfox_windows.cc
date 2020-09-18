#ifdef _WIN32
#include "lzfox_windows.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include "exception.h"

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
SerialConn::~SerialConn() {
  this->close_conn();
}
string SerialConn::send(string msg){
  int len = msg.length();
  char cmd[len+1];
  strcpy(cmd, msg.c_str());
  DWORD dNoOFBytestoWrite = sizeof(cmd);
  DWORD dNoOfBytesWritten = 0;
  Status = WriteFile(this->serial_port,        // Handle to the Serial port
    cmd,     // Data to be written to the port
    dNoOFBytestoWrite,  //No of bytes to write
    &dNoOfBytesWritten, //Bytes written
    NULL);
  return this->read_data();
}
string SerialConn::send_wait(string msg){}
string SerialConn::send_long(string msg){}

string read_data() {
  Status = SetCommMask(this->serial_port, EV_RXCHAR);
  DWORD dwEventMask; 
  Status = WaitCommEvent(this->serial_port, &dwEventMask, NULL); 
  char TempChar; //Temporary character used for reading
  char SerialBuffer[256];//Buffer for storing Rxed Data
  DWORD NoBytesRead;
  int i = 0;

  do
  {
    ReadFile( this->serial_port, 
              &TempChar,       //Temporary character
              sizeof(TempChar),//Size of TempChar
              &NoBytesRead,    //Number of bytes read
              NULL);

    SerialBuffer[i] = TempChar;// Store Tempchar into buffer
    i++;
  }
  while (NoBytesRead > 0);
  string response(SerialBuffer);
  return response
}
void SerialConn::open_conn() {
  if(this->port.empty()) {
    throw GeneralException("No port was selected!");
  }
  if(this->serial_port > 0) {
    this->close_conn();
  }
  this->serial_port = CreateFile("\\\\.\\" + this->port,
    GENERIC_READ | GENERIC_WRITE, //Read/Write
    0,               // No Sharing
    NULL,            // No Security
    OPEN_EXISTING,   // Open existing port only
    0,               // Non Overlapped I/O
    NULL);
  DCB dcbSerialParams = { 0 }; // Initializing DCB structure
  
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  dcbSerialParams.BaudRate = CBR_38400;  // Setting BaudRate = 38400
  dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
  dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
  dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None

  SetCommState(this->serial_port, &dcbSerialParams);
}
bool SerialConn::check_conn() {
  if (this->serial_port == INVALID_HANDLE_VALUE)
      printf("Error in opening serial port");
  else
      printf("vopening serial port successful");
}
void SerialConn::close_conn() {
  CloseHandle(this->serial_port);
}

#endif