#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string>
#include <map>
#include <time.h>

#include "pg_handler.h"
#include "sqlite_handler.h"
#include "query_builder.h"
#include "networkmanager.h"

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
            << "\t-p,--port PORT\tSpecify the PORT the service is listening"
            << "\t-P,--provider PROVIDER_ID\tSpecify the provider_id"
            << std::endl;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string sql;
    const char* data = "Callback function called";
    pg_handler pg;
    sqlite_handler sq;

    //parse the command line options
    if (argc < 8) {
        show_usage(argv[0]);
        return 1;
    }
    std::vector <std::string> sources;
    std::string pg_database;
    std::string sq_database;
    std::string provider_id;
    short port;

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
        } else if ((arg == "-p") || (arg == "--port")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                port = atoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--database option requires one argument." << std::endl;
                return 1;
            }
        } else if ((arg == "-P") || (arg == "--provider")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                provider_id = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--database option requires one argument." << std::endl;
                return 1;
            }
        } else {
            sources.push_back(argv[i]);
        }
    }

    //TODO - Remove this block
    std::cout << "::STARTING SERVER::" << std::endl;
    network_manager nm(port, sq_database);
    nm.server_loop();

    std::cout <<"PG-DB:: " << pg_database
              << "\nSQLITE-DB:: " << sq_database
              << "\nPROVIDER:: " << provider_id
              << std::endl;

    /* Open sqlite database for test*/
    rc = sqlite3_open(sq_database.c_str(), &db);

    if( rc ) {
        fprintf(stderr, "Can't open database %s: %s\n", sq_database.c_str(), sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Test::Opened database %s successfully\n", sq_database.c_str());
    }

    sqlite3_close(db);

    //clock the entire operation
    clock_t t, final = clock();
    std::vector<std::string> column_names, result_values;
    query_builder queries;
    //auto tables = queries.get_query_map();
    int i = 0;
    std::string table_name = "";
    for(auto it:queries.get_query_map()) {
        t = clock();
        table_name = it.first;
        pg.select_data(pg_database, table_name, provider_id, column_names, result_values);
        //sqlite_handler::create_table(sq_database);
        sq.insert_data(sq_database, table_name, column_names, result_values);
        t = clock() - t;
        std::cout <<"\nTABLE '" << table_name << "' took " << t <<" ticks and " \
                << (((float)t)/CLOCKS_PER_SEC) << " seconds" << std::endl;

        result_values.clear();
        column_names.clear();
        //if(i++> 3) break;
    }

    final = clock() - final;
    std::cout <<"\nTHE DATA LOADING TOOK " << final <<" ticks and " \
                    << (((float)final)/CLOCKS_PER_SEC) << " seconds" << std::endl;
    return 0;
}
