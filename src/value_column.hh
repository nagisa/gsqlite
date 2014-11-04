#pragma once

#include <gtkmm.h>
#include <sigc++/sigc++.h>
#include <sqlitemm/value.hpp>

namespace GSQLiteui {

class ValueColumn : public Gtk::TreeViewColumn {
    typedef sigc::slot<bool, Gtk::CellRendererText *, Value *> CDFSlot;
    Gtk::CellRendererText   rndr;
    CDFSlot                 cdf;
    public:
        ValueColumn(const Glib::ustring &, const int);
        ValueColumn(const Glib::ustring &,
                    const Gtk::TreeModelColumn<Value *> &);
        void set_cell_data_func(CDFSlot cdf);
};

}
