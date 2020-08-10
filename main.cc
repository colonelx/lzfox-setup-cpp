// File: main.cc

#include "lzfoxwindow.h"
#include "stationinfowindow.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "com.benchparty.lzfox");

  LZFoxWindow lzfoxwindow;
  //StationInfoWindow stInfoWindow;
  //Shows the window and returns when it is closed.
  //app->add_window(stInfoWindow);
  return app->run(lzfoxwindow);
}
