#pragma once

#include <gtkmm.h>
#include <glibmm.h>
#include <sqlitemm/sqlitemm.hh>

#include "table_view.hh"

namespace GSQLiteui {

/// Main application window
class Window : public Gtk::ApplicationWindow {
    // Header
    Gtk::HeaderBar                        header;
    Gtk::Button                           primary_button;
    // Content
    Gtk::Stack                            stack;
    std::unique_ptr<GSQLiteui::TableView> tables;

    // ETC
    Glib::Property<Glib::RefPtr<Gio::File>> database_file;
    Glib::Property<std::shared_ptr<Connection>> connection;

    public:
        Window();
        ~Window();

    private:
        void open_file_dialog();
        void update_header();
        void reconnect();

};

/// Dialog to open a database file.
class OpenDatabaseDialog : public Gtk::FileChooserDialog {
    public:
        OpenDatabaseDialog(Gtk::Window &);
        ~OpenDatabaseDialog();
};

}
