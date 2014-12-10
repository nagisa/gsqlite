#include "actions.hh"

StaticAction::StaticAction(Glib::ustring name)
{
    this->name = name;
}

StaticAction::action_t
StaticAction::get()
{
    return this->action
        ? this->action
        : (this->action = Gio::SimpleAction::create(this->name));
}

StaticAction open_action("open");
StaticAction about_action("about");
StaticAction quit_action("quit");
