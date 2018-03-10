#ifndef SEED_H
#define SEED_H

#include <string>

#include <sqlite3.h>

#include <glibmm/ustring.h>

class Seed{
    public:
        Seed();
        ~Seed();

        bool save_to_db();
        bool fill_from_sql(sqlite3_stmt*);

        int get_id(){return m_id;}
        std::string get_name(){return m_name;}
        std::string get_binomial_nomenclature(){return m_binomial_nomenclature;}
        std::string get_description(){return m_description;}

        void set_id(int id);
        void set_name(const unsigned char* name);
        void set_binomial_nomenclature(const unsigned char* binomial_nomenclature);
        void set_description(const unsigned char* description);

    private:
        // Object data
        bool m_is_changed;

        // DB data
        int m_id;
        std::string m_name;
        std::string m_binomial_nomenclature;
        std::string m_description;
};

#endif
