#include <sqlitemm/private/sigc_fix.hh>
#include "query_result_widget.hh"

using namespace GSQLiteui;

QueryResultsWidget::QueryResultsWidget(statement_t&& s)
    : Glib::ObjectBase("gsqliteui_QueryResultsWidget")
    , Gtk::TreeView()
    , statement(std::move(s))
    , cancellable(Gio::Cancellable::create())
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
    /// 2. Don't fill everything at once, instead do so as the user scrolls
    /// …?
    /// 4. Restrict the size of fields and ellipsize
    /// 5. When row is selected expand and show full contents (?)
    /// 6. Consider how to display the NULL columns (grayed out would be nice?)
    /// 7. Lots more…
    this->statement->iterate([this,cols](Statement::result_t &res){
        auto row = this->statement->iteration_finish(res);
        auto iter = this->store->append();

        for(int i = 0; i < cols; i += 1){
            auto val = (*row)[i];
            if(val == nullptr) continue;
            switch(val->getType()){
                case SQLITE_TEXT:
                    iter->set_value(i, Glib::ustring(
                        reinterpret_cast<const char *>(
                            **dynamic_cast<TextValue*>(val)
                        )
                    ));
                    break;
                case SQLITE_INTEGER:
                    iter->set_value(i, Glib::ustring(std::to_string(
                        **dynamic_cast<IntValue*>(val)
                    )));
                    break;
                case SQLITE_FLOAT:
                    iter->set_value(i, Glib::ustring(std::to_string(
                        **dynamic_cast<FloatValue*>(val)
                    )));
                    break;
                case SQLITE_BLOB:
                    iter->set_value(i, Glib::ustring("<Blob: not implemented>"));
                    break;
            }
        }
    }, this->cancellable);
}

QueryResultsWidget::~QueryResultsWidget(){
    this->cancellable->cancel();
}
