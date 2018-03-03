#ifndef SEED_H
#define SEED_H

#include <string>

class Seed{
    public:
        Seed();
        ~Seed();

        bool fill_from_db();
        bool save_to_db();

        int get_id(){return m_id;}
        std::string get_name(){return m_name;}
        std::string get_binomial_nomenclature(){return m_binomial_nomenclature;}
        std::string get_description(){return m_description;}

        void set_id(int id);
        void set_name(std::string name);
        void set_binomial_nomenclature(std::string binomial_nomenclature);
        void set_description(std::string description);

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
