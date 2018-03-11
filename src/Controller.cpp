#include "Controller.h"

Controller::Controller(Model* model){
    m_model = model;
}

Controller::~Controller(){

}

bool Controller::open_db(){
    m_model->open_db();
}

bool Controller::load_db(){
    m_model->load_db();
}

bool Controller::create_db(){
    m_model->create_db();
}
