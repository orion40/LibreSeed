#ifndef CATEGORIESCOLUMNSMODEL_H
#define CATEGORIESCOLUMNSMODEL_H

#include <gtkmm/treeview.h>

class CategoriesColumnsModel: public Gtk::TreeModel::ColumnRecord{
    public:
        CategoriesColumnsModel();
        ~CategoriesColumnsModel();

        Gtk::TreeModelColumn<int> m_category_id;
        Gtk::TreeModelColumn<Glib::ustring> m_category_name;

    private:

};

#endif
