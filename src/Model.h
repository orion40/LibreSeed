#ifndef MODEL_H
#define MODEL_H

#include <list>

#include "Seed.h"

class Model{
public:
	Model();
	~Model();

        void add(Seed* s);
        void remove(Seed* s);

private:
        std::list<Seed*> m_seeds;

};

#endif
