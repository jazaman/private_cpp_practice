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
#include <sqlite3.h>
#include "time_wrapper.h"


//template <typename T> std::string type_name();

//TODO: move this separate utility namespace
std::string container_to_string(const std::vector<std::string>& column_names, const char* delim) {
    std::ostringstream imploded;
    std::copy(column_names.begin(), column_names.end(),
            std::ostream_iterator<std::string>(imploded, delim));

    std::string columns = imploded.str();
    size_t pos;
    if((pos = columns.find_last_of(",")) != std::string::npos) {
        columns.erase(pos);
    }
    return columns;
}

int callback(void* notUsed, int argc, char* argv[], char* columns[]) {

    for (int i = 0; i< argc; i++) {
        std::cout << columns[i] << " : " << (argv[i] ? argv[i]:"NULL") << std::endl;
    }
    return 0;
}

sqlite_handler::sqlite_handler(const std::string& sq_database)
    : sq_database_(sq_database),
      c_time_(time_wrapper::get_timer()){
    try {
        /* Open database */
        int rc = sqlite3_open(sq_database.c_str(), &template_db_);
        int rc2= sqlite3_open(":memory:", &inmemorydb_);
        if( rc || rc2) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(template_db_));
            throw std::exception();
        } else {
            fprintf(stderr, "Opened database successfully\n");
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw e;
    } catch (...) {
        std::cerr << "UNKNOWN ERROR" << std::endl;
        throw std::exception();
    }

    //Load in_memory db
    load_memory_db();
}

