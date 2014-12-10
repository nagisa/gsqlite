#include <glibmm.h>
#include <sqlitemm/private/sigc_fix.hh>

#include "window.hh"
#include "application.hh"
#include "actions.hh"

using namespace GSQLiteui;

Window* create_window(){
    auto window = new Window();
    window->signal_delete_event().connect([window](GdkEventAny *e){
        delete window;
        return true;
    });
    return window;
};

Application::Application()
    : Glib::ObjectBase("gsqlite_Application")
    , Gtk::Application("org.apps.gSQLite")
{

    quit_action.get()->signal_activate().connect([this](const Glib::VariantBase&){
        this->quit();
    });

    about_action.get()->signal_activate().connect([this](const Glib::VariantBase&){
        this->on_show_about();
    });

    open_action.get()->signal_activate().connect([this](const Glib::VariantBase&){
        auto window = create_window();
        this->add_window(*window);
        window->open_file_dialog();
    });

    this->add_action(open_action.get());
    this->add_action(about_action.get());
    this->add_action(quit_action.get());
}

Application::~Application()
{
};

void
Application::on_startup()
{
    Gtk::Application::on_startup();
    auto menu = Gio::Menu::create();
    auto openitem = Gio::MenuItem::create("Open a database", "app.open");
    auto aboutitem = Gio::MenuItem::create("About", "app.about");
    auto quititem = Gio::MenuItem::create("Quit", "app.quit");
    menu->append_item(openitem);
    menu->append_item(aboutitem);
    menu->append_item(quititem);
    this->set_app_menu(menu);
}

void
Application::on_activate()
{
    Gtk::Application::on_activate();
    auto window = create_window();
    this->add_window(*window);
    window->open_file_dialog();
}

void
Application::on_show_about()
{
    auto dialog = new AboutDialog();
    dialog->set_transient_for(*this->get_active_window());
    dialog->set_logo_icon_name(this->get_active_window()->get_icon_name());
    dialog->show_all();
    dialog->signal_response().connect([dialog](int code){
        delete dialog;
    });
}
