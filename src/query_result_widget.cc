#include <sqlitemm/private/sigc_fix.hh>
#include "query_result_widget.hh"
#include "value_column.hh"

using namespace GSQLiteui;

QueryResultsWidget::QueryResultsWidget(statement_t&& s)
    : Glib::ObjectBase("gsqliteui_QueryResultsWidget")
    , Gtk::TreeView()
    , statement(std::move(s))
    , cancellable(Gio::Cancellable::create())
{
    auto cols = this->statement->columns();
    for(int i = 0; i < cols; i += 1) {
        Gtk::TreeModelColumn<Value *> col;
        this->columns.add(col);
        auto c = new ValueColumn(this->statement->column_name(i), col);
        this->append_column(*c);
    }

    this->store = Gtk::ListStore::create(this->columns);
    this->set_model(this->store);
    this->set_headers_visible(true);

    /// TODO:
    /// 2. Don't fill everything at once, instead do so as the user scrolls
    /// 4. Restrict the size of fields and ellipsize
    /// 5. When row is selected expand and show full contents (?)
    /// 7. Lots more…

    this->statement->iterate([this,cols](Statement::result_t &res){
        auto row = this->statement->iteration_finish(res);
        auto iter = this->store->append();
        for(int i = 0; i < cols; i += 1){
            auto value = row->steal(i);
            iter->set_value(i, value);
        }
    }, this->cancellable);
}

QueryResultsWidget::~QueryResultsWidget(){
    this->cancellable->cancel();
}
