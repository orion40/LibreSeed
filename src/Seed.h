#ifndef SEED_H
#define SEED_H

#include <string>
#include <iostream>

#include <sqlite3.h>

#include <glibmm/ustring.h>

class Seed{
    public:
        Seed();
        ~Seed();

        bool save_to_db(sqlite3* db);
        bool fill_from_sql(sqlite3_stmt*);

        int get_id(){return m_id;}
        Glib::ustring get_name(){return m_name;}
        Glib::ustring get_binomial_nomenclature(){return m_binomial_nomenclature;}
        Glib::ustring get_description(){return m_description;}

        void set_id(int id);
        void set_name(const unsigned char* name);
        void set_name(Glib::ustring name){m_name = name;}
        void set_binomial_nomenclature(const unsigned char* binomial_nomenclature);
        void set_binomial_nomenclature(Glib::ustring binomial_nomenclature){m_binomial_nomenclature = binomial_nomenclature;}
        void set_description(const unsigned char* description);
        void set_description(Glib::ustring description){m_description = description;}

        void print_seed();

    private:
        // Object data
        bool m_is_changed;

        // DB data
        int m_id;
        Glib::ustring m_name;
        Glib::ustring m_binomial_nomenclature;
        Glib::ustring m_description;
};

#endif
