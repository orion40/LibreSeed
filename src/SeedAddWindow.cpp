#include "SeedAddWindow.h"

// TODO: faire un bouton pour ajouter d'autre graines ?
// un new qui vide tout les champs et crée une nouvelle graine

SeedAddWindow::SeedAddWindow(){

}

SeedAddWindow::SeedAddWindow(Controller* controller, Glib::RefPtr<Gtk::ListStore> seed_model, SeedColumnsModel* columns){
    m_seed = NULL;
    m_controller = controller;
    m_seed_tree_model = seed_model;
    m_seed_columns = columns;
    create_gui();
    set_title("Add a new seed");
    set_icon_name("document-new");
    connect_signals();
}

SeedAddWindow::SeedAddWindow(Controller* controller, Glib::RefPtr<Gtk::ListStore> seed_model, SeedColumnsModel* columns, Seed* seed){
    m_seed = seed;
    m_controller = controller;
    m_seed_tree_model = seed_model;
    m_seed_columns = columns;
    create_gui();
    set_title("Edit a seed");
    set_icon_name("document-open");
    connect_signals();
    fill_gui();
    connect_signals();
}

SeedAddWindow::~SeedAddWindow(){
    destroy_gui();
}

void SeedAddWindow::create_gui(){
    m_main_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_edit_toolbar = Gtk::manage(new Gtk::Toolbar());
    m_save_button = Gtk::manage(new Gtk::ToolButton("Save"));
    m_save_button->set_icon_name("document-save");
    m_delete_button = Gtk::manage(new Gtk::ToolButton("Delete"));
    m_delete_button->set_icon_name("edit-delete");

    m_notebook = Gtk::manage(new Gtk::Notebook());
    m_notebook->set_tab_pos(Gtk::POS_LEFT);

    m_main_info_grid = Gtk::manage(new Gtk::Grid());

    m_name_label = Gtk::manage(new Gtk::Label("Name"));
    m_name_entry = Gtk::manage(new Gtk::Entry());
    m_binomial_name_label = Gtk::manage(new Gtk::Label("Binomial nomenclature"));
    m_binomial_name_entry = Gtk::manage(new Gtk::Entry());
    // TODO: a mettre dans un onglet à part ?
    m_description_textfield = Gtk::manage(Gtk::manage(new Gtk::TextView()));
    m_description_textfield->set_wrap_mode(Gtk::WRAP_WORD_CHAR);

    m_main_info_grid->attach(*m_name_label,0,0,1,1);
    m_main_info_grid->attach(*m_binomial_name_label,0,1,1,1);
    m_main_info_grid->attach(*m_name_entry,1,0,1,1);
    m_main_info_grid->attach(*m_binomial_name_entry,1,1,1,1);

    m_description_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_description_box->pack_start(*m_description_textfield, Gtk::PACK_EXPAND_WIDGET);

    m_dates_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    // How to add a Date ? Already here but empty
    // for int i = 0; i < MAX_DATES; i++
    //      create DateWidget

    m_stock_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_pictures_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_notebook->append_page(*m_main_info_grid, "Main Info");
    m_notebook->append_page(*m_description_box, "Description");
    m_notebook->append_page(*m_dates_box, "Dates");
    m_notebook->append_page(*m_stock_box, "Stock");
    m_notebook->append_page(*m_pictures_box, "Pictures");

    m_edit_toolbar->append(*m_save_button);
    m_edit_toolbar->append(*m_delete_button);

    m_main_box->pack_start(*m_edit_toolbar, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_notebook, Gtk::PACK_EXPAND_WIDGET);

    add(*m_main_box);

    show_all_children();

    m_name_entry->grab_focus();

    set_default_size(400,400);
}

void SeedAddWindow::connect_signals(){
    m_save_button->signal_clicked().connect(sigc::mem_fun(*this, &SeedAddWindow::on_save_button_clicked));
    m_delete_button->signal_clicked().connect(sigc::mem_fun(*this, &SeedAddWindow::on_delete_button_clicked));
}

void SeedAddWindow::on_save_button_clicked(){
    save_seed();
}

void SeedAddWindow::on_delete_button_clicked(){
    // TODO: Confirmation
    // Si oui, on detruit cette fenetre et supprimons la graine du model
    // et de la bdd
}

void SeedAddWindow::destroy_gui(){

}

bool SeedAddWindow::on_key_press_event(GdkEventKey* key_event){
    // TODO: add other stuff ?

    if((key_event->keyval == GDK_KEY_s) &&
            (((key_event->state & (GDK_CONTROL_MASK)) == GDK_CONTROL_MASK) || ((key_event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD2_MASK)) == GDK_CONTROL_MASK))){
        // CTRL + s save seed
        save_seed();
    }else if(key_event->keyval == GDK_KEY_Escape){
        //close the window, when the 'esc' key is pressed
        // TODO : ask for confirmation if fields are filed,
        //  SAVE, CANCEL, CLOSE
        hide();
        return true;
    }

    //if the event has not been handled, call the base class
    return Gtk::Window::on_key_press_event(key_event);
}

void SeedAddWindow::save_seed(){
    m_seed_tree_model->clear();
    if (m_seed == NULL){
        m_seed = new Seed();
        m_controller->get_model()->add_seed(m_seed);
    }
    m_seed->set_name(m_name_entry->get_buffer()->get_text());
    m_seed->set_binomial_nomenclature(m_binomial_name_entry->get_buffer()->get_text());
    m_seed->set_description(m_description_textfield->get_buffer()->get_text());
    m_seed->print_seed();

    m_controller->get_model()->save_content();

    // Update MainWindow display, is there a better way to do it ?
    // Via a callback or something ?
    std::list<Seed*> seeds = m_controller->get_model()->get_seeds();
    for (std::list<Seed*>::iterator it = seeds.begin(); it != seeds.end(); it++){
        Gtk::ListStore::Row row = *(m_seed_tree_model->append());
        row[m_seed_columns->m_seed_id] = (*it)->get_id();
        row[m_seed_columns->m_seed_name] = (*it)->get_name();
        row[m_seed_columns->m_seed_binomial_nomenclature] = (*it)->get_binomial_nomenclature();
        row[m_seed_columns->m_seed_description] = (*it)->get_description();
    }

    //hide();
}

void SeedAddWindow::fill_gui(){
    m_name_entry->set_text(m_seed->get_name());
    m_binomial_name_entry->set_text(m_seed->get_binomial_nomenclature());
    m_description_textfield->get_buffer()->set_text(m_seed->get_description());
}
