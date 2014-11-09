#pragma once

#include <gtkmm.h>
#include <gtksourceviewmm.h>

#include "query_result_widget.hh"

namespace GSQLiteui{

class SQLView : public Gtk::Paned {
    typedef std::pair<Gtk::TextBuffer::iterator, Gtk::TextBuffer::iterator>
            iterator_pair;

    std::shared_ptr<Connection>         c;
    Glib::RefPtr<Gsv::Buffer>           buffer;
    Gsv::View                           sqlentry;
    Gtk::ScrolledWindow                 entry_scroll;
    Gtk::Box                            entry_box;
    Gtk::ActionBar                      actions_bar;
    Gtk::Button                         execute_query;
    Gtk::InfoBar                        error_infobar;
    Gtk::Label                          error_message;

    std::unique_ptr<QueryResultsWidget> results;
    Gtk::ScrolledWindow                 results_scroll;
    public:
        SQLView(std::shared_ptr<Connection> c);
        ~SQLView();

    private:
        void update_button();
        void exec_action();
        void close_infobar();
};

}
