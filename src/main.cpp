#include "MainWindow.h"
#include "Controller.h"
#include "Model.h"
#include <gtkmm/application.h>

int main(int argc, char** argv){
    auto app =
        Gtk::Application::create(argc, argv, "org.seed_manager");

    Model* model = new Model("./seed.db");
    Controller* controller = new Controller(model);
    MainWindow* main_window = new MainWindow(app, controller);

    return app->run(*main_window);
}
