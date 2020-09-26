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
#include "configmanager.h"

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
            //<< "\t-d,--database SERVER-DATABASE\tSpecify the PostgreSQL Database"
            << "\t-s,--sqlite SQLITE-DATABASE\tSpecify the SQLITE Database"
            << "\t-p,--port PORT\tSpecify the PORT the service is listening"
            << "\t-c,--config CONFIG FILE\tCOnfiguration file"
            << std::endl;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string sql;
    const char* data = "Callback function called";

    //parse the command line options
    if (argc < 6) {
        show_usage(argv[0]);
        return 1;
    }
    std::vector <std::string> sources;
    std::string pg_database;
    std::string sq_database;
    std::string config_file;
    short port = -1;

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
        } else if ((arg == "-c") || (arg == "--config")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                config_file = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--database option requires one argument." << std::endl;
                return 1;
            }
        } else {
            sources.push_back(argv[i]);
        }
    }

    //TODO - Remove this block
    config_manager& conf = config_manager::instance(config_file);
    network_manager nm(port > 0? port : conf.port(), sq_database);
    std::cout << ":: STARTING SERVER at (" << nm.get_port() << ") ::" << std::endl;
    nm.server_loop();

    return 0;
}
