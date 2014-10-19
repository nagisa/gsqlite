#include "table_view.hh"

using namespace GSQLiteui;

TableViewColumnRecord::TableViewColumnRecord()
{
    this->add(this->table_name);
}

TableView::TableView()
{
    this->store = Gtk::ListStore::create(this->columns);
    this->set_model(this->store);
    this->set_headers_visible(false);
    this->append_column("Table name", this->columns.table_name);
}

TableView::~TableView()
{
}

#include <iostream>
void
TableView::refresh(Connection &connection)
{
    this->store->clear();
    std::string query("SELECT name FROM sqlite_master "
                           "WHERE type IN ('table','view') "
                           "AND name NOT LIKE 'sqlite_%' "
                       "UNION ALL SELECT name FROM sqlite_temp_master "
                           "WHERE type IN ('table','view') "
                       "ORDER BY 1");
    auto stmt = connection.prepare(query.c_str(), query.length());
    auto store = this->store;
    stmt->iterate([this, store](Statement::result_t r){
        if(r->state().get_value() == Row::DONE) return false;
        Glib::ustring name(reinterpret_cast<const char *>(
                               r->extract<const unsigned char *>(0)));
        auto entry = store->append();
        entry->set_value(this->columns.table_name, name);
        return true;
    });
}
