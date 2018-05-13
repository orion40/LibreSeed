#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>

#include <gtkmm/box.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/window.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/button.h>
#include <gtkmm/treeview.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/stock.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchbar.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/builder.h>
#include <gtkmm/menubar.h>
#include <gtkmm/aboutdialog.h>

#include <giomm.h>

#include "Controller.h"
#include "SeedColumnsModel.h"
#include "SeedAddWindow.h"
#include "CategoryWindow.h"
#include "Model.h"

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        MainWindow(const Glib::RefPtr<Gtk::Application>& app, Controller* controller);
        ~MainWindow();

    private:
        void create_gui(const Glib::RefPtr<Gtk::Application>& app);
        void connect_signals();
        void init_gui();
        void fill_tree_store();
        void destroy_gui();

        Model* get_model(){return m_controller->get_model();}

        void on_add_button_clicked();
        void on_open_seed_info_button_clicked();
        void on_delete_button_clicked();
        void open_categories_manager();

        void on_help_button_clicked();
        void on_about_button_clicked();

        std::string open_xml_dialog();
        void on_export_all_xml_clicked();
        void on_export_selected_xml_clicked();
        void on_import_xml_clicked();

        void delete_selected_seed();
        void open_add_seed_window();

        void display_seed_selection_needed();

        void on_list_store_row_activated(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn* column);
        void on_list_store_selection_change();

        void open_seed_info(Seed* s);

        bool on_key_press_event(GdkEventKey* event) override;

        // GUI Creation methods
        Gtk::TreeView* create_seed_list_store();
        Gtk::Toolbar* create_management_toolbar();

        // Controller
        Controller* m_controller;

        // Model
        Glib::RefPtr<Gtk::ListStore> m_seed_tree_model;
        SeedColumnsModel m_seed_columns;

        // Main window components
        Gtk::Window* m_window;
        Gtk::Box* m_main_box;
        Gtk::Box* m_control_box;

        Gtk::SearchBar* m_search_bar;

        // Menus

        Gtk::Toolbar* m_management_toolbar;
        Gtk::ToolButton* m_add_button;
        Gtk::ToolButton* m_delete_button;
        Gtk::ToolButton* m_open_seed_info_button;
        Gtk::ToolButton* m_export_all_xml;
        Gtk::ToolButton* m_export_selected_xml;
        Gtk::ToolButton* m_import_xml;
        Gtk::ToolButton* m_print_button;

        Gtk::ScrolledWindow* m_seed_tree_view_scrolled_window;

        Gtk::TreeView* m_seed_list_store;
        Gtk::SearchEntry* m_search_text_view;

        // Child windows
        SeedAddWindow* m_seed_add_window;
        CategoryWindow* m_category_window;

        // Menu

        Gtk::MenuBar* m_menu_bar;

        // AccelGroups for kb shortcuts
        Gtk::AccelGroup* actions;
        Glib::RefPtr<Gtk::Builder> m_builder;
        Glib::RefPtr<Gio::SimpleActionGroup> m_action_group;
};

#endif
