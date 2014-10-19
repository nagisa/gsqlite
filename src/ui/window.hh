#pragma once

#include <gtkmm.h>
#include <glibmm.h>

#include "table_view.hh"
#include "../sigc_common.hh"
#include "../connection.hh"

namespace GSQLiteui {

/// Main application window
class Window : public Gtk::ApplicationWindow {
    Gtk::HeaderBar              header;
    Gtk::Button                 primary_button;
    Gtk::Paned                  paned;
    /* List of tables */
    Gtk::ScrolledWindow         table_scrolled;
    GSQLiteui::TableView        table_view;

    Gtk::Button                 button;
    Glib::Property<Glib::RefPtr<Gio::File>> database_file;
    Glib::Property<Connection*> connection;

    void open_file_dialog();
    void update_header();
    void reconnect();

    public:
        Window();
        ~Window();
};

/// Dialog to open a database file.
class OpenDatabaseDialog : public Gtk::FileChooserDialog {
    public:
        OpenDatabaseDialog(Gtk::Window &);
        ~OpenDatabaseDialog();
};

}
