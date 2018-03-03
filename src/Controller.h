#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <iostream>
#include <sqlite3.h>

class Controller{
    public:
        Controller(std::string path);
        ~Controller();

        bool open_db();
    private:

        std::string m_path;
        sqlite3* m_db;
};

#endif
