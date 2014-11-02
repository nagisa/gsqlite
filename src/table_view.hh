#pragma once

#include <gtkmm.h>
#include <glibmm.h>
#include <sqlitemm/sqlitemm.hh>

#include "query_result_widget.hh"
#include "value_column.hh"

namespace GSQLiteui {

/// Displays the information about a given table
class TableInfo : public Gtk::Stack {
    class ColumnsRecord : public Gtk::TreeModel::ColumnRecord {
        public:
        Gtk::TreeModelColumn<Value *>       name;
        Gtk::TreeModelColumn<Value *>       type;
        Gtk::TreeModelColumn<Value *>       notnull;
        Gtk::TreeModelColumn<Value *>       default_value;
        Gtk::TreeModelColumn<Value *>       primary;
        ColumnsRecord();
    };

    std::shared_ptr<Connection>     c;
    Glib::RefPtr<Gio::Cancellable>  cancellable;
    // Can’t-show-table
    Gtk::Box                      error_box;
    Gtk::Image                    error_icon;
    Gtk::Label                    error_message;

    Gtk::Box                       info_box;
    Gtk::Label                     table_title;
    // List of columns
    Gtk::Frame                     columns_frame;
    // TODO: this TreeView needs a separate class, really.
    Gtk::TreeView                  columns_view;
    ColumnsRecord                  columns_record;
    Glib::RefPtr<Gtk::ListStore>   columns_store;
    Glib::RefPtr<Gio::Cancellable> columns_cancellable;
    // List of columns in columns table (inception?!)
    ValueColumn                    name_column;
    ValueColumn                    type_column;
    ValueColumn                    notnull_column;
    ValueColumn                    default_column;
    ValueColumn                    primary_column;


    public:

        TableInfo(std::shared_ptr<Connection>);
        ~TableInfo();
        void display_table(Glib::ustring &table_name);
        void clear_display();
};

/// List of tables in the database
class TableList : public Gtk::TreeView {
    typedef sigc::signal<void, Glib::ustring> table_select_sig_t;
    typedef sigc::signal<void>                table_deselect_sig_t;

    private:
        class TableListColumnRecord : public Gtk::TreeModel::ColumnRecord {
            public:
                Gtk::TreeModelColumn<Glib::ustring> table_name;
                TableListColumnRecord();
        };

        Gtk::TreeIter                  previous;
        table_select_sig_t             _signal_table_select;
        table_deselect_sig_t           _signal_table_deselect;

        TableListColumnRecord          columns;
        Glib::RefPtr<Gtk::ListStore>   store;
        Glib::RefPtr<Gio::Cancellable> cancellable;
    public:
        TableList(Connection &);
        ~TableList();

        table_select_sig_t           signal_table_select();
        table_deselect_sig_t         signal_table_deselect();
};

/// Root view for “Table view” section.
class TableView : public Gtk::Paned {
    std::shared_ptr<Connection>                        c;
    public:
        Gtk::ScrolledWindow                            table_list_sw;
        std::unique_ptr<GSQLiteui::TableList>          table_list;
        Gtk::ScrolledWindow                            table_info_sw;
        GSQLiteui::TableInfo                           table_info;

        TableView(std::shared_ptr<Connection>);
        ~TableView();
};

}
