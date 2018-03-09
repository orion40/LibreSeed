#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sqlite3.h>

#include "Model.h"

class Controller{
    public:
        Controller(std::string path, Model* model);
        ~Controller();

        bool open_db();

        Model* get_model(){return m_model;}

    private:
        bool load_db();
        bool create_db();
        Model* m_model;

        std::string m_path;
        sqlite3* m_db;
};

#endif
