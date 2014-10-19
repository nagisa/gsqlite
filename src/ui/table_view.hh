#pragma once

#include <gtkmm.h>
#include <glibmm.h>

#include "../sigc_common.hh"
#include "../connection.hh"

namespace GSQLiteui {

class TableViewColumnRecord : public Gtk::TreeModel::ColumnRecord {
    public:
        Gtk::TreeModelColumn<Glib::ustring> table_name;
        TableViewColumnRecord();
};

/// View for tables in database
class TableView : public Gtk::TreeView {
    public:
        TableViewColumnRecord        columns;
        Glib::RefPtr<Gtk::ListStore> store;

        TableView();
        ~TableView();

        void refresh(Connection&);
};

}
