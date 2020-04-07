/*
 * sqlite_handler.cpp
 *
 *  Created on: Mar 24, 2020
 *      Author: jamil.zaman
 */

#include "sqlite_handler.h"
#include <iostream>
#include <string>
#include <pqxx/pqxx>

#include <typeinfo>

#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#include <sstream>
#include <iterator>



//template <typename T> std::string type_name();

//TODO: move this separate utility namespace

std::string container_to_string(const std::vector<std::string>& column_names, const char* delim) {
    std::ostringstream imploded;
    std::copy(column_names.begin(), column_names.end(),
            std::ostream_iterator<std::string>(imploded, delim));

    std::string columns = imploded.str();
    columns.erase(columns.find_last_of(","));
    return columns;
}

int callback(void* notUsed, int argc, char* argv[], char* columns[]) {

    for (int i = 0; i< argc; i++) {
        std::cout << columns[i] << " : " << (argv[i] ? argv[i]:"NULL") << std::endl;
    }
    return 0;
}

int sqlite_handler::create_table(const std::string& sq_database) {
    std::string sql;
    sqlite3* db;
    int rc;
    char *zErrMsg = 0;

    try {
        /* Open database */
        rc = sqlite3_open(sq_database.c_str(), &db);

        if( rc ) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        } else {
            fprintf(stderr, "Opened database successfully\n");
        }

        /* Create SQL statement */
        sql = "CREATE TABLE COMPANY("  \
                "ID INT PRIMARY KEY     NOT NULL," \
                "NAME           TEXT    NOT NULL," \
                "AGE            INT     NOT NULL," \
                "ADDRESS        CHAR(50)," \
                "SALARY         REAL );";

        /* Execute SQL statement */
        rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
        rc = sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);

        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }
        sqlite3_close(db);

        std::cout << "Table created successfully" << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int sqlite_handler::insert_data(
        const std::string& sq_database,
        const std::string& sq_table,
        const std::vector<std::string>& columns,
        const std::vector<std::string>& data) {
    std::string sql = "";
    std::string base_sql = "INSERT INTO " + sq_table + " ( " +
            container_to_string(columns, ", ") + ") VALUES ";
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc, row_count = 0;
    char *zErrMsg = 0;
    try {
        /* Open database */
        rc = sqlite3_open(sq_database.c_str(), &db);

        if( rc ) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(0);
        } else {
            fprintf(stdout, "SQLITE INSERT:: database '%s' opened\n", sq_database.c_str());
        }

        for(auto rows = data.begin();rows != data.end();) {
            sql = sql + "( " + *rows + "), ";
            if(++rows == data.end() || (++row_count % 500) == 0 ) //every 500 row data must be inserted, SQLITE limitation
                     //every 500 row data must be inserted, SQLITE limitation
            {
                sql.erase(sql.find_last_of(","));
                sqlite3_prepare_v2(db, (base_sql+sql).c_str(), -1, &stmt, 0);
                rc = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

                if((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
                    std::cerr << "RC: ["<< rc << "] :: "<< sqlite3_errmsg(db);
                }

                rc = sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
                rc = sqlite3_finalize( stmt );

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else {
                    fprintf(stdout, "SQLITE INSERT: %d Records created successfully\n", row_count);
                }
                sql = "";
            }
        }

#ifdef DEBUG
        std::cout <<"INSERT SQL: " << sql << std::endl;
#endif

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    /* Create SQL statement */
    /*sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
            "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";*/

    sqlite3_close(db);

    return 0;
}


int sqlite_handler::select_data(std::vector<std::string>& column_names, std::vector<std::string>& result_values) {
    std::string sql;
    int rc;

    try {

        /*std::vector<std::string> column_names;
        std::vector<std::string> column_values;

        for(size_t i = 0; i < R.columns(); i++) {
            //std::cout << "Column: \'" << R.column_name(i) <<"\' "<<std::endl;
            column_names.push_back(R.column_name(i));
        }

        const char* const delim = ", ";

        std::ostringstream imploded;
        std::copy(column_names.begin(), column_names.end(),
                std::ostream_iterator<std::string>(imploded, delim));

        std::string columns = imploded.str();
        columns.erase(columns.find_last_of(","));
        std::cout << "COLUMNS:: " << columns << std::endl;

        std::string result_holder = "";

        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            for(auto name:column_names) {
                cout << name<<": " << c[name] << " db type: " << c[name].type() \
                        << " cpp type: "<< demangle(typeid(c[name]).name())<< endl;
                result_holder.append("'"+ std::string(c[name].c_str())+ "', ");
            }
            result_holder.erase(result_holder.find_last_of(","));
            std::cout << "imploded result: " << result_holder << std::endl;
            column_values.push_back(result_holder);
            result_holder = "";
        }

        for(auto values:result_holder) {
            std::cout << "-> " << values <<std::endl;
        }

        std::ostringstream imploded_values;
        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            std::copy(c.begin(), c.end, std::ostream_iterator<const char*>(imploded_values, delim));
        }
         */
        std::cout << "Operation done successfully" << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

