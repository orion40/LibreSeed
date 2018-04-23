#include "MainWindow.h"

MainWindow::MainWindow(){
    m_seed_add_window = NULL;
    m_seed_info_window = NULL;
}


MainWindow::MainWindow(Controller* controller): MainWindow(){
    m_controller = controller;
    if (m_controller->open_db()){
        // TODO: show_splashcreen
        create_gui();
        connect_signals();
        fill_tree_store();
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
    m_search_bar = new Gtk::SearchBar();

    m_management_toolbar = new Gtk::Toolbar();
    m_add_button = new Gtk::ToolButton("Add");
    //m_add_button->set_icon_widget(Gtk::Stock::ADD);
    m_delete_button = new Gtk::ToolButton("Delete");
    m_open_seed_info_button = new Gtk::ToolButton("Open Seed card");
    m_print_button = new Gtk::ToolButton("Print");

    m_tree_view_scrolled_window = new Gtk::ScrolledWindow();
    m_tree_view_scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_seed_list_store = new Gtk::TreeView();
    m_search_text_view = new Gtk::SearchEntry();

    // Add buttons to toolbar
    m_management_toolbar->append(*m_add_button);
    m_management_toolbar->append(*m_delete_button);
    m_management_toolbar->append(*m_open_seed_info_button);
    m_management_toolbar->append(*m_print_button);

    // Layout and graphic for "toolbar"
    //m_management_toolbar->set_layout(Gtk::BUTTONBOX_START);
    //m_management_toolbar->set_border_width(5);

    // Search elements
    m_search_bar->connect_entry(*m_search_text_view);
    m_search_bar->set_search_mode(true);

    m_control_box->pack_start(*m_management_toolbar, Gtk::PACK_SHRINK);
    m_control_box->pack_start(*m_search_bar, Gtk::PACK_SHRINK);

    // Tree store and model
    m_seed_tree_model = Gtk::ListStore::create(m_seed_columns);
    // TODO: later for multiple selection
    //m_seed_list_store->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
    m_seed_list_store->set_model(m_seed_tree_model);
    m_tree_view_scrolled_window->add(*m_seed_list_store);

    m_seed_list_store->append_column("ID", m_seed_columns.m_seed_id);
    m_seed_list_store->get_column(0)->set_sort_column(m_seed_columns.m_seed_id);

    m_seed_list_store->append_column("Name", m_seed_columns.m_seed_name);
    m_seed_list_store->get_column(1)->set_sort_column(m_seed_columns.m_seed_name);

    m_seed_list_store->append_column("Binomial Nomenclature", m_seed_columns.m_seed_binomial_nomenclature);
    m_seed_list_store->get_column(2)->set_sort_column(m_seed_columns.m_seed_binomial_nomenclature);
    
    m_seed_list_store->append_column("Description", m_seed_columns.m_seed_description);
    m_seed_list_store->get_column(3)->set_sort_column(m_seed_columns.m_seed_description);



    // Add all elements to the main window
    m_main_box->pack_start(*m_control_box, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_tree_view_scrolled_window, Gtk::PACK_EXPAND_WIDGET);
    add(*m_main_box);

    show_all_children();

    m_seed_list_store->grab_focus();

    set_default_size(500,500);
    set_title("Seed Manager");
}

void MainWindow::connect_signals(){
    m_add_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_add_button_clicked));
    m_open_seed_info_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_open_seed_info_button_clicked));
    m_delete_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_delete_button_clicked));
    m_seed_list_store->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_list_store_row_activated));
}

void MainWindow::on_add_button_clicked(){
    if (m_seed_add_window == NULL){
        m_seed_add_window = new SeedAddWindow(m_controller, m_seed_tree_model, &m_seed_columns);
    }
    m_seed_add_window->show();
}

