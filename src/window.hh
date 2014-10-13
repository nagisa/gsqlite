#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/headerbar.h>

namespace GSQLiteui {

class Window : public Gtk::ApplicationWindow {
    Gtk::HeaderBar header;

    public:
        Window();
        ~Window();
};

}
