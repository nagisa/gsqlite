#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/filechooserdialog.h>

#include "common.hh"

namespace GSQLiteui {

/// Main application window
class Window : public Gtk::ApplicationWindow {
    Gtk::HeaderBar header;
    Gtk::Button    primary_button;

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
