#include <gtkmm/application.h>
#include <iostream>
#include <sqlitemm/private/sigc_fix.hh>

#include "window.hh"
#include "application.hh"

int main(int argc, char** argv){
    return GSQLiteui::Application().run(argc, argv);
}
