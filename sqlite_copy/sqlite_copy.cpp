#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string>
#include <time.h>

#include "pg_handler.h"
#include "sqlite_handler.h"
/*
static int callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    fprintf(stdout, "QUERY RESULT:\n");

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}*/

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES"
            << "Options:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-d,--database SERVER-DATABASE\tSpecify the PostgreSQL Database"
            << "\t-s,--sqlite SQLITE-DATABASE\tSpecify the SQLITE Database"
            << std::endl;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string sql;
    const char* data = "Callback function called";

    //parse the command line options
    if (argc < 5) {
        show_usage(argv[0]);
        return 1;
    }
    std::vector <std::string> sources;
    std::string pg_database;
    std::string sq_database;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        //TODO: Need to add username/password validation
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "-d") || (arg == "--database")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                pg_database = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--database option requires one argument." << std::endl;
                return 1;
            }
        } else if ((arg == "-s") || (arg == "--sqlite")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                sq_database = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--database option requires one argument." << std::endl;
                return 1;
            }
        } else {
            sources.push_back(argv[i]);
        }
    }

    std::cout <<"PG-DB:: " << pg_database << " SQLITE-DB:: " << sq_database <<std::endl;

    /* Open database */
    rc = sqlite3_open(sq_database.c_str(), &db);

    if( rc ) {
        fprintf(stderr, "Can't open database %s: %s\n", sq_database.c_str(), sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Test::Opened database %s successfully\n", sq_database.c_str());
    }

    /* Create SQL statement */
    //sql = "select sql from sqlite_master where name = 'member';";
    //sql = "SELECT unionid, count(*) as member FROM MEMBER group by unionid;";

    sqlite3_close(db);

    //handle pg
    //create_table (argc, argv);
    //insert data
    //insert_data(argc, argv);
    std::string table_name = (argc == 6 ? argv[5]:"ancservice");
    clock_t t = clock();
    std::vector<std::string> column_names, result_values;
    select_data(pg_database, table_name,  column_names, result_values);
    //sqlite_handler::create_table(sq_database);
    sqlite_handler::insert_data(sq_database, table_name, column_names, result_values);
    t = clock() - t;
    std::cout <<"TABLE \'" << table_name << "\' took " << t <<" ticks and " \
               << (((float)t)/CLOCKS_PER_SEC) << " seconds" << std::endl;
    return 0;
}
