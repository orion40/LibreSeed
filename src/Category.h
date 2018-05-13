#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <iostream>

#include <sqlite3.h>
#include <glibmm/ustring.h>

class Category{
    public:
        Category();
        Category(const unsigned char* name);
        Category(Glib::ustring name);
        ~Category();
    void set_modified(bool modified){m_is_modified = modified;}
    bool is_modified(){return m_is_modified;}
        bool save_to_db(sqlite3* db);
        bool fill_from_sql(sqlite3_stmt*);

        void print_category();

        void set_id(int id);
        int get_id(){return m_id;}
        Glib::ustring get_category_name(){return m_name;}
        void set_category_name(const unsigned char* name);
        void set_category_name(Glib::ustring name){set_modified(true); m_name = name;}

    private:

        int m_id;
        Glib::ustring m_name;
        bool m_is_modified;

};

#endif
