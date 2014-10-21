#pragma once

#include <gtkmm.h>
#include <glibmm.h>

#include "../statement.hh"

namespace GSQLiteui {

class QueryResultsWidget : public Gtk::TreeView {
    typedef std::unique_ptr<Statement> statement_t;
    Glib::RefPtr<Gtk::ListStore> store;
    Gtk::TreeModel::ColumnRecord columns;
    statement_t                  statement;

    public:
        QueryResultsWidget(statement_t &&);
        ~QueryResultsWidget();
};

}
