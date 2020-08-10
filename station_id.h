#include <string>
#include <gtkmm/window.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>

class StationIDColumns : public Gtk::TreeModel::ColumnRecord
{
  public:
    StationIDColumns();
    Gtk::TreeModelColumn<std::string> id;
    Gtk::TreeModelColumn<Glib::ustring> name;
};