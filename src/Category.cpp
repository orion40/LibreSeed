#include "Category.h"

Category::Category(){
    m_id = -1;

    set_modified(false);
}

Category::Category(const unsigned char* name):Category(){
    set_category_name(name);
}

Category::Category(Glib::ustring name):Category(){
    set_category_name(name);
}

Category::~Category(){

}

bool Category::save_to_db(sqlite3* db){
    int change_query_result;
    sqlite3_stmt* edition_stmt;
    if (get_id() == -1) {
        // non sauvegarde en BDD, il faut la créé
        // et récupéré l'ID associé
        std::string insert_stmt_string = "INSERT INTO category (category_name) VALUES (?);";
        change_query_result = sqlite3_prepare_v2(db, insert_stmt_string.c_str(), -1, &edition_stmt, NULL);
        switch (change_query_result){
            case SQLITE_OK:
                sqlite3_bind_text(edition_stmt, 1, m_name.c_str(), -1, SQLITE_STATIC);
                switch (sqlite3_step(edition_stmt)){
                    case SQLITE_DONE:
                        std::cout << "Successfully added Category to database\n";
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
            std::string insert_stmt_string = "UPDATE Category SET category_name = ? WHERE category_id = ?;";
            change_query_result = sqlite3_prepare_v2(db, insert_stmt_string.c_str(), -1, &edition_stmt, NULL);
            switch (change_query_result){
                case SQLITE_OK:
                    sqlite3_bind_text(edition_stmt, 1, m_name.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_int(edition_stmt, 2, m_id);
                    switch (sqlite3_step(edition_stmt)){
                        case SQLITE_DONE:
                            std::cout << "Successfully edited Category in database\n";
                            set_modified(false);
                            sqlite3_finalize(edition_stmt);
                            break;
                        default:
                            std::cerr << "Error editing Category.\n";
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

bool Category::fill_from_sql(sqlite3_stmt* stmt){
    set_id(sqlite3_column_int(stmt, 0));
    set_category_name(sqlite3_column_text(stmt, 1));
    set_modified(false);
    return true;
}

void Category::print_category(){
    std::cout << "[" << m_id << "] ";
    std::cout << "Category\t" << m_name<< std::endl;
}

void Category::set_category_name(const unsigned char* name){
    if (name != NULL){
        m_name = Glib::ustring(reinterpret_cast<const char*>(name));
        set_modified(true);
    }
}

void Category::set_id(int id){
    if (id > 0){
        m_id = id;
        set_modified(true);
    }
}
