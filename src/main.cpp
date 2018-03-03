#include "MainWindow.h"
#include "Controller.h"
#include <gtkmm/application.h>

int main(int argc, char** argv){
    auto app =
        Gtk::Application::create(argc, argv, "org.seed_manager");

    Controller* controller = new Controller("./seed.db");
    MainWindow* main_window = new MainWindow(controller);
    // TODO: connexion à la bdd
    // verifs si la co est ok

    return app->run(*main_window);
}
