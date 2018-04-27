#include "SeedInfoWindow.h"

SeedInfoWindow::SeedInfoWindow(){

}

SeedInfoWindow::SeedInfoWindow(Controller* controller, Seed* s, Glib::RefPtr<Gtk::ListStore> seed_model, SeedColumnsModel* columns){
    m_controller = controller;
    m_seed = s;
    m_seed_tree_model = seed_model;
    m_seed_columns = columns;
    create_gui();
    fill_gui();
    connect_signals();
}

SeedInfoWindow::~SeedInfoWindow(){
    destroy_gui();
}

void SeedInfoWindow::create_gui(){
    m_main_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_notebook = Gtk::manage(new Gtk::Notebook());
    m_notebook->set_tab_pos(Gtk::POS_LEFT);

    m_main_info_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    m_main_info_label_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_main_info_textfield_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_name_label = Gtk::manage(Gtk::manage(new Gtk::Label("Name"));
    m_name_entry = Gtk::manage(new Gtk::Entry());
    m_binomial_name_label = Gtk::manage(new Gtk::Label("Binomial nomenclature"));
    m_binomial_name_entry = Gtk::manage(new Gtk::Entry());
    m_description_label = Gtk::manage(new Gtk::Label("Description"));
    m_description_textfield = Gtk::manage(new Gtk::TextView());

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

    m_notebook->append_page(*m_main_info_box, "Main Info");
    m_notebook->append_page(*m_main_info_box, "Description");
    m_notebook->append_page(*m_dates_box, "Dates");
    m_notebook->append_page(*m_dates_box, "Stock");
    m_notebook->append_page(*m_pictures_box, "Pictures");

    m_edit_toolbar->append(*m_save_button);

    m_main_box->pack_start(*m_edit_toolbar, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_notebook, Gtk::PACK_EXPAND_WIDGET);

    add(*m_main_box);

    show_all_children();

    m_name_entry->grab_focus();

    set_default_size(400,400);
    set_title(m_seed->get_name());

}

void SeedInfoWindow::destroy_gui(){
    delete m_main_box;
    delete m_notebook;

    delete m_main_info_box;
    delete m_pictures_box;
    delete m_dates_box;

    delete m_edit_toolbar;
    delete m_save_button;
}

void SeedInfoWindow::connect_signals(){
    m_save_button->signal_clicked().connect(sigc::mem_fun(*this, &SeedInfoWindow::on_save_button_clicked));
}

void SeedInfoWindow::on_save_button_clicked(){
    m_seed->set_name(m_name_entry->get_text());
    m_seed->set_binomial_nomenclature(m_binomial_name_entry->get_text());
    m_seed->set_description(m_description_textfield->get_buffer()->get_text());
    m_controller->get_model()->save_content();

    // Update MainWindow display, is there a better way to do it ?
    // Via a callback or something ?

    std::list<Seed*> seeds = m_controller->get_model()->get_seeds();
    m_seed_tree_model->clear();
    for (std::list<Seed*>::iterator it = seeds.begin(); it != seeds.end(); it++){
        Gtk::ListStore::Row row = *(m_seed_tree_model->append());
        row[m_seed_columns->m_seed_id] = (*it)->get_id();
        row[m_seed_columns->m_seed_name] = (*it)->get_name();
        row[m_seed_columns->m_seed_binomial_nomenclature] = (*it)->get_binomial_nomenclature();
        row[m_seed_columns->m_seed_description] = (*it)->get_description();
    }

    /*
    Gtk::ListStore::Row row = *(m_seed_tree_model->append());
    row[m_seed_columns->m_seed_id] = m_seed->get_id();
    row[m_seed_columns->m_seed_name] = m_seed->get_name();
    row[m_seed_columns->m_seed_binomial_nomenclature] = m_seed->get_binomial_nomenclature();
    row[m_seed_columns->m_seed_description] = m_seed->get_description();
    */
}

void SeedInfoWindow::fill_gui(){
    m_name_entry->set_text(m_seed->get_name());
    m_binomial_name_entry->set_text(m_seed->get_binomial_nomenclature());
    m_description_textfield->get_buffer()->set_text(m_seed->get_description());
}
