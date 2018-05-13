#include "MainWindow.h"

MainWindow::MainWindow(){
    m_seed_add_window = NULL;
    m_category_window = NULL;
}


MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app, Controller* controller): MainWindow(){
    m_controller = controller;
    if (m_controller->open_db()){
        // TODO: show_splashcreen
        create_gui(app);
        init_gui();
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

void MainWindow::create_gui(const Glib::RefPtr<Gtk::Application>& app){

    m_action_group = Gio::SimpleActionGroup::create();
    m_action_group->add_action("new",
                sigc::mem_fun(*this, &MainWindow::on_add_button_clicked) );
    m_action_group->add_action("open-selected",
                sigc::mem_fun(*this, &MainWindow::on_open_seed_info_button_clicked) );
    m_action_group->add_action("manage-categories",
                sigc::mem_fun(*this, &MainWindow::open_categories_manager) );
    m_action_group->add_action("delete",
                sigc::mem_fun(*this, &MainWindow::on_delete_button_clicked) );

    m_action_group->add_action("help",
                sigc::mem_fun(*this, &MainWindow::on_help_button_clicked) );
    m_action_group->add_action("about",
                sigc::mem_fun(*this, &MainWindow::on_about_button_clicked) );

    insert_action_group("menu", m_action_group);

    m_builder = Gtk::Builder::create();

    //Layout the actions in a menubar and toolbar:
    const char* ui_info =
        "<interface>"
        "  <menu id='menubar'>"
        "    <submenu>"
        "      <attribute name='label' translatable='yes'>_File</attribute>"
        "      <section>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_New</attribute>"
        "          <attribute name='action'>menu.new</attribute>"
        "          <attribute name='accel'>&lt;Primary&gt;n</attribute>"
        "        </item>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_Manage categories</attribute>"
        "          <attribute name='action'>menu.manage-categories</attribute>"
        "          <attribute name='accel'>&lt;Primary&gt;g</attribute>"
        "        </item>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_Open Seed Info</attribute>"
        "          <attribute name='action'>menu.open-selected</attribute>"
        "          <attribute name='accel'>&lt;Primary&gt;o</attribute>"
        "        </item>"
        "      </section>"
        "      <section>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_Delete</attribute>"
        "          <attribute name='action'>menu.delete</attribute>"
        //"          <attribute name='accel'>&lt;Primary&gt;n</attribute>"
        "        </item>"
        "      </section>"
        "      <section>"
        "        <item>"
        "          <attribute name='label' translatable='yes'>_Quit</attribute>"
        "          <attribute name='action'>menu.quit</attribute>"
        "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
        "        </item>"
        "      </section>"
        "    </submenu>"
        "    <submenu>"
        "      <attribute name='label' translatable='yes'>_Help</attribute>"
        "      <item>"
        "        <attribute name='label' translatable='yes'>_Help</attribute>"
        "        <attribute name='action'>menu.help</attribute>"
        "        <attribute name='accel'>F1</attribute>"
        "      </item>"
        "      <item>"
        "        <attribute name='label' translatable='yes'>_About</attribute>"
        "        <attribute name='action'>menu.about</attribute>"
        "      </item>"
        "    </submenu>"
        "  </menu>"
        "</interface>";

    // When the menubar is a child of a Gtk::Window, keyboard accelerators are not
    // automatically fetched from the Gio::Menu.
    // See the menus/book/menus/main_menu menu for an alternative way of
    // adding the menubar when using Gtk::ApplicationWindow.
    // Gtk::Application::set_accel_for_action() is new in gtkmm 3.11.9.
    app->set_accel_for_action("menu.new", "<Primary>n");
    app->set_accel_for_action("menu.open-selected", "<Primary>o");
    app->set_accel_for_action("menu.manage-categories", "<Primary>g");
    app->set_accel_for_action("menu.quit", "<Primary>q");
    app->set_accel_for_action("menu.cut", "<Primary>x");
    app->set_accel_for_action("menu.copy", "<Primary>c");
    app->set_accel_for_action("menu.paste", "<Primary>v");

    try
    {
        m_builder->add_from_string(ui_info);
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "Building menus and toolbar failed: " <<  ex.what();
    }

    m_main_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_control_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_search_bar = Gtk::manage(new Gtk::SearchBar());

    //Get the menubar:
    auto object = m_builder->get_object("menubar");
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    if (!gmenu)
        g_warning("GMenu not found");
    else
    {
        m_menu_bar = Gtk::manage(new Gtk::MenuBar(gmenu));

        //Add the MenuBar to the window:
        m_main_box->pack_start(*m_menu_bar, Gtk::PACK_SHRINK);
    }

    m_management_toolbar = create_management_toolbar();
    m_seed_list_store = create_seed_list_store();

    m_seed_tree_view_scrolled_window = Gtk::manage(new Gtk::ScrolledWindow());
    m_seed_tree_view_scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_seed_tree_view_scrolled_window->add(*m_seed_list_store);

    m_search_text_view = Gtk::manage(new Gtk::SearchEntry());

    // Search elements
    m_search_bar->connect_entry(*m_search_text_view);
    m_search_bar->set_search_mode(true);

    m_control_box->pack_start(*m_management_toolbar, Gtk::PACK_SHRINK);
    m_control_box->pack_start(*m_search_bar, Gtk::PACK_SHRINK);

    // Add all elements to the main window
    m_main_box->pack_start(*m_control_box, Gtk::PACK_SHRINK);
    m_main_box->pack_start(*m_seed_tree_view_scrolled_window, Gtk::PACK_EXPAND_WIDGET);
    add(*m_main_box);

    show_all_children();

    m_seed_list_store->grab_focus();

    // set_icon_from_file("../icons/");
    set_default_size(600,500);
    set_title("Seed Manager");
}

void MainWindow::init_gui(){
    m_delete_button->set_sensitive(false);
    m_open_seed_info_button->set_sensitive(false);
    m_export_selected_xml->set_sensitive(false);
}

void MainWindow::connect_signals(){
    m_add_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_add_button_clicked));
    m_open_seed_info_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_open_seed_info_button_clicked));
    m_delete_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_delete_button_clicked));

    m_export_all_xml->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_export_all_xml_clicked));
    m_export_selected_xml->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_export_selected_xml_clicked));
    m_import_xml->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_import_xml_clicked));

    m_seed_list_store->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_list_store_row_activated));
    m_seed_list_store->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_list_store_selection_change));
}

