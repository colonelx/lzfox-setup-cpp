#include "lzfoxwindow.h"
#include "stationinfowindow.h"
#include "clock.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include "exception.h"
#include "lzfox.h"
#include <regex>

#ifdef _WIN32
#include <windows.h>
#include "serial_windows.h"
#else
#include <unistd.h>
#include "serial_linux.h"
#endif


using std::vector;
using std::to_string;

LZFoxWindow::LZFoxWindow()
: ui{Gtk::Builder::create_from_file("lzfoxwindow.glade")} 
{

  if(!Glib::thread_supported()) Glib::thread_init();
  if(ui) {
      ui->get_widget<Gtk::Box>("mainBox", mainBox);
      statusbar = Glib::RefPtr<Gtk::Statusbar>::cast_dynamic(
        ui->get_object("status")
      );
      
      lblSystemTime = Glib::RefPtr<Gtk::Label>::cast_dynamic(
          ui->get_object("lblSystemTime")
      );
      selTty = Glib::RefPtr<Gtk::ComboBoxText>::cast_dynamic(
          ui->get_object("selTty")
      );
      btnSetTime = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        ui->get_object("btnSetTime")
      );
      btnResetBackup = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        ui->get_object("btnResetBackup")
      );
      btnGetBackup = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        ui->get_object("btnGetBackup")
      );
      btnWriteInfo = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        ui->get_object("btnWriteInfo")
      );
      iptCardInfo = Glib::RefPtr<Gtk::Entry>::cast_dynamic(
        ui->get_object("iptCardInfo")
      );

      btnSetTime->signal_clicked().connect(
        [this]() {
                    Glib::ustring port = selTty->get_active_text();
                    if(port == "") {
                      this->showSerialDialog();
                    } else {
                      this->btnSetTimeClick();
                    }
                });
      selStationID = Glib::RefPtr<Gtk::ComboBox>::cast_dynamic(
        ui->get_object("selStationID")
      );
      btnResetBackup->signal_clicked().connect(
        [this]() {
          this->btnResetBackupClick();
        }
      );

      btnGetBackup->signal_clicked().connect([this]() { this->btnGetBackupClick(); });
      btnWriteInfo->signal_clicked().connect([this]() { this->btnWriteInfoClick(); });
      Glib::RefPtr<Gtk::ListStore> station_id_tree_model = Gtk::ListStore::create(stationIdColumns);
      selStationID->set_model(station_id_tree_model);
      Gtk::TreeModel::Row row_start = *(station_id_tree_model->append());
      row_start[stationIdColumns.id] = "START";
      row_start[stationIdColumns.name] = "-Start-";
      Gtk::TreeModel::Row row_finish = *(station_id_tree_model->append());
      row_finish[stationIdColumns.id] = "FINISH";
      row_finish[stationIdColumns.name] = "-Finish-";
      for (int i = 1; i <=250; i++) {
        Gtk::TreeModel::Row row = *(station_id_tree_model->append()); 
        row[stationIdColumns.id] = to_string(i);
        row[stationIdColumns.name] = "Control: " + to_string(i);
      }
      selStationID->pack_start(stationIdColumns.name);
      selStationID->set_entry_text_column(stationIdColumns.id);

      btnSetControlMode = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        ui->get_object("btnSetControlMode")
      );
      btnGetInfo = Glib::RefPtr<Gtk::Button>::cast_dynamic(
        ui->get_object("btnGetInfo")
      );
      selControlMode = Glib::RefPtr<Gtk::ComboBox>::cast_dynamic(
        ui->get_object("selControlMode")
      );
      btnSetControlMode->signal_clicked().connect(
        [this]() {
          Glib::ustring umode = selControlMode->get_active_id();
          string mode = string(umode);
          string control;
          if (mode.compare("CONTROL") == 0) {
              Gtk::TreeModel::iterator iter = selStationID->get_active();
              if(iter)
              {
                Gtk::TreeModel::Row row = *iter;
                control = row[stationIdColumns.id];
              }
          }
          this->btnSetControlModeClick(mode, control);
        }
      );
      btnGetInfo->signal_clicked().connect(
        [this]() {
          this->showStationInfoDialog();
        }
      );

    add(*mainBox);
  }
  set_title("LZFox");
  set_default_size(400, 250);
  show_all();
  this->populateSerialPorts();
  std::thread* clockThread = new std::thread(
    [this]
    {
      clock.start(this);
    }
  );
}

void LZFoxWindow::showStationInfoDialog() {
  try {
    auto port = this->get_port();
    LZFox lzfox = LZFox(port);
    string stID = lzfox.getID();
    string stTime = lzfox.getTime();
    string stMode = lzfox.getMode();
    string stVersion = lzfox.getVersion();
    string stVoltage = lzfox.getVoltage();
    stationInfoWindow.lblStID->set_text(stID);
    stationInfoWindow.lblStTime->set_text(stTime);
    stationInfoWindow.lblStMode->set_text(stMode);
    stationInfoWindow.lblStVersion->set_text(stVersion);
    stationInfoWindow.lblStVoltage->set_text(stVoltage);
    stationInfoWindow.show();
  } catch (GeneralException& ex) {
    this->showExceptionDialog(ex.what());
  } 
}