sqlite_handler::~sqlite_handler() {
    sqlite3_close(template_db_);
    sqlite3_close(inmemorydb_);
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
        //const std::string& sq_database,
        const std::string& sq_table,
        const std::vector<std::string>& columns,
        const std::vector<std::string>& data) {
    std::string sql = "";
    std::string base_sql = "INSERT INTO " + sq_table + " ( " +
            container_to_string(columns, ", ") + ") VALUES ";
    //sqlite3* db;
    sqlite3_stmt* stmt;
    int rc, row_count = 0;
    char *zErrMsg = 0;
    try {

        int iteration = 0;
        for(auto rows = data.begin();rows != data.end();) {

            sql = sql + "( " + *rows + "), ";
            if(++rows == data.end() || (++row_count % 500) == 0)
            //MAX 500 row data can be inserted at once, SQLITE limitation
            {
                size_t pos;
                if((pos = sql.find_last_of(",")) != std::string::npos) { //Get rid of the last comma
                    sql.erase(pos);
                }

                //TODO - DEBUG - DELETE
                if(sq_table.compare("child_care_service_detail") == 0) {
                    std::cout <<"INSERT SQL: " << (base_sql+sql) << std::endl;
                }
                //sql.erase(sql.find_last_of(","));
                sqlite3_prepare_v2(inmemorydb_, (base_sql+sql).c_str(), -1, &stmt, 0);
                rc = sqlite3_exec(inmemorydb_, "BEGIN TRANSACTION", 0, 0, 0);

                if((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
                    std::cerr << c_time_ << "[SQLITE INSERT] TABLE " << sq_table<< " ERROR RC: ["<< rc << "] :: "<< sqlite3_errmsg(inmemorydb_) << " iteration: " << ++iteration <<std::endl;
                    std::cerr << " ================== " << std::endl << sql << std::endl<< "=============================" << std::endl;
                }

                else {
                    std::cerr << c_time_ << "[SQLITE INSERT] TABLE " << sq_table<< " iteration: " << ++iteration << " row count: " << row_count <<std::endl;
#ifdef DEBUG
                    std::cerr << " ================== " << std::endl << sql << std::endl<< "=============================" << std::endl;
#endif
                }



                rc = sqlite3_exec(inmemorydb_, "END TRANSACTION", 0, 0, 0);
                rc = sqlite3_finalize( stmt );

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else {
#ifdef DEBUG
                    fprintf(stdout, "SQLITE INSERT: %d Records created successfully\n", row_count);
#endif
                }
                sql = "";
            }
        }
        //fprintf(stdout, "SQLITE INSERT: Total %d Records created successfully for '%s' table\n", row_count, sq_table.c_str());
        std::cout << c_time_ << "[SQLITE INSERT] " << ++row_count << " Records created successfully for " << sq_table << std::endl;

#ifdef DEBUG
        std::cout <<"INSERT SQL: " << sql << std::endl;
#endif

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return SQLITE_ERROR;
    }

    /* Create SQL statement */
    //sqlite3_close(db);

    return SQLITE_OK;
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

void sqlite_handler::load_memory_db()
{
    sqlite3* destination;
    int rc;

    // Create the in-memory schema from the backup
    sqlite3_exec(template_db_, "BEGIN", NULL, NULL, NULL);
    sqlite3_exec(
            template_db_,
            "SELECT sql FROM sqlite_master WHERE sql NOT NULL",
            &process_ddl_row,
            inmemorydb_,
            NULL);
    sqlite3_exec(template_db_, "COMMIT", NULL, NULL, NULL);
    sqlite3_close(template_db_);

    // Attach the backup to the in memory
    const std::string attach_cmd = std::string("ATTACH DATABASE '") + sq_database_.c_str() + std::string("' as backup");
    sqlite3_exec(inmemorydb_, attach_cmd.c_str(),
            NULL, NULL, NULL);

    // Copy the data from the backup to the in memory
    sqlite3_exec(inmemorydb_, "BEGIN", NULL, NULL, NULL);
    sqlite3_exec(inmemorydb_,
            "SELECT name FROM backup.sqlite_master WHERE type='table'",
            &process_dml_row, inmemorydb_, NULL);
    sqlite3_exec(inmemorydb_, "COMMIT", NULL, NULL, NULL);

    sqlite3_exec(inmemorydb_, "DETACH DATABASE backup", NULL, NULL,
            NULL);

    //process index
    sqlite3_exec(template_db_, "BEGIN", NULL, NULL, NULL);
    sqlite3_exec(
            template_db_,
            "SELECT index FROM sqlite_master WHERE sql NOT NULL",
            &process_ddl_row,
            inmemorydb_,
            NULL);
    sqlite3_exec(template_db_, "COMMIT", NULL, NULL, NULL);
    //sqlite3_close(template_db_);


    //rc = sqlite3_errcode(destination);
    //sqlite3_close(inmemorydb_);
    //sqlite3_close(template_db_);
    //sqlite3_close(destination);
}


/**
 * Exec an sql statement in values[0] against
 * the database in pData.
 */
int sqlite_handler::process_ddl_row(void * pData, int nColumns,
        char **values, char **columns)
{
    if (nColumns != 1)
        return 1; // Error

    sqlite3* db = (sqlite3*)pData;
    sqlite3_exec(db, values[0], NULL, NULL, NULL);

    return 0;
}

/**
 * Insert from a table named by backup.{values[0]}
 * into main.{values[0]} in database pData.
 */
int sqlite_handler::process_dml_row(void *pData, int nColumns,
        char **values, char **columns)
{
    if (nColumns != 1)
        return 1; // Error

    sqlite3* db = (sqlite3*)pData;

    char *stmt = sqlite3_mprintf("insert into main.%q "
            "select * from backup.%q", values[0], values[0]);
    sqlite3_exec(db, stmt, NULL, NULL, NULL);
    sqlite3_free(stmt);

    return 0;
}

int sqlite_handler::dump_db(const std::string& destination_db) {
    sqlite3* destination {};
    int rc;
    if((rc = sqlite3_open(destination_db.c_str(), &destination)) != SQLITE_OK) {
        std::cerr << "Could not open backup file RC: ["<< rc << "] :: "<< sqlite3_errmsg(destination);
    }
    sqlite3_backup* pBackup = sqlite3_backup_init(destination, "main", inmemorydb_, "main");
    if( pBackup ){
        if((rc = sqlite3_backup_step(pBackup, -1)) == SQLITE_DONE){
            if((rc = sqlite3_backup_finish(pBackup)) != SQLITE_OK) {
                std::cerr << "Could not open backup file RC: ["<< rc << "] :: "<< sqlite3_errmsg(destination);
            }
        }
    }
    if((rc = sqlite3_errcode(destination))!= SQLITE_OK) {
        std::cerr << "Something Went Wrong RC: ["<< rc << "] :: "<< sqlite3_errmsg(destination);
    }
    sqlite3_close(destination);
    return rc;
}
