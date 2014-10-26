#include <sstream>
#include <sqlitemm/private/sigc_fix.hh>

#include "table_view.hh"

using namespace GSQLiteui;

TableList::TableListColumnRecord::TableListColumnRecord()
{
    this->add(this->table_name);
}

TableList::TableList(Connection &c)
    : Glib::ObjectBase("gsqlite_TableList")
    , Gtk::TreeView()
    , cancellable(Gio::Cancellable::create())
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
    // Headache-free resource management (note: stmt in lambda capture is
    // necessary)
    std::shared_ptr<Statement> stmt(c.prepare(query));
    stmt->iterate([this, stmt](Statement::result_t &res){
        auto row = stmt->iteration_finish(res);
        Glib::ustring name(reinterpret_cast<const char *>(
                           row->extract<const unsigned char *>(0)));
        auto entry = this->store->append();
        entry->set_value(this->columns.table_name, name);
    }, this->cancellable);
    this->get_selection()->signal_changed().connect([this](){
        auto row = this->get_selection()->get_selected();
        if(!row){
            this->_signal_table_deselect.emit();
        }
        else if(!this->previous || this->previous != row){
            this->previous = row;
            this->_signal_table_select.emit(
                row->get_value(this->columns.table_name)
            );
        }
    });
}

TableList::~TableList()
{
    this->cancellable->cancel();
}

TableList::table_select_sig_t
TableList::signal_table_select()
{
    return this->_signal_table_select;
}

TableList::table_deselect_sig_t
TableList::signal_table_deselect()
{
    return this->_signal_table_deselect;
}

TableView::TableView(std::shared_ptr<Connection> c)
    : Glib::ObjectBase("gsqlite_TableView")
    , Gtk::Paned()
    , c(c)
{
    this->table_list.reset(new GSQLiteui::TableList(*c));
    this->pack2(this->table_contents_sw, true, false);
    this->pack1(this->table_list_sw, false, false);
    this->table_list_sw.set_policy(Gtk::PolicyType::POLICY_NEVER,
                                   Gtk::PolicyType::POLICY_AUTOMATIC);
    this->table_list_sw.add(*this->table_list);
    this->table_list_sw.get_style_context()->add_class("sidebar");

    this->table_list->signal_table_select().connect([this](Glib::ustring s){
        std::stringstream ss;
        ss << "SELECT * FROM " << s << ";";
        std::unique_ptr<Statement> stmt;
        try {
            stmt.reset(this->c->prepare(ss.str().c_str(), ss.str().length()));
        } catch (SQLiteError& e) {
            delete this->table_contents.release();
            this->table_contents_sw.remove();
            return;
        }
        this->table_contents.reset(new QueryResultsWidget(std::move(stmt)));
        this->table_contents_sw.remove();
        this->table_contents_sw.add(*this->table_contents);
        this->table_contents->show_all();
    });
    this->table_list->signal_table_deselect().connect([this](){
        this->table_contents_sw.remove();
        delete this->table_contents.release();
    });
}

TableView::~TableView()
{
}

void
TableView::table_selection_changed()
{
    // Should take care of signal emission when table selection changes
}
