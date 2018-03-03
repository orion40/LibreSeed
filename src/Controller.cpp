#include "Controller.h"

Controller::Controller(std::string path){
    m_path = path;
}

Controller::~Controller(){

}

bool Controller::open_db(){
    if (!m_path.empty()){
        if (!sqlite3_open(m_path.c_str(), &m_db)){
            std::cerr << "Error opening database: " << sqlite3_errmsg(m_db) << "\n";
        }
        else
        {
            std::cerr << "Succesfully opened database\n";
        }
    } else {
        return false;
    }
    return true;
}
