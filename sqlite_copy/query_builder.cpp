/*
 * query_builder.cpp
 *
 *  Created on: Apr 6, 2020
 *      Author: jamil.zaman
 */
#include "query_builder.h"
#include <sstream> 

query_builder::query_builder()
    : db_builder_queries_{
        {std::string("pregwomen"), pregwomen_query},
        {std::string("ancservice"), service_query},
        {std::string("delivery"), service_query},
        {std::string("newborn"), service_query},
        {std::string("pncservicemother"), service_query},
        {std::string("pncservicechild"), service_query},
        {std::string("pacservice"), service_query},
        {std::string("elco"), service_query},
        {std::string("fpinfo"), service_query},
        {std::string("fpexamination"), service_query},
        {std::string("womaninjectable"), service_query},
        {std::string("pillcondomservice"), service_query},
        {std::string("iudservice"), service_query},
        {std::string("iudfollowupservice"), service_query},
        {std::string("implantservice"), service_query},
        {std::string("implantfollowupservice"), service_query},
        {std::string("permanent_method_service"), service_query},
        {std::string("permanent_method_followup_service"), service_query},
        {std::string("immunizationhistory"), service_query},
        {std::string("gpservice"), service_query},
        {std::string("death"), service_query},
        {std::string("clientmap"), clientmap_query},
        {std::string("clientmap_extension"), clientmap_extn_query},
        {std::string("member"), member_query},
    }
{}

query_builder::~query_builder() {}

const std::string query_builder::get_query(const std::string& table_name) {
    std::string table_query = "";
    auto table = db_builder_queries_.find(table_name);
    if(table != db_builder_queries_.end()) {
        table_query = search_replace_all(table->second, "<table_name>", table_name);
    }
    return table_query;
}

std::string query_builder::search_replace_all(const std::string& str, const std::string& search_str, const std::string& replace_str) {

    size_t search_pos = 0;
    std::string copy_str = str;
    //loop through the string to find all occurrence and replace them
    while((search_pos = copy_str.find(search_str, search_pos)) != std::string::npos) {
        copy_str.replace(search_pos, search_str.length(), replace_str.c_str(), replace_str.length());
    }

    return copy_str;
}
