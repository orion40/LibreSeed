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
                return false;
            }
        }
        else
        {
            std::cerr << "Error opening database: " << sqlite3_errmsg(m_db) << "\n";
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool Model::load_db(){
    int query_result, load_query_result;
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
                m_model->addSeed(s);

            }
            switch (load_query_result){
                case SQLITE_ERROR:
                    std::cerr << "Error loading database content: " << sqlite3_errmsg(m_db) << "\n";
                    return false;
                    break;
                case SQLITE_DONE:
                    std::cout << "Loaded database content successfully !\n";
                    return true;
                    break;
                default:
                    std::cerr << "Error loading database content: " << sqlite3_errmsg(m_db) << "\n";
                    return false;
                    break;
            }
            break;
        case SQLITE_ERROR:
            std::cerr << "Error loading database content: " << sqlite3_errmsg(m_db) << "\n";
            return false;
            break;
        default:
            std::cerr << "Unhandled error code: " << query_result << " " <<  sqlite3_errmsg(m_db) << "\n";
            break;
    }
    return false;
}

bool Model::create_db(){
    std::cerr << "Creating database if needed...\n";
    char sql_script[] = "/home/whoami/programming/big_projects/seed_manager/src/create_db.sql";
    int script_length, r;
    std::ifstream script_fs(sql_script, std::fstream::in);
    if (script_fs) {
        sqlite3_stmt* create_db_stmt;
        char* buffer;
        script_fs.seekg(0, script_fs.end);
        script_length = script_fs.tellg();
        script_fs.seekg(0, script_fs.beg);


        buffer = new char[script_length];

        script_fs.read(buffer, script_length);

        if (sqlite3_prepare_v2(m_db, buffer, -1, &create_db_stmt, NULL) == SQLITE_OK){
            std::cerr << "Creating DB...\n";
            if ((r = sqlite3_step(create_db_stmt)) == SQLITE_DONE){
                std::cerr << "Successfully created DB.\n";
                return true;
            } else {
                std::cerr << "Error creating db(" << r << "): " << sqlite3_errmsg(m_db) << "\n";
            }
        } else {
            std::cerr << "Error preparing request: " << sqlite3_errmsg(m_db) << "\n";
        }
    } else {
        std::cerr << "Error opening db script: " << strerror(errno) << "\n";
    }

    return false;
}
