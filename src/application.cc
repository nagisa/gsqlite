#include <glibmm.h>
#include <sqlitemm/private/sigc_fix.hh>

#include "window.hh"
#include "application.hh"

using namespace GSQLiteui;

Application::Application()
    : Glib::ObjectBase("gsqlite_Application")
    , Gtk::Application("org.apps.gSQLite")
{
    auto openaction = Gio::SimpleAction::create("open");
    auto aboutaction = Gio::SimpleAction::create("about");
    auto quitaction = Gio::SimpleAction::create("quit");

    quitaction->signal_activate().connect([this](const Glib::VariantBase&){
        this->quit();
    });

    aboutaction->signal_activate().connect([this](const Glib::VariantBase&){
        this->on_show_about();
    });

    // this->add_action(openaction);
    this->add_action(aboutaction);
    this->add_action(quitaction);
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
    auto window = new Window();
    this->add_window(*window);
    window->show_all();
    window->signal_delete_event().connect([window](GdkEventAny *e){
        delete window;
        return true;
    });
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
