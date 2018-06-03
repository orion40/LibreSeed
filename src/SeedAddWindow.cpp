#include "SeedAddWindow.h"

// TODO: faire un bouton pour ajouter d'autre graines ?
// un new qui vide tout les champs et crée une nouvelle graine

SeedAddWindow::SeedAddWindow(){

}

SeedAddWindow::SeedAddWindow(Controller* controller, Glib::RefPtr<Gtk::ListStore> seed_model, SeedColumnsModel* columns): SeedAddWindow(){
    m_seed = NULL;
    m_controller = controller;
    m_seed_tree_model = seed_model;
    m_seed_columns = columns;
    set_title("Add a new seed");
    set_icon_name("document-new");
    create_gui();

    // Select No Category
    for (Gtk::TreeIter iter = m_category_combobox->get_model()->children().begin(); iter != m_category_combobox->get_model()->children().end(); iter++){
        if ((*iter)[m_category_columns.m_category_name] == "No category"){
            // We found the categories we have so we select it
            m_category_combobox->set_active(iter);
            break;
        }
    }

    connect_signals();
}

SeedAddWindow::SeedAddWindow(Controller* controller, Glib::RefPtr<Gtk::ListStore> seed_model, SeedColumnsModel* columns, Seed* seed) : SeedAddWindow(){
    m_seed = seed;
    m_controller = controller;
    m_seed_tree_model = seed_model;
    m_seed_columns = columns;
    set_title("Edit a seed");
    set_icon_name("document-open");
    create_gui();
    fill_gui();
    connect_signals();
}

SeedAddWindow::~SeedAddWindow(){
    destroy_gui();
}

void SeedAddWindow::create_gui(){
    m_main_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_edit_toolbar = create_edit_toolbar();

    m_notebook = Gtk::manage(new Gtk::Notebook());
    m_notebook->set_tab_pos(Gtk::POS_LEFT);

    m_main_info_grid = create_main_info_grid();
    m_description_box = create_description_box();
    m_dates_box = create_date_box();
    m_stock_box = create_stock_box();
    m_pictures_box = create_picture_box();

    m_notebook->append_page(*m_main_info_grid, "Main Info");
    m_notebook->append_page(*m_description_box, "Description");
    /* m_notebook->append_page(*m_dates_box, "Dates"); */
    /* m_notebook->append_page(*m_stock_box, "Stock"); */
    /* m_notebook->append_page(*m_pictures_box, "Pictures"); */

    m_main_box->pack_start(*m_edit_toolbar, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_notebook, Gtk::PACK_EXPAND_WIDGET);

    add(*m_main_box);

    show_all_children();

    m_plant_name_entry->grab_focus();

    set_default_size(400,400);
}

void SeedAddWindow::connect_signals(){
    m_save_button->signal_clicked().connect(sigc::mem_fun(*this, &SeedAddWindow::on_save_button_clicked));
    m_delete_button->signal_clicked().connect(sigc::mem_fun(*this, &SeedAddWindow::on_delete_button_clicked));

    signal_delete_event().connect(sigc::mem_fun(*this, &SeedAddWindow::on_window_close_event));
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
        close();
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
    m_seed->set_plant_name(m_plant_name_entry->get_buffer()->get_text());
    m_seed->set_variety_name(m_variety_name_entry->get_buffer()->get_text());
    m_seed->set_binomial_nomenclature(m_binomial_name_entry->get_buffer()->get_text());
    m_seed->set_description(m_description_textfield->get_buffer()->get_text());

    int plant_id = 0;
    Gtk::TreeModel::iterator iter = m_category_combobox->get_active();
    if (iter){
        Gtk::TreeModel::Row row = *iter;
        Category* c = m_controller->get_model()->get_category_by_name(row[m_category_columns.m_category_name]);
        if (c != NULL){
            plant_id = c->get_id();
        }
    }

    m_seed->set_plant_category_id(plant_id);
    m_seed->print_seed();

    m_controller->get_model()->save_content();

    // Update MainWindow display, is there a better way to do it ?
    // Via a callback or something ? VIA A PROPERTY !
    // TODO: add property on model so automatic updates !
    std::list<Seed*> seeds = m_controller->get_model()->get_seeds();
    for (std::list<Seed*>::iterator it = seeds.begin(); it != seeds.end(); it++){
        Gtk::ListStore::Row row = *(m_seed_tree_model->append());
        row[m_seed_columns->m_seed_id] = (*it)->get_id();
        row[m_seed_columns->m_seed_plant_name] = (*it)->get_plant_name();
        row[m_seed_columns->m_seed_variety_name] = (*it)->get_variety_name();
        row[m_seed_columns->m_seed_binomial_nomenclature] = (*it)->get_binomial_nomenclature();
        row[m_seed_columns->m_seed_description] = (*it)->get_description();
        Category* c = m_controller->get_model()->get_category_by_id((*it)->get_plant_category_id());
        // TODO: if category id not found, we return the empty category
        if (c != NULL){
            row[m_seed_columns->m_seed_category] = c->get_category_name();
        }
    }

    //hide();
}

