#include "MainWindow.h"

MainWindow::MainWindow(){

}


MainWindow::MainWindow(Controller* controller): MainWindow(){
    m_controller = controller;
    if (m_controller->open_db()){
        // TODO: show_splashcreen
        create_gui();
    } else {
        Gtk::MessageDialog dialog(*this, 
                "An error occured while loading the database.",
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK);
        dialog.set_secondary_text("Please run this program from a terminal to see more information.");
        dialog.run();
    }
}

MainWindow::~MainWindow(){
    destroy_gui();
}

void MainWindow::create_gui(){
    m_main_box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    m_control_box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    m_search_box = new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);

    m_management_button_box = new Gtk::HButtonBox();
    m_add_button = new Gtk::Button("Add");
    m_delete_button = new Gtk::Button("Delete");
    m_open_seed_info_button = new Gtk::Button("Open Seed card");
    m_print_button = new Gtk::Button("Print");

    m_tree_view_scrolled_window = new Gtk::ScrolledWindow();
    m_tree_view_scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_seed_list_store = new Gtk::TreeView();
    m_search_button = new Gtk::Button("Search");
    m_search_text_view = new Gtk::TextView();

    // Add buttons to "toolbar"
    m_management_button_box->pack_start(*m_add_button, Gtk::PACK_SHRINK);
    m_management_button_box->pack_start(*m_delete_button, Gtk::PACK_SHRINK);
    m_management_button_box->pack_start(*m_open_seed_info_button, Gtk::PACK_SHRINK);
    m_management_button_box->pack_start(*m_print_button, Gtk::PACK_SHRINK);

    // Layout and graphic for "toolbar"
    m_management_button_box->set_layout(Gtk::BUTTONBOX_START);
    m_management_button_box->set_border_width(5);

    // Search elements
    m_search_box->pack_start(*m_search_text_view);
    m_search_box->pack_start(*m_search_button);

    m_control_box->pack_start(*m_management_button_box, Gtk::PACK_SHRINK);
    m_control_box->pack_start(*m_search_box, Gtk::PACK_SHRINK);

    // Tree store and model
    m_seed_tree_model = Gtk::ListStore::create(m_seed_columns);
    m_seed_list_store->set_model(m_seed_tree_model);
    m_tree_view_scrolled_window->add(*m_seed_list_store);
    Gtk::ListStore::Row row = *(m_seed_tree_model->append());
    row[m_seed_columns.m_seed_id] = 0;
    row[m_seed_columns.m_seed_name] = "Test";

    m_seed_list_store->append_column("ID", m_seed_columns.m_seed_id);
    m_seed_list_store->append_column("Name", m_seed_columns.m_seed_name);

    // Add all elements to the main window
    m_main_box->pack_start(*m_control_box, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_tree_view_scrolled_window, Gtk::PACK_SHRINK);
    add(*m_main_box);

    show_all_children();

    set_default_size(200,400);
}

void MainWindow::destroy_gui(){
    delete m_main_box;
    delete m_control_box;
    delete m_search_box;

    delete m_management_button_box;
    delete m_add_button;
    delete m_delete_button;
    delete m_open_seed_info_button;
    delete m_print_button;

    delete m_tree_view_scrolled_window;

    delete m_seed_list_store;
    delete m_search_button;
    delete m_search_text_view;
}
