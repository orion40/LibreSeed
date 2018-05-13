#include "Model.h"

Model::Model(std::string path){
    m_path = path;
}

Model::~Model(){
    sqlite3_close(m_db);
}

void Model::add_seed(Seed* s){
    m_seeds.push_back(s);
}

void Model::remove_seed(Seed* s){
    int change_query_result;
    sqlite3_stmt* edition_stmt;
    std::string insert_stmt_string = "DELETE FROM seed WHERE seed_id=?;";
    change_query_result = sqlite3_prepare_v2(m_db, insert_stmt_string.c_str(), -1, &edition_stmt, NULL);
    switch (change_query_result){
        case SQLITE_OK:
            sqlite3_bind_int(edition_stmt, 1, s->get_id());
            switch (sqlite3_step(edition_stmt)){
                case SQLITE_DONE:
                    std::cout << "Successfully removed Seed from database\n";
                    sqlite3_finalize(edition_stmt);
                    break;
                default:
                    std::cerr << "Error removing from database.\n";
                    break;
            }
            break;
        default:
            std::cerr << "Error preparing sql statement: " << sqlite3_errmsg(m_db) << std::endl;
    }
    m_seeds.remove(s);
}

bool Model::open_db(){
    std::cerr << "Opening database\n";
    if (!m_path.empty()){
        if (sqlite3_open(m_path.c_str(), &m_db) == SQLITE_OK){
            std::cerr << "Succesfully opened database.\n";
            if (create_db()) {
                if (load_db()){
                    std::cerr << "Succesfully loaded database\n";
                } else {
                    std::cerr << "Error loading database: " << sqlite3_errmsg(m_db) << "\n";
                    return false;
                }
            } else {
                std::cerr << "Error creating database.\n";
                return false;
            }
        }
        else
        {
            std::cerr << "Error opening database: " << sqlite3_errmsg(m_db) << "\n";
            return false;
        }
    } else {
        std::cerr << "No database path supplied.\n";
        return false;
    }
    return true;
}

bool Model::load_db(){
    int query_result, load_query_result;
    bool result = false;

    // LOAD SEED

    const char* load_seed = "SELECT * FROM Seed";
    sqlite3_stmt* load_stmt;
    /*
       int sqlite3_prepare_v2(
       sqlite3 *db,             Database handle
       const char *zSql,       SQL statement, UTF-8 encoded
       int nByte,              Maximum length of zSql in bytes.
       sqlite3_stmt **ppStmt,  OUT: Statement handle
       const char **pzTail     OUT: Pointer to unused portion of zSql
       );
       */
    query_result = sqlite3_prepare_v2(m_db, load_seed, -1, &load_stmt, NULL);
    switch (query_result){
        case SQLITE_OK:
            while ((load_query_result = sqlite3_step(load_stmt)) == SQLITE_ROW){
                Seed* s = new Seed();

                s->fill_from_sql(load_stmt);
                add_seed(s);

            }
            switch (load_query_result){
                case SQLITE_ERROR:
                    std::cerr << "Error loading categories from database : " << sqlite3_errmsg(m_db) << "\n";
                    return false;
                    break;
                case SQLITE_DONE:
                    std::cout << "Loaded seeds sucessfully...\n";
                    result = true;
                    break;
                default:
                    std::cerr << "Error loading categories from database : " << sqlite3_errmsg(m_db) << "\n";
                    return false;
                    break;
            }
            break;
        case SQLITE_ERROR:
            std::cerr << "Error loading categories from database : " << sqlite3_errmsg(m_db) << "\n";
            return false;
            break;
        default:
            std::cerr << "Unhandled error code: " << query_result << " " <<  sqlite3_errmsg(m_db) << "\n";
            break;
    }

    // LOADING CATEGORY
    const char* load_category = "SELECT * FROM Category";
    /*
       int sqlite3_prepare_v2(
       sqlite3 *db,             Database handle
       const char *zSql,       SQL statement, UTF-8 encoded
       int nByte,              Maximum length of zSql in bytes.
       sqlite3_stmt **ppStmt,  OUT: Statement handle
       const char **pzTail     OUT: Pointer to unused portion of zSql
       );
       */
    query_result = sqlite3_prepare_v2(m_db, load_category, -1, &load_stmt, NULL);
    switch (query_result){
        case SQLITE_OK:
            while ((load_query_result = sqlite3_step(load_stmt)) == SQLITE_ROW){
                Category* c = new Category();

                c->fill_from_sql(load_stmt);
                add_category(c);
            }
            switch (load_query_result){
                case SQLITE_ERROR:
                    std::cerr << "Error loading categories from database : " << sqlite3_errmsg(m_db) << "\n";
                    return false;
                    break;
                case SQLITE_DONE:
                    std::cout << "Loaded categories sucessfully...\n";
                    result = true;
                    break;
                default:
                    std::cerr << "Error loading categories from database : " << sqlite3_errmsg(m_db) << "\n";
                    return false;
                    break;
            }
            break;
        case SQLITE_ERROR:
            std::cerr << "Error loading categories from database : " << sqlite3_errmsg(m_db) << "\n";
            return false;
            break;
        default:
            std::cerr << "Unhandled error code: " << query_result << " " <<  sqlite3_errmsg(m_db) << "\n";
            break;
    }

    return result;
}

