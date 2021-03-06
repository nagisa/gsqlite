#include <giomm/file.h>
#include <sqlitemm/private/sigc_fix.hh>

#include "window.hh"

using namespace GSQLiteui;

Window::Window()
    : Glib::ObjectBase("gsqlite_Window")
    , Gtk::ApplicationWindow()
    , database_file(*this, "database-file")
    , connection(*this, "connection")
{
    this->set_default_size(500, 300);
    this->set_titlebar(this->header);
    this->set_icon_name("system-run");
    this->header.pack_start(this->stack_switcher);
    this->stack_switcher.set_stack(this->stack);
    this->stack_switcher.set_no_show_all(true);
    this->add(this->stack);

    this->database_file.get_proxy().signal_changed().connect([&](){
        this->update_header();
        this->reconnect();
    });
    this->connection.get_proxy().signal_changed().connect([&](){
        this->stack_switcher.set_no_show_all(false);
        this->stack_switcher.show_all();
        if(this->tables)
            this->stack.remove(*this->tables);
        this->tables.reset(new TableView(this->connection.get_value()));
        this->tables->show_all();
        this->sqlview.reset(new SQLView(this->connection));
        this->sqlview->show_all();
        this->stack.add(*this->tables, "tables", "Database info");
        this->stack.add(*this->sqlview, "sql", "Custom SQL");
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
    auto chooser = new OpenDatabaseDialog(*this);
    chooser->set_local_only(true);
    chooser->signal_response().connect([chooser,this](int resp){
        if(resp == Gtk::RESPONSE_ACCEPT) {
            this->database_file = chooser->get_file();
            this->show_all();
        } else {
            delete this;
        }
        delete chooser;
    });
    chooser->show_all();
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

AboutDialog::AboutDialog()
    : Glib::ObjectBase("gsqlite_AboutDialog")
{
    this->set_program_name("gSQLite");
    this->set_version("0.1");
    this->set_authors({"Simonas Kazlauskas"});
    this->set_license_type(Gtk::License::LICENSE_BSD);
}

AboutDialog::~AboutDialog()
{
}
