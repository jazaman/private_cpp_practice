/*
 * sqlite_handler.h
 *
 *  Created on: Mar 24, 2020
 *      Author: jamil.zaman
 */

#ifndef SQLITE_COPY_SQLITE_HANDLER_H_
#define SQLITE_COPY_SQLITE_HANDLER_H_

#include <vector>
#include <string>


class sqlite3;
class time_wrapper;
class sqlite_handler {

public:
    sqlite_handler(const std::string& sq_database);
    ~sqlite_handler();
    int create_table (const std::string& sq_database);
    int insert_data(
            //const std::string& sq_database,
            const std::string& sq_table,
            const std::vector<std::string>& columns,
            const std::vector<std::string>& data);
    int select_data(std::vector<std::string>& column_names, std::vector<std::string>& result);
    int dump_db(const std::string& destination_db);

private:
    const std::string sq_database_;
    sqlite3* template_db_;
    sqlite3* inmemorydb_;
    time_wrapper& c_time_;
    static int process_ddl_row(void * pData, int nColumns, char **values, char **columns);
    static int process_dml_row(void *pData, int nColumns, char **values, char **columns);
    void load_memory_db();
};

#endif /* SQLITE_COPY_SQLITE_HANDLER_H_ */
