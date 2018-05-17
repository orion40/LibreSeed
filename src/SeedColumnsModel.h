#ifndef SEEDCOLUMNSMODEL_H
#define SEEDCOLUMNSMODEL_H

#include <gtkmm/treeview.h>

class SeedColumnsModel : public Gtk::TreeModel::ColumnRecord{
    public:
        SeedColumnsModel();
        ~SeedColumnsModel();

        Gtk::TreeModelColumn<int> m_seed_id;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_plant_name;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_variety_name;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_binomial_nomenclature;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_description;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_category;

    private:

};

#endif
