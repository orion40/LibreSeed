#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <iostream>
#include <cstring>
#include <fstream>

#include "Model.h"

class Controller{
    public:
        Controller(Model* model);
        ~Controller();

        bool open_db();

        Model* get_model(){return m_model;}

    private:
        bool load_db();
        bool create_db();
        Model* m_model;

        std::string m_path;
};

#endif
