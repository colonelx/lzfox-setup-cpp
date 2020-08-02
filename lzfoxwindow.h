#ifndef LZFOXWINDOW_H
#define LZFOXWINDOW_H

#include "clock.h"
#include <thread>
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/builder.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class LZFoxWindow : public Gtk::ApplicationWindow
{

public:
  LZFoxWindow();
  Gtk::Box *mainBox;
  Glib::RefPtr<Gtk::Box> box1Row;
  Glib::RefPtr<Gtk::Box> box2Row;
  Glib::RefPtr<Gtk::Box> box3Row;
  Glib::RefPtr<Gtk::Box> box4Row;
  Glib::RefPtr<Gtk::Box> box5Row;
  Glib::RefPtr<Gtk::ComboBoxText> selTty;
  Glib::RefPtr<Gtk::Entry> iptBaudRate;
  Glib::RefPtr<Gtk::Label> lblSystemTime;
  Glib::RefPtr<Gtk::Button> btnSetTime;
  Glib::RefPtr<Gtk::Button> selControlMode;
  Glib::RefPtr<Gtk::ComboBoxText> selStationID;
  Glib::RefPtr<Gtk::Button> btnSetControlMode;
  Glib::RefPtr<Gtk::Button> btnGetInfo;
  Glib::RefPtr<Gtk::Button> btnGetBackup;
  Glib::RefPtr<Gtk::Button> btnResetBackup;
  Glib::RefPtr<Gtk::Entry> iptCardInfo;
  Glib::RefPtr<Gtk::Button> btnWriteInfo;
  Glib::RefPtr<Gtk::Statusbar> status;
  
  Glib::RefPtr<Gtk::Builder> ui;
  void updateClock();
  virtual ~LZFoxWindow();

protected:
  // Glib::Dispatcher clockDispatcher;
  Clock clock;
  std::thread *clockThread;

  void populateSerialPorts();
};

#endif
