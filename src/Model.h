#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <string>
#include <fstream>
#include <cstring>
#include <sqlite3.h>

#include "Seed.h"
#include "Category.h"


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

        void add_category(Category* c);
        void remove_category(Category* c);
        Category* get_category_by_id(int);
        std::list<Category*> get_categories(){return m_categories;}

        bool export_seeds_to_xml(std::string filename, std::list<Seed*> seed_list);
        bool export_all_to_xml(std::string filename);
        bool import_to_xml(std::string filename);

        bool open_file(std::string filename);

        bool save_content();

    private:
        std::list<Seed*> m_seeds;
        std::list<Category*> m_categories;
        std::string m_path;
        sqlite3* m_db;

};

#endif
