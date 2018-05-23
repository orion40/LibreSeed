#include "DateInfo.h"

DateInfo::DateInfo(){
    for (int i = 0; i < MONTH_AMOUNT; i++){
        for (int j = 0; j < MONTH_FRACTION; j++){
            m_months[i][j] = false;
        }
    }
}

DateInfo::DateInfo(const Glib::ustring& name) : DateInfo(){
    set_name(name);
}

DateInfo::~DateInfo(){

}

void DateInfo::set_name(const Glib::ustring& name){
    m_name = name;
}
