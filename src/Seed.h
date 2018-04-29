#ifndef SEED_H
#define SEED_H

#include <string>
#include <ctime>
#include <iostream>

#include <sqlite3.h>

#include <glibmm/ustring.h>

#include "MonthsEnum.h"
#include "DateInfo.h"
#include "Category.h"

#define MAX_DATES 5

class Seed{
    public:
        Seed();
        ~Seed();

        void set_modified(bool modified){m_is_modified = modified;}
        bool is_modified(){return m_is_modified;}
        bool save_to_db(sqlite3* db);
        bool fill_from_sql(sqlite3_stmt*);

        int get_id(){return m_id;}
        Glib::ustring get_plant_name(){return m_plant_name;}
        Glib::ustring get_variety_name(){return m_variety_name;}
        Glib::ustring get_binomial_nomenclature(){return m_binomial_nomenclature;}
        Glib::ustring get_description(){return m_description;}

        uint32_t get_plant_sowing_date(){return m_plant_sowing_date;}
        uint32_t get_plant_pruning_date(){return m_plant_pruning_date;}
        uint32_t get_plant_harvest_date(){return m_plant_harvest_date;}

        uint32_t get_quantity_left(){return m_quantity_left;}
        Glib::ustring get_quantity_unit(){return m_quantity_unit;}
        struct tm get_seed_harvest_date(){return m_seed_harvest_date;}
        struct tm get_seed_expiration_date(){return m_seed_expiration_date;}

        void set_id(int id);
        void set_plant_name(const unsigned char* name);
        void set_plant_name(Glib::ustring name){set_modified(true); m_plant_name = name;}
        void set_variety_name(const unsigned char* name);
        void set_variety_name(Glib::ustring name){set_modified(true); m_variety_name = name;}
        void set_binomial_nomenclature(const unsigned char* binomial_nomenclature);
        void set_binomial_nomenclature(Glib::ustring binomial_nomenclature){set_modified(true); m_binomial_nomenclature = binomial_nomenclature;}
        void set_description(const unsigned char* description);
        void set_description(Glib::ustring description){set_modified(true); m_description = description;}

        void print_seed();


    private:
        // Object data
        bool m_is_modified;

        // DB data
        int m_id;
        Glib::ustring m_plant_name;
        Glib::ustring m_variety_name;
        Glib::ustring m_binomial_nomenclature;
        Glib::ustring m_description;

        DateInfo* m_dates[MAX_DATES];
        uint32_t m_plant_sowing_date;
        uint32_t m_plant_pruning_date;
        uint32_t m_plant_harvest_date;

        //uint32_t m_plant_germination_time;

        uint32_t m_quantity_left;
        Glib::ustring m_quantity_unit;
        struct tm m_seed_harvest_date;
        struct tm m_seed_expiration_date;
};

#endif
