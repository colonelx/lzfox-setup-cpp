#if defined(__linux__)

#include "serial_linux.h"
#include "lzfox.h"

#include <vector>
#include <string>
#include <iostream>
#include <glob.h>
#include <fstream>


Serial::Serial() {}

using std::istringstream;
using std::ifstream;
using std::vector;
using std::string;
using std::cout;
using std::endl;

string read_line(const string& file)
{
  ifstream ifs(file.c_str(), ifstream::in);
  string line;
  if(ifs) {
    getline(ifs, line);
  }
  return line;
}
string dirname(const string& path)
{
  size_t pos = path.rfind("/");
  if(pos == std::string::npos)
    return path;
  else if(pos == 0)
    return "/";
  return string(path, 0, pos);
}

vector<PortInfo> Serial::list_ports() {
  vector<string> paths_found;
  glob_t glob_results;
  int glob_retval = glob("/dev/ttyUSB*", 0, NULL, &glob_results);
  cout << "Found: " << glob_results.gl_pathc << endl;
  for(int path_index = 0; path_index < glob_results.gl_pathc; path_index++) {
    paths_found.push_back(glob_results.gl_pathv[path_index]);
  }

  globfree(&glob_results);
  vector<PortInfo> results;
  vector<string>::iterator iter = paths_found.begin();    
  while( iter != paths_found.end() ) {
    string device = *iter++;
    string device_name;
    size_t pos = device.rfind("/");
    if(pos == std::string::npos)
      device_name = device;
    else
      device_name = string(device, pos+1, string::npos);    
    string sys_device_path = "/sys/class/tty/" + string(device_name) + "/device";
    sys_device_path =  dirname( string(sys_device_path) );
    cout << sys_device_path << endl;
    string manufacturer = read_line( sys_device_path + "/manufacturer" );
    string product = read_line( sys_device_path + "/product" );
    string serial = read_line( sys_device_path + "/serial" );

    string serial_number = read_line( sys_device_path + "/serial" );
    if( serial_number.length() > 0 )
    {
      serial_number = "SNR=" + string(serial_number);
    }
    string vid = read_line( sys_device_path + "/idVendor" );
    string pid = read_line( sys_device_path + "/idProduct" );
    PortInfo device_entry;
    device_entry.port = device_name;
    device_entry.description = string(manufacturer) + " " + string(product) + " " + string(serial);
    device_entry.hardware_id = "USB VID:PID=" + string(vid) + ":" + string(pid) + " " + string(serial_number);
    results.push_back( device_entry );
  }
  return results;
}

#endif // defined(__linux__)