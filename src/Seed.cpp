#include "Seed.h"

Seed::Seed(){
    m_id = -1;
}

Seed::~Seed(){

}

void Seed::set_id(int id){
    if (id > 0){
        m_id = id;
    }
}

void Seed::set_name(const unsigned char* name){
    if (name != NULL)
        m_name = std::string(reinterpret_cast<const char*>(name));
}

void Seed::set_binomial_nomenclature(const unsigned char* binomial_nomenclature){
    if (binomial_nomenclature != NULL)
        m_binomial_nomenclature = std::string(reinterpret_cast<const char*>(binomial_nomenclature));
}

void Seed::set_description(const unsigned char* description){
    if (description != NULL)
        m_description = std::string(reinterpret_cast<const char*>(description));
}

bool Seed::fill_from_sql(sqlite3_stmt* stmt){
    //TODO: check for errors
    set_id(sqlite3_column_int(stmt, 0));
    set_name(sqlite3_column_text(stmt, 1));
    set_binomial_nomenclature(sqlite3_column_text(stmt, 2));
    set_description(sqlite3_column_text(stmt, 3));
    return true;
}

bool Seed::save_to_db(){
    if (get_id() == -1) {
        // non sauvegarde en BDD, il faut la créé
    } else {
        // existe en BDD, il faut udpate
    }
}
