/*
 * pg_handler.h
 *
 *  Created on: Mar 20, 2020
 *      Author: jamil.zaman
 */

#ifndef SQLITE_COPY_PG_HANDLER_H_
#define SQLITE_COPY_PG_HANDLER_H_
#include <vector>
#include <string>

int create_table (int argc, char* argv[]);
int insert_data(int argc, char* argv[]);
int select_data(
        const std::string& db_name,
        const std::string& table_name,
        std::vector<std::string>& column_names,
        std::vector<std::string>& result_values);



#endif /* SQLITE_COPY_PG_HANDLER_H_ */
