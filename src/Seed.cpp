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
        m_name = Glib::ustring(reinterpret_cast<const char*>(name));
}

void Seed::set_binomial_nomenclature(const unsigned char* binomial_nomenclature){
    if (binomial_nomenclature != NULL)
        m_binomial_nomenclature = Glib::ustring(reinterpret_cast<const char*>(binomial_nomenclature));
}

void Seed::set_description(const unsigned char* description){
    if (description != NULL)
        m_description = Glib::ustring(reinterpret_cast<const char*>(description));
}

bool Seed::fill_from_sql(sqlite3_stmt* stmt){
    //TODO: check for errors
    set_id(sqlite3_column_int(stmt, 0));
    set_name(sqlite3_column_text(stmt, 1));
    set_binomial_nomenclature(sqlite3_column_text(stmt, 2));
    set_description(sqlite3_column_text(stmt, 3));
    return true;
}

bool Seed::save_to_db(sqlite3* db){
    int change_query_result, read_query_result;
    sqlite3_stmt* edition_stmt;
    sqlite3_stmt* read_stmt;
    if (get_id() == -1) {
        // non sauvegarde en BDD, il faut la créé
        std::string insert_stmt_string = "INSERT INTO seed (seed_name, seed_binomial_name, seed_description) VALUES (?, ?, ?);";
        change_query_result = sqlite3_prepare_v2(db, insert_stmt_string.c_str(), -1, &edition_stmt, NULL);
        switch (change_query_result){
            case SQLITE_OK:
                sqlite3_bind_text(edition_stmt, -1, m_name.c_str(), SQLITE_STATIC);
                sqlite3_bind_text(edition_stmt, -1, m_binomial_nomenclature.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(edition_stmt, -1, m_description.c_str(), SQLITE_STATIC);
                sqlite3_step(edition_stmt);
                std::cout << "Successfully added Seed to database\n";
                return true;
                break;
            default:
                std::cerr << "Error preparing sql statement: " << sqlite3_errmsg(db) << std::endl;
                return false;
        }
    } else {
        // existe en BDD, il faut udpate
    }
}

void Seed::print_seed(){
    std::cout << "[" << m_id << "] " << m_name << " - " << m_binomial_nomenclature << " - " << m_description << std::endl;
}