void SeedAddWindow::fill_gui(){
    m_plant_name_entry->set_text(m_seed->get_plant_name());
    m_variety_name_entry->set_text(m_seed->get_variety_name());
    m_binomial_name_entry->set_text(m_seed->get_binomial_nomenclature());
    m_description_textfield->get_buffer()->set_text(m_seed->get_description());

    // Fill in the combobox
    Category* c = m_controller->get_model()->get_category_by_id(m_seed->get_plant_category_id());
    if (c != NULL){
        for (Gtk::TreeIter iter = m_category_combobox->get_model()->children().begin(); iter != m_category_combobox->get_model()->children().end(); iter++){
            if ((*iter)[m_category_columns.m_category_name] == c->get_category_name()){
                // We found the categories we have so we select it
                m_category_combobox->set_active(iter);
                break;
            }
        }
    }
    m_seed->print_seed();
}

Gtk::Box* SeedAddWindow::create_stock_box(){
    Gtk::Box* box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    return box;
}

Gtk::Box* SeedAddWindow::create_picture_box(){
    Gtk::Box* box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    return box;
}

Gtk::Box* SeedAddWindow::create_date_box(){
    Gtk::Box* box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    // How to add a Date ? Already here but empty
    // for int i = 0; i < MAX_DATES; i++
    //      create DateWidget

    return box;
}

Gtk::Grid* SeedAddWindow::create_main_info_grid(){
    Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());

    m_plant_name_label = Gtk::manage(new Gtk::Label("Plant Name"));
    m_plant_name_entry = Gtk::manage(new Gtk::Entry());
    m_variety_name_label = Gtk::manage(new Gtk::Label("Variety Name"));
    m_variety_name_entry = Gtk::manage(new Gtk::Entry());
    m_binomial_name_label = Gtk::manage(new Gtk::Label("Binomial nomenclature"));
    m_binomial_name_entry = Gtk::manage(new Gtk::Entry());
    m_category_label = Gtk::manage(new Gtk::Label("Category"));
    m_category_tree_model = Gtk::ListStore::create(m_category_columns);
    m_category_combobox = Gtk::manage(new Gtk::ComboBox());
    m_category_combobox->set_model(m_category_tree_model);

    std::list<Category*> categories = m_controller->get_model()->get_categories();
    m_category_tree_model->clear();
    for (std::list<Category*>::iterator it = categories.begin(); it != categories.end(); it++){
        Gtk::ListStore::Row row = *(m_category_tree_model->append());
        row[m_category_columns.m_category_name] = (*it)->get_category_name();
    }

    m_category_combobox->pack_start(m_category_columns.m_category_name);

    // Labels
    grid->attach(*m_plant_name_label,0,0,1,1);
    grid->attach(*m_variety_name_label,0,1,1,1);
    grid->attach(*m_binomial_name_label,0,2,1,1);
    grid->attach(*m_category_label,0,3,1,1);

    // Entry widgets
    grid->attach(*m_plant_name_entry,1,0,1,1);
    grid->attach(*m_variety_name_entry,1,1,1,1);
    grid->attach(*m_binomial_name_entry,1,2,1,1);
    grid->attach(*m_category_combobox,1,3,1,1);


    return grid;
}

Gtk::Box* SeedAddWindow::create_description_box(){
    Gtk::Box* box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    m_description_textfield = Gtk::manage(Gtk::manage(new Gtk::TextView()));
    m_description_textfield->set_wrap_mode(Gtk::WRAP_WORD_CHAR);

    box->pack_start(*m_description_textfield, Gtk::PACK_EXPAND_WIDGET);

    return box;
}

Gtk::Toolbar* SeedAddWindow::create_edit_toolbar(){
    Gtk::Toolbar* toolbar = Gtk::manage(new Gtk::Toolbar());

    m_save_button = Gtk::manage(new Gtk::ToolButton("Save"));
    m_save_button->set_icon_name("document-save");
    m_delete_button = Gtk::manage(new Gtk::ToolButton("Delete"));
    m_delete_button->set_icon_name("edit-delete");

    toolbar->append(*m_save_button);
    toolbar->append(*m_delete_button);

    return toolbar;
}

bool SeedAddWindow::on_window_close_event(GdkEventAny* event){
    //int result = display_confirm_close();
    int result = 0;

    switch (result){
        case 0:
            // Don't save, just close
            return Gtk::Window::on_delete_event(event);
            break;
        case 1:
            // Cancel
            return true;
            break;
        case 2:
            // Save and exit
            save_seed();
            return Gtk::Window::on_delete_event(event);
            break;
        default:
            return true;
            break;
    }

    return Gtk::Window::on_delete_event(event);
}

int SeedAddWindow::display_confirm_close(){
    // TODO : ask for confirmation if fields are filed
    /* if (m_seed->is_modified()){ */
    //  SAVE, CANCEL, CLOSE
    Gtk::MessageDialog dialog(*this, "Save changes to current seed ?",
            false /* use_markup */, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_NONE);

    dialog.set_icon_name("dialog-warning");

    dialog.add_button("Don't save", 0);
    dialog.add_button("Cancel", 1);
    dialog.add_button("Save", 2);

    return dialog.run();
    /* } else { */
    /*     return 0; */
    /* } */
}
