#include <iostream>
#include <sqlite3.h>

int main(int argc, char* argv[]){
    sqlite3 *db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc){
        std::cout << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        return -1;
    } else {
        std::cout << "Opened database successfully !\n";
    }

    sqlite3_close(db);
}
