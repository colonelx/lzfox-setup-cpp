#ifndef STATIONINFOWINDOW_CC
#define STATIONINFOWINDOW_CC

#include "stationinfowindow.h"

StationInfoWindow::StationInfoWindow()
: stui{Gtk::Builder::create_from_file("lzfoxwindow.glade")} 
{
    if(stui) {
      stui->get_widget<Gtk::Box>("boxStInfo", stInfoBox);
      lblStID = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        stui->get_object("lblStID")
      );
      lblStMode = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        stui->get_object("lblStMode")
      );
      lblStTime = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        stui->get_object("lblStTime")
      );
      lblStVersion = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        stui->get_object("lblStVersion")
      );
      lblStVoltage = Glib::RefPtr<Gtk::Label>::cast_dynamic(
        stui->get_object("lblStVoltage")
      );
      add(*stInfoBox);
    }
    set_title("Info");
    set_default_size(400, 400);
    hide();
}

StationInfoWindow::~StationInfoWindow() {}
#endif