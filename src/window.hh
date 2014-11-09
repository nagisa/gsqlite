#pragma once

#include <gtkmm.h>
#include <glibmm.h>
#include <sqlitemm/sqlitemm.hh>

#include "sqlview.hh"
#include "tableview.hh"

namespace GSQLiteui {

/// Main application window
class Window : public Gtk::ApplicationWindow {
    // Header
    Gtk::HeaderBar                        header;
    Gtk::Button                           primary_button;
    // Content
    Gtk::Stack                            stack;
    Gtk::StackSwitcher                    stack_switcher;
    std::unique_ptr<TableView>            tables;
    std::unique_ptr<SQLView>              sqlview;

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