bool Model::create_db(){
    std::cerr << "Creating database if needed...\n";
    char sql_script[] = "/home/whoami/programming/big_projects/seed_manager/src/create_db.sql";
    int script_length;
    bool result = false;
    std::ifstream script_fs(sql_script, std::fstream::in);

    if (script_fs) {
        char* buffer;
        char** err_msg = NULL;
        script_fs.seekg(0, script_fs.end);
        script_length = script_fs.tellg();
        script_fs.seekg(0, script_fs.beg);

        buffer = new char[script_length];
        script_fs.read(buffer, script_length);

        //std::cout << "sql:\n" << buffer << "\n";

        if (sqlite3_exec(m_db, buffer, NULL, NULL, err_msg) == SQLITE_OK){
            result = true;
        } else {
            std::cerr << "Error executing request: " << sqlite3_errmsg(m_db) << "\n" << err_msg << "\n";
        }
        delete buffer;
    } else {
        std::cerr << "Error opening db script: " << strerror(errno) << "\n";
    }

    return result;
}

bool Model::save_content(){
    bool result = true;
    // TODO: handle this properly
    for (std::list<Seed*>::iterator it = m_seeds.begin(); it != m_seeds.end(); it++){
        result = (*it)->save_to_db(m_db);
    }

    for (std::list<Category*>::iterator it = m_categories.begin(); it != m_categories.end(); it++){
        (*it)->save_to_db(m_db);
    }

    return result;
}

Seed* Model::getSeedById(int id){
    for (std::list<Seed*>::iterator it = m_seeds.begin(); it != m_seeds.end(); it++){
        if ((*it)->get_id() == id){
            return (*it);
        }
    }

    return NULL;
}

bool Model::export_seeds_to_xml(std::string filename, std::list<Seed*> seed_list){
    std::ifstream xml_fs(filename, std::fstream::in);
    if (xml_fs){

    } else {
        std::cerr << "Error opening XML file: " << strerror(errno) << "\n";
    }

    return false;
}

bool Model::export_all_to_xml(std::string filename){
    std::ifstream xml_fs(filename, std::fstream::in);
    if (xml_fs){

    } else {
        std::cerr << "Error opening XML file: " << strerror(errno) << "\n";
    }

    return false;
}

bool Model::import_to_xml(std::string filename){
    std::ifstream xml_fs(filename, std::fstream::in);
    if (xml_fs){

    } else {
        std::cerr << "Error opening XML file: " << strerror(errno) << "\n";
    }

    return false;
}

bool Model::open_file(std::string filename){
    std::ifstream (filename, std::fstream::in);

    return false;
}

Category* Model::get_category_by_id(int id){
    for (std::list<Category*>::iterator it = m_categories.begin(); it != m_categories.end(); it++){
        (*it)->print_category();
        if ((*it)->get_id() == id){
            return (*it);
        }
    }

    return NULL;
}

void Model::add_category(Category* c){
    m_categories.push_back(c);
}

void Model::remove_category(Category* c){
    if (c == NULL){
        std::cout << "Error, empty category given. Nothing removed\n";
        return;
    }
    int change_query_result;
    sqlite3_stmt* edition_stmt;
    std::string insert_stmt_string = "DELETE FROM category WHERE category_id=?;";
    change_query_result = sqlite3_prepare_v2(m_db, insert_stmt_string.c_str(), -1, &edition_stmt, NULL);
    switch (change_query_result){
        case SQLITE_OK:
            sqlite3_bind_int(edition_stmt, 1, c->get_id());
            switch (sqlite3_step(edition_stmt)){
                case SQLITE_DONE:
                    std::cout << "Successfully removed Category from database\n";
                    sqlite3_finalize(edition_stmt);
                    break;
                default:
                    std::cerr << "Error removing from database.\n";
                    break;
            }
            break;
        default:
            std::cerr << "Error preparing sql statement: " << sqlite3_errmsg(m_db) << std::endl;
    }
    m_categories.remove(c);
}
