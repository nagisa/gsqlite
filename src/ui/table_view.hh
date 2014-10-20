#pragma once

#include <gtkmm.h>
#include <glibmm.h>

#include "../sigc_common.hh"
#include "../connection.hh"
#include "query_result_widget.hh"

namespace GSQLiteui {


/// View for tables in database
class TableList : public Gtk::TreeView {
    private:
        class TableListColumnRecord : public Gtk::TreeModel::ColumnRecord {
            public:
                Gtk::TreeModelColumn<Glib::ustring> table_name;
                TableListColumnRecord();
        };
    public:
        TableListColumnRecord        columns;
        Glib::RefPtr<Gtk::ListStore> store;

        TableList(Connection &);
        ~TableList();
};

class TableView : public Gtk::Paned {
    std::shared_ptr<Connection>       c;
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
