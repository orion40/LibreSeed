#include "Model.h"

Model::Model(){

}

Model::~Model(){

}

void Model::add(Seed* s){
    m_seeds.push_back(s);
}

void Model::remove(Seed* s){
    m_seeds.remove(s);
}
