#ifndef MODEL_H
#define MODEL_H

#include <list>

#include "Seed.h"

class Model{
public:
	Model();
	~Model();

        void addSeed(Seed* s);
        void removeSeed(Seed* s);
        std::list<Seed*> get_seeds(){return m_seeds;}

private:
        std::list<Seed*> m_seeds;

};

#endif
