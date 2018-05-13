#ifndef CATEGORYWINDOW_H
#define CATEGORYWINDOW_H

#include <iostream>

#include <gtkmm/window.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/headerbar.h>

#include "Controller.h"
#include "CategoriesColumnsModel.h"

class CategoryWindow : public Gtk::Window{
    public:
        CategoryWindow(Controller* controller);
        ~CategoryWindow();

    private:
        void create_gui();
        Gtk::ScrolledWindow* create_category_scrolled_window();
        Gtk::HeaderBar* create_header_bar();


        void connect_signals();

        bool on_key_press_event(GdkEventKey* key_event);

        void on_add_button_clicked();
        void on_delete_button_clicked();

        void add_category();
        void delete_category();
        void on_list_store_selection_change();
        void on_list_store_row_change(const Gtk::TreeModel::Path&, Gtk::TreeIter iter);
        void fill_tree_store();

        void display_category_selection_needed();

        Controller* m_controller;

        Gtk::Box* m_main_box;
        Gtk::ScrolledWindow* m_category_scrolled_window;

        Gtk::HeaderBar* m_header_bar;

        Gtk::ToolButton* m_add_button;
        Gtk::ToolButton* m_delete_button;

        Glib::RefPtr<Gtk::ListStore> m_category_tree_model;
        CategoriesColumnsModel m_category_columns;
        Gtk::TreeView* m_category_list_store;
};

#endif
