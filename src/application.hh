#pragma once

#include <gtkmm/application.h>

namespace GSQLiteui {

class Application : public Gtk::Application {
    public:
    Application();
    ~Application();

    protected:
    void on_startup();
    void on_activate();
    void on_show_about();
};

}
