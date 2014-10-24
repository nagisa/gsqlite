#include <giomm/file.h>

#include "window.hh"

using namespace GSQLiteui;

Window::Window()
    : Glib::ObjectBase("gsqlite_Window")
    , Gtk::ApplicationWindow()
    , database_file(*this, "database-file")
    , connection(*this, "connection")
{
    this->set_titlebar(this->header);
    this->primary_button.set_label("Open");
    this->primary_button.get_style_context()->add_class("suggested-action");
    this->header.pack_start(this->primary_button);
    this->add(this->stack);

    this->primary_button.signal_clicked().connect([&](){
        this->open_file_dialog();
    });
    this->database_file.get_proxy().signal_changed().connect([&](){
        this->update_header();
        this->reconnect();
    });
    this->connection.get_proxy().signal_changed().connect([&](){
        if(this->tables)
            this->stack.remove(*this->tables);
        this->tables.reset(new GSQLiteui::TableView(this->connection.get_value()));
        this->tables->show_all();
        this->stack.add(*this->tables, "tables", "Tables");
        this->stack.set_visible_child(*this->tables);
    });

    this->update_header();
};

Window::~Window()
{
};

void
Window::open_file_dialog()
{
    OpenDatabaseDialog chooser(*this);
    chooser.set_local_only(true);
    chooser.signal_response().connect([&](int resp){
        if(resp == Gtk::RESPONSE_ACCEPT)
            this->database_file = chooser.get_file();
    });
    chooser.run();
}

void
Window::update_header()
{
    this->header.set_show_close_button(true);
    auto file = this->database_file.get_value();
    if(file){
        if(file->is_native())
            this->header.set_subtitle(file->get_path());
        else
            this->header.set_subtitle(file->get_uri());
    } else {
        this->header.set_subtitle("");
    }
    this->header.set_title("gSQLite");
}

void
Window::reconnect()
{
    auto fname = this->database_file.get_value()->get_path();
    {
    this->connection.set_value(Connection::create(fname.c_str()));
    }
}

OpenDatabaseDialog::OpenDatabaseDialog(Gtk::Window &w)
    : Glib::ObjectBase("gsqlite_OpenDatabaseDialog")
    , Gtk::FileChooserDialog(w, "Pick a SQLite file")
{
    this->add_button("Cancel", Gtk::RESPONSE_REJECT);
    this->add_button("Open", Gtk::RESPONSE_ACCEPT);
    auto sqlitefilter = Gtk::FileFilter::create();
    auto anyfilter = Gtk::FileFilter::create();
    sqlitefilter->set_name("SQLite files");
    sqlitefilter->add_custom(Gtk::FILE_FILTER_URI, [&](const Gtk::FileFilter::Info& finfo){
        char buffer[16];
        const char *ref = "SQLite format 3\000";
        auto file = Gio::File::create_for_uri(finfo.uri);
        try {
            if(file->read()->read(buffer, 16) != 16) return false;
        } catch (Glib::Error& e){
            // Most likely we are checking a directory.
            return true;
        }
        for(size_t i = 0; i < 16; i += 1)
            if(buffer[i] != ref[i]) return false;
        return true;
    });
    anyfilter->set_name("All files");
    anyfilter->add_pattern("*");
    this->add_filter(sqlitefilter);
    this->add_filter(anyfilter);
};

OpenDatabaseDialog::~OpenDatabaseDialog()
{
};