bool MainWindow::on_key_press_event(GdkEventKey* key_event){

    if(key_event->keyval == GDK_KEY_Delete){
        // Delete key will delete selected seed(s ?)
        delete_selected_seed();
        return true;
    }else if((key_event->keyval == GDK_KEY_n) &&
            (((key_event->state & (GDK_CONTROL_MASK)) == GDK_CONTROL_MASK) || ((key_event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD2_MASK)) == GDK_CONTROL_MASK))){
        // CTRL + n open seed add window
        open_add_seed_window();
        return true;
    }else if(key_event->keyval == GDK_KEY_Escape){
        //close the window, when the 'esc' key is pressed
        // TODO: ask for confirmation before closing
        hide();
        return true;
    }

    //if the event has not been handled, call the base class
    return Gtk::Window::on_key_press_event(key_event);
}

void MainWindow::on_add_button_clicked(){
    open_add_seed_window();
}

void MainWindow::open_add_seed_window(){
    if (m_seed_add_window == NULL){
        m_seed_add_window = new SeedAddWindow(m_controller, m_seed_tree_model, &m_seed_columns);
    } else {
        delete m_seed_add_window;
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
        std::cout << "Row selected: ID=" << row[m_seed_columns.m_seed_id] << ", Name="             << row[m_seed_columns.m_seed_plant_name] << std::endl;
        open_seed_info(m_controller->get_model()->getSeedById(row[m_seed_columns.m_seed_id]));
    } else {
        display_seed_selection_needed();
    }
}

void MainWindow::on_delete_button_clicked(){
    delete_selected_seed();
}

