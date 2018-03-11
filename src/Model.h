#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <string>
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
        std::list<Seed*> get_seeds(){return m_seeds;}

private:
        std::list<Seed*> m_seeds;
        std::string m_path;
        sqlite3* m_db;

};

#endif
