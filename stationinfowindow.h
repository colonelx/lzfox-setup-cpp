#ifndef STATIONINFOWINDOW_H
#define STATIONINFOWINDOW_H

#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <stdio.h>

class StationInfoWindow : public Gtk::ApplicationWindow {
  public:
    StationInfoWindow();
    Gtk::Box *stInfoBox;
    virtual ~StationInfoWindow();
    Glib::RefPtr<Gtk::Box> boxStInfo;
    Glib::RefPtr<Gtk::Label> lblStTime;
    Glib::RefPtr<Gtk::Label> lblStMode;
    Glib::RefPtr<Gtk::Label> lblStID;
    Glib::RefPtr<Gtk::Label> lblStVersion;
    Glib::RefPtr<Gtk::Label> lblStVoltage;
    Glib::RefPtr<Gtk::Builder> stui;
};
#endif