#pragma once

#include <gtkmm.h>
#include <glibmm.h>
#include <sqlitemm/sqlitemm.hh>

#include "query_result_widget.hh"

namespace GSQLiteui {


/// View for tables in database
class TableList : public Gtk::TreeView {
    typedef sigc::signal<void, Glib::ustring> table_select_sig_t;
    typedef sigc::signal<void>                table_deselect_sig_t;

    private:
        class TableListColumnRecord : public Gtk::TreeModel::ColumnRecord {
            public:
                Gtk::TreeModelColumn<Glib::ustring> table_name;
                TableListColumnRecord();
        };

        Gtk::TreeIter                previous;
        table_select_sig_t           _signal_table_select;
        table_deselect_sig_t         _signal_table_deselect;

        Glib::RefPtr<Gio::Cancellable> cancellable;
    public:
        TableListColumnRecord        columns;
        Glib::RefPtr<Gtk::ListStore> store;

        TableList(Connection &);
        ~TableList();

        table_select_sig_t           signal_table_select();
        table_deselect_sig_t         signal_table_deselect();
};

class TableView : public Gtk::Paned {
    std::shared_ptr<Connection>                        c;
    public:
        Gtk::ScrolledWindow                            table_list_sw;
        std::unique_ptr<GSQLiteui::TableList>          table_list;
        Gtk::ScrolledWindow                            table_contents_sw;
        std::unique_ptr<GSQLiteui::QueryResultsWidget> table_contents;

        TableView(std::shared_ptr<Connection>);
        ~TableView();

        void table_selection_changed();
};

}
