#pragma once
#include <gtkmm.h>
#include <glibmm.h>
#include <sqlitemm/sqlitemm.hh>

namespace GSQLiteui {

class QueryResultsWidget : public Gtk::TreeView {
    typedef std::unique_ptr<Statement> statement_t;
    Glib::RefPtr<Gtk::ListStore>   store;
    Gtk::TreeModel::ColumnRecord   columns;
    statement_t                    statement;
    Glib::RefPtr<Gio::Cancellable> cancellable;

    public:
        QueryResultsWidget(statement_t &&);
        ~QueryResultsWidget();
};

}
