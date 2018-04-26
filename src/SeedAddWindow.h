#ifndef SEEDEDITWINDOW_H
#define SEEDEDITWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/entry.h>
#include <gtkmm/entrybuffer.h>
#include <gtkmm/textview.h>
#include <gtkmm/liststore.h>

#include <iostream>

#include "Controller.h"
#include "SeedColumnsModel.h"

class SeedAddWindow : public Gtk::Window{
    public:
        SeedAddWindow();
        SeedAddWindow(Controller* controller, Glib::RefPtr<Gtk::ListStore> seed_model, SeedColumnsModel* columns);
        ~SeedAddWindow();

    private:
        void create_gui();
        void destroy_gui();

        void connect_signals();

        void on_save_button_clicked();
        void on_delete_button_clicked();

        bool on_key_press_event(GdkEventKey* event) override;

        void save_seed();

        Controller* m_controller;

        Gtk::Box* m_main_box;
        Gtk::Notebook* m_notebook;

        Gtk::Box* m_main_info_box;
        Gtk::Box* m_main_info_label_box;
        Gtk::Box* m_main_info_textfield_box;
        Gtk::Label* m_name_label;
        Gtk::Entry* m_name_entry;
        Gtk::Label* m_binomial_name_label;
        Gtk::Entry* m_binomial_name_entry;
        Gtk::Label* m_description_label;
        Gtk::TextView* m_description_textfield;

        Gtk::Box* m_pictures_box;
        Gtk::Box* m_dates_box;

        Gtk::Toolbar* m_edit_toolbar;
        Gtk::ToolButton* m_save_button;
        Gtk::ToolButton* m_delete_button;

        // MainWindow ListStore model
        Glib::RefPtr<Gtk::ListStore> m_seed_tree_model;
        SeedColumnsModel* m_seed_columns;
};

#endif
