#include "CategoryWindow.h"

// TODO: right click on treestore to add, delete
// Add keyboard shortcuts (Delete)

CategoryWindow::CategoryWindow(Controller* controller){
    m_controller = controller;
    create_gui();
    fill_tree_store();
    connect_signals();
}

CategoryWindow::~CategoryWindow(){

}

void CategoryWindow::create_gui(){

    m_category_scrolled_window = create_category_scrolled_window();

    m_main_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_main_box->pack_start(*m_category_scrolled_window, Gtk::PACK_EXPAND_WIDGET);

    m_header_bar = create_header_bar();

    Gtk::Box* box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    box->pack_start(*m_header_bar, Gtk::PACK_SHRINK);
    box->pack_start(*m_main_box, Gtk::PACK_EXPAND_WIDGET);

    //add(*m_header_bar);
    add(*box);

    show_all_children();
    set_default_size(300,250);
    set_title("Category Manager");
}

void CategoryWindow::connect_signals(){
    m_add_button->signal_clicked().connect(sigc::mem_fun(*this, &CategoryWindow::on_add_button_clicked));
    m_delete_button->signal_clicked().connect(sigc::mem_fun(*this, &CategoryWindow::on_delete_button_clicked));

   m_category_list_store->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &CategoryWindow::on_list_store_selection_change));

   m_category_tree_model->signal_row_changed().connect(sigc::mem_fun(*this, &CategoryWindow::on_list_store_row_change));
}

Gtk::ScrolledWindow* CategoryWindow::create_category_scrolled_window(){
    Gtk::ScrolledWindow* scrolled_window = Gtk::manage(new Gtk::ScrolledWindow());

    m_category_tree_model = Gtk::ListStore::create(m_category_columns);
    m_category_list_store = Gtk::manage(new Gtk::TreeView());

    m_category_list_store->set_model(m_category_tree_model);
    scrolled_window->add(*m_category_list_store);

    m_category_list_store->append_column_editable("Category Name", m_category_columns.m_category_name);
    m_category_list_store->get_column(0)->set_sort_column(m_category_columns.m_category_name);
    m_category_list_store->get_column(0)->set_resizable(true);

    return scrolled_window;
}

Gtk::HeaderBar* CategoryWindow::create_header_bar(){
    Gtk::HeaderBar* header_bar = Gtk::manage(new Gtk::HeaderBar());

    m_add_button = Gtk::manage(new Gtk::ToolButton());
    m_add_button->set_icon_name("list-add");

    m_delete_button = Gtk::manage(new Gtk::ToolButton());
    m_delete_button->set_icon_name("list-remove");

    header_bar->add(*m_add_button);
    header_bar->add(*m_delete_button);

    m_delete_button->set_sensitive(false);

    return header_bar;
}

bool CategoryWindow::on_key_press_event(GdkEventKey* key_event){

    if(key_event->keyval == GDK_KEY_Delete){
        // Delete key will delete selected categorie(s) ?
        delete_category();
        return true;
    }else if((key_event->keyval == GDK_KEY_n) &&
            (((key_event->state & (GDK_CONTROL_MASK)) == GDK_CONTROL_MASK) || ((key_event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD2_MASK)) == GDK_CONTROL_MASK))){
        add_category();
    }else if(key_event->keyval == GDK_KEY_Escape){
        //close the window, when the 'esc' key is pressed
        // TODO: ask for confirmation before closing
        hide();
        return true;
    }

    //if the event has not been handled, call the base class
    return Gtk::Window::on_key_press_event(key_event);
}

void CategoryWindow::on_add_button_clicked(){
    add_category();
}

void CategoryWindow::on_delete_button_clicked(){
    delete_category();
}

void CategoryWindow::add_category(){
    Gtk::ListStore::Row row = *(m_category_tree_model->prepend());
    Category* category = new Category("New Category");
    Gtk::TreeModel::iterator iter = m_category_tree_model->children().begin();
        if(iter){
            m_category_list_store->set_cursor(m_category_list_store->get_model()->get_path(iter), *m_category_list_store->get_column(0), true);
        }
    m_controller->get_model()->add_category(category);
    m_controller->get_model()->save_content();

    /* row[m_category_columns.m_category_id] = category->get_id(); */
    /* row[m_category_columns.m_category_name] = category->get_category_name(); */
}

void CategoryWindow::delete_category(){
    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_category_list_store->get_selection();
    Gtk::TreeModel::iterator iter = refTreeSelection->get_selected();
    if (iter){
        Gtk::TreeModel::Row row = *iter;
        std::cout << "[" << __FUNCTION__ << "] Selected: ";
        std::cout << row[m_category_columns.m_category_id] << " - ";
        std::cout << row[m_category_columns.m_category_name] << "\n";

        Gtk::MessageDialog dialog_warning(*this, "Are you sure you want to delete this category ?",
                false /* use_markup */, Gtk::MESSAGE_WARNING,
                Gtk::BUTTONS_YES_NO);

        dialog_warning.set_icon_name("dialog-warning");
        int result = dialog_warning.run();

        //Handle the response:
        switch(result)
        {
            case(Gtk::RESPONSE_YES):
                {
                    Category* c = m_controller->get_model()->get_category_by_id(row[m_category_columns.m_category_id]);
                    m_controller->get_model()->remove_category(c);
                    m_category_tree_model->erase(iter);
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
        display_category_selection_needed();
    }
}

void CategoryWindow::on_list_store_selection_change(){
    if (m_category_list_store->get_selection()->count_selected_rows() <= 0){
        m_delete_button->set_sensitive(false);
    } else {
        m_delete_button->set_sensitive(true);
    }
}

void CategoryWindow::fill_tree_store(){
    std::list<Category*> categories = m_controller->get_model()->get_categories();
    m_category_tree_model->clear();
    for (std::list<Category*>::iterator it = categories.begin(); it != categories.end(); it++){
        Gtk::ListStore::Row row = *(m_category_tree_model->append());
        row[m_category_columns.m_category_id] = (*it)->get_id();
        row[m_category_columns.m_category_name] = (*it)->get_category_name();
        std::cout << "[" << __FUNCTION__ << "] " << (*it)->get_category_name() << std::endl;
    }
}

void CategoryWindow::display_category_selection_needed(){
    Gtk::MessageDialog dialog(*this, 
            "Please select a category first.",
            false,
            Gtk::MESSAGE_ERROR,
            Gtk::BUTTONS_OK);
    dialog.set_icon_name("dialog-error");
    dialog.run();
}

void CategoryWindow::on_list_store_row_change(const Gtk::TreeModel::Path& path, Gtk::TreeIter iter){
    // TODO: save in db the changd Category
    Gtk::TreeModel::Row row = *iter;
    std::cout << "[" << __FUNCTION__ << "] row id : " << row[m_category_columns.m_category_id] << " - ";
    std::cout << row[m_category_columns.m_category_name] << "\n";

    /* Category* c =  m_controller->get_model()->get_category_by_id(row[m_category_columns.m_category_id]); */
    /* c->set_category_name(row[m_category_columns.m_category_name]); */
    /* m_controller->get_model()->save_content(); */
}
