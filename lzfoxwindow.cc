#include "lzfoxwindow.h"
#include "clock.h"
#include <iostream>
#include <thread>
#include <ctime>  

#ifdef _WIN32
#include <windows.h>
#include "serial_windows.h"
#else
#include <unistd.h>
#include "serial_linux.h"
#endif


using std::vector;

LZFoxWindow::LZFoxWindow()
: ui{Gtk::Builder::create_from_file("lzfoxwindow.glade")} 
{

  if(!Glib::thread_supported()) Glib::thread_init();
  if(ui) {
      ui->get_widget<Gtk::Box>("mainBox", mainBox);
      lblSystemTime = Glib::RefPtr<Gtk::Label>::cast_dynamic(
          ui->get_object("lblSystemTime")
      );
      selTty = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
          ui->get_object("selTty")
      );
    add(*mainBox);
  }
  set_title("LZFox");
  set_default_size(400, 400);
  show_all();
  this->populateSerialPorts();
  std::thread* clockThread = new std::thread(
    [this]
    {
      clock.start(this);
    }
  );
}

void LZFoxWindow::updateClock() {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    lblSystemTime->set_text(std::ctime(&end_time));
}

void LZFoxWindow::populateSerialPorts() {
    vector<PortInfo> ports = Serial::list_ports();
    vector<PortInfo>::iterator iter = ports.begin();
     while( iter != ports.end() )
    {
        PortInfo device = *iter++;
        std::cout << device.port << std::endl;
        selTty->append( Glib::ustring{device.port});
    }
}

LZFoxWindow::~LZFoxWindow()
{
}