void MainWindow::delete_selected_seed(){
    Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = m_seed_list_store->get_selection();
    Gtk::TreeModel::iterator iter = refTreeSelection->get_selected();
    if (iter){
        Gtk::TreeModel::Row row = *iter;
        std::cout << "Selected: ";
        std::cout << row[m_seed_columns.m_seed_id] << " - ";
        std::cout << row[m_seed_columns.m_seed_plant_name] << " - ";
        std::cout << row[m_seed_columns.m_seed_binomial_nomenclature] << " - ";
        std::cout << row[m_seed_columns.m_seed_description] << "\n";

        Gtk::MessageDialog dialog_warning(*this, "Are you sure you want to delete this seed ?",
                false /* use_markup */, Gtk::MESSAGE_WARNING,
                Gtk::BUTTONS_YES_NO);

        dialog_warning.set_icon_name("dialog-warning");
        int result = dialog_warning.run();

        //Handle the response:
        switch(result)
        {
            case(Gtk::RESPONSE_YES):
                {
                    Seed*s = m_controller->get_model()->getSeedById(row[m_seed_columns.m_seed_id]);
                    m_controller->get_model()->remove_seed(s);
                    m_seed_tree_model->erase(iter);
                    //fill_tree_store();
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
        display_seed_selection_needed();
    }
}

void MainWindow::fill_tree_store(){
    std::list<Seed*> seeds = m_controller->get_model()->get_seeds();
    m_seed_tree_model->clear();
    for (std::list<Seed*>::iterator it = seeds.begin(); it != seeds.end(); it++){
        Gtk::ListStore::Row row = *(m_seed_tree_model->append());
        row[m_seed_columns.m_seed_id] = (*it)->get_id();
        row[m_seed_columns.m_seed_plant_name] = (*it)->get_plant_name();
        row[m_seed_columns.m_seed_variety_name] = (*it)->get_variety_name();
        row[m_seed_columns.m_seed_binomial_nomenclature] = (*it)->get_binomial_nomenclature();
        row[m_seed_columns.m_seed_description] = (*it)->get_description();
    }
}

void MainWindow::destroy_gui(){

}

void MainWindow::on_list_store_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    Gtk::TreeModel::iterator iter = m_seed_tree_model->get_iter(path);
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        std::cout << "Row activated: ID=" << row[m_seed_columns.m_seed_id] << ", Name="             << row[m_seed_columns.m_seed_plant_name] << std::endl;
        open_seed_info(m_controller->get_model()->getSeedById(row[m_seed_columns.m_seed_id]));
    }
}

void MainWindow::open_seed_info(Seed* s){
    if (s){
        if (m_seed_add_window == NULL){
            m_seed_add_window = new SeedAddWindow(m_controller, m_seed_tree_model, &m_seed_columns, s);
        } else {
            delete m_seed_add_window;
            m_seed_add_window = new SeedAddWindow(m_controller, m_seed_tree_model, &m_seed_columns, s);
        }
        m_seed_add_window->show();
    }
}

void MainWindow::display_seed_selection_needed(){
    Gtk::MessageDialog dialog(*this, 
            "Please select a seed first.",
            false,
            Gtk::MESSAGE_ERROR,
            Gtk::BUTTONS_OK);
    dialog.set_icon_name("dialog-error");
    dialog.run();
}

void MainWindow::on_list_store_selection_change(){
    if (m_seed_list_store->get_selection()->count_selected_rows() <= 0){
        m_delete_button->set_sensitive(false);
        m_open_seed_info_button->set_sensitive(false);
        m_export_selected_xml->set_sensitive(false);
    } else {
        m_delete_button->set_sensitive(true);
        m_open_seed_info_button->set_sensitive(true);
        m_export_selected_xml->set_sensitive(true);
    }
}

void MainWindow::on_export_all_xml_clicked(){
    std::cout << "EXPORT ALL\n";

    if (get_model()->export_all_to_xml(open_xml_dialog()) == false){
        // ERROR MESSAGE
        std::cout << "unable to export\n";
    }
}

void MainWindow::on_export_selected_xml_clicked(){
    std::cout << "EXPORT ONE\n";

    //if (get_model()->export_seeds_to_xml(open_xml_dialog(), ) == false) {
    //  // ERROR MSG
    //}
}

void MainWindow::on_import_xml_clicked(){
    std::cout << "IMPORT\n";

    if (get_model()->import_to_xml(open_xml_dialog()) == false){
        std::cout << "unable to import\n";
        // ERROR MSG
    }
}

std::string MainWindow::open_xml_dialog(){
    std::string filename = "";
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("XML files");
    filter_text->add_mime_type("text/xml");
    filter_text->add_mime_type("application/xml");
    dialog.add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);
    dialog.set_action(Gtk::FILE_CHOOSER_ACTION_SAVE);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
        case(Gtk::RESPONSE_OK):
            {
                filename = dialog.get_filename();
                break;
            }
        case(Gtk::RESPONSE_CANCEL):
            {
                break;
            }
        default:
            {
                break;
            }
    }

    return filename;
}

