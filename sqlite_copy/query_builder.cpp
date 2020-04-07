/*
 * query_builder.cpp
 *
 *  Created on: Apr 6, 2020
 *      Author: jamil.zaman
 */
#include "query_builder.h"
#include <sstream>

query_builder::query_builder()
    : query_map{} {

}

query_builder::~query_builder() {

}

const std::string query_builder::get_query(const std::string& table_name) {
    //std::string table_query;

    return general_query;
}
