#include "table_view.hh"

using namespace GSQLiteui;

TableList::TableListColumnRecord::TableListColumnRecord()
{
    this->add(this->table_name);
}

TableList::TableList(Connection &c)
    : Glib::ObjectBase("gsqlite_TableList")
    , Gtk::TreeView()
{
    this->store = Gtk::ListStore::create(this->columns);
    this->set_model(this->store);
    this->set_headers_visible(false);
    this->append_column("Table name", this->columns.table_name);

    const char *query("SELECT name FROM sqlite_master "
                           "WHERE type IN ('table','view') "
                           "AND name NOT LIKE 'sqlite_%' "
                       "UNION ALL SELECT name FROM sqlite_temp_master "
                           "WHERE type IN ('table','view') "
                       "ORDER BY 1");
    // Head-ache-free resource management (note: stmt in lambda capture is
    // necessary)
    std::shared_ptr<Statement> stmt(c.prepare(query));
    stmt->iterate([this, stmt](Statement::result_t r){
        if(r->state().get_value() == Row::DONE) return false;
        Glib::ustring name(reinterpret_cast<const char *>(
                               r->extract<const unsigned char *>(0)));
        auto entry = this->store->append();
        entry->set_value(this->columns.table_name, name);
        return true;
    });
}

TableList::~TableList()
{
}

TableView::TableView(std::shared_ptr<Connection> c)
    : Glib::ObjectBase("gsqlite_TableView")
    , Gtk::Paned()
{
    this->table_list.reset(new GSQLiteui::TableList(*c));
    this->pack2(this->table_contents_sw, true, false);
    this->pack1(this->table_list_sw, false, false);
    this->table_list_sw.set_policy(Gtk::PolicyType::POLICY_NEVER,
                                   Gtk::PolicyType::POLICY_AUTOMATIC);
    this->table_list_sw.add(*this->table_list);
    this->table_list_sw.get_style_context()->add_class("sidebar");
    // TODO: move to TableLisT
    // this->table_list->get_selection()->signal_changed().connect([this](){
    //     this->table_selection_changed();
    // });
    // this->table_contents_sw.add(*this->table_contents);
}

TableView::~TableView()
{
}

void
TableView::table_selection_changed()
{
    // Should take care of signal emission when table selection changes
}
