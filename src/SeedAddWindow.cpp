#include "SeedAddWindow.h"

SeedAddWindow::SeedAddWindow(){

}

SeedAddWindow::SeedAddWindow(Controller* controller, Glib::RefPtr<Gtk::ListStore> seed_model, SeedColumnsModel* columns){
    m_controller = controller;
    m_seed_tree_model = seed_model;
    m_seed_columns = columns;
    create_gui();
    connect_signals();
}

SeedAddWindow::~SeedAddWindow(){
    destroy_gui();
}

void SeedAddWindow::create_gui(){
    m_main_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_notebook = Gtk::manage(new Gtk::Notebook());
    m_notebook->set_tab_pos(Gtk::POS_LEFT);

    m_main_info_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    // TODO: grid instead of vbox
    m_main_info_label_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_main_info_textfield_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_name_label = Gtk::manage(new Gtk::Label("Name"));
    m_name_entry = Gtk::manage(new Gtk::Entry());
    m_binomial_name_label = Gtk::manage(new Gtk::Label("Binomial nomenclature"));
    m_binomial_name_entry = Gtk::manage(new Gtk::Entry());
    // TODO: a mettre dans un onglet à part ?
    m_description_label = Gtk::manage(new Gtk::Label("Description"));
    m_description_textfield = Gtk::manage(Gtk::manage(new Gtk::TextView()));

    m_main_info_label_box->pack_start(*m_name_label, Gtk::PACK_SHRINK);
    m_main_info_label_box->pack_start(*m_binomial_name_label, Gtk::PACK_SHRINK);
    m_main_info_label_box->pack_start(*m_description_label, Gtk::PACK_SHRINK);

    m_main_info_textfield_box->pack_start(*m_name_entry, Gtk::PACK_SHRINK);
    m_main_info_textfield_box->pack_start(*m_binomial_name_entry, Gtk::PACK_SHRINK);
    m_main_info_textfield_box->pack_start(*m_description_textfield, Gtk::PACK_EXPAND_WIDGET);

    m_main_info_box->pack_start(*m_main_info_label_box, Gtk::PACK_SHRINK);
    m_main_info_box->pack_start(*m_main_info_textfield_box, Gtk::PACK_SHRINK);

    m_pictures_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_dates_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_edit_toolbar = Gtk::manage(new Gtk::Toolbar());
    m_save_button = Gtk::manage(new Gtk::ToolButton("Save"));
    m_delete_button = Gtk::manage(new Gtk::ToolButton("Delete"));

    m_notebook->append_page(*m_main_info_box, "Main Info");
    m_notebook->append_page(*m_pictures_box, "Pictures");
    m_notebook->append_page(*m_dates_box, "Dates");

    m_edit_toolbar->append(*m_save_button);
    m_edit_toolbar->append(*m_delete_button);

    m_main_box->pack_start(*m_edit_toolbar, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_notebook, Gtk::PACK_EXPAND_WIDGET);

    add(*m_main_box);

    show_all_children();

    m_name_entry->grab_focus();

    set_default_size(400,400);
    set_title("Add a new seed");
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
            ((key_event->state & (GDK_CONTROL_MASK)) == GDK_CONTROL_MASK) || ((key_event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD2_MASK)) == GDK_CONTROL_MASK)){
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
    Seed* seed = new Seed();
    seed->set_name(m_name_entry->get_buffer()->get_text());
    seed->set_binomial_nomenclature(m_binomial_name_entry->get_buffer()->get_text());
    seed->set_description(m_description_textfield->get_buffer()->get_text());
    seed->print_seed();

    m_controller->get_model()->add_seed(seed);
    m_controller->get_model()->save_content();

    // Update MainWindow display, is there a better way to do it ?
    // Via a callback or something ?
    Gtk::ListStore::Row row = *(m_seed_tree_model->append());
    row[m_seed_columns->m_seed_id] = seed->get_id();
    row[m_seed_columns->m_seed_name] = seed->get_name();
    row[m_seed_columns->m_seed_binomial_nomenclature] = seed->get_binomial_nomenclature();
    row[m_seed_columns->m_seed_description] = seed->get_description();
}
