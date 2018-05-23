#ifndef DATEINFO_H
#define DATEINFO_H

#include <glibmm/ustring.h>

#define MONTH_AMOUNT 12
#define MONTH_FRACTION 2

class DateInfo{
    public:
        DateInfo();
        DateInfo(const Glib::ustring&);
        ~DateInfo();

        //int get_id(){return m_id;}
        //void set_id(int id){m_id = id;}

        Glib::ustring get_name(){return m_name;}
        void set_name(const Glib::ustring& name);

    private:
        int m_date_info_id;
        int m_seed_id;
        Glib::ustring m_name;
        bool m_months[MONTH_AMOUNT][MONTH_FRACTION];
};

#endif
