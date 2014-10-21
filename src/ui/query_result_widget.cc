#include "query_result_widget.hh"

using namespace GSQLiteui;

QueryResultsWidget::QueryResultsWidget(statement_t&& s)
    : Glib::ObjectBase("gsqliteui_QueryResultsWidget")
    , Gtk::TreeView()
    , statement(std::move(s))
{
    auto cols = this->statement->columns();
    for(int i = 0; i < cols; i += 1) {
        Gtk::TreeModelColumn<Glib::ustring> col;
        this->columns.add(col);
        this->append_column(this->statement->origin_name(i), col);
    }

    this->store = Gtk::ListStore::create(this->columns);
    this->set_model(this->store);
    this->set_headers_visible(true);

    /// TODO:
    /// 1. Cancel on destruction
    /// 2. Don't fill everything at once, instead do so as the user scrolls
    /// …?
    /// 4. Restrict the size of fields and ellipsize
    /// 5. When row is selected expand and show full contents (?)
    /// 6. Consider how to display the NULL columns (grayed out would be nice?)
    /// 7. Lots more…
    this->statement->iterate([this, cols](Statement::result_t row){
        if(row->state().get_value() == Row::DONE) return false;
        auto iter = this->store->append();
        for(int i = 0; i < cols; i += 1){
            auto val = (*row)[i];
            if(val == nullptr) return true;
            switch(val->getType()){
                case SQLITE_TEXT:
                    iter->set_value(i, Glib::ustring(
                        reinterpret_cast<const char *>(
                            **dynamic_cast<TextValue*>(val)
                        )
                    ));
                    break;
                default:
                    iter->set_value(i, Glib::ustring("<not implemented>"));
                    break;
            }
        }
        return true;
    });
}

QueryResultsWidget::~QueryResultsWidget(){
}