void MainWindow::on_open_seed_info_button_clicked(){
    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_seed_list_store->get_selection();
    Gtk::TreeModel::iterator iter = refTreeSelection ->get_selected();
    if(iter) //If anything is selected
    {
        Gtk::TreeModel::Row row = *iter;
        //Do something with the row.
        std::cout << "Row selected: ID=" << row[m_seed_columns.m_seed_id] << ", Name="             << row[m_seed_columns.m_seed_name] << std::endl;
        open_seed_info(m_controller->get_model()->getSeedById(row[m_seed_columns.m_seed_id]));
    } else {
        Gtk::MessageDialog dialog(*this, 
                "Please select a seed first.",
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK);
        dialog.run();
    }
}

void MainWindow::on_delete_button_clicked(){
    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_seed_list_store->get_selection();
    Gtk::TreeModel::iterator iter = refTreeSelection ->get_selected();
    if (iter){
        Gtk::TreeModel::Row row = *iter;
        std::cout << "Selected: ";
        std::cout << row[m_seed_columns.m_seed_id] << " - ";
        std::cout << row[m_seed_columns.m_seed_name] << " - ";
        std::cout << row[m_seed_columns.m_seed_binomial_nomenclature] << " - ";
        std::cout << row[m_seed_columns.m_seed_description] << "\n";

        Gtk::MessageDialog dialog_warning(*this, "Are you sure you want to delete this seed ?",
                false /* use_markup */, Gtk::MESSAGE_WARNING,
                Gtk::BUTTONS_YES_NO);

        int result = dialog_warning.run();

        //Handle the response:
        switch(result)
        {
            case(Gtk::RESPONSE_YES):
                {
                    Seed*s = m_controller->get_model()->getSeedById(row[m_seed_columns.m_seed_id]);
                    m_controller->get_model()->remove_seed(s);
                    fill_tree_store();
                    break;
                }
            case (Gtk::RESPONSE_DELETE_EVENT):
            case(Gtk::RESPONSE_NO):
                {
                    break;
                }
            default:
                {
                    std::cerr << "Unexpected button clicked." << std::endl;
                    break;
                }
        }
    } else {
        Gtk::MessageDialog dialog(*this, 
                "Please select a seed first.",
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK);
        dialog.run();
    }
}

void MainWindow::fill_tree_store(){
    std::list<Seed*> seeds = m_controller->get_model()->get_seeds();
    m_seed_tree_model->clear();
    for (std::list<Seed*>::iterator it = seeds.begin(); it != seeds.end(); it++){
        Gtk::ListStore::Row row = *(m_seed_tree_model->append());
        row[m_seed_columns.m_seed_id] = (*it)->get_id();
        row[m_seed_columns.m_seed_name] = (*it)->get_name();
        row[m_seed_columns.m_seed_binomial_nomenclature] = (*it)->get_binomial_nomenclature();
        row[m_seed_columns.m_seed_description] = (*it)->get_description();
    }
}

void MainWindow::destroy_gui(){
    delete m_main_box;
    delete m_control_box;
    delete m_search_bar;

    delete m_management_toolbar;
    delete m_add_button;
    delete m_delete_button;
    delete m_open_seed_info_button;
    delete m_print_button;

    delete m_tree_view_scrolled_window;

    delete m_seed_list_store;
    delete m_search_text_view;
}

void MainWindow::on_list_store_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Gtk::TreeModel::iterator iter = m_seed_tree_model->get_iter(path);
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        std::cout << "Row activated: ID=" << row[m_seed_columns.m_seed_id] << ", Name="             << row[m_seed_columns.m_seed_name] << std::endl;
        open_seed_info(m_controller->get_model()->getSeedById(row[m_seed_columns.m_seed_id]));
    }
}

void MainWindow::open_seed_info(Seed* s){
    if (s){
        if (m_seed_info_window == NULL){
            m_seed_info_window = new SeedInfoWindow(m_controller, s, m_seed_tree_model, &m_seed_columns);
        }
        m_seed_info_window->show();
    }
}
