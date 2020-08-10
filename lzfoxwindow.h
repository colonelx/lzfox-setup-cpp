#ifndef LZFOXWINDOW_H
#define LZFOXWINDOW_H

#include "clock.h"
#include "station_id.h"
#include "stationinfowindow.h"
#include <thread>
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/combobox.h>
#include <gtkmm/entry.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/builder.h>
#include <stdio.h>

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
  Glib::RefPtr<Gtk::ComboBox> selControlMode;
  Glib::RefPtr<Gtk::ComboBox> selStationID;
  Glib::RefPtr<Gtk::Button> btnSetControlMode;
  Glib::RefPtr<Gtk::Button> btnGetInfo;
  Glib::RefPtr<Gtk::Button> btnGetBackup;
  Glib::RefPtr<Gtk::Button> btnResetBackup;
  Glib::RefPtr<Gtk::Entry> iptCardInfo;
  Glib::RefPtr<Gtk::Button> btnWriteInfo;
  Glib::RefPtr<Gtk::Statusbar> statusbar;
  
  Glib::RefPtr<Gtk::Builder> ui;

  
  void updateClock();
  void showSerialDialog();
  void showExceptionDialog(std::string);
  void showStationInfoDialog();
  virtual ~LZFoxWindow();

protected:
  // Glib::Dispatcher clockDispatcher;
  Clock clock;
  std::thread *clockThread;
  void populateSerialPorts();
  void btnSetTimeClick();
  Glib::ustring get_port();
  void btnSetControlModeClick(std::string, std::string);
  void btnResetBackupClick();
  void btnGetBackupClick();
  void btnWriteInfoClick();
  StationIDColumns stationIdColumns;
  StationInfoWindow stationInfoWindow;
};

#endif
