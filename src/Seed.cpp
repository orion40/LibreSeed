#include "Seed.h"

Seed::Seed(){

}

Seed::~Seed(){

}

void Seed::set_id(int id){
    if (id >= 0){
        m_id = id;
    }
}

void Seed::set_name(const unsigned char* name){
    m_name = std::string(reinterpret_cast<const char*>(name));
}

void Seed::set_binomial_nomenclature(const unsigned char* binomial_nomenclature){
    m_binomial_nomenclature = std::string(reinterpret_cast<const char*>(binomial_nomenclature));
}

void Seed::set_description(const unsigned char* description){
    m_description = std::string(reinterpret_cast<const char*>(description));
}

void Seed::fill_from_sql(sqlite3_stmt* stmt){
    set_id(sqlite3_column_int(stmt, 0));
    set_name(sqlite3_column_text(stmt, 1));
    set_binomial_nomenclature(sqlite3_column_text(stmt, 2));
    set_description(sqlite3_column_text(stmt, 3));
}
