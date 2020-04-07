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
#include <sqlite3.h>

class sqlite_handler {

public:
int create_table (const std::string& sq_database);
int insert_data(const std::string& sq_database,
        const std::string& sq_table,
        const std::vector<std::string>& columns,
        const std::vector<std::string>& data);
int select_data(std::vector<std::string>& column_names, std::vector<std::string>& result);

};

#endif /* SQLITE_COPY_SQLITE_HANDLER_H_ */
