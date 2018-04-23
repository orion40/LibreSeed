#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <string>
#include <fstream>
#include <cstring>
#include <sqlite3.h>

#include "Seed.h"

class Model{
    public:
        Model(std::string path);
        ~Model();

        bool open_db();
        bool load_db();
        bool create_db();

        void add_seed(Seed* s);
        void remove_seed(Seed* s);
        Seed* getSeedById(int);
        std::list<Seed*> get_seeds(){return m_seeds;}

        bool save_content();

    private:
        std::list<Seed*> m_seeds;
        std::string m_path;
        sqlite3* m_db;

};

#endif
