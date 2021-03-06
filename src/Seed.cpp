#include "Seed.h"

Seed::Seed(){
    m_id = -1;

    m_dates[0] = new DateInfo("Sowing");
    m_dates[1] = new DateInfo("Pruning");
    m_dates[2] = new DateInfo("Harvesting");

    set_modified(false);
}

Seed::~Seed(){

}

void Seed::set_id(int id){
    if (id > 0){
        m_id = id;
        set_modified(true);
    }
}

void Seed::set_plant_name(const unsigned char* name){
    if (name != NULL){
        m_plant_name = Glib::ustring(reinterpret_cast<const char*>(name));
        set_modified(true);
    }
}

void Seed::set_variety_name(const unsigned char* name){
    if (name != NULL){
        m_variety_name = Glib::ustring(reinterpret_cast<const char*>(name));
        set_modified(true);
    }
}

void Seed::set_binomial_nomenclature(const unsigned char* binomial_nomenclature){
    if (binomial_nomenclature != NULL){
        m_binomial_nomenclature = Glib::ustring(reinterpret_cast<const char*>(binomial_nomenclature));
        set_modified(true);
    }
}

void Seed::set_description(const unsigned char* description){
    if (description != NULL){
        m_description = Glib::ustring(reinterpret_cast<const char*>(description));
        set_modified(true);
    }
}

bool Seed::fill_from_sql(sqlite3_stmt* stmt){
    //TODO: check for errors
    set_id(sqlite3_column_int(stmt, 0));
    set_plant_name(sqlite3_column_text(stmt, 1));
    set_variety_name(sqlite3_column_text(stmt, 2));
    set_binomial_nomenclature(sqlite3_column_text(stmt, 3));
    set_description(sqlite3_column_text(stmt, 4));
    set_plant_category_id(sqlite3_column_int(stmt, 5));
    set_modified(false);
    return true;
}

bool Seed::save_to_db(sqlite3* db){
    int change_query_result;
    sqlite3_stmt* edition_stmt;
    if (get_id() == -1) {
        // non sauvegarde en BDD, il faut la créé
        // et récupéré l'ID associé
        std::string insert_stmt_string = "INSERT INTO seed (seed_plant_name, seed_variety_name, seed_binomial_name, seed_description, category_id) VALUES (?, ?, ?, ?, ?);";
        change_query_result = sqlite3_prepare_v2(db, insert_stmt_string.c_str(), -1, &edition_stmt, NULL);
        switch (change_query_result){
            case SQLITE_OK:
                sqlite3_bind_text(edition_stmt, 1, m_plant_name.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(edition_stmt, 2, m_variety_name.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(edition_stmt, 3, m_binomial_nomenclature.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(edition_stmt, 4, m_description.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(edition_stmt, 5, m_plant_category_id);
                switch (sqlite3_step(edition_stmt)){
                    case SQLITE_DONE:
                        std::cout << "Successfully added Seed to database\n";
                        sqlite3_finalize(edition_stmt);
                        set_id(sqlite3_last_insert_rowid(db));
                        set_modified(false);
                        break;
                    default:
                        std::cerr << "Error inserting into database.\n";
                        break;
                }
                return true;
                break;
            default:
                std::cerr << "Error preparing sql statement: " << sqlite3_errmsg(db) << std::endl;
                return false;
        }
    } else {
        if (is_modified()){
        // existe en BDD, et a été modifié, il faut update
            std::string insert_stmt_string = "UPDATE seed SET seed_plant_name = ?, seed_variety_name = ?, seed_binomial_name = ?, seed_description = ?, category_id = ? WHERE seed_id = ?;";
            change_query_result = sqlite3_prepare_v2(db, insert_stmt_string.c_str(), -1, &edition_stmt, NULL);
            switch (change_query_result){
                case SQLITE_OK:
                    sqlite3_bind_text(edition_stmt, 1, m_plant_name.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_text(edition_stmt, 2, m_variety_name.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_text(edition_stmt, 3, m_binomial_nomenclature.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_text(edition_stmt, 4, m_description.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_int(edition_stmt, 5, m_plant_category_id);
                    sqlite3_bind_int(edition_stmt, 6, m_id);
                    switch (sqlite3_step(edition_stmt)){
                        case SQLITE_DONE:
                            std::cout << "Successfully edited Seed in database\n";
                            set_modified(false);
                            sqlite3_finalize(edition_stmt);
                            break;
                        default:
                            std::cerr << "Error editing seed.\n";
                            break;
                    }
                    return true;
                    break;
                default:
                    std::cerr << "Error preparing sql statement: " << sqlite3_errmsg(db) << std::endl;
                    return false;
            }

        }
    }
    return false;
}

void Seed::print_seed(){
    std::cout << "[" << m_id << "] " << std::endl;
    std::cout << "Plant\t" << m_plant_name << std::endl;
    std::cout << "Variety\t" << m_variety_name << std::endl;
    std::cout << "Binomial\t" << m_binomial_nomenclature << std::endl;
    std::cout << "Desc\t" << m_description << std::endl;
    std::cout << "Category id\t" << m_plant_category_id << std::endl;
}
