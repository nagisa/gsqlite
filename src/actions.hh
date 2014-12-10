#pragma once
#include <giomm.h>

class StaticAction {
    public:
    typedef Glib::RefPtr<Gio::SimpleAction> action_t;
    protected:
    Glib::ustring name;
    action_t action;
    public:
    StaticAction(Glib::ustring);
    action_t get();
};

extern StaticAction open_action;
extern StaticAction about_action;
extern StaticAction quit_action;
