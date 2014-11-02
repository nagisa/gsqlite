#include <sqlitemm/private/sigc_fix.hh>
#include <sqlitemm/value.hpp>

#include "value_column.hh"

using namespace GSQLiteui;

ValueColumn::ValueColumn(const Glib::ustring &title, const int colnum)
    : Glib::ObjectBase("gsqliteui_ValueColumn")
    , Gtk::TreeViewColumn(title)
{
    this->pack_start(this->rndr);
    this->set_cell_data_func(this->rndr, [this,colnum](Gtk::CellRenderer *,
                             const Gtk::TreeModel::const_iterator& i){
        Value *value;
        i->get_value(colnum, value);
        if(value == nullptr){
            Gdk::RGBA rgba;
            rgba.set_grey(0.5, 0.15);
            this->rndr.property_background_rgba() = rgba;
            this->rndr.property_text() = "NULL";
            return;
        }
        switch(value->getType()){
            case SQLITE_TEXT:
                this->rndr.property_text() = Glib::ustring(
                    reinterpret_cast<const char *>(
                        **dynamic_cast<TextValue *>(value)
                    )
                );
                break;
            case SQLITE_INTEGER:
                this->rndr.property_text() = Glib::ustring(std::to_string(
                    **dynamic_cast<IntValue *>(value)
                ));
                break;
            case SQLITE_FLOAT:
                this->rndr.property_text() = Glib::ustring(std::to_string(
                    **dynamic_cast<FloatValue *>(value)
                ));
                break;
            default:
                break;
        }
    });
}

ValueColumn::ValueColumn(const Glib::ustring &title,
                         const Gtk::TreeModelColumn<Value *> &col)
    : ValueColumn(title, col.index())
{
}

