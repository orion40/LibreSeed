#include "SeedColumnsModel.h"

SeedColumnsModel::SeedColumnsModel(){
    add(m_seed_id);
    add(m_seed_plant_name);
    add(m_seed_variety_name);
    add(m_seed_binomial_nomenclature);
    add(m_seed_description);
    add(m_seed_category);
}

SeedColumnsModel::~SeedColumnsModel(){

}
