#include <sqlitemm/private/sigc_fix.hh>
#include <sqlitemm/value.hpp>

#include "value_column.hh"

using namespace GSQLiteui;


bool
default_cdf(Gtk::CellRendererText *r, Value *value)
{
    r->property_text() = "";
    r->property_background_set() = false;
    r->property_weight_set() = false;
    if(value == nullptr){
       Gdk::RGBA rgba;
       rgba.set_grey(0.5, 0.15);
       r->property_background_rgba() = rgba;
       r->property_background_set() = true;
       r->property_weight() = 600;
       r->property_weight_set() = true;
       r->property_text() = "NULL";
       return true;
   }
   switch(value->getType()){
       case SQLITE_TEXT:
           r->property_text() = Glib::ustring(
               reinterpret_cast<const char *>(
                   **dynamic_cast<TextValue *>(value)
               )
           );
           return true;
       case SQLITE_INTEGER:
           r->property_text() = Glib::ustring(std::to_string(
               **dynamic_cast<IntValue *>(value)
           ));
           return true;
       case SQLITE_FLOAT:
           r->property_text() = Glib::ustring(std::to_string(
               **dynamic_cast<FloatValue *>(value)
           ));
           return true;
       case SQLITE_BLOB:
       default:
           break;
   }
   return false;
}


ValueColumn::ValueColumn(const Glib::ustring &title, const int colnum)
    : Glib::ObjectBase("gsqliteui_ValueColumn")
    , Gtk::TreeViewColumn(title)
    , cdf(sigc::ptr_fun(default_cdf))
{
    this->pack_start(this->rndr);
    auto data_func = [this, colnum](Gtk::CellRenderer* rnd,
                                    const Gtk::TreeModel::const_iterator& i){
        Value *value;
        i->get_value(colnum, value);
        auto ret = this->cdf(static_cast<Gtk::CellRendererText *>(rnd), value);
        if(!ret)
            default_cdf(static_cast<Gtk::CellRendererText *>(rnd), value);
    };
    Gtk::TreeViewColumn::set_cell_data_func(this->rndr, data_func);
}

ValueColumn::ValueColumn(const Glib::ustring &title,
                         const Gtk::TreeModelColumn<Value *> &col)
    : ValueColumn(title, col.index())
{
}

void
ValueColumn::set_cell_data_func(CDFSlot cdf)
{
    this->cdf = cdf;
}
