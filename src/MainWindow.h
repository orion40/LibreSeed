#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>

#include <gtkmm/box.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/window.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/button.h>
#include <gtkmm/treeview.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/stock.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/liststore.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>

#include "Controller.h"
#include "SeedColumnsModel.h"
#include "SeedEditWindow.h"
#include "Model.h"

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        MainWindow(Controller* controller);
        ~MainWindow();

    private:
        void create_gui();
        void connect_signals();
        void fill_tree_store();
        void destroy_gui();

        void on_add_button_clicked();

        // Controller
        Controller* m_controller;

        // Model
        Glib::RefPtr<Gtk::ListStore> m_seed_tree_model;
        SeedColumnsModel m_seed_columns;

        // Main window components
        Gtk::Window* m_window;
        Gtk::Box* m_main_box;
        Gtk::Box* m_control_box;
        Gtk::Box* m_search_box;

        // Menus


        Gtk::Toolbar* m_management_toolbar;
        Gtk::ToolButton* m_add_button;
        Gtk::ToolButton* m_delete_button;
        Gtk::ToolButton* m_open_seed_info_button;
        Gtk::ToolButton* m_print_button;

        Gtk::ScrolledWindow* m_tree_view_scrolled_window;

        Gtk::TreeView* m_seed_list_store;
        Gtk::TextView* m_search_text_view;
        Gtk::Button* m_search_button;
};

#endif
