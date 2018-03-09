#include "SeedEditWindow.h"

SeedEditWindow::SeedEditWindow(){
    createGUI();
}

SeedEditWindow::~SeedEditWindow(){

}

void SeedEditWindow::createGUI(){
    show_all_children();

    set_default_size(200,400);
}
