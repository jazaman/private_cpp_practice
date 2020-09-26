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
#include "query_builder.h"

class config_manager;

class pg_handler {

public:
    enum table_status {NOT_READY=0, READY, INSERTED, ERRORED};
    pg_handler();
    int create_table(int argc, char* argv[]);
    int insert_data(int argc, char* argv[]);
    int select_data(const std::string& db_name, const std::string& table_name,
            const std::string& provider_id,
            std::vector<std::string>& column_names,
            std::vector<std::string>& result_values,
            table_status& query_status);
    const std::string get_providers_db(const std::string _providerid);
    int get_providerid(const std::string& providerid, int providerid_int,
            std::vector<std::string>& db_name) {
        return 0;
    }
private:
    query_builder query;
    config_manager& cm;

};
#endif /* SQLITE_COPY_PG_HANDLER_H_ */
