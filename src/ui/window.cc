#include "window.hh"

using namespace GSQLiteui;

Window::Window()
{
    this->set_title("gSQLite");
    this->set_titlebar(this->header);
    this->header.set_show_close_button(true);
    this->header.pack_start(this->primary_button);
    this->primary_button.set_label("Open");
    this->primary_button.get_style_context()->add_class("suggested-action");
    this->primary_button.signal_clicked().connect([&](){
        OpenDatabaseDialog chooser(*this);
        chooser.signal_response().connect([&](int resp){
        });
        chooser.run();
    });
};

Window::~Window()
{
};

OpenDatabaseDialog::OpenDatabaseDialog(Gtk::Window &w)
    : Gtk::FileChooserDialog(w, "Pick a SQLite file")
{
    this->add_button("Cancel", Gtk::RESPONSE_REJECT);
    this->add_button("Open", Gtk::RESPONSE_ACCEPT);
    auto sqlitefilter = Gtk::FileFilter::create();
    auto anyfilter = Gtk::FileFilter::create();
    sqlitefilter->set_name("SQLite files");
    sqlitefilter->add_custom(Gtk::FILE_FILTER_URI, [&](const Gtk::FileFilter::Info& finfo){
        // TODO: test for file validity
        // See: http://www.sqlite.org/fileformat.html
        return false;
    });
    anyfilter->set_name("All files");
    anyfilter->add_pattern("*");
    this->add_filter(sqlitefilter);
    this->add_filter(anyfilter);
};

OpenDatabaseDialog::~OpenDatabaseDialog()
{
};
