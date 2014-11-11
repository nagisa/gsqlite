#include <sstream>
#include <sqlitemm/private/sigc_fix.hh>

#include "tableview.hh"

#define TABLE_INFO_SP 10

using namespace GSQLiteui;

TableInfo::ColumnsRecord::ColumnsRecord()
{
    this->add(name);
    this->add(type);
    this->add(notnull);
    this->add(default_value);
    this->add(primary);
}

TableInfo::TableInfo(std::shared_ptr<Connection> c)
    : Glib::ObjectBase("gsqlite_TableInfo")
    , c(c)
    , cancellable(Gio::Cancellable::create())
    , name_column("Column name", this->columns_record.name)
    , type_column("Column type", this->columns_record.type)
    , notnull_column("Nullable", this->columns_record.notnull)
    , default_column("Default value", this->columns_record.default_value)
    , primary_column("Primary", this->columns_record.primary)
{
    // Error view
    this->error_box.set_orientation(Gtk::Orientation::ORIENTATION_VERTICAL);
    this->error_icon.property_icon_name() = "action-unavailable-symbolic";
    this->error_icon.property_pixel_size() = 128;
    this->error_icon.set_valign(Gtk::Align::ALIGN_END);
    this->error_message.set_valign(Gtk::Align::ALIGN_START);
    this->error_message.set_justify(Gtk::Justification::JUSTIFY_CENTER);
    this->error_box.pack_start(this->error_icon, true, true, TABLE_INFO_SP);
    this->error_box.pack_start(this->error_message, true, true, TABLE_INFO_SP);
    this->error_message.set_text({"Table info cannot be displayed"});
    this->add(this->error_box, "error");

    // Table info
    this->info_box.set_orientation(Gtk::Orientation::ORIENTATION_VERTICAL);
    this->table_title.property_halign() = Gtk::Align::ALIGN_START;
    this->table_title.property_valign() = Gtk::Align::ALIGN_START;
    auto attrs = Pango::AttrList();
    auto attr = Pango::Attribute::create_attr_scale(2);
    attrs.insert(attr);
    this->table_title.property_attributes() = attrs;
    this->info_box.pack_start(this->table_title, false, true, TABLE_INFO_SP);

    this->info_box.pack_start(this->columns_frame, true, true, 0);
    this->columns_frame.set_label("Columns");
    this->columns_frame.add(this->columns_view);
    this->columns_frame.set_shadow_type(Gtk::SHADOW_NONE);
    this->columns_store = Gtk::ListStore::create(this->columns_record);
    this->columns_view.set_model(this->columns_store);
    this->columns_view.append_column(this->name_column);
    this->columns_view.append_column(this->type_column);
    this->columns_view.append_column(this->notnull_column);
    this->notnull_column.set_cell_data_func([](Gtk::CellRendererText *r,
                                               Value *v){
        if(!v || v->getType() != SQLITE_INTEGER)
            return false;
        int64_t val = **dynamic_cast<IntValue *>(v);
        r->property_text() = val != 0 ? "NO" : "YES";
        return true;
    });
    this->columns_view.append_column(this->default_column);
    this->columns_view.append_column(this->primary_column);
    this->primary_column.set_cell_data_func([](Gtk::CellRendererText *r,
                                               Value *v){
        if(!v || v->getType() != SQLITE_INTEGER)
            return false;
        int64_t val = **dynamic_cast<IntValue *>(v);
        r->property_text() = val == 0 ? "NO" : "YES";
        return true;
    });

    this->cancellable->signal_cancelled().connect([this](){
        if(this->columns_cancellable)
            this->columns_cancellable->cancel();
    });

    this->add(this->info_box, "info");
    this->set_no_show_all(true);
}

TableInfo::~TableInfo()
{
    this->cancellable->cancel();
}

void
TableInfo::display_table(Glib::ustring &table_name)
{
    this->set_no_show_all(false);
    this->show_all();

    if(this->columns_cancellable)
        this->columns_cancellable->cancel();
    this->columns_cancellable = Gio::Cancellable::create();
    std::shared_ptr<Statement> stmt;

    try {
        Glib::ustring query("PRAGMA table_info(");
        query += table_name + ");";
        stmt.reset(this->c->prepare(query.c_str()));
    } catch (SQLiteError &e) {
        Glib::ustring error("Cannot display ");
        error += table_name + "\n" + e.what();
        this->error_message.set_text(error);
        this->set_visible_child(this->error_box);
        return;
    }

    this->table_title.set_text(table_name);
    this->columns_store->clear();
    stmt->iterate([this,stmt](Statement::result_t res){
        auto row = stmt->iteration_finish(res);
        auto iter = this->columns_store->append();
        iter->set_value(this->columns_record.name, row->steal(1));
        iter->set_value(this->columns_record.type, row->steal(2));
        iter->set_value(this->columns_record.notnull, row->steal(3));
        iter->set_value(this->columns_record.default_value, row->steal(4));
        iter->set_value(this->columns_record.primary, row->steal(5));
    }, this->columns_cancellable);

    this->set_visible_child(this->info_box);
}

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
        Glib::ustring name(row->extract<Glib::ustring>(0));
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
    , table_list(new GSQLiteui::TableList(*c))
    , table_info(c)
{
    this->pack2(this->table_info_sw, true, false);
    this->pack1(this->table_list_sw, false, false);
    this->table_info_sw.add(this->table_info);
    this->table_list_sw.set_policy(Gtk::PolicyType::POLICY_NEVER,
                                   Gtk::PolicyType::POLICY_AUTOMATIC);
    this->table_list_sw.add(*this->table_list);
    this->table_list_sw.get_style_context()->add_class("sidebar");

    this->table_list->signal_table_select().connect([this](Glib::ustring s){
        this->table_info.display_table(s);
    });
}

TableView::~TableView()
{
}
