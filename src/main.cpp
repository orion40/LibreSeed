#include "MainWindow.h"
#include "Controller.h"
#include "Model.h"
#include <gtkmm/application.h>

int main(int argc, char** argv){
    auto app =
        Gtk::Application::create(argc, argv, "org.seed_manager");

    Model* model = new Model();
    Controller* controller = new Controller("./seed.db", model);
    MainWindow* main_window = new MainWindow(controller);
    // TODO: connexion à la bdd
    // verifs si la co est ok

    return app->run(*main_window);
}
