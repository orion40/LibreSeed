#include "SeedAddWindow.h"

SeedAddWindow::SeedAddWindow(){

}

SeedAddWindow::SeedAddWindow(Controller* controller){
    m_controller = controller;
    create_gui();
    connect_signals();
}

SeedAddWindow::~SeedAddWindow(){
    destroy_gui();
}

void SeedAddWindow::create_gui(){

    m_main_box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    m_notebook = new Gtk::Notebook();
    m_notebook->set_tab_pos(Gtk::POS_LEFT);

    m_main_info_box = new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL);
    m_main_info_label_box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    m_main_info_textfield_box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);

    m_name_label = new Gtk::Label("Name");
    m_name_entry = new Gtk::Entry();
    m_binomial_name_label = new Gtk::Label("Binomial nomenclature");
    m_binomial_name_entry = new Gtk::Entry();
    m_description_label = new Gtk::Label("Description");
    m_description_textfield = new Gtk::TextView();

    m_main_info_label_box->pack_start(*m_name_label, Gtk::PACK_SHRINK);
    m_main_info_label_box->pack_start(*m_binomial_name_label, Gtk::PACK_SHRINK);
    m_main_info_label_box->pack_start(*m_description_label, Gtk::PACK_SHRINK);

    m_main_info_textfield_box->pack_start(*m_name_entry, Gtk::PACK_SHRINK);
    m_main_info_textfield_box->pack_start(*m_binomial_name_entry, Gtk::PACK_SHRINK);
    m_main_info_textfield_box->pack_start(*m_description_textfield, Gtk::PACK_EXPAND_WIDGET);

    m_main_info_box->pack_start(*m_main_info_label_box, Gtk::PACK_SHRINK);
    m_main_info_box->pack_start(*m_main_info_textfield_box, Gtk::PACK_SHRINK);

    m_pictures_box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
    m_dates_box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);

    m_edit_toolbar = new Gtk::Toolbar();
    m_save_button = new Gtk::ToolButton("Save");

    m_notebook->append_page(*m_main_info_box, "Main Info");
    m_notebook->append_page(*m_pictures_box, "Pictures");
    m_notebook->append_page(*m_dates_box, "Dates");

    m_edit_toolbar->append(*m_save_button);

    m_main_box->pack_start(*m_edit_toolbar, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_notebook, Gtk::PACK_EXPAND_WIDGET);

    add(*m_main_box);

    show_all_children();

    m_name_entry->grab_focus();

    set_default_size(400,400);
}

void SeedAddWindow::connect_signals(){
    m_save_button->signal_clicked().connect(sigc::mem_fun(*this, &SeedAddWindow::on_save_button_clicked));
}

void SeedAddWindow::on_save_button_clicked(){
    Seed* seed = new Seed();
    seed->set_name(m_name_entry->get_buffer()->get_text());
    seed->set_binomial_nomenclature(m_binomial_name_entry->get_buffer()->get_text());
    seed->set_description(m_description_textfield->get_buffer()->get_text());
    seed->print_seed();
    m_controller->get_model()->add_seed(seed);
    m_controller->get_model()->save_content();
}

void SeedAddWindow::destroy_gui(){
    delete m_main_box;
    delete m_notebook;

    delete m_main_info_box;
    delete m_pictures_box;
    delete m_dates_box;

    delete m_edit_toolbar;
    delete m_save_button;
}
