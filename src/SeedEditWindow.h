#ifndef SEEDEDITWINDOW_H
#define SEEDEDITWINDOW_H

#include <gtkmm/window.h>

#include "Controller.h"
#include "SeedColumnsModel.h"

class SeedEditWindow : public Gtk::Window{
public:
	SeedEditWindow();
	~SeedEditWindow();

private:
        void createGUI();

};

#endif
