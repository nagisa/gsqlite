#pragma once

#include <gtkmm.h>
#include <glibmm.h>

#include "../statement.hh"

namespace GSQLiteui {

class QueryResultsWidget : public Gtk::TreeView {
    Glib::RefPtr<Gtk::ListStore> store;
    public:
        QueryResultsWidget(Statement *s);
        ~QueryResultsWidget();
};

}
