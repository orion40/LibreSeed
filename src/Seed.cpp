#include "Seed.h"

Seed::Seed(){

}

Seed::~Seed(){

}

void Seed::set_id(int id){
    if (id >= 0){
        m_id = id;
    }
}

void Seed::set_name(std::string name){
    m_name = name;
}

void Seed::set_binomial_nomenclature(std::string binomial_nomenclature){
    m_binomial_nomenclature = binomial_nomenclature;
}

void Seed::set_description(std::string description){
    m_description = description;
}