void MainWindow::on_help_button_clicked(){
    std::cout << "HALP\n";
}

void MainWindow::on_about_button_clicked(){

    Gtk::AboutDialog* dialog = new Gtk::AboutDialog();

    dialog->set_transient_for(*this);

    //dialog->set_logo(Gdk::Pixbuf::create_from_resource("/about/gtkmm_logo.gif", -1, 40, true));
    dialog->set_program_name("Seed Manager");
    dialog->set_version("0.1.0");
    dialog->set_copyright("orion40");
    dialog->set_comments("This is just a kickass application.");
    dialog->set_license("LGPL");

    dialog->set_website("http://www.github.com/orion40/");
    dialog->set_website_label("Project Repo");

    dialog->show();
    dialog->present();
}

void MainWindow::open_categories_manager(){
    if (m_category_window == NULL){
        m_category_window = new CategoryWindow(m_controller);
    } else {
        delete m_category_window;
        m_category_window = new CategoryWindow(m_controller);
    }
    m_category_window->show();
}

Gtk::TreeView* MainWindow::create_seed_list_store(){

    Gtk::TreeView* list_store = Gtk::manage(new Gtk::TreeView());
    // Tree store and model
    m_seed_tree_model = Gtk::ListStore::create(m_seed_columns);
    // TODO: later for multiple selection
    //list_store->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
    list_store->set_model(m_seed_tree_model);

    list_store->append_column("ID", m_seed_columns.m_seed_id);
    list_store->get_column(0)->set_sort_column(m_seed_columns.m_seed_id);
    list_store->get_column(0)->set_resizable(true);
    list_store->get_column(0)->set_visible(false);

    list_store->append_column("Plant Name", m_seed_columns.m_seed_plant_name);
    list_store->get_column(1)->set_sort_column(m_seed_columns.m_seed_plant_name);
    list_store->get_column(1)->set_resizable(true);


    list_store->append_column("Plant Variety", m_seed_columns.m_seed_variety_name);
    list_store->get_column(2)->set_sort_column(m_seed_columns.m_seed_variety_name);
    list_store->get_column(2)->set_resizable(true);


    list_store->append_column("Binomial Nomenclature", m_seed_columns.m_seed_binomial_nomenclature);
    list_store->get_column(3)->set_sort_column(m_seed_columns.m_seed_binomial_nomenclature);
    list_store->get_column(3)->set_resizable(true);


    list_store->append_column("Description", m_seed_columns.m_seed_description);
    list_store->get_column(4)->set_sort_column(m_seed_columns.m_seed_description);
    list_store->get_column(4)->set_resizable(true);

    return list_store;
}

Gtk::Toolbar* MainWindow::create_management_toolbar(){
    Gtk::Toolbar* toolbar = Gtk::manage(new Gtk::Toolbar());
    m_add_button = Gtk::manage(new Gtk::ToolButton("Add"));
    m_add_button->set_icon_name("document-new");
    m_delete_button = Gtk::manage(new Gtk::ToolButton("Delete"));
    m_delete_button->set_icon_name("edit-delete");
    m_open_seed_info_button = Gtk::manage(new Gtk::ToolButton("Open Seed card"));
    m_open_seed_info_button->set_icon_name("document-open");
    // TODO : add icons
    m_export_selected_xml = Gtk::manage(new Gtk::ToolButton("Export Selected"));
    m_export_all_xml = Gtk::manage(new Gtk::ToolButton("Export All"));
    m_import_xml = Gtk::manage(new Gtk::ToolButton("Import"));
    m_print_button = Gtk::manage(new Gtk::ToolButton("Print"));
    m_print_button->set_icon_name("document-print");
    m_print_button->set_sensitive(false);

    // Add buttons to toolbar
    toolbar->append(*m_add_button);
    toolbar->append(*m_delete_button);
    toolbar->append(*m_open_seed_info_button);

    /* toolbar->append(*m_export_all_xml); */
    /* toolbar->append(*m_export_selected_xml); */
    /* toolbar->append(*m_import_xml); */

    /* toolbar->append(*m_print_button); */

    // Layout and graphic for "toolbar"
    //m_management_toolbar->set_layout(Gtk::BUTTONBOX_START);
    //m_management_toolbar->set_border_width(5);

    return toolbar;
}
