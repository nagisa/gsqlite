#include "window.hh"

using namespace GSQLiteui;

Window::Window(){
    this->set_titlebar(this->header);
    this->header.set_show_close_button(true);
};

Window::~Window(){
};
