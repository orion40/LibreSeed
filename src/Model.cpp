#include "Model.h"

Model::Model(){

}

Model::~Model(){

}

void Model::addSeed(Seed* s){
    m_seeds.push_back(s);
}

void Model::removeSeed(Seed* s){
    m_seeds.remove(s);
}