void LZFoxWindow::showSerialDialog() {
  Gtk::MessageDialog dialog(*this, "No Serial port was selected!",
          false /* use_markup */, Gtk::MESSAGE_WARNING,
          Gtk::BUTTONS_OK);
  dialog.set_secondary_text(
          "Before running a command you must select a serial port.");

  int result = dialog.run();
}

void LZFoxWindow::showExceptionDialog(string str) {
  Gtk::MessageDialog dialog(*this, "General Exception",
          false /* use_markup */, Gtk::MESSAGE_ERROR,
          Gtk::BUTTONS_OK);
  dialog.set_secondary_text(str);

  int result = dialog.run();
}

void LZFoxWindow::btnSetTimeClick() {
  try {
    auto port = this->get_port();
    LZFox lzfox = LZFox(port);
    string result = lzfox.setTime();
    statusbar->push("Response: " + result);
  } catch (GeneralException& ex) {
    this->showExceptionDialog(ex.what());
  }
}

void LZFoxWindow::btnGetBackupClick() {
  try {
    auto port = this->get_port();
    LZFox lzfox = LZFox(port);
    string result = lzfox.getBackup();
    this->showSaveDialog(result);
  } catch (GeneralException& ex) {
    this->showExceptionDialog(ex.what());
  }
}

void LZFoxWindow::showSaveDialog(string content) {
  Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for(*this);

  //Add response buttons the the dialog:
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Save", Gtk::RESPONSE_OK);

  //Add filters, so that only certain file types can be selected:

  auto filter_text = Gtk::FileFilter::create();
  filter_text->set_name("Text files");
  filter_text->add_mime_type("text/plain");
  dialog.add_filter(filter_text);

  auto filter_cpp = Gtk::FileFilter::create();
  filter_cpp->set_name("C/C++ files");
  filter_cpp->add_mime_type("text/x-c");
  filter_cpp->add_mime_type("text/x-c++");
  filter_cpp->add_mime_type("text/x-c-header");
  dialog.add_filter(filter_cpp);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  //Show the dialog and wait for a user response:
  int result = dialog.run();

  //Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      
      std::string filename = dialog.get_filename();
      std::ofstream _file;
      _file.open (filename);
      _file << content;
      _file.close();
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      break;
    }
    default:
    {
      std::cout << "Unexpected button clicked." << std::endl;
      break;
    }
  }
}

void LZFoxWindow::btnWriteInfoClick() {
  try {
    auto port = this->get_port();
    string mode, id;
    LZFox lzfox = LZFox(port);
    string mode_raw = lzfox.getMode();
    string id_raw = lzfox.getID();

    const std::regex re(".*:(.*)>");
    std::smatch re_match;
    if (std::regex_match(mode_raw, re_match, re)) {
      if (re_match.size() == 2) {
        std::ssub_match match = re_match[1];
        mode = match.str();
      }
    } else {
      throw GeneralException("Cannot find MODE!");
    }
    if (std::regex_match(id_raw, re_match, re)) {
      if (re_match.size() == 2) {
        std::ssub_match match = re_match[1];
        id = match.str();
      }
    } else {
      throw GeneralException("Cannot find CNTR number!");
    }
    lzfox.setMode("WRITER");
    string info = string(iptCardInfo->get_text());
    statusbar->push("Waiting to write to card ...");
    string result = lzfox.writeInfo(info);
    statusbar->push("Response: " + result);
    lzfox.setMode("CONTROL");
    lzfox.setControl(id);
    lzfox.setMode(mode);
  } catch (GeneralException& ex) {
    this->showExceptionDialog(ex.what());
  }
}

Glib::ustring LZFoxWindow::get_port() {
  Glib::ustring port = selTty->get_active_text();
  return port;
}

void LZFoxWindow::btnSetControlModeClick(string mode, string control){
  try {
    auto port = this->get_port();
    LZFox lzfox = LZFox(port);
    string result = lzfox.setMode(mode);
    statusbar->push(result);
    if(control.compare("") != 0) {
      string result = lzfox.setControl(control);
      statusbar->push("Response: " + result);
    }
  } catch (GeneralException& ex) {
    this->showExceptionDialog(ex.what());
  }
}

void LZFoxWindow::btnResetBackupClick() {
  try {
    auto port = this->get_port();
    LZFox lzfox = LZFox(port);
    string result = lzfox.resetBackup();
    statusbar->push("Response: " + result);
  } catch (GeneralException& ex) {
    this->showExceptionDialog(ex.what());
  }
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