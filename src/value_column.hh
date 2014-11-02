#pragma once

#include <gtkmm.h>

namespace GSQLiteui {

class ValueColumn : public Gtk::TreeViewColumn {
    Gtk::CellRendererText   rndr;
    public:
        ValueColumn(const Glib::ustring &, const int);
        ValueColumn(const Glib::ustring &,
                    const Gtk::TreeModelColumn<Value *> &);

};

}
