// File: main.cc

#include "lzfoxwindow.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "com.benchparty.lzfox");

  LZFoxWindow lzfoxwindow;

  //Shows the window and returns when it is closed.
  return app->run(lzfoxwindow);
}
