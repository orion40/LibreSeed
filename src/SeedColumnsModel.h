#ifndef SEEDCOLUMNSMODEL_H
#define SEEDCOLUMNSMODEL_H

#include <gtkmm/treeview.h>

class SeedColumnsModel : public Gtk::TreeModel::ColumnRecord{
    public:
        SeedColumnsModel();
        ~SeedColumnsModel();

        Gtk::TreeModelColumn<int> m_seed_id;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_name;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_binomial_nomenclature;
        Gtk::TreeModelColumn<Glib::ustring> m_seed_description;

    private:

};

#endif
