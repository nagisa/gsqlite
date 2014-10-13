#include <gtkmm/application.h>
#include <iostream>

#include "window.hh"

namespace sigc {
    SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE
}

int main(int argc, char** argv){
    auto app = Gtk::Application::create("org.apps.gSQLite");

    app->signal_activate().connect([&app](){
        auto window = new GSQLiteui::Window();
        window->set_application(app);
        window->show_all();
        window->signal_delete_event().connect([window](GdkEventAny *e){
            delete window;
            return true;
        });
    });

    return app->run(argc, argv);
}
